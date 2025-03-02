// <vcc:vccproj gen="DEMAND"/>
#ifndef DLL_FUNCTIONS_H
#define DLL_FUNCTIONS_H

#include <string>

#ifdef _WIN32
#define DLLEXPORT __declspec (dllexport) 
#else
#define DLLEXPORT extern 
#endif

// <vcc:dllInterfaceHeader gen="REPLACE">
#include "object_factory.hpp"
#include "property_accessor_factory.hpp"
#include "property_accessor_macro.hpp"

using namespace vcc;
// </vcc:dllInterfaceHeader>

extern "C"
{

DLLEXPORT int GetVersion(wchar_t **str);

DLLEXPORT void *CreateObject(int64_t objectType);

// <vcc:dllInterface gen="REPLACE">
DLLEXPORT int64_t ApplicationClearFormAction(void *form);
DLLEXPORT bool ApplicationCloseForm(void *form, bool isForce);
DLLEXPORT void *ApplicationCreateActionArgument(int64_t objectType);
DLLEXPORT void *ApplicationCreateForm(int64_t objectType);
DLLEXPORT void ApplicationDoFormAction(void *form, int64_t formProperty, void *argument);
DLLEXPORT int64_t ApplicationGetFormActionFirstSeqNo(void *form);
DLLEXPORT int64_t ApplicationGetFormActionLastSeqNo(void *form);
DLLEXPORT bool ApplicationIsFormClosable(void *form);
DLLEXPORT bool ApplicationIsFormClosed(void *form);
DLLEXPORT int64_t ApplicationRedoFormAction(void *form, int64_t noOfStep);
DLLEXPORT int64_t ApplicationRedoFormActionToSeqNo(void *form, int64_t seqNo);
DLLEXPORT void ApplicationStart();
DLLEXPORT int64_t ApplicationTruncateFormAction(void *form);
DLLEXPORT int64_t ApplicationUndoFormAction(void *form, int64_t noOfStep);
DLLEXPORT int64_t ApplicationUndoFormActionToSeqNo(void *form, int64_t seqNo);
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(bool, Bool)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(char, Char)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(wchar_t, Wchar)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(int, Int8)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(int, Uint8)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(int, Short)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(int, UnsignedShort)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(long, Int)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(long, UnsignedInt)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(long, Long)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(long, UnsignedLong)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(float, Float)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(double, Double)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_STRING
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_OBJECT
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_CONTAINER
// </vcc:dllInterface>
}

#endif