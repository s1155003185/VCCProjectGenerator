#include "string_helper.hpp"

#include <algorithm>
#include <map>
#include <math.h>
#include <memory>
#include <string>
#include <vector>

#include "exception.hpp"
#include "exception_macro.hpp"

namespace vcc
{
	std::wstring str2wstr(const std::string& str)
	{
		if (str.empty())
			return L"";
		int len = str.size() + 1;
		std::unique_ptr<wchar_t[]> p(new wchar_t[len]);
		mbstowcs(p.get(), str.c_str(), len);
		std::wstring wstr(p.get());
		return wstr;
	}

	std::string wstr2str(const std::wstring &wstr)
	{
		if (wstr.empty())
			return "";
		int len = wstr.size() * 4 + 1;
		std::unique_ptr<char[]> p(new char[len]);
		wcstombs(p.get(), wstr.c_str(), len);
		std::string str(p.get());
		return str;
	}

	std::string PadLeft(const std::string str, size_t length, char c)
	{
		if (length <= str.length())
			return str;
		return std::string(length - std::min(length, str.length()), c) + str;
	}

	std::string PadRight(const std::string str, size_t length, char c)
	{
		if (length <= str.length())
			return str;
		return str + std::string(length - std::min(length, str.length()), c);
	}

	void LTrim(std::string &str)
	{
		str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](char c) {
			return !std::isspace(c);
		}));
	}

	void RTrim(std::string &str)
	{
		str.erase(std::find_if(str.rbegin(), str.rend(), [](char c) {
			return !std::isspace(c);
		}).base(), str.end());
	}

	void Trim(std::string &str)
	{
		LTrim(str);
		RTrim(str);
	}

	void LTrim(std::wstring &str)
	{
		str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](wchar_t c) {
			return !std::iswspace(c);
		}));
	}

	void RTrim(std::wstring &str)
	{
		str.erase(std::find_if(str.rbegin(), str.rend(), [](wchar_t c) {
			return !std::iswspace(c);
		}).base(), str.end());
	}

	void Trim(std::wstring &str)
	{
		LTrim(str);
		RTrim(str);
	}

    void Replace(std::wstring &str, const std::wstring& from, const std::wstring &to)
    {
        str.replace(str.find(from), from.length(), to);
    }

	void ReplaceAll(std::wstring &str, const std::wstring& from, const std::wstring &to)
    {
        size_t startPos = 0;
        size_t foundPos;
        while ((foundPos = str.find(from, startPos)) != std::wstring::npos) {
            str.replace(foundPos, from.length(), to);
            startPos = foundPos + to.length();
        }
    }

    std::vector<wchar_t> GetSpecialCharacters(const EscapeStringType &type)
    {
        switch (type)
        {
            case EscapeStringType::Regex:
                return { L'\\', L'^', L'$', L'.', L'|', L'?', L'*', L'+', L'(', L')', L'[', L']', L'{', L'}' };
			case EscapeStringType::XML:
				return { L'<', L'>', L'"', L'`', L'&' };
            default:
                assert(false);
        }
        return {};
    }

	std::map<wchar_t, std::wstring> GetEscapeStringMap(const EscapeStringType &type)
	{
		std::map<wchar_t, std::wstring> result;
		switch (type)
        {
            case EscapeStringType::XML:
				return {
					{L'<', L"&lt;"},
					{L'>', L"&gt;"},
					{L'"', L"&quot;"},
					{L'`', L"&apos;"},
					{L'&', L"&amp;"}
				};
                break;
            default:
                assert(false);
        }
		return {};
	}

	std::wstring ConvertSpecialCharacterToEscapeString(const EscapeStringType &type, const wchar_t &c)
	{
        try 
        {
			vector<wchar_t> specialChars = GetSpecialCharacters(type);
			if (!std::count(specialChars.begin(), specialChars.end(), c))
				return std::wstring(1, c);
	
			switch (type)
			{
				case EscapeStringType::Regex:
					return L"\\" + std::wstring(1, c);
				case EscapeStringType::XML:
					return GetEscapeStringMap(type)[c];
				default:
					assert(false);
			}
        }
        catch(std::exception& ex)
        {
            THROW_EXCEPTION(ex);
        }
		return std::wstring(1, c);
	}


	std::wstring GetEscapeString(const EscapeStringType &type, const std::wstring &str)
    {
        std::wstring result = L"";
        try 
        {
			for (auto c : str)
				result += ConvertSpecialCharacterToEscapeString(type, c);
        }
        catch(std::exception& ex)
        {
            THROW_EXCEPTION(ex);
        }
        return result;
    }


    std::wstring GetRegexFromFileFilter(const std::wstring &fileFilter)
    {
        std::wstring patten = GetEscapeString(EscapeStringType::Regex, fileFilter);

        // Excape *
        ReplaceAll(patten, L"\\*", L".*");

        // Excape ?
        ReplaceAll(patten, L"\\?", L".");
        return patten;
    }
}