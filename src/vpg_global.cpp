#include "vpg_global.hpp"

#include <assert.h>
#include <string>
#include <memory>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_config.hpp"

#include "vpg_project_type.hpp"

constexpr auto URL = L"https://github.com/s1155003185/";
constexpr auto URL_GIT = L".git"; 
constexpr auto CPP_GIT_PROJ_NAME =  L"VCCSimpleTemplateCPP";
constexpr auto VCC_GIT_PROJ_NAME = L"VCCModule";

constexpr auto USER_HOME_VARIABLE = L"${userHome}";

std::shared_ptr<VPGEnumClassReader> VPGGlobal::_EnumClassReader = nullptr;
std::shared_ptr<VPGFileGenerationManager> VPGGlobal::_FileGenerationManager = nullptr;

std::wstring VPGGlobal::getVersion()
{
    return L"v0.4.2";
}

std::wstring VPGGlobal::getVccLocalResponseFolder()
{
    TRY
        return vcc::concatPaths({USER_HOME_VARIABLE, L"Documents", L"vcc"});
    CATCH
    return L"";
}

std::wstring VPGGlobal::getVccProjectLocalResponseDirectory(VPGProjectType projectType)
{
    TRY
        return vcc::concatPaths({VPGGlobal::getVccLocalResponseFolder(), VPGGlobal::getProjectName(projectType)});
    CATCH
    return L"";
}

std::wstring VPGGlobal::getProjectName(VPGProjectType projectType)
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

std::wstring VPGGlobal::getProjecURL(VPGProjectType projectType)
{
    return URL + VPGGlobal::getProjectName(projectType) + URL_GIT;
}

std::wstring VPGGlobal::getVccJsonFileName()
{
    return L".vcc/vcc.json";
}

std::wstring VPGGlobal::getCppDefaultIncludePathWindow()
{
    return L"C:\\msys64\\mingw64\\include\\c++\\12.2.0";
}

std::wstring VPGGlobal::getCppDefaultIncludePathLinux()
{
    return L"/usr/include";
}

std::wstring VPGGlobal::getCppDefaultIncludePathMacOs()
{
    return L"/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/V1";
}

std::wstring VPGGlobal::getVCCProjectManagerConfigFileName()
{
    return L"vpm.config.json";
}

std::wstring VPGGlobal::getVCCProjectManagerConfigFileFullPath()
{
    return VPGGlobal::getConvertedPath(vcc::concatPaths({ VPGGlobal::getVccLocalResponseFolder(), L"config", VPGGlobal::getVCCProjectManagerConfigFileName() }));
}

std::shared_ptr<VPGEnumClassReader> VPGGlobal::getEnumClassReader()
{
    assert(VPGGlobal::_EnumClassReader != nullptr);
    return VPGGlobal::_EnumClassReader;
}

std::shared_ptr<VPGFileGenerationManager> VPGGlobal::getFileGenerationManager()
{
    assert(VPGGlobal::_FileGenerationManager != nullptr);
    return VPGGlobal::_FileGenerationManager;
}

std::wstring VPGGlobal::getConvertedPath(const std::wstring &path)
{
    std::wstring result = path;
    TRY
        std::wstring userHomePath = vcc::getSystemFolderPath(vcc::SystemFolderType::UserHome);
        vcc::replaceAll(result, USER_HOME_VARIABLE, userHomePath);
    CATCH
    return result;
}

void VPGGlobal::initialize(const std::wstring &projectWorkspace)
{
    VPGGlobal::_FileGenerationManager = std::make_unique<VPGFileGenerationManager>(std::make_shared<vcc::LogConfig>(vcc::LogConfigInitialType::None), L"");
    VPGGlobal::_FileGenerationManager->getClassMacroList(projectWorkspace);
    VPGGlobal::_EnumClassReader = std::make_shared<VPGEnumClassReader>(VPGGlobal::_FileGenerationManager->getClassMacros());
}
