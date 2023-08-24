#pragma once
#include <string>
#include <regex>

namespace vcc
{
    constexpr auto NL = L"\n";

	enum class EscapeStringType
	{
		Regex
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

	void Replace(std::wstring &str, const std::wstring& from, const std::wstring &to);
	void ReplaceAll(std::wstring &str, const std::wstring& from, const std::wstring &to);

	// regex
	std::vector<wchar_t> GetSpecialCharacters(const EscapeStringType &type);
	wchar_t GetEscapeCharacter(const EscapeStringType &type);
	std::wstring GetEscapeString(const EscapeStringType &type, const std::wstring &str);
	std::wstring GetRegexFromFileFilter(const std::wstring &fileFilter);

}
