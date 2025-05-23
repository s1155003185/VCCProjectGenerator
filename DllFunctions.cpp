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

using namespace vcc;
// </vcc:dllInterfaceHeader>

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

#include "object_factory.hpp"
std::shared_ptr<IObject> obj = nullptr;
void *CreateObject(int64_t objectType)
{
    ObjectType tmpObjectType = static_cast<ObjectType>(objectType);
    obj = ObjectFactory::Create(tmpObjectType);
    return obj.get();
}

// <vcc:dllInterface gen="REPLACE">

int64_t ApplicationClearFormAction(void *form)
{
    TRY
        return Application::ClearFormAction(static_cast<IObject *>(form));
    CATCH
    return -1;
}

bool ApplicationCloseForm(void *form, bool isForce)
{
    TRY
        return Application::CloseForm(static_cast<IObject *>(form), isForce);
    CATCH
    return false;
}

void *ApplicationCreateActionArgument(int64_t objectType)
{
    TRY
        return Application::CreateActionArgument(static_cast<ObjectType>(objectType)).get();
    CATCH
    return nullptr;
}

void *ApplicationCreateForm(int64_t objectType)
{
    TRY
        return Application::CreateForm(static_cast<ObjectType>(objectType)).get();
    CATCH
    return nullptr;
}

void *ApplicationDoFormAction(void *form, int64_t formProperty, void *argument)
{
    TRY
        return Application::DoFormAction(static_cast<IObject *>(form), formProperty, static_cast<IObject *>(argument)).get();
    CATCH
    return nullptr;
}

void ApplicationEraseResult(void *result)
{
    TRY
        Application::EraseResult(static_cast<IObject *>(result));
    CATCH
}

int64_t ApplicationGetFormActionCurrentSeqNo(void *form)
{
    TRY
        return Application::GetFormActionCurrentSeqNo(static_cast<IObject *>(form));
    CATCH
    return -1;
}

int64_t ApplicationGetFormActionFirstSeqNo(void *form)
{
    TRY
        return Application::GetFormActionFirstSeqNo(static_cast<IObject *>(form));
    CATCH
    return -1;
}

int64_t ApplicationGetFormActionLastSeqNo(void *form)
{
    TRY
        return Application::GetFormActionLastSeqNo(static_cast<IObject *>(form));
    CATCH
    return -1;
}

int64_t ApplicationGetResultErrorCode(void *result)
{
    TRY
        return Application::GetResultErrorCode(static_cast<IObject *>(result));
    CATCH
    return 0;
}

void ApplicationGetResultMessage(void *result, wchar_t **value)
{
    TRY
        std::wstring message = Application::GetResultMessage(static_cast<IObject *>(result));
        size_t size = (message.length() + 1) * sizeof(wchar_t);
        *value = static_cast<wchar_t*>(malloc(size));
        wcscpy(*value, message.c_str());
    CATCH
}

bool ApplicationIsErrorResult(void *result)
{
    TRY
        return Application::IsErrorResult(static_cast<IObject *>(result));
    CATCH
    return false;
}

bool ApplicationIsFormClosed(void *form)
{
    TRY
        return Application::IsFormClosed(static_cast<IObject *>(form));
    CATCH
    return false;
}

bool ApplicationIsFormClosable(void *form)
{
    TRY
        return Application::IsFormClosable(static_cast<IObject *>(form));
    CATCH
    return false;
}

bool ApplicationIsWarningResult(void *result)
{
    TRY
        return Application::IsWarningResult(static_cast<IObject *>(result));
    CATCH
    return false;
}

void ApplicationRedoFormAction(void *form, int64_t noOfStep)
{
    TRY
        Application::RedoFormAction(static_cast<IObject *>(form), noOfStep);
    CATCH
}

void ApplicationRedoFormActionToSeqNo(void *form, int64_t seqNo)
{
    TRY
        Application::RedoFormActionToSeqNo(static_cast<IObject *>(form), seqNo);
    CATCH
}

void ApplicationStart()
{
    TRY
        Application::Run();
    CATCH
}

int64_t ApplicationTruncateFormAction(void *form)
{
    TRY
        return Application::TruncateFormAction(static_cast<IObject *>(form));
    CATCH
    return -1;
}

void ApplicationUndoFormAction(void *form, int64_t noOfStep)
{
    TRY
        Application::UndoFormAction(static_cast<IObject *>(form), noOfStep);
    CATCH
}

void ApplicationUndoFormActionToSeqNo(void *form, int64_t seqNo)
{
    TRY
        Application::UndoFormActionToSeqNo(static_cast<IObject *>(form), seqNo);
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