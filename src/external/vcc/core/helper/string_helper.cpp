#include "string_helper.hpp"

#include <algorithm>
#include <map>
#include <math.h>
#include <memory>
#include <iostream>
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
		TRY
			std::wstringstream wss;
			wss << str.c_str();
			return wss.str();
		CATCH
		return L"";
	}

	std::string wstr2str(const std::wstring &wstr)
	{
		if (wstr.empty())
			return "";
		TRY
			std::string str(wstr.begin(), wstr.end());
			return str;
		CATCH
		return "";
	}
	
	void toCamel(std::wstring &str)
	{
		TRY
			if (str.empty())
				return;
			if (str.length() > 1) {
				toPascal(str);
				str = std::wstring(1, std::towlower(str[0])) + str.substr(1);
			} else
				toLower(str);
		CATCH
	}	
	
	void toCapital(std::wstring &str)
	{
		TRY
			if (str.empty())
				return;
			if (str.length() > 1) {
				std::wstring tmpStr = L"";
				bool isCapitalNeeded = true;
				for (const auto &c : str) {
					if (std::iswspace(c)) {
						tmpStr += std::wstring(1, c);
						isCapitalNeeded = true;
					} else if (isCapitalNeeded && !std::iswspace(c)) {
						tmpStr += std::wstring(1, std::towupper(c));
						isCapitalNeeded = false;
					} else
						tmpStr += std::wstring(1, c);
				}
				str = tmpStr;
			} else
				toUpper(str);
		CATCH
	}
	
	void toLower(std::wstring &str)
	{
		TRY
			std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		CATCH
	}
	
	void toPascal(std::wstring &str)
	{
		TRY
			if (str.empty())
				return;
			if (str.length() > 1) {
				if (isContain(str, L" ")) {
					toLower(str);
					auto tokens = splitString(str, { L" " });
					str = L"";
					for (auto &token : tokens) {
						if (token.empty())
							continue;
						str += std::wstring(1, std::towupper(token[0])) + (token.length() > 1 ? token.substr(1) : L"" );
					}
				} else
					str = std::wstring(1, std::towupper(str[0])) + str.substr(1);
			} else
				toUpper(str);
		CATCH
	}

	void toUpper(std::wstring &str)
	{
		TRY
			std::transform(str.begin(), str.end(), str.begin(), ::toupper);
		CATCH
	}

	bool isLower(const std::wstring &str)
	{
		TRY
			if (str.empty())
				return false;
			for (wchar_t c : str) {
				if (!std::iswlower(c))
					return false;
			}
			return true;
		CATCH
		return false;
	}

	bool isUpper(const std::wstring &str)
	{
		TRY
			if (str.empty())
				return false;
			for (wchar_t c : str) {
				if (!std::iswupper(c))
					return false;
			}
			return true;
		CATCH
		return false;
	}

	bool isCapital(const std::wstring &str)
	{
		TRY
			if (str.empty())
				return false;
			return std::iswupper(str[0]);
		CATCH
		return false;
	}

	bool isEmpty(const std::wstring &str)
	{
		return str.empty();
	}

	bool isBlank(const std::wstring &str)
	{
		TRY
			for (const wchar_t &c : str) {
				if (!std::iswspace(c))
					return false;
			}
		CATCH
		return true;
	}

	bool isEqual(const std::wstring &str1, const std::wstring &str2, const bool &isIgnoreCase)
	{
		TRY
			if (str1.size() != str2.size())
				return false;
			if (str1.empty())
				return true;

			std::wstring tmpStr1 = str1;
			std::wstring tmpStr2 = str2;
			if (isIgnoreCase) {
				toLower(tmpStr1);
				toLower(tmpStr2);
			}
			return tmpStr1 == tmpStr2;
		CATCH
		return false;
	}
	
	bool isStartWith(const std::wstring &str, const std::wstring &prefix, const size_t &startFromPos, bool isIgnoreCase)
	{
		TRY
		 	if (str.length() - startFromPos < prefix.size())
		 		return false;
			return isEqual(str.substr(startFromPos, prefix.length()), prefix, isIgnoreCase);
		CATCH
		return false;
	}
	
	bool isStartWith(const std::wstring &str, const std::vector<std::wstring> &prefixes, const size_t &startFromPos, bool isIgnoreCase)
	{
		bool result = false;
		TRY
			for (auto const &prefix : prefixes) {
				if (isStartWith(str, prefix, startFromPos, isIgnoreCase)) {
					result = true;
					break;
				}
			}
		CATCH
		return result;
	}

	bool isStartWithTrimSpace(const std::wstring &str, const std::wstring &prefix, const size_t &startFromPos)
	{
		TRY
			std::wstring trimPrefix = std::wstring(prefix);
			trim(trimPrefix);
			if (trimPrefix != prefix)
				THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Prefix contains space.");

			if (prefix.empty())
				return true;

			bool result = true;
			try
			{
				size_t prefixPos = 0;
				size_t currentPos = startFromPos;
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
		CATCH
		return false;
	}

	bool isEndWith(const std::wstring &str, const std::wstring &suffix)
	{
		TRY
			return str.ends_with(suffix);
		CATCH
		return false;
	}

	std::vector<std::wstring> splitString(const std::wstring &str, const std::vector<std::wstring> &delimiters,
		const std::vector<std::wstring> &quoteOpenList, const std::vector<std::wstring> &quoteCloseList, const std::vector<std::wstring> &quoteEscapeList)
	{
		std::vector<std::wstring> results;
		if (str.empty())
			return results;

		TRY
			if (!(quoteOpenList.size() == quoteCloseList.size() && (quoteEscapeList.empty() || quoteCloseList.size() == quoteEscapeList.size())))
				THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Command Open, Close, Escape List having different size.");

			std::vector<size_t> quotes;
			size_t pos = 0;
			std::wstring currentStr = L"";
			while (pos < str.length()) {
				if (quotes.empty() && isStartWith(str, delimiters, pos)) {
					results.push_back(currentStr);
					currentStr = L"";

					std::wstring currentDelimiter = L"";
					for (auto const &delimiter : delimiters) {
						if (isStartWith(str, delimiter, pos)) {
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
							if (!escapeChar.empty() && isStartWith(str, escapeChar, pos)) {
								pos += escapeChar.length();
								currentStr += escapeChar + str[pos];
								pos++; // for escaped char
								continue;
							}
						}
						std::wstring closeQuote = quoteCloseList[quotes[quotes.size() - 1]];
						if (!closeQuote.empty() && isStartWith(str, closeQuote, pos)) {
							pos += closeQuote.length();
							currentStr += closeQuote;
							quotes.pop_back();
							continue;
						}
					}
					std::wstring currentQuoteOpen = L"";
					for (size_t i = 0; i < quoteOpenList.size(); i++) {
						std::wstring quoteOpen = quoteOpenList[i];
						if (isStartWith(str, quoteOpen, pos)) {
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
        CATCH
		return results;
	}
	
	std::vector<std::wstring> splitStringBySpace(const std::wstring &str,
		const std::vector<std::wstring> &quoteOpenList, const std::vector<std::wstring> &quoteCloseList, const std::vector<std::wstring> &quoteEscapeList)
	{
		std::vector<std::wstring> results;
		if (str.empty())
			return results;

		TRY
			if (!(quoteOpenList.size() == quoteCloseList.size() && (quoteEscapeList.empty() || quoteCloseList.size() == quoteEscapeList.size())))
				THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Command Open, Close, Escape List having different size.");

			std::vector<size_t> quotes;
			size_t pos = 0;
			std::wstring currentStr = L"";
			while (pos < str.length()) {
				if (quotes.empty() &&  std::iswspace(str[pos])) {
					results.push_back(currentStr);
					currentStr = L"";
					pos += 1; // space char length
				} else {
					// 1. if have quotes, check if it is escape chars
					// 2. if have quotes, check if it is close quote
					// 3. check if it is open quotes, if yes, then add to quote
					// Last. None of above, then pos++
					if (!quotes.empty()) {
						if (!quoteEscapeList.empty()) {
							std::wstring escapeChar = quoteEscapeList[quotes[quotes.size() - 1]];
							if (!escapeChar.empty() && isStartWith(str, escapeChar, pos)) {
								pos += escapeChar.length();
								currentStr += escapeChar + str[pos];
								pos++; // for escaped char
								continue;
							}
						}
						std::wstring closeQuote = quoteCloseList[quotes[quotes.size() - 1]];
						if (!closeQuote.empty() && isStartWith(str, closeQuote, pos)) {
							pos += closeQuote.length();
							currentStr += closeQuote;
							quotes.pop_back();
							continue;
						}
					}
					std::wstring currentQuoteOpen = L"";
					for (size_t i = 0; i < quoteOpenList.size(); i++) {
						std::wstring quoteOpen = quoteOpenList[i];
						if (isStartWith(str, quoteOpen, pos)) {
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
        CATCH
		return results;
	}

	std::vector<std::wstring> splitStringByLine(const std::wstring &str)
	{
		std::vector<std::wstring> results;
		if (str.empty())
			return results;
		
		TRY
			std::wistringstream iss(str);
			std::wstring line;
			while (std::getline(iss, line))
				results.push_back(line);
		CATCH
		return results;
	}

	std::vector<std::wstring> splitStringByUpperCase(const std::wstring &str, bool splitDigit, bool splitSpecialChar)
	{
		std::vector<std::wstring> results;
		TRY
			std::wstring currentToken;
			for (auto const &c : str) {
				bool pushCurrentToken = std::iswupper(c)
					|| std::iswspace(c)
					|| (splitSpecialChar && !std::isalnum(c))
					|| (splitDigit && std::iswdigit(c));

				bool addCharToCurrentToken = !std::iswspace(c) && (std::iswalnum(c) || !splitSpecialChar);

				if (pushCurrentToken) {
					trim(currentToken);
					if (!currentToken.empty())
						results.push_back(currentToken);
					currentToken = L"";
				}
				if (addCharToCurrentToken)
					currentToken += std::wstring(1, c);
			}
			if (!currentToken.empty())
				results.push_back(currentToken);
		CATCH
		return results;
	}

	std::string padLeft(const std::string &str, const size_t &length, const char &c)
	{
		if (length <= str.length())
			return str;
		TRY
			return std::string(length - std::min(length, str.length()), c) + str;
		CATCH
		return "";
	}

	std::string padRight(const std::string &str, const size_t &length, const char &c)
	{
		if (length <= str.length())
			return str;
		TRY
			return str + std::string(length - std::min(length, str.length()), c);
		CATCH
		return "";
	}

	std::wstring padLeft(const std::wstring &str, const size_t &length, const wchar_t &c)
	{
		if (length <= str.length())
			return str;
		TRY
			return std::wstring(length - std::min(length, str.length()), c) + str;
		CATCH
		return L"";
	}

	std::wstring padRight(const std::wstring &str, const size_t &length, const wchar_t &c)
	{
		if (length <= str.length())
			return str;
		TRY
			return str + std::wstring(length - std::min(length, str.length()), c);
		CATCH
		return L"";
	}

	void lTrim(std::string &str)
	{
		TRY
			str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](char c) {
				return !std::isspace(c);
			}));
		CATCH
	}

	void rTrim(std::string &str)
	{
		TRY
			str	.erase(std::find_if(str.rbegin(), str.rend(), [](char c) {
				return !std::isspace(c);
			}).base(), str.end());
		CATCH
	}

	void trim(std::string &str)
	{
		TRY
			lTrim(str);
			rTrim(str);
		CATCH
	}

	void lTrim(std::wstring &str)
	{
		TRY
			str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](wchar_t c) {
				return !std::iswspace(c);
			}));
		CATCH
	}

	void rTrim(std::wstring &str)
	{
		TRY
			str.erase(std::find_if(str.rbegin(), str.rend(), [](wchar_t c) {
				return !std::iswspace(c);
			}).base(), str.end());
		CATCH
	}

	void trim(std::wstring &str)
	{
		TRY
			lTrim(str);
			rTrim(str);
		CATCH
	}

	std::wstring convertNamingStyle(const std::wstring &str, const NamingStyle &from, const NamingStyle &to, const std::wstring& seperator)
	{
		std::wstring result = L"";
		TRY
			if (from == to)
				return str;
			
			std::vector<std::wstring> tokens;
			switch (from)
			{
			case NamingStyle::CamelCase:
			case NamingStyle::PascalCase:
				tokens = splitStringByUpperCase(str, true, true);
				break;
			case NamingStyle::ConstantCase:
			case NamingStyle::ScreamingSnakeCase:
			case NamingStyle::SnakeCase:
				tokens = splitString(str, { L"_" });
				break;
			case NamingStyle::DotSeparatedLowercase:
				tokens = splitString(str, { L"." });
				break;
			case NamingStyle::KebabCase:
				tokens = splitString(str, { L"-" });
				break;
			case NamingStyle::Lowercase:
			case NamingStyle::Uppercase:
			{
				std::vector<std::wstring> sepeartorSet;
				sepeartorSet.push_back(seperator);
				tokens = splitString(str, sepeartorSet);
				break;				
			}
			default:
				assert(false);
				break;
			}
			size_t cnt = 0;
			for (auto &token : tokens) {
				switch (to)
				{
				case NamingStyle::CamelCase:
					if (cnt == 0)
						toLower(token);
					else
						toCapital(token);
					result += token;
					break;
				case NamingStyle::ConstantCase:
				case NamingStyle::ScreamingSnakeCase:
					toUpper(token);
					result += (cnt > 0 ? L"_" : L"") + token;
					break;
				case NamingStyle::DotSeparatedLowercase:
					toLower(token);
					result += (cnt > 0 ? L"." : L"") + token;
					break;
				case NamingStyle::KebabCase:
					toLower(token);
					result += (cnt > 0 ? L"-" : L"") + token;
					break;
				case NamingStyle::Lowercase:
					toLower(token);
					result += (cnt > 0 ? seperator : L"") + token;
					break;
				case NamingStyle::PascalCase:
					toCapital(token);
					result += token;
					break;
				case NamingStyle::SnakeCase:
					toLower(token);
					result += (cnt > 0 ? L"_" : L"") + token;
					break;
				case NamingStyle::Uppercase:
					toUpper(token);
					result += (cnt > 0 ? seperator : L"") + token;
					break;
				default:
					assert(false);
					break;
				}
				cnt++;
			}
		CATCH
		return result;
	}

	size_t find(const std::wstring &str, const wchar_t &c, const size_t &pos, const bool &isIgnoreCase)
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

	size_t find(const std::wstring &str, const std::wstring &subStr, const size_t &pos, const bool &isIgnoreCase)
	{
		if (str.empty() || str.length() - pos < subStr.length())
			return std::wstring::npos;
		TRY
			if (subStr.length() == 1)
				return find(str, subStr[0], pos, isIgnoreCase);

			if (isIgnoreCase) {
				std::wstring tmpStr = str.substr(pos);
				std::wstring tmpSubStr = subStr;
				toUpper(tmpStr);
				toUpper(tmpSubStr);
				return find(tmpStr, tmpSubStr);
			} else {
				// cannot check at beginning as there is ignoreCase
				if (str.length() == subStr.length() && str != subStr)
					return std::wstring::npos;
				return str.find(subStr, pos);
			}
		CATCH
		return std::wstring::npos;
	}

	void getCharacterRowAndColumn(const std::wstring &str, size_t pos, size_t &row, size_t &column)
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

	void getNextCharPos(const std::wstring &str, size_t &pos, bool fromCurrentPos)
	{
		TRY
			if (!fromCurrentPos)
				pos++;
			while (pos < str.length()) {
				if (!std::iswspace(str[pos]))
					return;
				pos++;
			}
		CATCH
	}

	std::wstring getTailingSubstring(const std::wstring &str, const size_t &length)
	{
		if (str.length() <= length)
			return str;
		TRY
			return str.substr(str.length() - length, length);
		CATCH
		return str;
	}

	std::wstring getNextStringSplitBySpace(const std::wstring &str, size_t &pos,
		const std::vector<std::wstring> &quoteOpenList, const std::vector<std::wstring> &quoteCloseList, const std::vector<std::wstring> &quoteEscapeList)
	{
		TRY
			// \r\n must before \n and \r
			return getNextString(str, pos, {L" ", L"\r\n", L"\n", L"\r", L"\t"}, quoteOpenList, quoteCloseList, quoteEscapeList);
		CATCH
		return str;
	}
	
	std::wstring getNextString(const std::wstring &str, size_t &pos, const std::vector<std::wstring> &delimiters,
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
		std::wstring result = L"";
		TRY
			getNextCharPos(str, pos, true);
			if (!(quoteOpenList.size() == quoteCloseList.size() && (quoteEscapeList.empty() || quoteCloseList.size() == quoteEscapeList.size())))
				THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Quote Open, Close, Escape List having different size.");

			size_t startPos = pos;
			std::vector<size_t> quotes;
			while (pos < str.length()) {
				if (quotes.empty() && isStartWith(str, delimiters, pos)) {
					break;
				} else {
					// 1. if have quotes, check if it is escape chars
					// 2. if have quotes, check if it is close quote
					// 3. check if it is open quotes, if yes, then add to quote
					// Last. None of above, then pos++
					if (!quotes.empty()) {
						if (!quoteEscapeList.empty()) {
							std::wstring escapeChar = quoteEscapeList[quotes[quotes.size() - 1]];
							if (!escapeChar.empty() && isStartWith(str, escapeChar, pos)) {
								pos += escapeChar.length();
								pos++; // for escaped char
								continue;
							}
						}
						std::wstring closeQuote = quoteCloseList[quotes[quotes.size() - 1]];
						if (!closeQuote.empty() && isStartWith(str, closeQuote, pos)) {
							pos += closeQuote.length();
							quotes.pop_back();
							continue;
						}
					}
					std::wstring currentQuoteOpen = L"";
					for (size_t i = 0; i < quoteOpenList.size(); i++) {
						std::wstring quoteOpen = quoteOpenList[i];
						if (isStartWith(str, quoteOpen, pos)) {
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
        CATCH
		return result;
	}
	
	std::wstring getNextQuotedString(const std::wstring& str, size_t &pos, const std::vector<std::wstring> &delimiters,
		const std::vector<std::wstring> &quoteOpenList, const std::vector<std::wstring> &quoteCloseList, const std::vector<std::wstring> &quoteEscapeList,
		const std::vector<std::wstring> &stringOpenList)
	{
		if (str.empty())
			return str;
		if (pos >= str.length())
			return L"";
		std::wstring result = L"";
		TRY
			getNextCharPos(str, pos, true);
			if (!(quoteOpenList.size() == quoteCloseList.size() && (quoteEscapeList.empty() || quoteCloseList.size() == quoteEscapeList.size())))
				THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Quote Open, Close, Escape List having different size.");
			if (!isContain(quoteOpenList, stringOpenList))
				THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Quote Open does not contained String Open List.");

			size_t startPos = pos;
			if (isStartWith(str, quoteOpenList, pos)) {
				std::vector<size_t> quotes;
				bool isInString = false;
				while (pos < str.length()) {
					// first pos must be open quote
					// if quotes become empty, that mean all quote closed, then return
					// 1. if have string, skip all content in string until string close
					// 2. if have quotes, check if it is escape chars
					// 3. if have quotes, check if it is close quote
					// 4. check if it is open quotes, if yes, then add to quote
					// Last. None of above, then pos++
					if (!quotes.empty()) {
						if (!quoteEscapeList.empty()) {
							std::wstring escapeChar = quoteEscapeList[quotes[quotes.size() - 1]];
							if (!escapeChar.empty() && isStartWith(str, escapeChar, pos)) {
								pos += escapeChar.length();
								pos++; // for escaped char
								continue;
							}
						}
						std::wstring closeQuote = quoteCloseList[quotes[quotes.size() - 1]];
						if (!closeQuote.empty() && isStartWith(str, closeQuote, pos)) {
							pos += closeQuote.length();
							quotes.pop_back();
							isInString = false;
							if (quotes.empty())
								break;
							continue;
						}
					}
					std::wstring currentQuoteOpen = L"";
					if (!isInString) {
						for (size_t i = 0; i < quoteOpenList.size(); i++) {
							std::wstring quoteOpen = quoteOpenList[i];
							if (isStartWith(str, quoteOpen, pos)) {
								currentQuoteOpen = quoteOpen;
								quotes.push_back(i);
	
								for (size_t i = 0; i < stringOpenList.size(); i++) {
									std::wstring quoteOpen = stringOpenList[i];
									if (isStartWith(str, quoteOpen, pos)) {
										isInString = true;
										break;
									}
								}
								break;
							}
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
					if (isStartWith(str, delimiters, pos))
						break;
					pos++;
				}
			}
			if (startPos != pos) {
				result = str.substr(startPos, pos - startPos);
				pos--;
			}
        CATCH
		return result;
	}
	
	bool isContain(const std::wstring &str, const std::wstring &subStr, const size_t &pos, const bool &isIgnoreCase)
	{
		TRY
			return find(str, subStr, pos, isIgnoreCase) != std::wstring::npos;
		CATCH
		return false;
	}

	size_t countSubstring(const std::wstring &str, const std::wstring &subStr, const bool &isIgnoreCase)
	{
		size_t count = 0;
		size_t pos = 0;

		TRY
			while (pos < str.length())
			{
				pos = find(str, subStr, pos, isIgnoreCase);
				if (pos == std::wstring::npos)
					break;
				count++;
				pos += subStr.length();
			}
		CATCH
		return count;
	}

    void replace(std::wstring &str, const std::wstring& from, const std::wstring &to)
    {
		TRY
			size_t pos = find(str, from);
			if (pos == std::wstring::npos)
				return;
			str.replace(pos, from.length(), to);
		CATCH
    }
	
	void replaceRegex(std::wstring &str, const std::wstring &regex, const std::wstring &replacement)
	{
		TRY
			str = std::regex_replace(str, std::wregex(regex), replacement, std::regex_constants::format_first_only);
		CATCH
	}

	void replaceAll(std::wstring &str, const std::wstring& from, const std::wstring &to)
    {
		TRY
			size_t startPos = 0;
			size_t foundPos;
			while ((foundPos = find(str, from, startPos)) != std::wstring::npos) {
				str.replace(foundPos, from.length(), to);
				startPos = foundPos + to.length();
			}
		CATCH
    }
	
	void replaceRegexAll(std::wstring &str, const std::wstring &regex, const std::wstring &replacement)
	{
		TRY
			str = std::regex_replace(str, std::wregex(regex), replacement, std::regex_constants::match_any);
		CATCH
	}

    std::vector<wchar_t> getSpecialCharacters(const EscapeStringType &type)
    {
		TRY
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
		CATCH
        return {};
    }

	std::map<wchar_t, std::wstring> getEscapeStringMap(const EscapeStringType &type)
	{
		std::map<wchar_t, std::wstring> result;
		TRY
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
		CATCH
		return {};
	}

	std::wstring convertSpecialCharacterToEscapeString(const EscapeStringType &type, const wchar_t &c)
	{
        TRY
			std::vector<wchar_t> specialChars = getSpecialCharacters(type);
			if (!std::count(specialChars.begin(), specialChars.end(), c))
				return std::wstring(1, c);
	
			switch (type)
			{
				case EscapeStringType::DoubleQuote:
				case EscapeStringType::Regex:
				case EscapeStringType::SingleQuote:
					return L"\\" + std::wstring(1, c);
				case EscapeStringType::XML:
					return getEscapeStringMap(type)[c];
				default:
					assert(false);
			}
        CATCH
		return std::wstring(1, c);
	}

	std::wstring getEscapeString(const EscapeStringType &type, const std::wstring &str)
    {
        std::wstring result = L"";
        TRY
			for (auto c : str)
				result += convertSpecialCharacterToEscapeString(type, c);
        CATCH
        return result;
    }

	std::wstring getEscapeStringWithQuote(const EscapeStringType &type, const std::wstring &str)
	{
        std::wstring result = L"";
        TRY
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
			result = openQuote + getEscapeString(type, str) + closeQuote;
        CATCH
        return result;
	}

	std::wstring getUnescapeString(const EscapeStringType &type, const std::wstring &str)
	{
        std::wstring result = L"";
        TRY
			std::map<wchar_t, std::wstring> escapeMap;
			switch (type)
			{
			case EscapeStringType::XML:
				escapeMap = getEscapeStringMap(type);
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
	
	std::wstring getUnescapeStringWithQuote(const EscapeStringType &type, const std::wstring &str)
	{
		TRY
			switch (type)
			{
			case EscapeStringType::DoubleQuote: {
				bool isNeedRemoveHeadAndTail = str.starts_with(L"\"") && str.ends_with(L"\"") && str.length() >= 2;
				if (!isNeedRemoveHeadAndTail)
					THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"GetUnescapeStringWithQuote: " + str + L" does not start with \"");
				break;
			}
			case EscapeStringType::SingleQuote: {
				bool isNeedRemoveHeadAndTail = str.starts_with(L"\'") && str.ends_with(L"\'") && str.length() >= 2;
				if (!isNeedRemoveHeadAndTail)
					THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"GetUnescapeStringWithQuote: " + str + L" does not start with \'");
				break;
			}
			default:
				assert(false);
				break;
			}
			return getUnescapeString(type, str.substr(1, str.length() - 2));
		CATCH
		return str;
	}
}