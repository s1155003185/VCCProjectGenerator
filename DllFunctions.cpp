// <vcc:vccproj gen="DEMAND"/>
#include "DllFunctions.h"

#include <locale.h>
#include <stdio.h>
#include <wchar.h>


// <vcc:dllInterfaceHeader gen="REPLACE">
#include "application.hpp"
#include "exception_macro.hpp"
#include "i_object.hpp"
#include "lock_type.hpp"
#include "object_type.hpp"
#include "property_accessor_factory.hpp"
#include "property_accessor_macro.hpp"
// </vcc:dllInterfaceHeader>

int getVersion(wchar_t **str)
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

// <vcc:dllInterface gen="REPLACE">

int64_t applicationClearFormAction(void *form)
{
    TRY
        return Application::clearFormAction(static_cast<vcc::IObject *>(form));
    CATCH
    return -1;
}

bool applicationCloseForm(void *form, bool isForce)
{
    TRY
        return Application::closeForm(static_cast<vcc::IObject *>(form), isForce);
    CATCH
    return false;
}

void *applicationCreateActionArgument(int64_t objectType)
{
    TRY
        return Application::createActionArgument(static_cast<ObjectType>(objectType)).get();
    CATCH
    return nullptr;
}

void *applicationCreateForm(int64_t objectType)
{
    TRY
        return Application::createForm(static_cast<ObjectType>(objectType)).get();
    CATCH
    return nullptr;
}

void *applicationDoFormAction(void *form, int64_t formProperty, void *argument)
{
    TRY
        return Application::doFormAction(static_cast<vcc::IObject *>(form), formProperty, static_cast<vcc::IObject *>(argument)).get();
    CATCH
    return nullptr;
}

void applicationEraseResult(void *result)
{
    TRY
        Application::eraseResult(static_cast<vcc::IObject *>(result));
    CATCH
}

int64_t applicationGetFormActionCurrentSeqNo(void *form)
{
    TRY
        return Application::getFormActionCurrentSeqNo(static_cast<vcc::IObject *>(form));
    CATCH
    return -1;
}

int64_t applicationGetFormActionFirstSeqNo(void *form)
{
    TRY
        return Application::getFormActionFirstSeqNo(static_cast<vcc::IObject *>(form));
    CATCH
    return -1;
}

int64_t applicationGetFormActionLastSeqNo(void *form)
{
    TRY
        return Application::getFormActionLastSeqNo(static_cast<vcc::IObject *>(form));
    CATCH
    return -1;
}

int64_t applicationGetResultErrorCode(void *result)
{
    TRY
        return Application::getResultErrorCode(static_cast<vcc::IObject *>(result));
    CATCH
    return 0;
}

void applicationGetResultMessage(void *result, wchar_t **value)
{
    TRY
        std::wstring message = Application::getResultMessage(static_cast<vcc::IObject *>(result));
        size_t size = (message.length() + 1) * sizeof(wchar_t);
        *value = static_cast<wchar_t*>(malloc(size));
        wcscpy(*value, message.c_str());
    CATCH
}

bool applicationIsErrorResult(void *result)
{
    TRY
        return Application::isErrorResult(static_cast<vcc::IObject *>(result));
    CATCH
    return false;
}

bool applicationIsFormClosed(void *form)
{
    TRY
        return Application::isFormClosed(static_cast<vcc::IObject *>(form));
    CATCH
    return false;
}

bool applicationIsFormClosable(void *form)
{
    TRY
        return Application::isFormClosable(static_cast<vcc::IObject *>(form));
    CATCH
    return false;
}

bool applicationIsWarningResult(void *result)
{
    TRY
        return Application::isWarningResult(static_cast<vcc::IObject *>(result));
    CATCH
    return false;
}

void applicationRedoFormAction(void *form, int64_t noOfStep)
{
    TRY
        Application::redoFormAction(static_cast<vcc::IObject *>(form), noOfStep);
    CATCH
}

void applicationRedoFormActionToSeqNo(void *form, int64_t seqNo)
{
    TRY
        Application::redoFormActionToSeqNo(static_cast<vcc::IObject *>(form), seqNo);
    CATCH
}

void applicationStart()
{
    TRY
        Application::Run();
    CATCH
}

int64_t applicationTruncateFormAction(void *form)
{
    TRY
        return Application::truncateFormAction(static_cast<vcc::IObject *>(form));
    CATCH
    return -1;
}

void applicationUndoFormAction(void *form, int64_t noOfStep)
{
    TRY
        Application::undoFormAction(static_cast<vcc::IObject *>(form), noOfStep);
    CATCH
}

void applicationUndoFormActionToSeqNo(void *form, int64_t seqNo)
{
    TRY
        Application::undoFormActionToSeqNo(static_cast<vcc::IObject *>(form), seqNo);
    CATCH
}
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(bool, Bool, false)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(char, Char, L'\0')
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(wchar_t, Wchar, L'\0')
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(int, Int8, 0)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(int, Uint8, 0)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(int, Short, 0)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(int, UnsignedShort, 0)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(long, Int, 0)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(long, UnsignedInt, 0)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(long, Long, 0)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(long, UnsignedLong, 0)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(float, Float, 0)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(double, Double, 0)
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_STRING
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_OBJECT
PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_CONTAINER
// </vcc:dllInterface>