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
        virtual std::wstring SerializeJson(const IDocumentBuilder *builder) const override
        {
            TRY
                return builder->Serialize(ToJson().get());
            CATCH
            return L"";
        }

        virtual void DeserializeJsonString(const IDocumentBuilder *builder, const std::wstring &jsonStr) const override
        {
            TRY
                auto json = std::make_shared<Json>();
                builder->Deserialize(jsonStr, json);
                DeserializeJson(json);
            CATCH
        }
    };
}
