#pragma once

#include <memory>
#include <string>
#include <vector>

#include "class_macro.hpp"
#include "vpg_enum_class.hpp"

using namespace vcc;

class VPGEnumClassReader
{
    SET(std::wstring, ClassMacroList);
    private:
        std::wstring _GetErrorMessage(const size_t &pos, const wchar_t &c, const std::wstring &msg) const;

        std::wstring _GetEnum(const std::wstring &propertyStr, size_t &pos) const;
        std::wstring _GetMacro(const std::wstring &propertyCommand, size_t &pos) const;
        std::wstring _GetType(const std::wstring &macroStr, size_t &pos) const;
        std::wstring _GetPropertyName(const std::wstring &macroStr, size_t &pos) const;
        std::wstring _GetDefaultValue(const std::wstring &macroStr, size_t &pos) const;
        void _AssignEnumClassProperty(const std::wstring &propertyCommand, std::shared_ptr<VPGEnumClassProperty> property) const;

        std::wstring _GetCommand(const std::wstring &cppCode, size_t &pos) const;

        void _ParseProperties(const std::wstring &cppCode, size_t &pos, std::shared_ptr<VPGEnumClass>enumClass) const;
        void _ParseClass(const std::wstring &cppCode, size_t &pos, std::shared_ptr<VPGEnumClass>enumClass) const;
    public:
        VPGEnumClassReader() = default;
        VPGEnumClassReader(const std::set<std::wstring> &classMacroList);
        ~VPGEnumClassReader() {}

        // all attribute start with @@
        std::vector<std::wstring> GetAttribute(const std::wstring &str) const;
        void Parse(const std::wstring &cppCode, std::vector<std::shared_ptr<VPGEnumClass>> &results) const;
};