#pragma once

#include "base_object.hpp"
#include "base_document.hpp"
#include "class_macro.hpp"

namespace vcc
{
    class XmlAttribute : public BaseObject<XmlAttribute>
    {
        friend class XmlBuilder;
        GETSET(std::wstring, Name, L"");
        GETSET(std::wstring, Value, L"");
        
        public:
            XmlAttribute() : BaseObject() {}
            virtual ~XmlAttribute() {}
    };

    class Xml : public BaseDocument<Xml>
    {
        friend class XmlBuilder;

        GETSET(std::wstring, Name, L"");
        VECTOR_SPTR(XmlAttribute, Attributes);
        VECTOR_SPTR(Xml, Children);
        GETSET(std::wstring, OpeningTag, L"");
        GETSET(std::wstring, ClosingTag, L"");
        GETSET(std::wstring, Text, L"");
        GETSET(std::wstring, FullText, L"");

        public:
            Xml() : BaseDocument() {}
            virtual ~Xml() {}

            virtual std::shared_ptr<IObject> Clone() const override {
                std::shared_ptr<Xml> obj = std::make_shared<Xml>(*this);
                obj->CloneAttributes(this->GetAttributes());
                obj->CloneChildren(this->GetChildren());
                return obj;
            }
    };
}
