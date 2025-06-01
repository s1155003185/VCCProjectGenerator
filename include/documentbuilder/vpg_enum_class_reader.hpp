#pragma once

#include <memory>
#include <string>
#include <vector>

#include "class_macro.hpp"
#include "json.hpp"
#include "vpg_enum_class.hpp"

class VPGEnumClassReader
{
    private:
        std::vector<std::wstring> _OpenCommands = { L"/*", L"//"};
        std::vector<std::wstring> _CloseCommands = { L"*/", L"\n" };
        std::vector<std::wstring> _OpenQuotes = { L"{", L"[" };
        std::vector<std::wstring> _CloseQuotes = { L"}", L"]"};
        std::vector<std::wstring> _OpenCommandAndQuotes = { L"/*", L"//", L"{", L"<<", L"<" };
        std::vector<std::wstring> _CloseCommandAndQuotes = { L"*/", L"\n", L"}", L";", L">" };
        std::vector<std::wstring> _Delimiter = { L" ", L"\t", L"\n", L",", L";", L"{", L"}" };


    SET(std::wstring, ClassMacroList);
    GETSET(int64_t, EnumValue, -1);
    private:
        std::wstring _GetErrorMessage(const std::wstring &str, const size_t &pos, const std::wstring &msg) const;

        // Validate
        size_t IsCommand(const std::wstring &cppCode, const size_t &pos) const;

        // class structure
        std::wstring _GetEnum(const std::wstring &propertyStr, size_t &pos) const;
        std::wstring _GetMacro(const std::wstring &propertyCommand, size_t &pos) const;
        std::wstring _GetType(const std::wstring &macroStr, size_t &pos) const;
        std::wstring _GetPropertyName(const std::wstring &macroStr, size_t &pos) const;
        std::wstring _GetDefaultValue(const std::wstring &macroStr, size_t &pos) const;
        void _AssignEnumClassProperty(const VPGEnumClass *enumClass, const std::wstring &propertyCommand, std::vector<std::shared_ptr<VPGEnumClassAttribute>> &properties) const;

        std::wstring _GetCommand(const std::wstring &cppCode, const bool &isClassCommand, size_t &pos) const;

        void _ParseProperties(const std::wstring &cppCode, size_t &pos, std::shared_ptr<VPGEnumClass>enumClass) const;
        bool _ParseClass(const std::wstring &cppCode, size_t &pos, std::shared_ptr<VPGEnumClass>enumClass) const;

        void ParseCustom(const std::wstring &cppCode, const std::wstring &currentNamespace, std::vector<std::shared_ptr<VPGEnumClass>> &results) const;
    public:
        VPGEnumClassReader() = default;
        VPGEnumClassReader(const std::set<std::wstring> &classMacroList);
        ~VPGEnumClassReader() {}

        std::wstring GetCppCodeLine(const std::wstring &str, size_t &pos, bool fromCurrentPos) const;
        std::shared_ptr<vcc::Json> GetJsonAttributes(const std::wstring &command, const std::wstring &attributeName) const;

        // all attribute start with @@
        std::vector<std::wstring> GetAttribute(const std::wstring &str) const;
        void Parse(const std::wstring &cppCode, std::vector<std::shared_ptr<VPGEnumClass>> &results) const;
};