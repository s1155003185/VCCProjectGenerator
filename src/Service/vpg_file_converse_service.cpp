#include "vpg_file_converse_service.hpp"

#include <filesystem>
#include <string>
#include <vector>

#include "file_helper.hpp"
#include "log_property.hpp"
#include "log_service.hpp"
#include "vector_helper.hpp"

using namespace std;
using namespace vcc;

void VPGFileConverseService::CheckAndCreateDirectory(LogProperty &logProperty, std::wstring workspace)
{
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
}

void VPGFileConverseService::GetFileDifferenceInWorkspaces(std::wstring sourceWorkspace, std::wstring targetWorkspace, 
    std::vector<std::wstring> &needToAdd, std::vector<std::wstring> &needToDelete, std::vector<std::wstring> &needToModify)
{
    std::vector<std::wstring> srcFileList, tarFileList;
    for (auto &filePath : std::filesystem::recursive_directory_iterator(PATH(sourceWorkspace)))
        srcFileList.push_back(std::wstring(filePath.path().wstring().substr(sourceWorkspace.length())));
    for (auto &filePath : std::filesystem::recursive_directory_iterator(PATH(targetWorkspace)))
        tarFileList.push_back(std::wstring(filePath.path().wstring().substr(targetWorkspace.length())));

    std::sort(srcFileList.begin(), srcFileList.end());
    std::sort(tarFileList.begin(), tarFileList.end());

    std::vector<std::wstring> equalFiles;
    std::set_intersection(srcFileList.begin(),srcFileList.end(), tarFileList.begin(), tarFileList.end(), back_inserter(equalFiles));

    RemoveVectorIfContainElements(srcFileList, equalFiles);
    RemoveVectorIfContainElements(tarFileList, equalFiles);

    needToAdd.assign(srcFileList.begin(), srcFileList.end());
    needToDelete.assign(tarFileList.begin(), tarFileList.end());

    for (auto &str : equalFiles) {
        std::wstring srcFile = ConcatPath(sourceWorkspace, str);
        std::wstring tarFile = ConcatPath(targetWorkspace, str);
        if (IsFile(srcFile) && IsFile(tarFile)) {
            if (!IsFileEqual(srcFile, tarFile))
                needToModify.push_back(std::wstring(str));
        }
    }
    
}