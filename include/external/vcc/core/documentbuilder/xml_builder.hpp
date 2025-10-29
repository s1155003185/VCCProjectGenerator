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
            virtual std::wstring getString(const std::wstring &xmlData, size_t &pos) const;
            virtual std::wstring getTag(const std::wstring &xmlData, size_t &pos) const;
            virtual bool isNextCharTagEnd(const std::wstring &xmlData, size_t &pos) const;
            virtual bool isXMLHeader(const std::wstring &xmlData, size_t &pos) const;
            virtual void parseXMLHeader(const std::wstring &xmlData, size_t &pos) const; // TODO: handle <?...?> and <!...>
            
        public:
            XmlBuilder() = default;
            virtual ~XmlBuilder() {}

            // tag
            virtual bool parseXMLTagHeader(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const; // ture if have tag end
            virtual void parseXMLTagContent(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const;
            virtual void removeXMLTagTail(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const;

            // content
            virtual void parseXMLTag(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const;
            virtual void parseXml(const std::wstring &xmlData, size_t &pos, std::shared_ptr<Xml> element) const;

            virtual std::wstring serialize(const IDocument *doc) const  override;
            virtual void deserialize(const std::wstring &str, size_t &pos, std::shared_ptr<IDocument> doc) const override;
            virtual void deserialize(const std::wstring &str, std::shared_ptr<IDocument> doc) const override;
    };
}