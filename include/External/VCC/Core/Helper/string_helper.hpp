#pragma once
#include <map>
#include <string>
#include <regex>

namespace vcc
{
    constexpr auto NL = L"\n";

	enum class EscapeStringType
	{
		Regex,
		XML
	};

	std::wstring str2wstr(const std::string& str);
	std::string wstr2str(const std::wstring &wstr);

	std::string PadLeft(const std::string str, size_t length, char c);
	std::string PadRight(const std::string str, size_t length, char c);

	void LTrim(std::string &str);
	void RTrim(std::string &str);
	void Trim(std::string &str);

	void LTrim(std::wstring &str);
	void RTrim(std::wstring &str);
	void Trim(std::wstring &str);

	// search
	void GetNextCharPos(const std::wstring &str, size_t &pos, bool fromCurrentPos);
	//std::wstring GetNextToken(const std::wstring &str, size_t &pos, bool fromCurrentPos = true);

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
