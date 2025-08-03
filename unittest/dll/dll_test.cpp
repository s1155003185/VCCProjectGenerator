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
    EXPECT_TRUE(h.getH());
    
    typedef int (*getVersionFunction)(wchar_t **);
    const getVersionFunction getVersion = reinterpret_cast<getVersionFunction>(h.getProcedure(L"getVersion"));
    EXPECT_TRUE(getVersion != nullptr);
    
    wchar_t* versionStr = nullptr;
    int result = getVersion(&versionStr);
    EXPECT_EQ(result, 0);
    EXPECT_EQ(std::wstring(versionStr), L"v0.0.1");
    free(versionStr);
}
