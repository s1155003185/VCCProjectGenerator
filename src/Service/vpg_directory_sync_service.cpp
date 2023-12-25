#include "vpg_directory_sync_service.hpp"

#include <filesystem>
#include <regex>
#include <string>
#include <vector>

#include "exception.hpp"
#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_property.hpp"
#include "log_service.hpp"

#include "vpg_file_sync_service.hpp"
#include "vpg_global.hpp"
#include "vpg_project_type.hpp"

using namespace std;
using namespace vcc;

void VPGDirectorySyncService::CheckAndCreateDirectory(LogProperty &logProperty, std::wstring workspace)
{
    try {
        // All type has same project structure
        vector<wstring> checkList;
        // bin
        checkList.push_back(L"bin");
        checkList.push_back(L"bin/Debug");
        checkList.push_back(L"bin/Release");

        // include
        checkList.push_back(L"include");

        // src
        checkList.push_back(L"src");

        // unittest
        checkList.push_back(L"unittest");
        
        for (auto path : checkList) {
            std::wstring absPath = ConcatPath(workspace, path);
            if (!IsDirectoryExists(absPath)) {
                CreateDirectory(absPath);
                LogService::LogInfo(logProperty, L"", L"Create Directory: " + path);
            }        
        }
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
}

bool VPGDirectorySyncService::_ShouldInclude(const std::wstring &path, const std::vector<std::wstring> &includeOnlyFileFilter)
{
    if (includeOnlyFileFilter.empty())
        return true;
    
    bool result = false;
    try {
        for (std::wstring patten : includeOnlyFileFilter) {
            if (std::regex_match(path, std::wregex(GetRegexFromFileFilter(patten)))) {
                result = true;
                break;
            }
        }
        return result;
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
    return result;
}

bool VPGDirectorySyncService::_ShouldExclude(const std::wstring &path, const std::vector<std::wstring> &excludeOnlyFileFilter)
{
    if (excludeOnlyFileFilter.empty())
        return false;

    bool result = false;
    try {
        for (std::wstring patten : excludeOnlyFileFilter) {
            if (std::regex_match(path, std::wregex(GetRegexFromFileFilter(patten)))) {
                result = true;
                break;
            }
        }
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
    return result;
}

void VPGDirectorySyncService::_SyncWorkspace(LogProperty &logProperty, std::wstring sourceWorkspace, std::wstring targetWorkspace,
            const std::vector<std::wstring> &includeOnlyFileFilter, const std::vector<std::wstring> &excludeOnlyFileFilter)
{
    try {
        std::vector<std::wstring> needToAdd, needToModify, needToDelete;
        GetFileDifferenceBetweenWorkspaces(sourceWorkspace, targetWorkspace, needToAdd, needToModify, needToDelete);

        // Delete
        for (auto path : needToDelete) {
            std::wstring targetPath = ConcatPath(targetWorkspace, path);
            if (!std::filesystem::exists(targetPath))
                continue;
            if (std::filesystem::is_directory(targetPath)) {
                if (!path.ends_with(L"/")) {
                    path += L"/";
                    targetPath += L"/";
                }                
            }

            if (!VPGDirectorySyncService::_ShouldInclude(path, includeOnlyFileFilter))
                continue;
            if (VPGDirectorySyncService::_ShouldExclude(path, excludeOnlyFileFilter))
                continue;


            if (IsFile(targetPath)) {
                remove(PATH(targetPath));
                LogService::LogInfo(logProperty, L"", L"Removed Directory: " + targetPath);
            } else {
                std::filesystem::remove_all(PATH(targetPath));
                LogService::LogInfo(logProperty, L"", L"Removed File: " + targetPath);
            }
        }

        // Add
        for (auto path : needToAdd) {
            std::wstring sourcePath = ConcatPath(sourceWorkspace, path);
            std::wstring targetPath = ConcatPath(targetWorkspace, path);
            if (!std::filesystem::exists(sourcePath))
                continue;
            if (std::filesystem::exists(targetPath))
                continue;
            if (std::filesystem::is_directory(sourcePath)) {
                if (!path.ends_with(L"/")) {
                    path += L"/";
                    sourcePath += L"/";
                    targetPath += L"/";
                }                
            }

            if (!VPGDirectorySyncService::_ShouldInclude(path, includeOnlyFileFilter))
                continue;
            if (VPGDirectorySyncService::_ShouldExclude(path, excludeOnlyFileFilter))
                continue;

            if (IsFile(sourcePath)) {
                VPGFileSyncService::CopyFile(logProperty, sourcePath, targetPath);
            } else {
                CreateDirectory(targetPath);
                LogService::LogInfo(logProperty, L"", L"Added Directory: " + targetPath);
            }
        }

        // Modify
        for (auto path : needToModify) {
            if (!VPGDirectorySyncService::_ShouldInclude(path, includeOnlyFileFilter))
                continue;
            if (VPGDirectorySyncService::_ShouldExclude(path, excludeOnlyFileFilter))
                continue;

            std::wstring sourcePath = ConcatPath(sourceWorkspace, path);
            std::wstring targetPath = ConcatPath(targetWorkspace, path);
            if (!IsFile(sourcePath) || !IsFile(targetPath))
                continue;
            
            // modify file
            VPGFileSyncService::CopyFile(logProperty, sourcePath, targetPath);
        }
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
}

void VPGDirectorySyncService::SyncWorkspace(LogProperty &logProperty, VPGProjectType projectType, std::wstring workspace,
    const std::vector<std::wstring> &includeOnlyFileFilter, const std::vector<std::wstring> &excludeOnlyFileFilter)
{
    try {
        std::wstring projectLocalDirectory = VPGGlobal::GetVccProjectLocalResponseDirectory(projectType);
        VPGDirectorySyncService::_SyncWorkspace(logProperty, projectLocalDirectory, workspace, includeOnlyFileFilter, excludeOnlyFileFilter);
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
}
