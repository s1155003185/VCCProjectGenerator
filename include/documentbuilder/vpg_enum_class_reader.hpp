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
        std::wstring _getErrorMessage(const std::wstring &str, const size_t &pos, const std::wstring &msg) const;

        // Validate
        size_t isCommand(const std::wstring &cppCode, const size_t &pos) const;

        // class structure
        std::wstring _getEnum(const std::wstring &propertyStr, size_t &pos) const;
        std::wstring _getMacro(const std::wstring &propertyCommand, size_t &pos) const;
        std::wstring _getType(const std::wstring &macroStr, size_t &pos) const;
        std::wstring _getPropertyName(const std::wstring &macroStr, size_t &pos) const;
        std::wstring _getDefaultValue(const std::wstring &macroStr, size_t &pos) const;
        void _assignEnumClassProperty(const VPGEnumClass *enumClass, const std::wstring &propertyCommand, std::vector<std::shared_ptr<VPGEnumClassAttribute>> &properties) const;

        std::wstring _getCommand(const std::wstring &cppCode, const bool &isClassCommand, size_t &pos) const;

        void _parseProperties(const std::wstring &cppCode, size_t &pos, std::shared_ptr<VPGEnumClass>enumClass) const;
        bool _parseClass(const std::wstring &cppCode, size_t &pos, std::shared_ptr<VPGEnumClass>enumClass) const;

        void parseCustom(const std::wstring &cppCode, const std::wstring &currentNamespace, std::vector<std::shared_ptr<VPGEnumClass>> &results) const;
    public:
        VPGEnumClassReader() = default;
        VPGEnumClassReader(const std::set<std::wstring> &classMacroList);
        ~VPGEnumClassReader() {}

        std::wstring getCppCodeLine(const std::wstring &str, size_t &pos, bool fromCurrentPos) const;
        std::shared_ptr<vcc::Json> getJsonAttributes(const std::wstring &command, const std::wstring &attributeName) const;

        // all attribute start with @@
        std::vector<std::wstring> getAttribute(const std::wstring &str) const;
        void parse(const std::wstring &cppCode, std::vector<std::shared_ptr<VPGEnumClass>> &results) const;
};