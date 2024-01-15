#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "base_object.hpp"
#include "class_macro.hpp"

namespace vcc
{
    // TODO: Not Complete
    class XMLAttribute : public BaseObject
    {
        friend class XMLReader;
        GETSET(std::wstring, Name, L"");
        GETSET(std::wstring, Value, L"");
        
        public:
            XMLAttribute() : BaseObject() {}
            virtual ~XMLAttribute() {}

            virtual std::shared_ptr<IObject> Clone() override {
                return std::make_shared<XMLAttribute>(*this);
            }
    };

    class XMLElement : public BaseObject 
    {
        friend class XMLReader;

        GETSET(std::wstring, Namespace, L"");
        GETSET(std::wstring, Name, L"");
        VECTOR_SPTR(XMLAttribute, Attributes);
        VECTOR_SPTR(XMLElement, Children);
        GETSET(std::wstring, Text, L"");
        GETSET(std::wstring, FullText, L"");

        public:
            XMLElement() : BaseObject() {}
            virtual ~XMLElement() {}

            virtual std::shared_ptr<IObject> Clone() override {
                std::shared_ptr<XMLElement> obj = std::make_shared<XMLElement>(*this);
                obj->CloneAttributes(this->GetAttributes().get());
                obj->CloneChildren(this->GetChildren().get());
                return obj;
            }
    };

    class XMLReader
    {
        protected:
            virtual std::wstring _GetErrorMessage(const size_t &pos, const wchar_t &c, const std::wstring &msg);
            virtual std::wstring _GetString(const std::wstring &xmlData, size_t &pos);
            virtual std::wstring _GetTag(const std::wstring &xmlData, size_t &pos);
            virtual bool _IsNextCharTagEnd(const std::wstring &xmlData, size_t &pos);
            virtual bool _IsXMLHeader(const std::wstring &xmlData, size_t &pos);
            virtual void _ParseXMLHeader(const std::wstring &xmlData, size_t &pos); // TODO: handle <?...?> and <!...>
            
        public:
            XMLReader() = default;
            virtual ~XMLReader() {}

            // tag
            virtual bool ParseXMLTagHeader(const std::wstring &xmlData, size_t &pos, std::shared_ptr<XMLElement> element); // ture if have tag end
            virtual void ParseXMLTagContent(const std::wstring &xmlData, size_t &pos, std::shared_ptr<XMLElement> element);
            virtual void RemoveXMLTagTail(const std::wstring &xmlData, size_t &pos, std::shared_ptr<XMLElement> element);

            // content
            virtual void ParseXMLTag(const std::wstring &xmlData, size_t &pos, std::shared_ptr<XMLElement> element);
            virtual void ParseXMLElement(const std::wstring &xmlData, size_t &pos, std::shared_ptr<XMLElement> element);

            virtual void Parse(const std::wstring &xml, std::shared_ptr<XMLElement> element);
            virtual void Parse(const std::wstring &xml, size_t &pos, std::shared_ptr<XMLElement> element);
    };
}