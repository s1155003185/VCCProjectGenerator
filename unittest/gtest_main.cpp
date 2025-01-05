#include <gtest/gtest.h>

#include "vpg_global.hpp"

GTEST_API_ int main() {
    printf("Running main() from %s\n", __FILE__);
    ::testing::InitGoogleTest();
    VPGGlobal::Initialize(L"");
    return RUN_ALL_TESTS();
}