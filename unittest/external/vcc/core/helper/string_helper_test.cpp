#include <gtest/gtest.h>

#include <regex>
#include <string>
#include <vector>

#include "string_helper.hpp"

using namespace vcc;

/* ---------------------------------------------------------------------------------------------------- */
/*                                      Has Prefix                                                      */
/* ---------------------------------------------------------------------------------------------------- */
TEST(StringHelperTest, IsStartWithTrimSpace_NoSpace)
{
    std::wstring prefix = L"a<vcc:abc";
    EXPECT_TRUE(IsStartWithTrimSpace(prefix, prefix));
}

TEST(StringHelperTest, IsStartWithTrimSpace_Space)
{
    std::wstring prefix = L"a<vcc:abc";
    std::wstring text = L"a <vcc:abc";
    EXPECT_TRUE(IsStartWithTrimSpace(text, prefix));
}

/* ---------------------------------------------------------------------------------------------------- */
/*                                      Conversion                                                      */
/* ---------------------------------------------------------------------------------------------------- */

TEST(StringHelperTest, ConvertNamingStyle)
{
    std::wstring str = L"PascalCase";
    EXPECT_EQ(ConvertNamingStyle(str, NamingStyle::PascalCase, NamingStyle::CamelCase), L"pascalCase");
    EXPECT_EQ(ConvertNamingStyle(str, NamingStyle::PascalCase, NamingStyle::ConstantCase), L"PASCAL_CASE");
    EXPECT_EQ(ConvertNamingStyle(str, NamingStyle::PascalCase, NamingStyle::DotSeparatedLowercase), L"pascal.case");
    EXPECT_EQ(ConvertNamingStyle(str, NamingStyle::PascalCase, NamingStyle::KebabCase), L"pascal-case");
    EXPECT_EQ(ConvertNamingStyle(str, NamingStyle::PascalCase, NamingStyle::Lowercase), L"pascal case");
    EXPECT_EQ(ConvertNamingStyle(str, NamingStyle::PascalCase, NamingStyle::PascalCase), L"PascalCase");
    EXPECT_EQ(ConvertNamingStyle(str, NamingStyle::PascalCase, NamingStyle::ScreamingSnakeCase), L"PASCAL_CASE");
    EXPECT_EQ(ConvertNamingStyle(str, NamingStyle::PascalCase, NamingStyle::SnakeCase), L"pascal_case");
    EXPECT_EQ(ConvertNamingStyle(str, NamingStyle::PascalCase, NamingStyle::Uppercase), L"PASCAL CASE");
}
/* ---------------------------------------------------------------------------------------------------- */
/*                                      Split String                                                    */
/* ---------------------------------------------------------------------------------------------------- */
TEST(StringHelperTest, SplitString_HeadAndTail)
{
    std::wstring str = L";Ab;Cd;\"CommandA;CommandB;\";;Last;";
    std::vector<std::wstring> expectedResult = { L"", L"Ab", L"Cd", L"\"CommandA", L"CommandB", L"\"", L"", L"Last", L"" };
    EXPECT_EQ(expectedResult, SplitString(str, { L";" }));
}

TEST(StringHelperTest, SplitString_SplitDelimiterInString)
{
    std::wstring str = L"Ab;Cd;\"CommandA;CommandB;\";;Last";
    std::vector<std::wstring> expectedResult = { L"Ab", L"Cd", L"\"CommandA", L"CommandB", L"\"", L"", L"Last" };
    EXPECT_EQ(expectedResult, SplitString(str, { L";" }));
}

TEST(StringHelperTest, SplitString_NotSplitDelimiterInString_NoEscape)
{
    std::wstring str = L"Ab;Cd;\"CommandA\\\";\\\"CommandB;\";Last";
    std::vector<std::wstring> expectedResult = { L"Ab", L"Cd", L"\"CommandA\\\"", L"\\\"CommandB;\"", L"Last" };
    EXPECT_EQ(expectedResult, SplitString(str, { L";" },  { L"\"" }, { L"\"" }, { L"" }));
}

