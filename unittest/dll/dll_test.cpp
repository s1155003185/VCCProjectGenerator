#include <gtest/gtest.h>

#include <string>

#include "dll.hpp"

#define DLL_NAME L"libvpg"

TEST(DllTest, LoadDll) {
    std::wstring dllName = DLL_NAME;

    #ifdef _WIN32
    dllName += L".dll";
    #elif __aarch64__
    dllName = L"bin/Debug/" + dllName + L".dylib";
    #else
    dllName = L"bin/Debug/" + dllName + L".so";
    #endif
    vcc::Dll h(dllName);
    EXPECT_TRUE(h.GetH());
    
    typedef int (*GetVersionFunction)(wchar_t **);
    const GetVersionFunction GetVersion = reinterpret_cast<GetVersionFunction>(h.GetProcedure(L"GetVersion"));
    EXPECT_TRUE(GetVersion != nullptr);
    
    wchar_t* versionStr = nullptr;
    int result = GetVersion(&versionStr);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(std::wstring(versionStr), L"v0.0.1");
    free(versionStr);
}
