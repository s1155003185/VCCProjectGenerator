#pragma once

#include <memory>
#include <string>

#include "i_type_union.hpp"
#include "property_accessor_macro.hpp"

namespace vcc
{
    class IObject;
    enum class LockType;

    // All property must be size_t for dynamic_cast object property type
    // Only allow basic type as need to export to interface
    // Virtual Function not support template, cannot batch update Container with property accessor
    class IPropertyAccessor
    {
        // For each property accessor, override all listed in protected
        I_PROPERTY_ACCESSOR_HEADER(bool, Bool);
        // char, byte
        I_PROPERTY_ACCESSOR_HEADER(char, Char);
        // wchar_t
        I_PROPERTY_ACCESSOR_HEADER(wchar_t, Wchar);
        // int8_t
        I_PROPERTY_ACCESSOR_HEADER(int8_t, Int8);
        // uint8_t
        I_PROPERTY_ACCESSOR_HEADER(uint8_t, Uint8);
        // short, int16_t
        I_PROPERTY_ACCESSOR_HEADER(short, Short);
        // uint16_t
        I_PROPERTY_ACCESSOR_HEADER(uint16_t, UnsignedShort)
        // int, int32_t
        I_PROPERTY_ACCESSOR_HEADER(int, Int)
        // uint32_t
        I_PROPERTY_ACCESSOR_HEADER(uint32_t, UnsignedInt)
        // long, int64_t, time_t, enum
        I_PROPERTY_ACCESSOR_HEADER(long, Long)
        // size_t
        I_PROPERTY_ACCESSOR_HEADER(size_t, UnsignedLong)
        // float
        I_PROPERTY_ACCESSOR_HEADER(float, Float)
        // double
        I_PROPERTY_ACCESSOR_HEADER(double, Double)
        // wstring
        I_PROPERTY_ACCESSOR_HEADER(std::wstring, String)
        // Object
        I_PROPERTY_ACCESSOR_OBJECT_HEADER(std::shared_ptr<IObject>, Object)

        // vector, set, map
        I_PROPERTY_ACCESSOR_CONTAINER_HEADER
        
        protected:
            IPropertyAccessor() {}
            virtual ~IPropertyAccessor() {}

        public:
            // lock
            virtual void ReadLock() const = 0;
            virtual void WriteLock() const = 0;
            virtual void ReadWriteLock() const = 0;
            virtual void Unlock() const = 0;
    };
}
