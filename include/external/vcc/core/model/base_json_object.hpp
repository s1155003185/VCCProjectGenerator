#pragma once

#include <memory>

#include "i_json_object.hpp"
#include "json.hpp"
#include "json_builder.hpp"

namespace vcc
{
    class BaseJsonObject : public IJsonObject
    {
    protected:
        BaseJsonObject() = default;
        virtual ~BaseJsonObject() {}

    public:
        virtual std::wstring serializeJson(const IDocumentBuilder *builder) const override
        {
            TRY
                return builder->serialize(ToJson().get());
            CATCH
            return L"";
        }

        virtual void deserializeJsonString(const IDocumentBuilder *builder, const std::wstring &jsonStr) override
        {
            TRY
                auto json = std::make_shared<Json>();
                builder->deserialize(jsonStr, json);
                deserializeJson(json);
            CATCH
        }
    };
}
