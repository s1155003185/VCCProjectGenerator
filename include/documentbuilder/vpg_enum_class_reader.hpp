#pragma once

#include <memory>
#include <string>
#include <vector>

#include "class_macro.hpp"
#include "json.hpp"
#include "vpg_enum_class.hpp"

using namespace vcc;

class VPGEnumClassReader
{
    SET(std::wstring, ClassMacroList);
    GETSET(int64_t, EnumValue, -1);
    private:
        std::wstring _GetErrorMessage(const std::wstring &str, const size_t &pos, const std::wstring &msg) const;

        // class structure
        std::wstring _GetEnum(const std::wstring &propertyStr, size_t &pos) const;
        std::wstring _GetMacro(const std::wstring &propertyCommand, size_t &pos) const;
        std::wstring _GetType(const std::wstring &macroStr, size_t &pos) const;
        std::wstring _GetPropertyName(const std::wstring &macroStr, size_t &pos) const;
        std::wstring _GetDefaultValue(const std::wstring &macroStr, size_t &pos) const;
        void _AssignEnumClassProperty(const VPGEnumClass *enumClass, const std::wstring &propertyCommand, std::vector<std::shared_ptr<VPGEnumClassProperty>> &properties) const;

        std::wstring _GetCommand(const std::wstring &cppCode, const bool &isClassCommand, size_t &pos) const;

        void _ParseProperties(const std::wstring &cppCode, size_t &pos, std::shared_ptr<VPGEnumClass>enumClass) const;
        bool _ParseClass(const std::wstring &cppCode, size_t &pos, std::shared_ptr<VPGEnumClass>enumClass) const;
    public:
        VPGEnumClassReader() = default;
        VPGEnumClassReader(const std::set<std::wstring> &classMacroList);
        ~VPGEnumClassReader() {}

        std::wstring GetCppCodeLine(const std::wstring &str, size_t &pos, bool fromCurrentPos) const;
        std::shared_ptr<Json> GetJsonAttributes(const std::wstring &command, const std::wstring &attributeName) const;

        // all attribute start with @@
        std::vector<std::wstring> GetAttribute(const std::wstring &str) const;
        void Parse(const std::wstring &cppCode, std::vector<std::shared_ptr<VPGEnumClass>> &results) const;
};