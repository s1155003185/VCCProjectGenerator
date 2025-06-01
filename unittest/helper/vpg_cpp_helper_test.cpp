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

TEST(VPGCppHelperTest, GetIndentStringWithNamespaceLevel)
{
    size_t namespaceLevel = 0;
    std::wstring expectedResult = L"";
    EXPECT_EQ(GetIndentStringWithNamespaceLevel(namespaceLevel), expectedResult);

    namespaceLevel = 1;
    expectedResult = L"    "; // 4 spaces
    EXPECT_EQ(GetIndentStringWithNamespaceLevel(namespaceLevel), expectedResult);

    namespaceLevel = 2;
    expectedResult = L"        "; // 8 spaces
    EXPECT_EQ(GetIndentStringWithNamespaceLevel(namespaceLevel), expectedResult);
}

TEST(VPGCppHelperTest, GetNamespaceCommonPrefix)
{
    std::wstring namespace1 = L"std::vector";
    std::wstring namespace2 = L"std::list";
    std::wstring expectedResult = L"std";
    EXPECT_EQ(GetNamespaceCommonPrefix(namespace1, namespace2), expectedResult);

    namespace1 = L"std::vector::iterator";
    namespace2 = L"std::vector::const_iterator";
    expectedResult = L"std::vector";
    EXPECT_EQ(GetNamespaceCommonPrefix(namespace1, namespace2), expectedResult);

    namespace1 = L"vector";
    namespace2 = L"map";
    expectedResult = L"";
    EXPECT_EQ(GetNamespaceCommonPrefix(namespace1, namespace2), expectedResult);
}
