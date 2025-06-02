#pragma once

#include <string>

#include "vpg_enum_class.hpp"

bool IsFileStartWithProjectPrefix(const std::wstring &str, const std::wstring &projectPrefix);
bool IsClassStartWithProjectPrefix(const std::wstring &str, const std::wstring &projectPrefix);
bool IsPropertyClassNameValidToGenerateClass(const std::wstring &str, const std::wstring &projectPrefix);
bool IsPropertyFile(const std::wstring &filePath, const std::wstring &projectPrefix);
bool IsPropertyClass(const std::wstring &className, const std::wstring &projectPrefix);

std::wstring GetActionClassName(const VPGEnumClass* enumClass, const VPGEnumClassProperty * property);
std::wstring GetActionFileNameWithoutExtension(const std::wstring &actionClassName, const std::wstring &projectPrefix);
std::wstring GetClassNameFromPropertyClassName(const std::wstring &className);

std::wstring GetActionMessage(const std::wstring &id, const std::wstring &msg);