#pragma once
#include <math.h>
#include <memory>
#include <string>

namespace vcc
{
    constexpr auto NL = L"\n";

	inline std::wstring str2wstr(const std::string& str)
	{
		if (str.empty())
			return L"";
		int len = str.size() + 1;
		std::unique_ptr<wchar_t[]> p(new wchar_t[len]);
		mbstowcs(p.get(), str.c_str(), len);
		std::wstring wstr(p.get());
		return wstr;
	}

	inline std::string wstr2str(const std::wstring &wstr)
	{
		if (wstr.empty())
			return "";
		int len = wstr.size() * 4 + 1;
		std::unique_ptr<char[]> p(new char[len]);
		wcstombs(p.get(), wstr.c_str(), len);
		std::string str(p.get());
		return str;
	}

	inline std::string PadLeft(const std::string str, size_t length, char c)
	{
		if (length <= str.length())
			return str;
		return std::string(length - std::min(length, str.length()), c) + str;
	}

	inline std::string PadRight(const std::string str, size_t length, char c)
	{
		if (length <= str.length())
			return str;
		return str + std::string(length - std::min(length, str.length()), c);
	}

	inline void ltrim(std::string &str)
	{
		str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](char c) {
			return !std::isspace(c);
		}));
	}

	inline void rtrim(std::string &str)
	{
		str.erase(std::find_if(str.rbegin(), str.rend(), [](char c) {
			return !std::isspace(c);
		}).base(), str.end());
	}

	inline void trim(std::string &str)
	{
		ltrim(str);
		rtrim(str);
	}

	inline void ltrim(std::wstring &str)
	{
		str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](wchar_t c) {
			return !std::iswspace(c);
		}));
	}

	inline void rtrim(std::wstring &str)
	{
		str.erase(std::find_if(str.rbegin(), str.rend(), [](wchar_t c) {
			return !std::iswspace(c);
		}).base(), str.end());
	}

	inline void trim(std::wstring &str)
	{
		ltrim(str);
		rtrim(str);
	}
}
