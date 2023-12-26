#include "vpg_file_update_service.hpp"

#include <filesystem>
#include <string>

#include "command_service.hpp"
#include "exception.hpp"
#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "git_service.hpp"
#include "log_property.hpp"

#include "vpg_global.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

std::wstring VPGFileUpdateService::DownloadVCCResource(LogProperty &logProperty, VPGProjectType projectType, std::wstring branch, std::wstring directory)
{
    try {
        if (directory.empty()) 
            directory = VPGGlobal::GetDefaultFolder();
        return GitService::CloneResponse(logProperty, VPGGlobal::GetProjecURL(projectType), branch, directory);
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
    return L"";
}

std::wstring VPGFileUpdateService::UpdateVCCResource(LogProperty &logProperty, VPGProjectType projectType, std::wstring directory)
{
    try {
        return GitService::Pull(logProperty, VPGGlobal::GetProjectLocalDirectory(projectType, directory));
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
    return L"";
}
