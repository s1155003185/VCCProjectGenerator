// <vcc:vccproj gen="DEMAND"/>
#include "DllFunctions.h"

#include <locale.h>
#include <stdio.h>
#include <wchar.h>

int GetVersion(wchar_t **str)
{
    std::wstring versionString = L"v0.0.1";
    size_t size = (versionString.length() + 1) * sizeof(wchar_t);
    *str = static_cast<wchar_t*>(malloc(size));
    if (*str == nullptr) {
        return -1;
    }
    wcscpy(*str, versionString.c_str());
    return 0;
}

// #define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(exportType, typeName) \
//     exportType Read##typeName(void *ref, int64_t property, int64_t index); \
//     exportType Read##typeName(void *ref, int64_t property, void *key); \
//     void Write##typeName(void *ref, int64_t property, exportType value, int64_t index); \
//     void Write##typeName(void *ref, int64_t property, exportType value, void *key);

// #define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_STRING(typeName) \
//     void Read##typeName(void *ref, int64_t property, wchar_t **value, int64_t index); \
//     void Read##typeName(void *ref, int64_t property, wchar_t **value, void *key); \
//     void Write##typeName(void *ref, int64_t property, wchar_t **value, int64_t index); \
//     void Write##typeName(void *ref, int64_t property, wchar_t **value, void *key);

// #define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_OBJECT(typeName) \
//     void *Read##typeName(void *ref, int64_t property, int64_t index); \
//     void *Read##typeName(void *ref, int64_t property, void *key); \
//     void Write##typeName(void *ref, int64_t property, void *value, int64_t index); \
//     void Write##typeName(void *ref, int64_t property, void *value, void *key);

// #define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_CONTAINER \
//     long GetContainerCount(void *ref, int64_t property); \
//     bool IsContainKey(void *ref, int64_t property, void *key); \
//     void RemoveContainerElement(void *ref, int64_t property, int64_t index); \
//     void RemoveContainerElement(void *ref, int64_t property, void *key);

//PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(bool, Bool);
//#include "property_accessor_factory.hpp"
// bool ReadBool(void *ref, int64_t property, int64_t index)
// {

// }

// bool ReadBoolByKey(void *ref, int64_t property, void *key)
// {

// }

// void WriteBool(void *ref, int64_t property, bool value, int64_t index)
// {

// }

// void WriteBoolByKey(void *ref, int64_t property, bool value, void *key)
// {

// }

// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(char, Char);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(wchar_t, Wchar);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(int, Int8);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(int, Uint8);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(int, Short);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(int, UnsignedShort);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(long, Int);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(long, UnsignedInt);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(long, Long);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(long, UnsignedLong);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(float, Float);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(double, Double);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_STRING(String);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_STRING(Wstring);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_OBJECT(Object);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_CONTAINER;

// <vcc:propertyAccessor gen="FORCE">
// </vcc:propertyAccessor>