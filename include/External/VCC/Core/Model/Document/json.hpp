#pragma once

#include <memory>

#include "base_document.hpp"
#include "class_macro.hpp"

namespace vcc
{
    enum class JsonType
    {
        Array,
        Boolean,
        Number,
        Null,
        Object,
        Json,
        String
    };

    class JsonObject : public BaseDocument<JsonObject>
    {
        GETSET(JsonType, Type, JsonType::Null);
        // Json
        ORDERED_MAP_SPTR_R(std::wstring, JsonObject, NameValuePairs);
        // Json Object
        GETSET(std::wstring, Value, L"");
        GETSET(size_t, DecimalPlaces, 0);
        VECTOR_SPTR(JsonObject, Array);
        GETSET_SPTR_NULL(JsonObject, Object);

        public:
            JsonObject() : BaseDocument() {}
            virtual ~JsonObject() {}
            
            virtual std::shared_ptr<IObject> Clone() const override {
                std::shared_ptr<JsonObject> obj = std::make_shared<JsonObject>(*this);
                obj->CloneNameValuePairs(this->GetNameValuePairs());
                obj->CloneArray(this->GetArray());
                obj->CloneObject(this->GetObject());
                return obj;
            }
    };
}
