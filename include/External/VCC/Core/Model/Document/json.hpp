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
        Json,
        String
    };

    class ClassA : public BaseObject<ClassA> {
        public:
            ClassA() {}
            virtual ~ClassA() {}
    };

    class JsonObject : public BaseObject<JsonObject>
    {
        GETSET(JsonType, Type, JsonType::Null);
        // Json
        MAP_SPTR_R(std::wstring, JsonObject, NameValuePairs);
        // Json Object
        GETSET(std::wstring, Value, L"");
        GETSET(size_t, DecimalPlaces, 0);
        VECTOR_SPTR(JsonObject, Array);
        GETSET_SPTR_NULL(JsonObject, Object);

        public:
            JsonObject() : BaseObject() {}
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
