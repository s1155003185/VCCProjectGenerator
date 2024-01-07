#pragma once

#include <string>

#include "base_object.hpp"

namespace vcc
{
    class ConfigElement : public BaseObject
    {
        friend class ConfigReader;
        MAP(std::wstring, std::wstring, Configs)
        
        public:
            ConfigElement() : BaseObject() {}
            virtual ~ConfigElement() {}

            virtual std::shared_ptr<IObject> Clone() override {
                return std::make_shared<ConfigElement>(*this);
            }
    };

    class ConfigReader
    {
        public:
            ConfigReader() = default;
            virtual ~ConfigReader() {}

            virtual void Parse(const std::wstring &str, ConfigElement &configElement);
    };
}