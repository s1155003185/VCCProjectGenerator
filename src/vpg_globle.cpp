#include "vpg_global.hpp"

#include <string>

#include "command_service.hpp"
#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_property.hpp"

#include "vpg_project_type.hpp"

constexpr auto URL = L"https://github.com/s1155003185/";
constexpr auto URL_GIT = L".git"; 
constexpr auto CPP_DLL =  L"VCCSimpleTemplateDLL";
constexpr auto CPP_EXE = L"VCCSimpleTemplateEXE";
constexpr auto VCC_DLL = L"VCCModuleDLL";
constexpr auto VCC_EXE = L"VCCModuleDLL";

using namespace vcc;


std::wstring VPGGlobal::GetVersion()
{
    return L"0.0.1";
}

std::wstring VPGGlobal::GetDefaultFolder()
{
    try {
        return GetSystemFolderPath(SystemFolderType::LocalDocuments);
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
    return L"";
}

std::wstring VPGGlobal::GetProjectLocalDirectory(VPGProjectType projectType, std::wstring parentFolder)
{
    if (parentFolder.length() == 0) 
        parentFolder = VPGGlobal::GetDefaultFolder();
    return ConcatPath(parentFolder, VPGGlobal::GetProjectName(projectType));
}

std::wstring VPGGlobal::GetProjectName(VPGProjectType projectType)
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

std::wstring VPGGlobal::GetProjecURL(VPGProjectType projectType)
{
    return URL + VPGGlobal::GetProjectName(projectType) + URL_GIT;
}