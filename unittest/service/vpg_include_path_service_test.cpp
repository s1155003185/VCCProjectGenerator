#include <gtest/gtest.h>

#include "platform_type.hpp"
#include "vpg_global.hpp"
#include "vpg_include_path_service.hpp"

// Mode
TEST(VPGIncludePathSerciceTest, getWorkspaceIncludePath)
{
    std::map<std::wstring, std::wstring> classPathMapping;
    std::set<std::wstring> classMacroList;
    std::map<std::wstring, std::shared_ptr<VPGEnumClass>> enumClassMapping;
    VPGIncludePathService::getWorkspaceIncludePath(L"", classMacroList, classPathMapping, enumClassMapping);
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
//     workspace = VPGGlobal::getCppDefaultIncludePathWindow();
//     #elif __APPLE__
//     platformType = PlatformType::MacOs;
//     workspace = VPGGlobal::getCppDefaultIncludePathMacOs();
//     #else
//     platformType = PlatformType::Linux;
//     workspace = VPGGlobal::getCppDefaultIncludePathLinux();
//     #endif
//     VPGIncludePathService::GetSystemIncludePath(platformType, workspace, classPathMapping);

//     EXPECT_EQ(classPathMapping.at(L"string"), L"string");
//     EXPECT_EQ(classPathMapping.at(L"std::complex"), L"complex");
// }
