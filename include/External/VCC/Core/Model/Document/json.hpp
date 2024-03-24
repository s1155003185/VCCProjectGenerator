#pragma once

#include <memory>

#include "base_object.hpp"
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
        String
    };

    class JsonObject : public BaseObject<JsonObject>
    {
        GETSET(JsonType, Type, JsonType::Null);
        GETSET(std::wstring, Value, L"");
        GETSET(size_t, DecimalPlaces, 0);
        VECTOR_SPTR(JsonObject, Array);
        GETSET_SPTR(JsonObject, Object);

        public:
            JsonObject() : BaseObject() {}
            virtual ~JsonObject() {}
            
            virtual std::shared_ptr<JsonObject> Clone() const override {
                std::shared_ptr<JsonObject> obj = std::make_shared<JsonObject>(*this);
                obj->CloneArray(this->GetArray());
                obj->CloneObject(this->GetObject());
                return obj;
            }
    };

    class Json : public BaseObject<Json>
    {
        MAP_SPTR_R(wstring, JsonObject, Objects)
        public:
            Json() : BaseObject() {}
            virtual ~Json() {}
    };
}
