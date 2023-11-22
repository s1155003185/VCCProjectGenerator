#include "string_helper.hpp"

#include <algorithm>
#include <map>
#include <math.h>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "exception.hpp"
#include "exception_macro.hpp"
#include "map_helper.hpp"

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

	void ToLower(std::wstring &str)
	{
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	}

	void ToUpper(std::wstring &str)
	{
		std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	}
	
	bool HasPrefix(const std::wstring &str, const std::wstring &prefix, const size_t &pos)
	{
		return str.find(prefix, pos) == pos;
	}

	bool HasPrefixTrimSpace(const std::wstring &str, const std::wstring &prefix, const size_t &pos)
	{
		std::wstring trimPrefix = std::wstring(prefix);
		Trim(trimPrefix);
		if (trimPrefix != prefix)
			THROW_EXCEPTION_M(ExceptionType::CUSTOM_ERROR, L"Prefix contains space.");

		if (prefix.empty())
			return true;

		bool result = true;
		try
		{
			size_t prefixPos = 0;
			size_t currentPos = pos;
			while (currentPos < str.length()) {
				if (!std::iswspace(str[currentPos])) {
					if (str[currentPos] != prefix[prefixPos]) {
						result = false;
						break;
					}
					prefixPos++;
				}
				if (prefixPos >= prefix.length())
					break;
				currentPos++;
			}
		}
		catch(...)
		{
			result = false;
		}
		return result;
	}

	std::vector<std::wstring> SplitString(std::wstring str, const std::wstring &delimiter, 
		const std::vector<std::wstring> &commandOpenList, const std::vector<std::wstring> &commandCloseList, const std::vector<std::wstring> &commandEscapeList)
	{
		std::vector<std::wstring> results;
		if (str.empty())
			return results;

		try
		{
			if (!(commandOpenList.size() == commandCloseList.size() && (commandEscapeList.empty() || commandCloseList.size() == commandEscapeList.size())))
				THROW_EXCEPTION_M(ExceptionType::CUSTOM_ERROR, L"Command Open, Close, Escape List having different size.");

			size_t pos = 0;
			std::wstring currentStr = L"";
			int64_t commandIndex = -1;
			while (pos < str.length()) {
				if (commandIndex >= 0) {
					if (HasPrefix(str, commandCloseList[commandIndex], pos)) {
						currentStr += commandCloseList[commandIndex];
						pos += commandCloseList[commandIndex].length();
						commandIndex = -1;
					} else if (!commandEscapeList.empty() && !commandEscapeList[commandIndex].empty() && HasPrefix(str, commandEscapeList[commandIndex], pos)) {
						currentStr += commandEscapeList[commandIndex];
						pos += commandEscapeList[commandIndex].length();
						for (size_t i = 0; i < commandCloseList[commandIndex].size(); i++) {
							if (pos < str.length()) {
								currentStr += str[pos];
								pos++;
							}
						}
					} else {
						currentStr += std::wstring(1, str[pos]);
						pos++;
					}
				} else {
					if (HasPrefix(str, delimiter, pos)) {
						results.push_back(currentStr);
						currentStr = L"";
						pos += delimiter.length();
					} else {
						for (size_t i = 0; i < commandOpenList.size(); i++) {
							if (HasPrefix(str, commandOpenList[i], pos)) {
								commandIndex = i;
								break;
							}
						}
						currentStr += std::wstring(1, str[pos]);
						pos++;
					}
				}
			}
			if (!currentStr.empty() || !results.empty())
				results.push_back(currentStr);

        }
        catch(const std::exception& e)
        {
            THROW_EXCEPTION(e);
        }
		return results;
	}

	std::vector<std::wstring> SplitStringByLine(std::wstring str)
	{
		std::vector<std::wstring> results;
		if (str.empty())
			return results;
		
		std::wistringstream iss(str);
		std::wstring line;
		while (std::getline(iss, line)) {
			results.push_back(line);
		}
		return results;
	}

	std::vector<std::wstring> SplitStringByUpperCase(const std::wstring &str, bool splitDigit, bool splitSpecialChar)
	{
		std::vector<std::wstring> results;
		std::wstring currentToken;
		for (wchar_t c : str) {
			bool pushCurrentToken = std::iswupper(c)
				|| std::iswspace(c)
				|| (splitSpecialChar && !std::isalnum(c))
				|| (splitDigit && std::iswdigit(c));

			bool addCharToCurrentToken = !std::iswspace(c) && (std::iswalnum(c) || !splitSpecialChar);

			if (pushCurrentToken) {
				Trim(currentToken);
				if (!currentToken.empty())
					results.push_back(currentToken);
				currentToken = L"";
			}
			if (addCharToCurrentToken)
				currentToken += std::wstring(1, c);
		}
		if (!currentToken.empty())
			results.push_back(currentToken);
		return results;
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

	std::wstring PadLeft(const std::wstring str, size_t length, wchar_t c)
	{
		if (length <= str.length())
			return str;
		return std::wstring(length - std::min(length, str.length()), c) + str;
	}

	std::wstring PadRight(const std::wstring str, size_t length, wchar_t c)
	{
		if (length <= str.length())
			return str;
		return str + std::wstring(length - std::min(length, str.length()), c);
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

	void GetNextCharPos(const std::wstring &str, size_t &pos, bool fromCurrentPos)
	{
		if (!fromCurrentPos)
			pos++;
		while (pos < str.length()) {
			if (!std::iswspace(str[pos])) {
				return;
			}
			pos++;
		}
	}
	
	size_t CountSubstr(const std::wstring &str, const std::wstring subStr)
	{
		size_t count = 0;
		size_t pos = 0;

		while (pos < str.length())
		{
			pos = str.find(subStr, pos);
			if (pos == std::wstring::npos)
				break;
			count++;
			pos += subStr.length();
		}
		return count;
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
        catch(const std::exception& e)
        {
            THROW_EXCEPTION(e);
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
        catch(const std::exception& e)
        {
            THROW_EXCEPTION(e);
        }
        return result;
    }

	std::wstring GetUnescapeString(const EscapeStringType &type, const std::wstring &str)
	{
        std::wstring result = L"";
        try 
        {
			std::map<wchar_t, std::wstring> escapeMap;
			switch (type)
			{
			case EscapeStringType::XML:
				escapeMap = GetEscapeStringMap(type);
				break;
			default:
				break;
			}

			for (size_t i = 0; i < str.length(); i++) {
				switch (type)
				{
					case EscapeStringType::Regex:
						if (str[i] == L'\\')
							i++;
						result += str[i];
						break;
					case EscapeStringType::XML:
					{
						bool isEscape = false;
						for (auto &pair : escapeMap) {
							if (str.substr(i).starts_with(pair.second)) {
								result += pair.first;
								isEscape = true;
								i += pair.second.length() - 1;
								break;
							}
						}
						if (!isEscape)
							result += str[i];
						break;
					}
				default:
					break;
				}
			}
        }
        catch(const std::exception& e)
        {
            THROW_EXCEPTION(e);
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