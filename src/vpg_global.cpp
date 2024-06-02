#include "vpg_global.hpp"

#include <string>

#include "terminal_service.hpp"
#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_property.hpp"

#include "vpg_project_type.hpp"

constexpr auto URL = L"https://github.com/s1155003185/";
constexpr auto URL_GIT = L".git"; 
constexpr auto CPP_GIT_PROJ_NAME =  L"VCCSimpleTemplateCPP";
constexpr auto VCC_GIT_PROJ_NAME = L"VCCModule";

using namespace vcc;


std::wstring VPGGlobal::GetVersion()
{
    return L"v0.0.5";
}

std::wstring VPGGlobal::GetVccLocalResponseFolder()
{
    try {
        return ConcatPaths({GetSystemFolderPath(SystemFolderType::LocalDocuments), L"vcc"});
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
    return L"";
}

std::wstring VPGGlobal::GetVccProjectLocalResponseDirectory(VPGProjectType projectType)
{
    try {
        return ConcatPaths({VPGGlobal::GetVccLocalResponseFolder(), VPGGlobal::GetProjectName(projectType)});
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
    return L"";
}

std::wstring VPGGlobal::GetProjectName(VPGProjectType projectType)
{
    std::wstring result = L"";
    switch (projectType) {
        case VPGProjectType::VccComplex:
        case VPGProjectType::VccDll:
        case VPGProjectType::VccExe:
            result += VCC_GIT_PROJ_NAME;
            break;
        case VPGProjectType::CppComplex:
        case VPGProjectType::CppDll:
        case VPGProjectType::CppExe:
            result += CPP_GIT_PROJ_NAME;
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

std::wstring VPGGlobal::GetVccJsonFileName()
{
    return L".vcc/vcc.json";
}

std::wstring VPGGlobal::GetCppDefaultIncludePathWindow()
{
    return L"C:\\msys64\\mingw64\\include\\c++\\12.2.0";
}

std::wstring VPGGlobal::GetCppDefaultIncludePathLinux()
{
    return L"/usr/include";
}

std::wstring VPGGlobal::GetCppDefaultIncludePathMacOs()
{
    return L"/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/V1";
}
