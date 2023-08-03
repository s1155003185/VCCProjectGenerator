#include "vpg_file_update_service.hpp"

#include <filesystem>
#include <string>

#include "vpg_file_update_type.hpp"

#include "command_service.hpp"
#include "exception.hpp"
#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "git_service.hpp"
#include "log_property.hpp"

using namespace vcc;

constexpr auto URL = L"https://github.com/s1155003185/";
constexpr auto URL_GIT = L".git"; 
constexpr auto CPP_DLL =  L"VCCSimpleTemplateDLL";
constexpr auto CPP_EXE = L"VCCSimpleTemplateEXE";
constexpr auto VCC_DLL = L"VCCModuleDLL";
constexpr auto VCC_EXE = L"VCCModuleDLL";



std::wstring VPGFileUpdateService::_DefaultFolder(LogProperty &logProperty)
{
    try {
        #ifdef __WIN32
        static_assert(false); // need to consider window documents path
        #endif

        return CommandService::Execute(logProperty, L"", L"echo ~/Documents");
    } catch (std::exception &ex) {
        THROW_EXCEPTION(ex);
    }
    return L"";
}

std::wstring VPGFileUpdateService::_GetProjectName(VPGProjectType projectType)
{
    std::wstring result = L"";
    switch (projectType) {
        case VPGProjectType::VCCDll:
            result += VCC_DLL;
            break;
        case VPGProjectType::VCCInterface:
            result += VCC_EXE;
            break;
        case VPGProjectType::CPPDll:
            result += CPP_DLL;
            break;
        case VPGProjectType::CPPInterface:
            result += CPP_EXE;
            break;
        default:
            assert(false);
    }
    return result;
}

std::wstring VPGFileUpdateService::_DownloadVCCResource(LogProperty &logProperty, std::wstring url, wstring branch, std::wstring directory)
{
    try {
        return GitService::CloneResponse(logProperty, url, branch, directory);
    } catch (std::exception &ex) {
        THROW_EXCEPTION(ex);
    }
    return L"";
}

std::wstring VPGFileUpdateService::DownloadVCCResource(LogProperty &logProperty, VPGProjectType projectType, std::wstring directory)
{
    try {
        if (directory.length() == 0) 
            directory = VPGFileUpdateService::_DefaultFolder(logProperty);

        std::wstring url = URL + VPGFileUpdateService::_GetProjectName(projectType) + URL_GIT;
        return VPGFileUpdateService::_DownloadVCCResource(logProperty, url, L"", directory);
    } catch (std::exception &ex) {
        THROW_EXCEPTION(ex);
    }
    return L"";
}

std::wstring VPGFileUpdateService::_UpdateVCCResource(LogProperty &logProperty, std::wstring workspace)
{
    try {
        return GitService::Pull(logProperty, workspace);
    } catch (std::exception &ex) {
        THROW_EXCEPTION(ex);
    }
    return L"";
}

std::wstring VPGFileUpdateService::UpdateVCCResource(LogProperty &logProperty, VPGProjectType projectType, std::wstring directory)
{
    try {
        if (directory.length() == 0) 
            directory = VPGFileUpdateService::_DefaultFolder(logProperty);

        std::wstring workspace = ConcatPath(directory, VPGFileUpdateService::_GetProjectName(projectType));
        return GitService::Pull(logProperty, workspace);
    } catch (std::exception &ex) {
        THROW_EXCEPTION(ex);
    }
    return L"";
}
