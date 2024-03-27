#pragma once

#include <memory>
#include <string>
#include <vector>

#include "base_object.hpp"
#include "class_macro.hpp"

using namespace vcc;

class VPGEnumClassProperty : public BaseObject<VPGEnumClassProperty>
{
    friend class VPGEnumClassReader;
    GETSET(std::wstring, Enum, L"");
    GETSET(std::wstring, Macro, L"");
    GETSET(std::wstring, Type, L"");
    GETSET(std::wstring, PropertyName, L"");
    GETSET(std::wstring, DefaultValue, L"");
    GETSET(std::wstring, Command, L"");

    public:
        VPGEnumClassProperty() : BaseObject() {}
        virtual ~VPGEnumClassProperty() {}
};

class VPGEnumClass : public BaseObject<VPGEnumClass>
{
    friend class VPGEnumClassReader;
    GETSET(std::wstring, Name, L"");
    GETSET(std::wstring, Command, L"");
    VECTOR_SPTR(VPGEnumClassProperty, Properties);

    public:
        VPGEnumClass() : BaseObject() {}
        virtual ~VPGEnumClass() {}

        virtual std::shared_ptr<IObject> Clone() const override {
            std::shared_ptr<VPGEnumClass> obj = std::make_shared<VPGEnumClass>(*this);
            obj->CloneProperties(this->GetProperties());
            return obj;
        }
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
        void _AssignEnumClassProperty(const std::wstring &propertyCommand, std::shared_ptr<VPGEnumClassProperty> property);

        std::wstring _GetCommand(const std::wstring &cppCode, size_t &pos);

        void _ParseProperties(const std::wstring &cppCode, size_t &pos, std::shared_ptr<VPGEnumClass>enumClass);
        void _ParseClass(const std::wstring &cppCode, size_t &pos, std::shared_ptr<VPGEnumClass>enumClass);
    public:
        VPGEnumClassReader(const std::set<std::wstring> &classMacroList);
        ~VPGEnumClassReader() {}

        void Parse(const std::wstring &cppCode, std::vector<std::shared_ptr<VPGEnumClass>> &results);
};