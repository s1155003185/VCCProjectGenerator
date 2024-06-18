// <vcc:vccproj gen="DEMAND"/>
#ifndef DLL_FUNCTIONS_H
#define DLL_FUNCTIONS_H

#include <string>

#ifdef _WIN32
#define DLLEXPORT __declspec (dllexport) 
#else
#define DLLEXPORT extern 
#endif

#ifdef __cplusplus
extern "C"
{
#endif

DLLEXPORT int GetVersion(wchar_t **str);

// DLLEXPORT 

// I_PROPERTY_ACCESSOR_HEADER(bool, Bool);
//         // char, byte
//         I_PROPERTY_ACCESSOR_HEADER(char, Char);
//         // wchar_t
//         I_PROPERTY_ACCESSOR_HEADER(wchar_t, Wchar);
//         // int8_t
//         I_PROPERTY_ACCESSOR_HEADER(int8_t, Int8);
//         // uint8_t
//         I_PROPERTY_ACCESSOR_HEADER(uint8_t, Uint8);
//         // short, int16_t
//         I_PROPERTY_ACCESSOR_HEADER(short, Short);
//         // uint16_t
//         I_PROPERTY_ACCESSOR_HEADER(uint16_t, UnsignedShort)
//         // int, int32_t
//         I_PROPERTY_ACCESSOR_HEADER(int, Int)
//         // uint32_t
//         I_PROPERTY_ACCESSOR_HEADER(uint32_t, UnsignedInt)
//         // long, int64_t, time_t, enum
//         I_PROPERTY_ACCESSOR_HEADER(long, Long)
//         // size_t
//         I_PROPERTY_ACCESSOR_HEADER(size_t, UnsignedLong)
//         // float
//         I_PROPERTY_ACCESSOR_HEADER(float, Float)
//         // double
//         I_PROPERTY_ACCESSOR_HEADER(double, Double)
//         // string
//         I_PROPERTY_ACCESSOR_HEADER(std::string, String)
//         // wstring
//         I_PROPERTY_ACCESSOR_HEADER(std::wstring, Wstring)
//         // Object
//         I_PROPERTY_ACCESSOR_OBJECT_HEADER(std::shared_ptr<IObject>, Object)

    // #define I_PROPERTY_ACCESSOR_HEADER(type, name) \
    //     public: \
    //         virtual type Read##name(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const = 0; \
    //         virtual type Read##name(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0; \
    //         virtual void Write##name(const LockType &lockType, const size_t &objectProperty, const type &value, const int64_t &index = -1) const = 0; \
    //         virtual void Write##name(const LockType &lockType, const size_t &objectProperty, const type &value, const ITypeUnion *key) const = 0; \

    // #define I_PROPERTY_ACCESSOR_OBJECT_HEADER(type, name) \
    //     public: \
    //         virtual type Read##name(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const = 0; \
    //         virtual type Read##name(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0; \
    //         virtual void Write##name(const LockType &lockType, const size_t &objectProperty, type value, const int64_t &index = -1) const = 0; \
    //         virtual void Write##name(const LockType &lockType, const size_t &objectProperty, type value, const ITypeUnion *key) const = 0; \
    //         virtual type Clone##name(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const = 0; \
    //         virtual type Clone##name(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0; \

    // #define I_PROPERTY_ACCESSOR_CONTAINER_HEADER \
    //     public: \
    //         virtual size_t GetContainerCount(const LockType &lockType, const size_t &objectProperty) const = 0; \
    //         virtual bool IsContainKey(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0;\
    //         virtual void RemoveContainerElement(const LockType &lockType, const size_t &objectProperty, const int64_t &index = -1) const = 0; \
    //         virtual void RemoveContainerElement(const LockType &lockType, const size_t &objectProperty, const ITypeUnion *key) const = 0; \
    //         virtual void ClearContainer(const LockType &lockType, const size_t &objectProperty) const = 0;


// <vcc:propertyAccessor gen="FORCE">
// </vcc:propertyAccessor>

#ifdef __cplusplus
}
#endif

#endif