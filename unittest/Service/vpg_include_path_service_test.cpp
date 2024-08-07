#include <gtest/gtest.h>

#include "platform_type.hpp"
#include "vpg_global.hpp"
#include "vpg_include_path_service.hpp"

using namespace vcc;

// Mode
TEST(VPGIncludePathSerciceTest, GetWorkspaceIncludePath)
{
    std::map<std::wstring, std::wstring> classPathMapping;
    std::set<std::wstring> classMacroList;
    std::map<std::wstring, std::shared_ptr<VPGEnumClass>> enumClassMapping;
    VPGIncludePathService::GetWorkspaceIncludePath(L"", classMacroList, classPathMapping, enumClassMapping);
    EXPECT_EQ(classPathMapping.at(L"vcc::Json"), L"json.hpp");
    EXPECT_TRUE(enumClassMapping.find(L"vcc::JsonInternalType") != enumClassMapping.end());
}

// TEST(VPGIncludePathSerciceTest, GetSystemIncludePath)
// {
//     std::map<std::wstring, std::wstring> classPathMapping;
//     PlatformType platformType = PlatformType::All;
//     std::wstring workspace = L"";
//     #ifdef _WIN32
//     platformType = PlatformType::Window;
//     workspace = VPGGlobal::GetCppDefaultIncludePathWindow();
//     #elif __APPLE__
//     platformType = PlatformType::MacOs;
//     workspace = VPGGlobal::GetCppDefaultIncludePathMacOs();
//     #else
//     platformType = PlatformType::Linux;
//     workspace = VPGGlobal::GetCppDefaultIncludePathLinux();
//     #endif
//     VPGIncludePathService::GetSystemIncludePath(platformType, workspace, classPathMapping);

//     EXPECT_EQ(classPathMapping.at(L"std::string"), L"string");
//     EXPECT_EQ(classPathMapping.at(L"std::complex"), L"complex");
// }
