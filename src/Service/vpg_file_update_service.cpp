#include "vpg_file_update_service.hpp"

#include <filesystem>
#include <string>

#include "vpg_file_update_type.hpp"

#include "command_service.hpp"
#include "exception.hpp"
#include "exception_macro.hpp"
#include "file_constant.hpp"
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

std::wstring VPGFileUpdateService::_GetProjectName(VPGDllType dllType)
{
    std::wstring result = L"";
    switch (dllType) {
        case VPGDllType::VCC:
            result += VCC_DLL;
            break;
        case VPGDllType::CPP:
            result += CPP_DLL;
            break;
        default:
            assert(false);
    }
    return result;
}

std::wstring VPGFileUpdateService::_GetProjectName(VPGInterfaceType interfaceType)
{
    std::wstring result = L"";
    switch (interfaceType) {
        case VPGInterfaceType::VCC:
            result += VCC_EXE;
            break;
        case VPGInterfaceType::CPP:
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

std::wstring VPGFileUpdateService::DownloadVCCResource(LogProperty &logProperty, VPGDllType dllType, std::wstring directory)
{
    try {
        if (directory.length() == 0) 
            directory = VPGFileUpdateService::_DefaultFolder(logProperty);

        std::wstring url = URL + VPGFileUpdateService::_GetProjectName(dllType) + URL_GIT;
        return VPGFileUpdateService::_DownloadVCCResource(logProperty, url, L"", directory);
    } catch (std::exception &ex) {
        THROW_EXCEPTION(ex);
    }
    return L"";
}

std::wstring VPGFileUpdateService::DownloadVCCResource(LogProperty &logProperty, VPGInterfaceType interfaceType, std::wstring directory)
{
    try {
        if (directory.length() == 0) 
            directory = VPGFileUpdateService::_DefaultFolder(logProperty);

        std::wstring url = URL + VPGFileUpdateService::_GetProjectName(interfaceType) + URL_GIT;
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

std::wstring VPGFileUpdateService::UpdateVCCResource(LogProperty &logProperty, VPGDllType dllType, std::wstring directory)
{
    try {
        if (directory.length() == 0) 
            directory = VPGFileUpdateService::_DefaultFolder(logProperty);

        std::wstring workspace = PathConcat(directory, VPGFileUpdateService::_GetProjectName(dllType));
        return GitService::Pull(logProperty, workspace);
    } catch (std::exception &ex) {
        THROW_EXCEPTION(ex);
    }
    return L"";
}

std::wstring VPGFileUpdateService::UpdateVCCResource(LogProperty &logProperty, VPGInterfaceType interfaceType, std::wstring directory)
{
    try {
        if (directory.length() == 0) 
            directory = VPGFileUpdateService::_DefaultFolder(logProperty);

        std::wstring workspace = PathConcat(directory, VPGFileUpdateService::_GetProjectName(interfaceType));
        return GitService::Pull(logProperty, workspace);
    } catch (std::exception &ex) {
        THROW_EXCEPTION(ex);
    }
    return L"";
}
