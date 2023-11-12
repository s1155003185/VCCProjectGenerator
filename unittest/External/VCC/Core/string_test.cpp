#include <gtest/gtest.h>

#include <regex>
#include <string>
#include <vector>

#include "string_helper.hpp"

using namespace std;
using namespace vcc;

/* ---------------------------------------------------------------------------------------------------- */
/*                                      Has Prefix                                                      */
/* ---------------------------------------------------------------------------------------------------- */
TEST(StringTest, HasPrefixTrimSpace_NoSpace)
{
    std::wstring prefix = L"//<vcc:abc";
    EXPECT_TRUE(HasPrefixTrimSpace(prefix, prefix));
}

TEST(StringTest, HasPrefixTrimSpace_Space)
{
    std::wstring prefix = L"//<vcc:abc";
    std::wstring text = L"// <vcc:abc";
    EXPECT_TRUE(HasPrefixTrimSpace(text, prefix));
}
/* ---------------------------------------------------------------------------------------------------- */
/*                                      Split String By Upper Case                                      */
/* ---------------------------------------------------------------------------------------------------- */
TEST(StringTest, SplitStringByUpperCase_Normal)
{
    std::wstring str = L"AbCd";
    std::vector<std::wstring> expectedResult = { L"Ab", L"Cd"};
    EXPECT_EQ(expectedResult, SplitStringByUpperCase(str, false, false));
}

TEST(StringTest, SplitStringByUpperCase_Space)
{
    std::wstring str = L"Ab cd";
    std::vector<std::wstring> expectedResult = { L"Ab", L"cd"};
    EXPECT_EQ(expectedResult, SplitStringByUpperCase(str, false, false));
}

TEST(StringTest, SplitStringByUpperCase_AllLowerCase)
{
    std::wstring str = L"abcd";
    std::vector<std::wstring> expectedResult = { str };
    EXPECT_EQ(expectedResult, SplitStringByUpperCase(str, false, false));
}

TEST(StringTest, SplitStringByUpperCase_AllUpperCase)
{
    std::wstring str = L"ABCD";
    std::vector<std::wstring> expectedResult = { L"A", L"B", L"C", L"D" };
    EXPECT_EQ(expectedResult, SplitStringByUpperCase(str, false, false));
}

TEST(StringTest, SplitStringByUpperCase_WithDigit)
{
    std::wstring str = L"A1B1C1D1";
    std::vector<std::wstring> expectedResult = { L"A1", L"B1", L"C1", L"D1" };
    EXPECT_EQ(expectedResult, SplitStringByUpperCase(str, false, false));

    expectedResult = { L"A", L"1", L"B", L"1", L"C", L"1", L"D", L"1" };
    EXPECT_EQ(expectedResult, SplitStringByUpperCase(str, true, false));
}

TEST(StringTest, SplitStringByUpperCase_WithSpecialChar)
{
    std::wstring str = L"Ab,cd,ef";
    std::vector<std::wstring> expectedResult = { str };
    EXPECT_EQ(expectedResult, SplitStringByUpperCase(str, false, false));

    expectedResult = { L"Ab", L"cd", L"ef" };
    EXPECT_EQ(expectedResult, SplitStringByUpperCase(str, false, true));
}

/* ---------------------------------------------------------------------------------------------------- */
/*                                             EscapeString                                             */
/* ---------------------------------------------------------------------------------------------------- */
TEST(StringTest, EscapeString)
{
    // Regex
    std::vector<wchar_t> specialChars = GetSpecialCharacters(EscapeStringType::Regex);
    std::wstring originalStr = L"";
    std::wstring expectedStr = L"";
    for (auto sc : specialChars) {
        originalStr += sc;
        expectedStr += L"\\" + std::wstring(1, sc);
    }
    EXPECT_EQ(GetEscapeString(EscapeStringType::Regex, originalStr), expectedStr);
    EXPECT_EQ(GetUnescapeString(EscapeStringType::Regex, expectedStr), originalStr);

    // XML
    specialChars = GetSpecialCharacters(EscapeStringType::XML);
    originalStr = L"";
    expectedStr = L"";
    for (auto sc : specialChars) {
        originalStr += sc;
        expectedStr += GetEscapeStringMap(EscapeStringType::XML)[sc];
    }
    EXPECT_EQ(GetEscapeString(EscapeStringType::XML, originalStr), expectedStr);
    EXPECT_EQ(GetUnescapeString(EscapeStringType::XML, expectedStr), originalStr);
}

/* ---------------------------------------------------------------------------------------------------- */
/*                                                 Regex                                                */
/* ---------------------------------------------------------------------------------------------------- */
TEST(StringTest, GetRegexFromFileFilter)
{
    EXPECT_EQ(GetRegexFromFileFilter(L"*.txt"), L".*\\.txt");
    EXPECT_EQ(GetRegexFromFileFilter(L"*abc*.txt"), L".*abc.*\\.txt");

    EXPECT_TRUE(std::regex_match(L"abcdef.txt", std::wregex(GetRegexFromFileFilter(L"*def*"))));
}