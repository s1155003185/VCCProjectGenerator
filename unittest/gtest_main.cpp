#include <gtest/gtest.h>

GTEST_API_ int main() {
    printf("Running main() from %s\n", __FILE__);
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}