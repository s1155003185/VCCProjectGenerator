#pragma once
#include <map>
#include <string>
#include <regex>

namespace vcc
{
    const auto NL = L"\n";
	const std::wstring INDENT = L"    ";

	enum class EscapeStringType
	{
		Regex,
		XML
	};

	std::wstring str2wstr(const std::string& str);
	std::string wstr2str(const std::wstring &wstr);

	void ToLower(std::wstring &str);
	void ToUpper(std::wstring &str);

	bool HasPrefix(const std::wstring &str, const std::wstring &prefix, const size_t &pos = 0);

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

	// search
	void GetNextCharPos(const std::wstring &str, size_t &pos, bool fromCurrentPos = false);
	size_t CountSubstr(const std::wstring &str, const std::wstring subStr);

	// process
	void Replace(std::wstring &str, const std::wstring& from, const std::wstring &to);
	void ReplaceAll(std::wstring &str, const std::wstring& from, const std::wstring &to);

	// regex
	std::vector<wchar_t> GetSpecialCharacters(const EscapeStringType &type);
	std::map<wchar_t, std::wstring> GetEscapeStringMap(const EscapeStringType &type);
	std::wstring ConvertSpecialCharacterToEscapeString(const EscapeStringType &type, const wchar_t &c);
	std::wstring GetEscapeString(const EscapeStringType &type, const std::wstring &str);
	std::wstring GetUnescapeString(const EscapeStringType &type, const std::wstring &str);
	std::wstring GetRegexFromFileFilter(const std::wstring &fileFilter);

}
