#include <gtest/gtest.h>

#include "vpg_cpp_helper.hpp"

// Mode
TEST(VPGCppHelperTest, getSimpleCode)
{
    std::wstring str = L"a b c";
    std::wstring expectedResult = L"a b c";
    EXPECT_EQ(getSimpleCode(str), expectedResult);

    str = L"a /* This is a comment */ b";
    expectedResult = L"a   b";
    EXPECT_EQ(getSimpleCode(str), expectedResult);
    
    str = L"a // This is a comment \n b";
    expectedResult = L"a   b";
    EXPECT_EQ(getSimpleCode(str), expectedResult);
    
    str = L"a /*// This is a comment \n*/ b";
    expectedResult = L"a   b";
    EXPECT_EQ(getSimpleCode(str), expectedResult);
    
    str = L"a /*// This is a */comment \n b";
    expectedResult = L"a  comment \n b";
    EXPECT_EQ(getSimpleCode(str), expectedResult);
}

TEST(VPGCppHelperTest, getIndentStringWithNamespaceLevel)
{
    size_t namespaceLevel = 0;
    std::wstring expectedResult = L"";
    EXPECT_EQ(getIndentStringWithNamespaceLevel(namespaceLevel), expectedResult);

    namespaceLevel = 1;
    expectedResult = L"    "; // 4 spaces
    EXPECT_EQ(getIndentStringWithNamespaceLevel(namespaceLevel), expectedResult);

    namespaceLevel = 2;
    expectedResult = L"        "; // 8 spaces
    EXPECT_EQ(getIndentStringWithNamespaceLevel(namespaceLevel), expectedResult);
}

TEST(VPGCppHelperTest, getNamespaceCommonPrefix)
{
    std::wstring namespace1 = L"std::vector";
    std::wstring namespace2 = L"std::list";
    std::wstring expectedResult = L"std";
    EXPECT_EQ(getNamespaceCommonPrefix(namespace1, namespace2), expectedResult);

    namespace1 = L"std::vector::iterator";
    namespace2 = L"std::vector::const_iterator";
    expectedResult = L"std::vector";
    EXPECT_EQ(getNamespaceCommonPrefix(namespace1, namespace2), expectedResult);

    namespace1 = L"vector";
    namespace2 = L"map";
    expectedResult = L"";
    EXPECT_EQ(getNamespaceCommonPrefix(namespace1, namespace2), expectedResult);
}
