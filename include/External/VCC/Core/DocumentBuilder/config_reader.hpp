#pragma once

#include <memory>
#include <string>

#include "base_object.hpp"
#include "memory_macro.hpp"

namespace vcc
{
    class ConfigElement : public BaseObject<ConfigElement>
    {
        friend class ConfigReader;
        MAP(std::wstring, std::wstring, Configs)
        
        public:
            ConfigElement() : BaseObject() {}
            virtual ~ConfigElement() {}

            virtual std::shared_ptr<IObject> Clone() const override {
                std::shared_ptr<ConfigElement> obj = std::make_shared<ConfigElement>(*this);
                obj->CloneConfigs(this->GetConfigs());
                return obj;
            }
    };

    class ConfigReader
    {
        public:
            ConfigReader() = default;
            virtual ~ConfigReader() {}

            virtual void Parse(const std::wstring &str, std::shared_ptr<ConfigElement> configElement);
    };
}