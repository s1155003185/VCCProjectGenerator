#include <gtest/gtest.h>

#include <string>

#include "dll.hpp"

#define DLL_NAME L"libvpg"

using namespace vcc;

TEST(DllTest, LoadDll) {
    std::wstring dllName = DLL_NAME;

    #ifdef _WIN32
    dllName += L".dll";
    #else
    dllName = L"bin/Debug/" + dllName + L".so";
    #endif
    Dll h(dllName);
    EXPECT_TRUE(h.GetH());
    
    typedef long long int (*GetVersionFunction)();
    const GetVersionFunction GetVersion = reinterpret_cast<GetVersionFunction>(h.GetProcedure(L"GetVersion"));
    EXPECT_TRUE(GetVersion != nullptr);
    EXPECT_EQ(GetVersion(), 1);
}