TEST(StringHelperTest, SplitString_NotSplitDelimiterInString)
{
    std::wstring str = L"Ab;Cd;\"CommandA\\\";\\\"CommandB;\";Last";
    std::vector<std::wstring> expectedResult = { L"Ab", L"Cd", L"\"CommandA\\\";\\\"CommandB;\"", L"Last" };
    EXPECT_EQ(expectedResult, SplitString(str, { L";" },  { L"\"" }, { L"\"" }, { L"\\" }));
}

TEST(StringHelperTest, SplitString_MultiDelimiter)
{
    std::wstring str = L"Ab;Cd;\"CommandA;CommandB;\";Last";
    std::vector<std::wstring> expectedResult = { L"Ab;Cd;\"", L"mmandA;", L"mmandB;\";Last" };
    EXPECT_EQ(expectedResult, SplitString(str, { L"Co" }));
}

TEST(StringHelperTest, SplitString_Nested)
{
    std::wstring str = L"Ab;Cd;[a; [ab; cd]; ef];Last";
    std::vector<std::wstring> expectedResult = { L"Ab", L"Cd", L"[a; [ab; cd]; ef]", L"Last" };
    EXPECT_EQ(expectedResult, SplitString(str, { L";" }, { L"[" }, { L"]" }));
}
/* ---------------------------------------------------------------------------------------------------- */
/*                                      Split String By Line                                            */
/* ---------------------------------------------------------------------------------------------------- */
TEST(StringHelperTest, SplitStringByLine)
{
    std::wstring str = L"Ab\r\nCd\r\n\"Command\\r\\nCommand\"\r\nLast Line\r\n";
    std::vector<std::wstring> expectedResult = { L"Ab\r", L"Cd\r", L"\"Command\\r\\nCommand\"\r", L"Last Line\r"};
    EXPECT_EQ(expectedResult, SplitStringByLine(str));
}

/* ---------------------------------------------------------------------------------------------------- */
/*                                      Split String By Upper Case                                      */
/* ---------------------------------------------------------------------------------------------------- */
TEST(StringHelperTest, SplitStringByUpperCase_Normal)
{
    std::wstring str = L"AbCd";
    std::vector<std::wstring> expectedResult = { L"Ab", L"Cd"};
    EXPECT_EQ(expectedResult, SplitStringByUpperCase(str, false, false));
}

TEST(StringHelperTest, SplitStringByUpperCase_Space)
{
    std::wstring str = L"Ab cd";
    std::vector<std::wstring> expectedResult = { L"Ab", L"cd"};
    EXPECT_EQ(expectedResult, SplitStringByUpperCase(str, false, false));
}

TEST(StringHelperTest, SplitStringByUpperCase_AllLowerCase)
{
    std::wstring str = L"abcd";
    std::vector<std::wstring> expectedResult = { str };
    EXPECT_EQ(expectedResult, SplitStringByUpperCase(str, false, false));
}

TEST(StringHelperTest, SplitStringByUpperCase_AllUpperCase)
{
    std::wstring str = L"ABCD";
    std::vector<std::wstring> expectedResult = { L"A", L"B", L"C", L"D" };
    EXPECT_EQ(expectedResult, SplitStringByUpperCase(str, false, false));
}

TEST(StringHelperTest, SplitStringByUpperCase_WithDigit)
{
    std::wstring str = L"A1B1C1D1";
    std::vector<std::wstring> expectedResult = { L"A1", L"B1", L"C1", L"D1" };
    EXPECT_EQ(expectedResult, SplitStringByUpperCase(str, false, false));

    expectedResult = { L"A", L"1", L"B", L"1", L"C", L"1", L"D", L"1" };
    EXPECT_EQ(expectedResult, SplitStringByUpperCase(str, true, false));
}

