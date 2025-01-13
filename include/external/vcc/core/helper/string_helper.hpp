#pragma once
#include <map>
#include <string>
#include <regex>

namespace vcc
{
    const auto NL = L"\r\n";
	
	const std::wstring INDENT = L"    ";

	enum class EscapeStringType
	{
		DoubleQuote,
		Regex,
		SingleQuote,
		XML
	};

	enum class NamingStyle
	{
		CamelCase, // camelCase
		ConstantCase, // CONSTANT_CASE
		DotSeparatedLowercase, // dot.seperated.lowercase
		KebabCase, // kebab-case
		Lowercase, // lowercase
		PascalCase, // PascalCase
		ScreamingSnakeCase, // SCREAMING_SNAKE_CASE
		SnakeCase, // snake_case
		Uppercase // UPPERCASE
	};

	std::wstring str2wstr(const std::string& str);
	std::string wstr2str(const std::wstring &wstr);

	void ToCamel(std::wstring &str); // camelCase
	void ToCapital(std::wstring &str); // Capitial Case
	void ToLower(std::wstring &str);
	void ToPascal(std::wstring &str); // PascalCase
	void ToUpper(std::wstring &str);

	bool IsLower(const std::wstring &str);
	bool IsUpper(const std::wstring &str);
	bool IsCapital(const std::wstring &str);

	std::wstring ToString(double value, size_t decimalPlaces);

	bool IsEmpty(const std::wstring &str);
	bool IsBlank(const std::wstring &str);

	bool IsStartWith(const std::wstring &str, const std::wstring &prefix, const size_t &pos = 0);
	bool IsStartWith(const std::wstring &str, const std::vector<std::wstring> &prefixes, const size_t &pos = 0);
	bool IsStartWithCaseInsensitive(const std::wstring &str, const std::wstring &prefix, const size_t &pos = 0);
	bool IsStartWithCaseInsensitive(const std::wstring &str, const std::vector<std::wstring> &prefixes, const size_t &pos = 0);
	bool IsStartWithTrimSpace(const std::wstring &str, const std::wstring &prefix, const size_t &pos = 0);
	bool IsEndWith(const std::wstring &str, const std::wstring &suffix);

	std::vector<std::wstring> SplitString(std::wstring str, const std::vector<std::wstring> &delimiters,
		const std::vector<std::wstring> &quoteOpenList = {}, const std::vector<std::wstring> &quoteCloseList = {}, const std::vector<std::wstring> &quoteEscapeList = {});
	std::vector<std::wstring> SplitStringByLine(const std::wstring &str);
	std::vector<std::wstring> SplitStringByUpperCase(const std::wstring &str, bool splitDigit, bool splitSpecialChar);

	// Padding
	std::string PadLeft(const std::string str, size_t length, char c);
	std::string PadRight(const std::string str, size_t length, char c);

	std::wstring PadLeft(const std::wstring str, size_t length, wchar_t c);
	std::wstring PadRight(const std::wstring str, size_t length, wchar_t c);

	// Trim
	void LTrim(std::string &str);
	void RTrim(std::string &str);
	void Trim(std::string &str);

	void LTrim(std::wstring &str);
	void RTrim(std::wstring &str);
	void Trim(std::wstring &str);

	// Conversion
	std::wstring ConvertNamingStyle(const std::wstring &str, const NamingStyle &from, const NamingStyle &to, const std::wstring& seperator = L" ");

	// search
	bool Equal(const std::wstring &str1, const std::wstring &str2, const bool &isIgnoreCase = false);
	size_t Find(const std::wstring &str, const wchar_t &c, const size_t &pos = 0, const bool &isIgnoreCase = false);
	size_t Find(const std::wstring &str, const std::wstring &subStr, const size_t &pos = 0, const bool &isIgnoreCase = false);
	void GetCharacterRowAndColumn(const std::wstring &str, size_t pos, size_t &row, size_t &column);
	void GetNextCharPos(const std::wstring &str, size_t &pos, bool fromCurrentPos = false);
	std::wstring GetTailingSubstring(const std::wstring &str, const size_t &length);
	// pos Point to end char after execution
	std::wstring GetNextStringSplitBySpace(const std::wstring &str, size_t &pos,
		const std::vector<std::wstring> &quoteOpenList = {}, const std::vector<std::wstring> &quoteCloseList = {}, const std::vector<std::wstring> &quoteEscapeList = {});
	std::wstring GetNextString(const std::wstring &str, size_t &pos, const std::vector<std::wstring> &delimiters,
		const std::vector<std::wstring> &quoteOpenList = {}, const std::vector<std::wstring> &quoteCloseList = {}, const std::vector<std::wstring> &quoteEscapeList = {});
	std::wstring GetNextQuotedString(const std::wstring& str, size_t &pos, const std::vector<std::wstring> &delimiters,
		const std::vector<std::wstring> &quoteOpenList = { L"\"", L"'", L"{", L"[", L"(" },
		const std::vector<std::wstring> &quoteCloseList = { L"\"", L"'", L"}", L"]", L")" },
		const std::vector<std::wstring> &quoteEscapeList = { L"\\", L"\\", L"", L"", L"" });
	bool IsContain(const std::wstring &str, const std::wstring &subStr, const size_t &pos = 0, const bool &isIgnoreCase = false);
	size_t CountSubstring(const std::wstring &str, const std::wstring &subStr, const bool &isIgnoreCase = false);

	// process
	void Replace(std::wstring &str, const std::wstring& from, const std::wstring &to);
	void ReplaceRegex(std::wstring &str, const std::wstring &regex, const std::wstring &replacement);
	void ReplaceAll(std::wstring &str, const std::wstring& from, const std::wstring &to);
	void ReplaceRegexAll(std::wstring &str, const std::wstring &regex, const std::wstring &replacement);

	// regex
	std::vector<wchar_t> GetSpecialCharacters(const EscapeStringType &type);
	std::map<wchar_t, std::wstring> GetEscapeStringMap(const EscapeStringType &type);
	std::wstring ConvertSpecialCharacterToEscapeString(const EscapeStringType &type, const wchar_t &c);
	std::wstring GetEscapeString(const EscapeStringType &type, const std::wstring &str);
	std::wstring GetEscapeStringWithQuote(const EscapeStringType &type, const std::wstring &str);
	std::wstring GetUnescapeString(const EscapeStringType &type, const std::wstring &str);
	std::wstring GetUnescapeStringWithQuote(const EscapeStringType &type, const std::wstring &str);

}
