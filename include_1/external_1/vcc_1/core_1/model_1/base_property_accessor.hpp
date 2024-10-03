#pragma once
#include "i_property_accessor.hpp"

#include <string>
#include <shared_mutex>

#include "class_macro.hpp"
#include "property_accessor_macro.hpp"
#include "i_object.hpp"

namespace vcc
{
    enum class LockType;
    class BasePropertyAccessor : public IPropertyAccessor
    {
        inline static std::shared_mutex _Mutex;
        GETSET_SPTR_NULL(IObject, Object);

        // For each property accessor, override all listed in protected        
        BASE_PROPERTY_ACCESSOR_HEADER(bool, Bool);
        BASE_PROPERTY_ACCESSOR_HEADER(char, Char);
        BASE_PROPERTY_ACCESSOR_HEADER(wchar_t, Wchar);
        BASE_PROPERTY_ACCESSOR_HEADER(int8_t, Int8);
        BASE_PROPERTY_ACCESSOR_HEADER(uint8_t, Uint8);
        BASE_PROPERTY_ACCESSOR_HEADER(short, Short);
        BASE_PROPERTY_ACCESSOR_HEADER(uint16_t, UnsignedShort)
        BASE_PROPERTY_ACCESSOR_HEADER(int, Int)
        BASE_PROPERTY_ACCESSOR_HEADER(uint32_t, UnsignedInt)
        BASE_PROPERTY_ACCESSOR_HEADER(long, Long)
        BASE_PROPERTY_ACCESSOR_HEADER(size_t, UnsignedLong)
        BASE_PROPERTY_ACCESSOR_HEADER(float, Float)
        BASE_PROPERTY_ACCESSOR_HEADER(double, Double)
        BASE_PROPERTY_ACCESSOR_HEADER(std::wstring, String)
        BASE_PROPERTY_ACCESSOR_OBJECT_HEADER(std::shared_ptr<IObject>, Object)

        BASE_PROPERTY_ACCESSOR_CONTAINER_HEADER
        
        protected:
            BasePropertyAccessor(std::shared_ptr<IObject> object) { this->_Object = object; }
            virtual ~BasePropertyAccessor() {}

        public:
            // lock
            virtual void ReadLock() const override;
            virtual void WriteLock() const override;
            virtual void ReadWriteLock() const override;
            virtual void Unlock() const override;
    };
}
