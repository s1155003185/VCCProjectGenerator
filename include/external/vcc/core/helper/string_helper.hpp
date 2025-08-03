#pragma once
#include <map>
#include <string>
#include <regex>

const std::wstring NL = L"\r\n";
const std::wstring INDENT = L"    ";

namespace vcc
{
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

	void toCamel(std::wstring &str); // camelCase
	void toCapital(std::wstring &str); // Capitial Case
	void toLower(std::wstring &str);
	void toPascal(std::wstring &str); // PascalCase
	void toUpper(std::wstring &str);

	bool isLower(const std::wstring &str);
	bool isUpper(const std::wstring &str);
	bool isCapital(const std::wstring &str);

	bool isEmpty(const std::wstring &str);
	bool isBlank(const std::wstring &str);

	bool isEqual(const std::wstring &str1, const std::wstring &str2, const bool &isIgnoreCase = false);
	bool isStartWith(const std::wstring &str, const std::wstring &prefix, const size_t &startFromPos = 0, bool isIgnoreCase = false);
	bool isStartWith(const std::wstring &str, const std::vector<std::wstring> &prefixes, const size_t &startFromPos = 0, bool isIgnoreCase = false);
	bool isStartWithTrimSpace(const std::wstring &str, const std::wstring &prefix, const size_t &startFromPos = 0);
	bool isEndWith(const std::wstring &str, const std::wstring &suffix);

	std::vector<std::wstring> splitString(const std::wstring &str, const std::vector<std::wstring> &delimiters,
		const std::vector<std::wstring> &quoteOpenList = {}, const std::vector<std::wstring> &quoteCloseList = {}, const std::vector<std::wstring> &quoteEscapeList = {});
	std::vector<std::wstring> splitStringBySpace(const std::wstring &str,
		const std::vector<std::wstring> &quoteOpenList = {}, const std::vector<std::wstring> &quoteCloseList = {}, const std::vector<std::wstring> &quoteEscapeList = {});
	std::vector<std::wstring> splitStringByLine(const std::wstring &str);
	std::vector<std::wstring> splitStringByUpperCase(const std::wstring &str, bool splitDigit, bool splitSpecialChar);

	// Padding
	std::string padLeft(const std::string &str, const size_t &length, const char &c);
	std::string padRight(const std::string &str, const size_t &length, const char &c);

	std::wstring padLeft(const std::wstring &str, const size_t &length, const wchar_t &c);
	std::wstring padRight(const std::wstring &str, const size_t &length, const wchar_t &c);

	// Trim
	void lTrim(std::string &str);
	void rTrim(std::string &str);
	void trim(std::string &str);

	void lTrim(std::wstring &str);
	void rTrim(std::wstring &str);
	void trim(std::wstring &str);

	// Conversion
	std::wstring convertNamingStyle(const std::wstring &str, const NamingStyle &from, const NamingStyle &to, const std::wstring& seperator = L" ");

	// search
	size_t find(const std::wstring &str, const wchar_t &c, const size_t &pos = 0, const bool &isIgnoreCase = false);
	size_t find(const std::wstring &str, const std::wstring &subStr, const size_t &pos = 0, const bool &isIgnoreCase = false);
	void getCharacterRowAndColumn(const std::wstring &str, size_t pos, size_t &row, size_t &column);
	void getNextCharPos(const std::wstring &str, size_t &pos, bool fromCurrentPos = false);
	std::wstring getTailingSubstring(const std::wstring &str, const size_t &length);
	// pos Point to end char after execution
	std::wstring getNextStringSplitBySpace(const std::wstring &str, size_t &pos,
		const std::vector<std::wstring> &quoteOpenList = {}, const std::vector<std::wstring> &quoteCloseList = {}, const std::vector<std::wstring> &quoteEscapeList = {});
	std::wstring getNextString(const std::wstring &str, size_t &pos, const std::vector<std::wstring> &delimiters,
		const std::vector<std::wstring> &quoteOpenList = {}, const std::vector<std::wstring> &quoteCloseList = {}, const std::vector<std::wstring> &quoteEscapeList = {});
	std::wstring getNextQuotedString(const std::wstring& str, size_t &pos, const std::vector<std::wstring> &delimiters,
		const std::vector<std::wstring> &quoteOpenList = { L"\"", L"'", L"{", L"[", L"(" },
		const std::vector<std::wstring> &quoteCloseList = { L"\"", L"'", L"}", L"]", L")" },
		const std::vector<std::wstring> &quoteEscapeList = { L"\\", L"\\", L"", L"", L"" },
		const std::vector<std::wstring> &stringOpenList = {});
	bool isContain(const std::wstring &str, const std::wstring &subStr, const size_t &pos = 0, const bool &isIgnoreCase = false);
	size_t countSubstring(const std::wstring &str, const std::wstring &subStr, const bool &isIgnoreCase = false);

	// process
	void replace(std::wstring &str, const std::wstring& from, const std::wstring &to);
	void replaceRegex(std::wstring &str, const std::wstring &regex, const std::wstring &replacement);
	void replaceAll(std::wstring &str, const std::wstring& from, const std::wstring &to);
	void replaceRegexAll(std::wstring &str, const std::wstring &regex, const std::wstring &replacement);

	// regex
	std::vector<wchar_t> getSpecialCharacters(const EscapeStringType &type);
	std::map<wchar_t, std::wstring> getEscapeStringMap(const EscapeStringType &type);
	std::wstring convertSpecialCharacterToEscapeString(const EscapeStringType &type, const wchar_t &c);
	std::wstring getEscapeString(const EscapeStringType &type, const std::wstring &str);
	std::wstring getEscapeStringWithQuote(const EscapeStringType &type, const std::wstring &str);
	std::wstring getUnescapeString(const EscapeStringType &type, const std::wstring &str);
	std::wstring getUnescapeStringWithQuote(const EscapeStringType &type, const std::wstring &str);

}
