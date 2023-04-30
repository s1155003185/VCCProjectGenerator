#pragma once
#include <math.h>
#include <memory>
#include <string>

namespace vcc
{
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

	inline bool HasPrefix(const std::string str, const std::string prefix)
	{
		return str.substr(0, prefix.length()).compare(prefix) == 0;
	}

	inline bool HasPrefix(const std::wstring str, const std::wstring prefix)
	{
		return str.substr(0, prefix.length()).compare(prefix) == 0;
	}

	inline bool HasSuffix(const std::string str, const std::string suffix)
	{
		return str.substr(str.length() - suffix.length(), str.length() - 1).compare(suffix) == 0;
	}

	inline bool HasSuffix(const std::wstring str, const std::wstring suffix)
	{
		return str.substr(str.length() - suffix.length(), str.length() - 1).compare(suffix) == 0;
	}
}
