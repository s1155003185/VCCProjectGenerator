#pragma once

#include <string>
#include <vector>

#include "class_macro.hpp"

struct VPGEnumClassProperty
{
    std::wstring Enum = L"";
    std::wstring Macro = L"";
    std::wstring Type = L"";
    std::wstring PropertyName = L"";
    std::wstring DefaultValue = L"";
    std::wstring Command = L"";
};

struct VPGEnumClass
{
    std::wstring Name = L"";
    std::wstring Command = L"";
    std::vector<VPGEnumClassProperty> Properties;
};

class VPGEnumClassReader
{
    SET(std::wstring, ClassMacroList);
    private:
        std::wstring _GetErrorMessage(const size_t &pos, const wchar_t &c, const std::wstring &msg);

        std::wstring _GetEnum(const std::wstring &propertyStr, size_t &pos);
        std::wstring _GetMacro(const std::wstring &propertyCommand, size_t &pos);
        std::wstring _GetType(const std::wstring &macroStr, size_t &pos);
        std::wstring _GetPropertyName(const std::wstring &macroStr, size_t &pos);
        std::wstring _GetDefaultValue(const std::wstring &macroStr, size_t &pos);
        void _AssignEnumClassProperty(const std::wstring &propertyCommand, VPGEnumClassProperty &property);

        std::wstring _GetCommand(const std::wstring &cppCode, size_t &pos);

        void _ParseProperties(const std::wstring &cppCode, size_t &pos, VPGEnumClass &enumClass);
        void _ParseClass(const std::wstring &cppCode, size_t &pos, VPGEnumClass &enumClass);
    public:
        VPGEnumClassReader(const std::set<std::wstring> &classMacroList);
        ~VPGEnumClassReader() {}

        void Parse(const std::wstring &cppCode, std::vector<VPGEnumClass> &results);
};