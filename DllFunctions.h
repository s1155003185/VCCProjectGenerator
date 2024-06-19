// <vcc:vccproj gen="DEMAND"/>
#ifndef DLL_FUNCTIONS_H
#define DLL_FUNCTIONS_H

#include <string>

#ifdef _WIN32
#define DLLEXPORT __declspec (dllexport) 
#else
#define DLLEXPORT extern 
#endif

extern "C"
{

DLLEXPORT int GetVersion(wchar_t **str);

// #define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(exportType, typeName) \
//     DLLEXPORT exportType Read##typeName(void *ref, int64_t property, int64_t index); \
//     DLLEXPORT exportType Read##typeName##ByKey(void *ref, int64_t property, void *key); \
//     DLLEXPORT void Write##typeName(void *ref, int64_t property, exportType value, int64_t index); \
//     DLLEXPORT void Write##typeName##ByKey(void *ref, int64_t property, exportType value, void *key);

// #define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_STRING(typeName) \
//     DLLEXPORT void Read##typeName(void *ref, int64_t property, wchar_t **value, int64_t index); \
//     DLLEXPORT void Read##typeName##ByKey(void *ref, int64_t property, wchar_t **value, void *key); \
//     DLLEXPORT void Write##typeName(void *ref, int64_t property, wchar_t **value, int64_t index); \
//     DLLEXPORT void Write##typeName##ByKey(void *ref, int64_t property, wchar_t **value, void *key);

// #define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_OBJECT(typeName) \
//     DLLEXPORT void *Read##typeName(void *ref, int64_t property, int64_t index); \
//     DLLEXPORT void *Read##typeName##ByKey(void *ref, int64_t property, void *key); \
//     DLLEXPORT void Write##typeName(void *ref, int64_t property, void *value, int64_t index); \
//     DLLEXPORT void Write##typeName##ByKey(void *ref, int64_t property, void *value, void *key);

// #define PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_CONTAINER \
//     DLLEXPORT long GetContainerCount(void *ref, int64_t property); \
//     DLLEXPORT bool IsContainKey(void *ref, int64_t property, void *key); \
//     DLLEXPORT void RemoveContainerElement(void *ref, int64_t property, int64_t index); \
//     DLLEXPORT void RemoveContainerElement(void *ref, int64_t property, void *key);

// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(bool, Bool);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(char, Char);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(wchar_t, Wchar);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(int, Int8);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(int, Uint8);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(int, Short);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(int, UnsignedShort);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(long, Int);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(long, UnsignedInt);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(long, Long);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(long, UnsignedLong);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(float, Float);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(double, Double);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_STRING(String);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_STRING(Wstring);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_OBJECT(Object);
// PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_CONTAINER;

// <vcc:propertyAccessor gen="FORCE">
// </vcc:propertyAccessor>
}

#endif