#pragma once

#include <memory>

#include "base_document.hpp"
#include "class_macro.hpp"

namespace vcc
{
    class Config : public BaseDocument
    {
        // If it is command, then Key is #, all including # will be stored as value
        // else key is config key, value is config value
        ORDERED_MAP(std::wstring, std::wstring, Configs);

        public:
            Config() : BaseDocument() {}
            virtual ~Config() {}
            
            virtual std::shared_ptr<IObject> clone() const override
            {
                return std::make_shared<Config>(*this);
            }
            
            bool isValue(const std::wstring &key) const;
            std::wstring getValue(const std::wstring &key) const;
            void AddValue(const std::wstring &key, const std::wstring &value);
            void AddLine(const std::wstring &value = L"");
            void AddCommand(const std::wstring &value);
    };
}
