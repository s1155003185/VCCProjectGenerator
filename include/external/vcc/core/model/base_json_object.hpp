#pragma once

#include <memory>

#include "i_json_object.hpp"
#include "json.hpp"
#include "json_builder.hpp"
#include "memory_macro.hpp"

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
    };
}
