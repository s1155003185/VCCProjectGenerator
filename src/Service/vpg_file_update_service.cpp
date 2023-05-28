#include "vpg_file_update_service.hpp"

#include <filesystem>
#include <string>

#include "vpg_file_update_type.hpp"

#include "command_service.hpp"
#include "exception.hpp"
#include "exception_macro.hpp"
#include "git_service.hpp"
#include "log_property.hpp"

using namespace vcc;

static LogProperty logProperty;

std::wstring VPGFileUpdateService::_DefaultFolder()
{
    #ifdef __WIN32
    static_assert(false); // need to consider window documents path
    #endif

    return CommandService::Execute(L"echo ~/Documents");
}

void InitLogProperty()
{
    logProperty.SetIsConsoleLog(true);
    logProperty.SetFilePath(std::filesystem::current_path());
    logProperty.SetIsLogCommand(true);
    logProperty.SetIsLogCommandResult(true);
}

std::wstring VPGFileUpdateService::_DownloadVCCResource(std::wstring url, wstring branch, std::wstring directory, bool forceUpdate, int64_t logLevel)
{
    try {
        InitLogProperty();
        GitService::CloneResponse(logProperty, url, branch, directory);
    } catch (Exception &ex) {
        THROW_EXCEPTION(ex.GetErrorType(), ex.GetErrorMessage());
    }
    return L"";
}

std::wstring VPGFileUpdateService::DownloadVCCResource(VPGDllType dllType, std::wstring directory, bool forceUpdate, int64_t logLevel)
{
    if (directory.length() == 0) 
        directory = VPGFileUpdateService::_DefaultFolder();

    std::wstring url = L"";
    switch (dllType) {
        case VPGDllType::VCC:
            url = L"https://github.com/s1155003185/VCCModuleDLL.git";
            break;
        case VPGDllType::CPP:
            url = L"https://github.com/s1155003185/VCCSimpleTemplateDLL.git";
            break;
    }
    VPGFileUpdateService::_DownloadVCCResource(url, L"", directory, forceUpdate, logLevel);
    return L"";
}

std::wstring VPGFileUpdateService::DownloadVCCResource(VPGInterfaceType interfaceType, std::wstring directory, bool forceUpdate, int64_t logLevel)
{
    std::wstring url = L"";
    switch (interfaceType) {
        case VPGInterfaceType::VCC:
            url = L"https://github.com/s1155003185/VCCModuleDLL.git";
            break;
        case VPGInterfaceType::CPP:
            url = L"https://github.com/s1155003185/VCCSimpleTemplateEXE.git";
            break;
    }
    VPGFileUpdateService::_DownloadVCCResource(url, L"", directory, forceUpdate, logLevel);
    return L"";
}
