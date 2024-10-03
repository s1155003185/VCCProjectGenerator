#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "base_document_builder.hpp"
#include "xml.hpp"

namespace vcc
{
    class IDocument;

    // TODO: Serialize, XML Header, etc.
    // XML ref: https://www.w3.org/TR/xml/
    class XmlBuilder : public BaseDocumentBuilder
    {
        protected:
            virtual std::wstring GetString(const std::wstring &xmlData, size_t &pos) const;
            virtual std::wstring GetTag(const std::wstring &xmlData, size_t &pos) const;
            virtual bool IsNextCharTagEnd(const std::wstring &xmlData, size_t &pos) const;
            virtual bool IsXMLHeader(const std::wstring &xmlData, size_t &pos) const;
            virtual void ParseXMLHeader(const std::wstring &xmlData, size_t &pos) const; // TODO: handle <?...?> and <!...>
            
        public:
            XmlBuilder() = default;
            virtual ~XmlBuilder() {}

            // tag
            virtual bool ParseXMLTagHeader(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const; // ture if have tag end
            virtual void ParseXMLTagContent(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const;
            virtual void RemoveXMLTagTail(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const;

            // content
            virtual void ParseXMLTag(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const;
            virtual void ParseXml(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const;

            virtual std::wstring Serialize(const IDocument *doc) const  override;
            virtual void Deserialize(const std::wstring &str, size_t &pos, std::shared_ptr<IDocument> doc) const override;
            virtual void Deserialize(const std::wstring &str, std::shared_ptr<IDocument> doc) const override;
    };
}