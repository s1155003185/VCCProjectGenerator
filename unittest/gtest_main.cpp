#include <gtest/gtest.h>

#include "vpg_global.hpp"
#include <gtest/gtest.h>

GTEST_API_ int main() {
    printf("Running main() from %s\n", __FILE__);
    ::testing::InitGoogleTest();
    VPGGlobal::initialize(L"");
    return RUN_ALL_TESTS();
}
