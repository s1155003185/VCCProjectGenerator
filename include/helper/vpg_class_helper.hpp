#pragma once

#include <string>

#include "vpg_enum_class.hpp"

bool IsFileStartWithProjectPrefix(const std::wstring &str, const std::wstring &projectPrefix);
bool IsClassStartWithProjectPrefix(const std::wstring &str, const std::wstring &projectPrefix);
bool IsPropertyClassNameValidToGenerateClass(const std::wstring &str, const std::wstring &projectPrefix);
bool IsPropertyFile(const std::wstring &filePath, const std::wstring &projectPrefix);
bool IsPropertyClass(const std::wstring &className, const std::wstring &projectPrefix);

bool IsCustomType(const std::wstring &value);

// Namespace
std::wstring getTypeOrClassWithoutNamespace(const std::wstring &value);
std::wstring getNamespaceFromClassName(const std::wstring &className);
bool IsClassInNamespace(const std::wstring &currentNamespace, const std::wstring &fullClassName);

// Action
std::wstring getActionClassName(const VPGEnumClass* enumClass, const VPGEnumClassAttribute * property);
std::wstring getActionFileNameWithoutExtension(const std::wstring &actionClassName, const std::wstring &projectPrefix);

// ConverC lass Name
std::wstring getClassNameFromPropertyClassName(const std::wstring &className);

std::wstring getActionMessage(const std::wstring &id, const std::wstring &msg);