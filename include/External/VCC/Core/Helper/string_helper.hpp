#pragma once
#include <string>

namespace vcc
{
    constexpr auto NL = L"\n";

	std::wstring str2wstr(const std::string& str);

	std::string wstr2str(const std::wstring &wstr);

	std::string PadLeft(const std::string str, size_t length, char c);

	std::string PadRight(const std::string str, size_t length, char c);

	void ltrim(std::string &str);
	void rtrim(std::string &str);
	void trim(std::string &str);

	void ltrim(std::wstring &str);
	void rtrim(std::wstring &str);
	void trim(std::wstring &str);
}