TEST(StringHelperTest, SplitStringByUpperCase_WithSpecialChar)
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
TEST(StringHelperTest, EscapeString)
{
    // Double Quote
    std::vector<wchar_t> specialChars = GetSpecialCharacters(EscapeStringType::DoubleQuote);
    std::wstring originalStr = L"";
    std::wstring expectedStr = L"";
    for (auto sc : specialChars) {
        originalStr += sc;
        expectedStr += L"\\" + std::wstring(1, sc);
    }
    EXPECT_EQ(GetEscapeString(EscapeStringType::DoubleQuote, originalStr), expectedStr);
    EXPECT_EQ(GetUnescapeString(EscapeStringType::DoubleQuote, expectedStr), originalStr);

    // Regex
    specialChars = GetSpecialCharacters(EscapeStringType::Regex);
    originalStr = L"";
    expectedStr = L"";
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
/*                                      Search                                                          */
/* ---------------------------------------------------------------------------------------------------- */
TEST(StringHelperTest, Find_wchar)
{
    EXPECT_EQ(Find(L"", L'a', 0, false), std::wstring::npos);
    EXPECT_EQ(Find(L"a", L'a', 0, false), 0UL);
    EXPECT_EQ(Find(L"a", L'A', 0, false), std::wstring::npos);
    EXPECT_EQ(Find(L"a", L'A', 0, true), 0UL);
    EXPECT_EQ(Find(L"aA", L'A', 0, false), 1UL);
    EXPECT_EQ(Find(L"aA", L'A', 0, true), 0UL);
    EXPECT_EQ(Find(L"aA", L'A', 1, false), 1UL);
}

TEST(StringHelperTest, Find_wstring)
{
    EXPECT_EQ(Find(L"", L"", 0, false), std::wstring::npos);
    EXPECT_EQ(Find(L"a", L"ab", 0, false), std::wstring::npos);
    EXPECT_EQ(Find(L"a", L"a", 0, false), 0UL);
    EXPECT_EQ(Find(L"a", L"A", 0, false), std::wstring::npos);
    EXPECT_EQ(Find(L"a", L"A", 0, true), 0UL);
    EXPECT_EQ(Find(L"aA", L"A", 0, false), 1UL);
    EXPECT_EQ(Find(L"aA", L"A", 0, true), 0UL);
    EXPECT_EQ(Find(L"aA", L"A", 1, false), 1UL);
    EXPECT_EQ(Find(L"aAb", L"Ab", 1, false), 1UL);
    EXPECT_EQ(Find(L"aAbc", L"Ab", 1, false), 1UL);
    EXPECT_EQ(Find(L"aAbcAb", L"Ab", 1, false), 1UL);
}

TEST(StringHelperTest, GetTailingSubstring)
{
    EXPECT_EQ(GetTailingSubstring(L"", 2), L"");
    EXPECT_EQ(GetTailingSubstring(L"1", 2), L"1");
    EXPECT_EQ(GetTailingSubstring(L"12", 2), L"12");
    EXPECT_EQ(GetTailingSubstring(L"123", 2), L"23");
    EXPECT_EQ(GetTailingSubstring(L"1234", 2), L"34");
}

TEST(StringHelperTest, GetNextString_Basic)
{
    std::wstring str = L"abc def ghi";
    std::wstring str1 = L"\"abc def\" ghi";
    std::wstring str2 = L"\"abc\\\" def\" ghi";
    
    size_t pos = 0;
    EXPECT_EQ(GetNextStringSplitBySpace(str, pos), L"abc");
    EXPECT_EQ(pos, (size_t)2);
    pos = 0;
    EXPECT_EQ(GetNextStringSplitBySpace(str1, pos), L"\"abc");
    EXPECT_EQ(pos, (size_t)3);
    pos = 0;
    EXPECT_EQ(GetNextStringSplitBySpace(str2, pos), L"\"abc\\\"");
    EXPECT_EQ(pos, (size_t)5);

    pos = 0;
    EXPECT_EQ(GetNextStringSplitBySpace(str, pos, {L"\""}, {L"\""}, {L"\\"}), L"abc");
    EXPECT_EQ(pos, (size_t)2);
    pos = 0;
    EXPECT_EQ(GetNextStringSplitBySpace(str1, pos, {L"\""}, {L"\""}, {L"\\"}), L"\"abc def\"");
    EXPECT_EQ(pos, (size_t)8);
    pos = 0;
    EXPECT_EQ(GetNextStringSplitBySpace(str2, pos, {L"\""}, {L"\""}, {L"\\"}), L"\"abc\\\" def\"");
    EXPECT_EQ(pos, (size_t)10);
    
}

TEST(StringHelperTest, GetNextQuotedString_Nested)
{
    std::wstring fullStr = L"[[1, 2], 2] [1,2]";
    size_t pos = 0;
    EXPECT_EQ(GetNextQuotedString(fullStr, pos, { L" " }, {L"["}, {L"]"}, {L""}), L"[[1, 2], 2]");
    EXPECT_EQ(pos, (size_t)10);
    pos++;
    EXPECT_EQ(GetNextQuotedString(fullStr, pos, { L" " }, {L"["}, {L"]"}, {L""}), L"[1,2]");
    EXPECT_EQ(pos, (size_t)16);
}

TEST(StringHelperTest, GetNextQuotedString_Full)
{
    std::wstring fullStr = L"{\"name\":\"John\"}";
    size_t pos = 0;
    EXPECT_EQ(GetNextQuotedString(fullStr, pos, { L":", L"," }, {L"\"", L"{"}, {L"\"", L"}"}, {L"\\", L""}), L"{\"name\":\"John\"}");
    EXPECT_EQ(pos, (size_t)14);
    pos = 1;
    EXPECT_EQ(GetNextQuotedString(fullStr, pos, { L":", L"," }, {L"\"", L"{"}, {L"\"", L"}"}, {L"\\", L""}), L"\"name\"");
    EXPECT_EQ(pos, (size_t)6);
    GetNextCharPos(fullStr, pos);
    EXPECT_EQ(pos, (size_t)7);
    EXPECT_EQ(GetNextQuotedString(fullStr, pos, { L":", L"," }, {L"\"", L"{"}, {L"\"", L"}"}, {L"\\", L""}), L"");
    EXPECT_EQ(pos, (size_t)7);
    pos = 8;
    EXPECT_EQ(GetNextQuotedString(fullStr, pos, { L":", L"," }, {L"\"", L"{"}, {L"\"", L"}"}, {L"\\", L""}), L"\"John\"");
    EXPECT_EQ(pos, (size_t)13);
    GetNextCharPos(fullStr, pos);
    EXPECT_EQ(pos, (size_t)14);
    EXPECT_EQ(GetNextQuotedString(fullStr, pos, { L":", L"," }, {L"\"", L"{"}, {L"\"", L"}"}, {L"\\", L""}), L"}");
    EXPECT_EQ(pos, (size_t)14);
    GetNextCharPos(fullStr, pos);
    EXPECT_EQ(pos, (size_t)15);
    EXPECT_EQ(GetNextQuotedString(fullStr, pos, { L":", L"," }, {L"\"", L"{"}, {L"\"", L"}"}, {L"\\", L""}), L"");
    EXPECT_EQ(pos, (size_t)15);    
}

/* ---------------------------------------------------------------------------------------------------- */
/*                                      Process                                                         */
/* ---------------------------------------------------------------------------------------------------- */

TEST(StringHelperTest, ReplaceRegex)
{
    std::wstring str = L"#define DLL_NAME \"xxx\"#define DLL_NAME \"xxx\"#define DLL_NAME \"xxx\"";
    std::wstring replacement = L"#define DLL_NAME \"yyy\"";
    ReplaceRegex(str, L"#define DLL_NAME \"([^\"]*)\"", replacement);
    EXPECT_EQ(str, L"#define DLL_NAME \"yyy\"#define DLL_NAME \"xxx\"#define DLL_NAME \"xxx\"");

    str = L"xxxxxxxxxxxxxxxxxxxx";
    replacement = L"xxx";
    ReplaceRegex(str, L"xxxx", replacement);
    EXPECT_EQ(str, L"xxxxxxxxxxxxxxxxxxx");
}

TEST(StringHelperTest, ReplaceRegexAll)
{
    std::wstring str = L"#define DLL_NAME \"xxx\"#define DLL_NAME \"xxx\"#define DLL_NAME \"xxx\"";
    std::wstring replacement = L"#define DLL_NAME \"yyy\"";
    ReplaceRegexAll(str, L"#define DLL_NAME \"([^\"]*)\"", replacement);
    EXPECT_EQ(str, L"#define DLL_NAME \"yyy\"#define DLL_NAME \"yyy\"#define DLL_NAME \"yyy\"");

    str = L"xxxxxxxxxxxxxxxxxxxx";
    replacement = L"xxx";
    ReplaceRegexAll(str, L"xxxx", replacement);
    EXPECT_EQ(str, L"xxxxxxxxxxxxxxx");
}
