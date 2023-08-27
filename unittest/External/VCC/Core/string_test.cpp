#include <gtest/gtest.h>

#include <regex>
#include <string>
#include <vector>

#include "string_helper.hpp"

using namespace std;
using namespace vcc;

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

    // XML
    specialChars = GetSpecialCharacters(EscapeStringType::XML);
    originalStr = L"";
    expectedStr = L"";
    for (auto sc : specialChars) {
        originalStr += sc;
        expectedStr += GetEscapeStringMap(EscapeStringType::XML)[sc];
    }
    EXPECT_EQ(GetEscapeString(EscapeStringType::XML, originalStr), expectedStr);
}

TEST(StringTest, GetRegexFromFileFilter)
{
    EXPECT_EQ(GetRegexFromFileFilter(L"*.txt"), L".*\\.txt");
    EXPECT_EQ(GetRegexFromFileFilter(L"*abc*.txt"), L".*abc.*\\.txt");

    EXPECT_TRUE(std::regex_match(L"abcdef.txt", std::wregex(GetRegexFromFileFilter(L"*def*"))));
}