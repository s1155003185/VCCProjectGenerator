#include "string_helper.hpp"

#include <algorithm>
#include <map>
#include <math.h>
#include <memory>
#include <iostream>
#include <iomanip>
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
		std::wstringstream wss;
		wss << str.c_str();
		return wss.str();
	}

	std::string wstr2str(const std::wstring &wstr)
	{
		if (wstr.empty())
			return "";
		std::string str(wstr.begin(), wstr.end());
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

	std::wstring ToString(double value, size_t decimalPlaces)
	{
		TRY
			std::wstringstream ss;
			ss << std::fixed << std::setprecision(decimalPlaces) << value;
			return ss.str();
		CATCH
		return L"";
	}

	bool IsEmpty(const std::wstring &str)
	{
		return str.empty();
	}

	bool IsBlank(const std::wstring &str)
	{
		for (const wchar_t &c : str) {
			if (!std::iswspace(c))
				return false;
		}
		return true;
	}
	
	bool HasPrefix(const std::wstring &str, const std::wstring &prefix, const size_t &pos)
	{
		TRY
		 	if (str.length() - pos < prefix.size())
		 		return false;

		 	size_t cnt = 0;
		 	for (auto const &c : prefix) {
				if (str[pos + cnt] != c)
					return false;
				cnt++;
			}
			return true;
		CATCH
		return false;
	}
	
	bool HasPrefix(const std::wstring &str, const std::vector<std::wstring> &prefixes, const size_t &pos)
	{
		bool result = false;
		for (auto const &prefix : prefixes) {
			if (HasPrefix(str, prefix, pos)) {
				result = true;
				break;
			}
		}
		return result;
	}

	bool HasPrefixTrimSpace(const std::wstring &str, const std::wstring &prefix, const size_t &pos)
	{
		std::wstring trimPrefix = std::wstring(prefix);
		Trim(trimPrefix);
		if (trimPrefix != prefix)
			THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Prefix contains space.");

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

	bool HasSuffix(const std::wstring &str, const std::wstring &suffix)
	{
		return str.ends_with(suffix);
	}

	std::vector<std::wstring> SplitString(std::wstring str, const std::vector<std::wstring> &delimiters,
		const std::vector<std::wstring> &quoteOpenList, const std::vector<std::wstring> &quoteCloseList, const std::vector<std::wstring> &quoteEscapeList)
	{
		std::vector<std::wstring> results;
		if (str.empty())
			return results;

		try
		{
			if (!(quoteOpenList.size() == quoteCloseList.size() && (quoteEscapeList.empty() || quoteCloseList.size() == quoteEscapeList.size())))
				THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Command Open, Close, Escape List having different size.");

			std::vector<size_t> quotes;
			size_t pos = 0;
			std::wstring currentStr = L"";
			while (pos < str.length()) {
				if (quotes.empty() && HasPrefix(str, delimiters, pos)) {
					results.push_back(currentStr);
					currentStr = L"";

					std::wstring currentDelimiter = L"";
					for (auto const &delimiter : delimiters) {
						if (HasPrefix(str, delimiter, pos)) {
							currentDelimiter = delimiter;
							break;
						}
					}
					pos += currentDelimiter.length();
				} else {
					// 1. if have quotes, check if it is escape chars
					// 2. if have quotes, check if it is close quote
					// 3. check if it is open quotes, if yes, then add to quote
					// Last. None of above, then pos++
					if (!quotes.empty()) {
						if (!quoteEscapeList.empty()) {
							std::wstring escapeChar = quoteEscapeList[quotes[quotes.size() - 1]];
							if (!escapeChar.empty() && HasPrefix(str, escapeChar, pos)) {
								pos += escapeChar.length();
								currentStr += escapeChar + str[pos];
								pos++; // for escaped char
								continue;
							}
						}
						std::wstring closeQuote = quoteCloseList[quotes[quotes.size() - 1]];
						if (!closeQuote.empty() && HasPrefix(str, closeQuote, pos)) {
							pos += closeQuote.length();
							currentStr += closeQuote;
							quotes.pop_back();
							continue;
						}
					}
					std::wstring currentQuoteOpen = L"";
					for (size_t i = 0; i < quoteOpenList.size(); i++) {
						std::wstring quoteOpen = quoteOpenList[i];
						if (HasPrefix(str, quoteOpen, pos)) {
							currentQuoteOpen = quoteOpen;
							quotes.push_back(i);
							break;
						}
					}
					if (!currentQuoteOpen.empty()) {
						pos += currentQuoteOpen.length();
						currentStr += currentQuoteOpen;
					} else {
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

	std::vector<std::wstring> SplitStringByLine(const std::wstring &str)
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

	size_t Find(const std::wstring &str, const wchar_t &c, const size_t &pos, const bool &isIgnoreCase)
	{
		if (str.empty())
			return std::wstring::npos;
		TRY
			size_t cnt = pos;
			for (auto it = str.begin() + pos; it != str.end(); it++) {
				if (isIgnoreCase) {
					if (std::towupper(*it) == std::towupper(c))
						return cnt;
				} else {
					if (*it == c)
						return cnt;
				}
				cnt++;
			}
		CATCH
		return std::wstring::npos;
	}

	size_t Find(const std::wstring &str, const std::wstring &subStr, const size_t &pos, const bool &isIgnoreCase)
	{
		if (str.empty() || str.length() - pos < subStr.length())
			return std::wstring::npos;
		TRY
			if (subStr.length() == 1)
				return Find(str, subStr[0], pos, isIgnoreCase);

			if (isIgnoreCase) {
				std::wstring tmpStr = str.substr(pos);
				std::wstring tmpSubStr = subStr;
				ToUpper(tmpStr);
				ToUpper(tmpSubStr);
				return Find(tmpStr, tmpSubStr);
			} else {
				// cannot check at beginning as there is ignoreCase
				if (str.length() == subStr.length() && str != subStr)
					return std::wstring::npos;
				
				// for (size_t i = pos; i < str.length(); i++) {
				// 	if (HasPrefix(str, subStr, i))
				// 		return i;
				// }
				return str.find(subStr, pos);
			}
		CATCH
		return std::wstring::npos;
	}

	void GetCharacterRowAndColumn(const std::wstring &str, size_t pos, size_t &row, size_t &column)
	{
		TRY
			row = 1;
			column = 1;
			size_t cnt =0;
			while (cnt <= pos)
			{
				if (str[cnt] == L'\n') {
					row++;
					column = 1;
				} else
					column++;
				cnt++;
			}
		CATCH
	}

	void GetNextCharacterPos(const std::wstring &str, size_t &pos, bool fromCurrentPos)
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

	std::wstring GetNextStringSplitBySpace(const std::wstring &str, size_t &pos,
		const std::vector<std::wstring> &quoteOpenList, const std::vector<std::wstring> &quoteCloseList, const std::vector<std::wstring> &quoteEscapeList)
	{
		// \r\n must before \n and \r
		return GetNextString(str, pos, {L" ", L"\r\n", L"\n", L"\r", L"\t"}, quoteOpenList, quoteCloseList, quoteEscapeList);
	}
	
	std::wstring GetNextString(const std::wstring &str, size_t &pos, const std::vector<std::wstring> &delimiters,
		const std::vector<std::wstring> &quoteOpenList, const std::vector<std::wstring> &quoteCloseList, const std::vector<std::wstring> &quoteEscapeList)
	{
		if (str.empty())
			return str;
		if (pos >= str.length())
			return L"";
		if (delimiters.empty()) {
			std::wstring result = str.substr(pos);
			pos = str.length() - 1;
			return result;
		}
		GetNextCharacterPos(str, pos, true);
		std::wstring result = L"";
		try
		{
			if (!(quoteOpenList.size() == quoteCloseList.size() && (quoteEscapeList.empty() || quoteCloseList.size() == quoteEscapeList.size())))
				THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Quote Open, Close, Escape List having different size.");

			size_t startPos = pos;
			std::vector<size_t> quotes;
			while (pos < str.length()) {
				if (quotes.empty() && HasPrefix(str, delimiters, pos)) {
					break;
				} else {
					// 1. if have quotes, check if it is escape chars
					// 2. if have quotes, check if it is close quote
					// 3. check if it is open quotes, if yes, then add to quote
					// Last. None of above, then pos++
					if (!quotes.empty()) {
						if (!quoteEscapeList.empty()) {
							std::wstring escapeChar = quoteEscapeList[quotes[quotes.size() - 1]];
							if (!escapeChar.empty() && HasPrefix(str, escapeChar, pos)) {
								pos += escapeChar.length();
								pos++; // for escaped char
								continue;
							}
						}
						std::wstring closeQuote = quoteCloseList[quotes[quotes.size() - 1]];
						if (!closeQuote.empty() && HasPrefix(str, closeQuote, pos)) {
							pos += closeQuote.length();
							quotes.pop_back();
							continue;
						}
					}
					std::wstring currentQuoteOpen = L"";
					for (size_t i = 0; i < quoteOpenList.size(); i++) {
						std::wstring quoteOpen = quoteOpenList[i];
						if (HasPrefix(str, quoteOpen, pos)) {
							currentQuoteOpen = quoteOpen;
							quotes.push_back(i);
							break;
						}
					}
					if (!currentQuoteOpen.empty()) {
						pos += currentQuoteOpen.length();
					} else
						pos++;
				}
			}
			if (startPos != pos) {
				result = str.substr(startPos, pos - startPos);
				pos--;
			}
        }
        catch(const std::exception& e)
        {
            THROW_EXCEPTION(e);
        }
		return result;
	}
	
	std::wstring GetNextQuotedString(const std::wstring& str, size_t &pos, const std::vector<std::wstring> &delimiters,
		const std::vector<std::wstring> &quoteOpenList, const std::vector<std::wstring> &quoteCloseList, const std::vector<std::wstring> &quoteEscapeList)
	{
		if (str.empty())
			return str;
		if (pos >= str.length())
			return L"";
		GetNextCharacterPos(str, pos, true);
		std::wstring result = L"";
		try
		{
			if (!(quoteOpenList.size() == quoteCloseList.size() && (quoteEscapeList.empty() || quoteCloseList.size() == quoteEscapeList.size())))
				THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Quote Open, Close, Escape List having different size.");

			size_t startPos = pos;
			if (HasPrefix(str, quoteOpenList, pos)) {
				std::vector<size_t> quotes;
				while (pos < str.length()) {
					// first pos must be open quote
					// if quotes become empty, that mean all quote closed, then return
					// 1. if have quotes, check if it is escape chars
					// 2. if have quotes, check if it is close quote
					// 3. check if it is open quotes, if yes, then add to quote
					// Last. None of above, then pos++
					if (!quotes.empty()) {
						if (!quoteEscapeList.empty()) {
							std::wstring escapeChar = quoteEscapeList[quotes[quotes.size() - 1]];
							if (!escapeChar.empty() && HasPrefix(str, escapeChar, pos)) {
								pos += escapeChar.length();
								pos++; // for escaped char
								continue;
							}
						}
						std::wstring closeQuote = quoteCloseList[quotes[quotes.size() - 1]];
						if (!closeQuote.empty() && HasPrefix(str, closeQuote, pos)) {
							pos += closeQuote.length();
							quotes.pop_back();
							continue;
						}
					}
					std::wstring currentQuoteOpen = L"";
					for (size_t i = 0; i < quoteOpenList.size(); i++) {
						std::wstring quoteOpen = quoteOpenList[i];
						if (HasPrefix(str, quoteOpen, pos)) {
							currentQuoteOpen = quoteOpen;
							quotes.push_back(i);
							break;
						}
					}
					if (!currentQuoteOpen.empty())
						pos += currentQuoteOpen.length();
					else if (quotes.empty())
						break;
					else
						pos++;
				}
			} else {
				// check if string is quoted. If not, return string end with special char
				while (pos < str.length()) {
					if (HasPrefix(str, delimiters, pos))
						break;
					pos++;
				}
			}
			if (startPos != pos) {
				result = str.substr(startPos, pos - startPos);
				pos--;
			}
        }
        catch(const std::exception& e)
        {
            THROW_EXCEPTION(e);
        }
		return result;
	}
	
	size_t CountSubstring(const std::wstring &str, const std::wstring subStr)
	{
		size_t count = 0;
		size_t pos = 0;

		while (pos < str.length())
		{
			pos = Find(str, subStr, pos);
			if (pos == std::wstring::npos)
				break;
			count++;
			pos += subStr.length();
		}
		return count;
	}

    void Replace(std::wstring &str, const std::wstring& from, const std::wstring &to)
    {
        str.replace(Find(str, from), from.length(), to);
    }
	
	void ReplaceRegex(std::wstring &str, const std::wstring &regex, const std::wstring &replacement)
	{
		TRY
			str = std::regex_replace(str, std::wregex(regex), replacement, std::regex_constants::format_first_only);
		CATCH
	}

	void ReplaceAll(std::wstring &str, const std::wstring& from, const std::wstring &to)
    {
        size_t startPos = 0;
        size_t foundPos;
        while ((foundPos = Find(str, from, startPos)) != std::wstring::npos) {
            str.replace(foundPos, from.length(), to);
            startPos = foundPos + to.length();
        }
    }
	
	void ReplaceRegexAll(std::wstring &str, const std::wstring &regex, const std::wstring &replacement)
	{
		TRY
			str = std::regex_replace(str, std::wregex(regex), replacement, std::regex_constants::match_any);
		CATCH
	}

    std::vector<wchar_t> GetSpecialCharacters(const EscapeStringType &type)
    {
        switch (type)
        {
			case EscapeStringType::DoubleQuote:
				return { L'\\', L'"' }; 
            case EscapeStringType::Regex:
                return { L'\\', L'^', L'$', L'.', L'|', L'?', L'*', L'+', L'(', L')', L'[', L']', L'{', L'}' };
			case EscapeStringType::SingleQuote:
				return { L'\\', L'\'' }; 
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
			std::vector<wchar_t> specialChars = GetSpecialCharacters(type);
			if (!std::count(specialChars.begin(), specialChars.end(), c))
				return std::wstring(1, c);
	
			switch (type)
			{
				case EscapeStringType::DoubleQuote:
				case EscapeStringType::Regex:
				case EscapeStringType::SingleQuote:
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

	std::wstring GetEscapeStringWithQuote(const EscapeStringType &type, const std::wstring &str)
	{
        std::wstring result = L"";
        try 
        {
			std::wstring openQuote = L"";
			std::wstring closeQuote = L"";
			switch (type)
			{
			case EscapeStringType::DoubleQuote:
				closeQuote = openQuote = L"\"";
				break;
			case EscapeStringType::SingleQuote:
				closeQuote = openQuote = L"'";
				break;
			default:
				assert(false);
				break;
			}
			result = openQuote + GetEscapeString(type, str) + closeQuote;
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
        TRY
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
					case EscapeStringType::DoubleQuote:
					case EscapeStringType::Regex:
					case EscapeStringType::SingleQuote:
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
        CATCH
        return result;
	}
	
	std::wstring GetUnescapeStringWithQuote(const EscapeStringType &type, const std::wstring &str)
	{		
		switch (type)
		{
		case EscapeStringType::DoubleQuote: {
			bool isNeedRemoveHeadAndTail = str.starts_with(L"\"") && str.ends_with(L"\"") && str.length() >= 2;
			assert(isNeedRemoveHeadAndTail);
			if (!isNeedRemoveHeadAndTail)
				THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"String does not start with \"");
			break;
		}
		case EscapeStringType::SingleQuote: {
			bool isNeedRemoveHeadAndTail = str.starts_with(L"\'") && str.ends_with(L"\'") && str.length() >= 2;
			assert(isNeedRemoveHeadAndTail);
			if (!isNeedRemoveHeadAndTail)
				THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"String does not start with \'");
			break;
		}
		default:
			assert(false);
			break;
		}
		return GetUnescapeString(type, str.substr(1, str.length() - 2));
	}
}