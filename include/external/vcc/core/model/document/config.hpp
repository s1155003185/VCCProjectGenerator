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
            
            virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<Config>(*this);
            }
            
            bool IsValue(const std::wstring &key) const;
            std::wstring GetValue(const std::wstring &key) const;
            void AddValue(const std::wstring &key, const std::wstring &value) const;
            void AddLine(const std::wstring &value = L"") const;
            void AddCommand(const std::wstring &value) const;
    };
}
