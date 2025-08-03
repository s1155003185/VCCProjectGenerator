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
// </vcc:dllInterfaceHeader>

extern "C"
{

DLLEXPORT int getVersion(wchar_t **str);

// <vcc:dllInterface gen="REPLACE">
DLLEXPORT int64_t applicationClearFormAction(void *form);
DLLEXPORT bool applicationCloseForm(void *form, bool isForce);
DLLEXPORT void *applicationCreateActionArgument(int64_t objectType);
DLLEXPORT void *applicationCreateForm(int64_t objectType);
DLLEXPORT void *applicationDoFormAction(void *form, int64_t formProperty, void *argument);
DLLEXPORT void applicationEraseResult(void *result);
DLLEXPORT int64_t applicationGetFormActionCurrentSeqNo(void *form);
DLLEXPORT int64_t applicationGetFormActionFirstSeqNo(void *form);
DLLEXPORT int64_t applicationGetFormActionLastSeqNo(void *form);
DLLEXPORT int64_t applicationGetResultErrorCode(void *result);
DLLEXPORT void applicationGetResultMessage(void *result, wchar_t **value);
DLLEXPORT bool applicationIsErrorResult(void *result);
DLLEXPORT bool applicationIsFormClosable(void *form);
DLLEXPORT bool applicationIsFormClosed(void *form);
DLLEXPORT bool applicationIsWarningResult(void *result);
DLLEXPORT void applicationRedoFormAction(void *form, int64_t noOfStep);
DLLEXPORT void applicationRedoFormActionToSeqNo(void *form, int64_t seqNo);
DLLEXPORT void applicationStart();
DLLEXPORT int64_t applicationTruncateFormAction(void *form);
DLLEXPORT void applicationUndoFormAction(void *form, int64_t noOfStep);
DLLEXPORT void applicationUndoFormActionToSeqNo(void *form, int64_t seqNo);
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