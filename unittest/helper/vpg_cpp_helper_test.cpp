#include <gtest/gtest.h>

#include "vpg_cpp_helper.hpp"

// Mode
TEST(VPGCppHelperTest, GetSimpleCode)
{
    std::wstring str = L"a b c";
    std::wstring expectedResult = L"a b c";
    EXPECT_EQ(GetSimpleCode(str), expectedResult);

    str = L"a /* This is a comment */ b";
    expectedResult = L"a   b";
    EXPECT_EQ(GetSimpleCode(str), expectedResult);
    
    str = L"a // This is a comment \n b";
    expectedResult = L"a   b";
    EXPECT_EQ(GetSimpleCode(str), expectedResult);
    
    str = L"a /*// This is a comment \n*/ b";
    expectedResult = L"a   b";
    EXPECT_EQ(GetSimpleCode(str), expectedResult);
    
    str = L"a /*// This is a */comment \n b";
    expectedResult = L"a  comment \n b";
    EXPECT_EQ(GetSimpleCode(str), expectedResult);
}
