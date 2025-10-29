#pragma once

#include <string>

#include "vpg_enum_class.hpp"

bool isFileStartWithProjectPrefix(const std::wstring& str, const std::wstring& projectPrefix);
bool isClassStartWithProjectPrefix(const std::wstring& str, const std::wstring& projectPrefix);
bool isPropertyClassNameValidToGenerateClass(const std::wstring& str,
                                             const std::wstring& projectPrefix);
bool isPropertyFile(const std::wstring& filePath, const std::wstring& projectPrefix);
bool isPropertyClass(const std::wstring& className, const std::wstring& projectPrefix);

bool isCustomType(const std::wstring& value);

// Namespace
std::wstring getTypeOrClassWithoutNamespace(const std::wstring& value);
std::wstring getNamespaceFromClassName(const std::wstring& className);
bool isClassInNamespace(const std::wstring& currentNamespace, const std::wstring& fullClassName);

// Action
std::wstring getActionClassName(const VPGEnumClass* enumClass,
                                const VPGEnumClassAttribute* property);
std::wstring getActionFileNameWithoutExtension(const std::wstring& actionClassName,
                                               const std::wstring& projectPrefix);

// Convert Class Name
std::wstring getClassNameFromPropertyClassName(const std::wstring& className);
std::wstring getClassNameFromFileName(const std::wstring& fileName,
                                      const std::wstring& projectPrefix);

std::wstring getActionMessage(const std::wstring& id, const std::wstring& msg);