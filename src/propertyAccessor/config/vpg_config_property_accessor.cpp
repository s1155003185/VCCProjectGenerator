#include "vpg_config_property_accessor.hpp"

#include <memory>
#include <string>
#include <vector>

#include "exception_macro.hpp"
#include "i_object.hpp"
#include "property_accessor_macro.hpp"
#include "vpg_config.hpp"
#include "vpg_config_interface_type.hpp"
#include "vpg_config_property.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

bool VPGConfigBehaviorPropertyAccessor::_ReadBool(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigBehavior>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigBehaviorProperty>(objectProperty))
        {
        case VPGConfigBehaviorProperty::IsResultThrowException:
            return obj->GetIsResultThrowException();
        default:
            assert(false);
        }
    CATCH
    return false;
}

bool VPGConfigBehaviorPropertyAccessor::_ReadBoolAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

bool VPGConfigBehaviorPropertyAccessor::_ReadBoolAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGConfigBehaviorPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool &value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigBehavior>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigBehaviorProperty>(objectProperty))
        {
        case VPGConfigBehaviorProperty::IsResultThrowException:
            obj->SetIsResultThrowException(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigBehaviorPropertyAccessor::_WriteBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigBehaviorPropertyAccessor::_WriteBoolAtKey(const int64_t &objectProperty, const bool &/*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigBehaviorPropertyAccessor::_InsertBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

bool VPGConfigExportPropertyAccessor::_ReadBool(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigExport>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigExportProperty>(objectProperty))
        {
        case VPGConfigExportProperty::IsExportExternalLib:
            return obj->GetIsExportExternalLib();
        default:
            assert(false);
        }
    CATCH
    return false;
}

bool VPGConfigExportPropertyAccessor::_ReadBoolAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

bool VPGConfigExportPropertyAccessor::_ReadBoolAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGConfigExportPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool &value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigExport>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigExportProperty>(objectProperty))
        {
        case VPGConfigExportProperty::IsExportExternalLib:
            obj->SetIsExportExternalLib(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigExportPropertyAccessor::_WriteBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigExportPropertyAccessor::_WriteBoolAtKey(const int64_t &objectProperty, const bool &/*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigExportPropertyAccessor::_InsertBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

long VPGConfigExportPropertyAccessor::_ReadLong(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigExport>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigExportProperty>(objectProperty))
        {
        case VPGConfigExportProperty::Interface:
            return static_cast<long>(obj->GetInterface());
        default:
            assert(false);
        }
    CATCH
    return 0L;
}

long VPGConfigExportPropertyAccessor::_ReadLongAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

long VPGConfigExportPropertyAccessor::_ReadLongAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

void VPGConfigExportPropertyAccessor::_WriteLong(const int64_t &objectProperty, const long &value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigExport>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigExportProperty>(objectProperty))
        {
        case VPGConfigExportProperty::Interface:
            obj->SetInterface(static_cast<VPGConfigInterfaceType>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigExportPropertyAccessor::_WriteLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigExportPropertyAccessor::_WriteLongAtKey(const int64_t &objectProperty, const long &/*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigExportPropertyAccessor::_InsertLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGConfigExportPropertyAccessor::_ReadString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigExport>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigExportProperty>(objectProperty))
        {
        case VPGConfigExportProperty::DllBridgeDirectory:
            return obj->GetDllBridgeDirectory();
        case VPGConfigExportProperty::ExportDirectoryDll:
            return obj->GetExportDirectoryDll();
        case VPGConfigExportProperty::ExportDirectoryExe:
            return obj->GetExportDirectoryExe();
        case VPGConfigExportProperty::FormDirectory:
            return obj->GetFormDirectory();
        case VPGConfigExportProperty::ObjectDirectory:
            return obj->GetObjectDirectory();
        case VPGConfigExportProperty::TypeDirectory:
            return obj->GetTypeDirectory();
        case VPGConfigExportProperty::Workspace:
            return obj->GetWorkspace();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGConfigExportPropertyAccessor::_ReadStringAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

std::wstring VPGConfigExportPropertyAccessor::_ReadStringAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGConfigExportPropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring &value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigExport>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigExportProperty>(objectProperty))
        {
        case VPGConfigExportProperty::DllBridgeDirectory:
            obj->SetDllBridgeDirectory(value);
            break;
        case VPGConfigExportProperty::ExportDirectoryDll:
            obj->SetExportDirectoryDll(value);
            break;
        case VPGConfigExportProperty::ExportDirectoryExe:
            obj->SetExportDirectoryExe(value);
            break;
        case VPGConfigExportProperty::FormDirectory:
            obj->SetFormDirectory(value);
            break;
        case VPGConfigExportProperty::ObjectDirectory:
            obj->SetObjectDirectory(value);
            break;
        case VPGConfigExportProperty::TypeDirectory:
            obj->SetTypeDirectory(value);
            break;
        case VPGConfigExportProperty::Workspace:
            obj->SetWorkspace(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigExportPropertyAccessor::_WriteStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigExportPropertyAccessor::_WriteStringAtKey(const int64_t &objectProperty, const std::wstring &/*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigExportPropertyAccessor::_InsertStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGConfigInputPropertyAccessor::_ReadString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigInput>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigInputProperty>(objectProperty))
        {
        case VPGConfigInputProperty::TypeWorkspace:
            return obj->GetTypeWorkspace();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGConfigInputPropertyAccessor::_ReadStringAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

std::wstring VPGConfigInputPropertyAccessor::_ReadStringAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGConfigInputPropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring &value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigInput>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigInputProperty>(objectProperty))
        {
        case VPGConfigInputProperty::TypeWorkspace:
            obj->SetTypeWorkspace(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigInputPropertyAccessor::_WriteStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigInputPropertyAccessor::_WriteStringAtKey(const int64_t &objectProperty, const std::wstring &/*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigInputPropertyAccessor::_InsertStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGConfigOutputPropertyAccessor::_ReadString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigOutput>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigOutputProperty>(objectProperty))
        {
        case VPGConfigOutputProperty::ActionDirectoryCpp:
            return obj->GetActionDirectoryCpp();
        case VPGConfigOutputProperty::ActionDirectoryHpp:
            return obj->GetActionDirectoryHpp();
        case VPGConfigOutputProperty::ApplicationDirectoryCpp:
            return obj->GetApplicationDirectoryCpp();
        case VPGConfigOutputProperty::ApplicationDirectoryHpp:
            return obj->GetApplicationDirectoryHpp();
        case VPGConfigOutputProperty::ExceptionTypeDirectory:
            return obj->GetExceptionTypeDirectory();
        case VPGConfigOutputProperty::FormDirectoryCpp:
            return obj->GetFormDirectoryCpp();
        case VPGConfigOutputProperty::FormDirectoryHpp:
            return obj->GetFormDirectoryHpp();
        case VPGConfigOutputProperty::ObjectDirectoryCpp:
            return obj->GetObjectDirectoryCpp();
        case VPGConfigOutputProperty::ObjectDirectoryHpp:
            return obj->GetObjectDirectoryHpp();
        case VPGConfigOutputProperty::ObjectFactoryDirectoryCpp:
            return obj->GetObjectFactoryDirectoryCpp();
        case VPGConfigOutputProperty::ObjectFactoryDirectoryHpp:
            return obj->GetObjectFactoryDirectoryHpp();
        case VPGConfigOutputProperty::ObjectTypeDirectory:
            return obj->GetObjectTypeDirectory();
        case VPGConfigOutputProperty::PropertyAccessorDirectoryCpp:
            return obj->GetPropertyAccessorDirectoryCpp();
        case VPGConfigOutputProperty::PropertyAccessorDirectoryHpp:
            return obj->GetPropertyAccessorDirectoryHpp();
        case VPGConfigOutputProperty::PropertyAccessorFactoryDirectoryCpp:
            return obj->GetPropertyAccessorFactoryDirectoryCpp();
        case VPGConfigOutputProperty::PropertyAccessorFactoryDirectoryHpp:
            return obj->GetPropertyAccessorFactoryDirectoryHpp();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGConfigOutputPropertyAccessor::_ReadStringAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

std::wstring VPGConfigOutputPropertyAccessor::_ReadStringAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGConfigOutputPropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring &value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigOutput>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigOutputProperty>(objectProperty))
        {
        case VPGConfigOutputProperty::ActionDirectoryCpp:
            obj->SetActionDirectoryCpp(value);
            break;
        case VPGConfigOutputProperty::ActionDirectoryHpp:
            obj->SetActionDirectoryHpp(value);
            break;
        case VPGConfigOutputProperty::ApplicationDirectoryCpp:
            obj->SetApplicationDirectoryCpp(value);
            break;
        case VPGConfigOutputProperty::ApplicationDirectoryHpp:
            obj->SetApplicationDirectoryHpp(value);
            break;
        case VPGConfigOutputProperty::ExceptionTypeDirectory:
            obj->SetExceptionTypeDirectory(value);
            break;
        case VPGConfigOutputProperty::FormDirectoryCpp:
            obj->SetFormDirectoryCpp(value);
            break;
        case VPGConfigOutputProperty::FormDirectoryHpp:
            obj->SetFormDirectoryHpp(value);
            break;
        case VPGConfigOutputProperty::ObjectDirectoryCpp:
            obj->SetObjectDirectoryCpp(value);
            break;
        case VPGConfigOutputProperty::ObjectDirectoryHpp:
            obj->SetObjectDirectoryHpp(value);
            break;
        case VPGConfigOutputProperty::ObjectFactoryDirectoryCpp:
            obj->SetObjectFactoryDirectoryCpp(value);
            break;
        case VPGConfigOutputProperty::ObjectFactoryDirectoryHpp:
            obj->SetObjectFactoryDirectoryHpp(value);
            break;
        case VPGConfigOutputProperty::ObjectTypeDirectory:
            obj->SetObjectTypeDirectory(value);
            break;
        case VPGConfigOutputProperty::PropertyAccessorDirectoryCpp:
            obj->SetPropertyAccessorDirectoryCpp(value);
            break;
        case VPGConfigOutputProperty::PropertyAccessorDirectoryHpp:
            obj->SetPropertyAccessorDirectoryHpp(value);
            break;
        case VPGConfigOutputProperty::PropertyAccessorFactoryDirectoryCpp:
            obj->SetPropertyAccessorFactoryDirectoryCpp(value);
            break;
        case VPGConfigOutputProperty::PropertyAccessorFactoryDirectoryHpp:
            obj->SetPropertyAccessorFactoryDirectoryHpp(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigOutputPropertyAccessor::_WriteStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigOutputPropertyAccessor::_WriteStringAtKey(const int64_t &objectProperty, const std::wstring &/*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigOutputPropertyAccessor::_InsertStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

bool VPGConfigPropertyAccessor::_ReadBool(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::IsGit:
            return obj->GetIsGit();
        default:
            assert(false);
        }
    CATCH
    return false;
}

bool VPGConfigPropertyAccessor::_ReadBoolAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

bool VPGConfigPropertyAccessor::_ReadBoolAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGConfigPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool &value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::IsGit:
            obj->SetIsGit(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigPropertyAccessor::_WriteBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigPropertyAccessor::_WriteBoolAtKey(const int64_t &objectProperty, const bool &/*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigPropertyAccessor::_InsertBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

long VPGConfigPropertyAccessor::_ReadLong(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::ProjectType:
            return static_cast<long>(obj->GetProjectType());
        default:
            assert(false);
        }
    CATCH
    return 0L;
}

long VPGConfigPropertyAccessor::_ReadLongAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

long VPGConfigPropertyAccessor::_ReadLongAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

void VPGConfigPropertyAccessor::_WriteLong(const int64_t &objectProperty, const long &value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::ProjectType:
            obj->SetProjectType(static_cast<VPGProjectType>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigPropertyAccessor::_WriteLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigPropertyAccessor::_WriteLongAtKey(const int64_t &objectProperty, const long &/*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigPropertyAccessor::_InsertLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGConfigPropertyAccessor::_ReadString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::ProjectName:
            return obj->GetProjectName();
        case VPGConfigProperty::ProjectNameDll:
            return obj->GetProjectNameDll();
        case VPGConfigProperty::ProjectNameExe:
            return obj->GetProjectNameExe();
        case VPGConfigProperty::ProjectPrefix:
            return obj->GetProjectPrefix();
        case VPGConfigProperty::Version:
            return obj->GetVersion();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGConfigPropertyAccessor::_ReadStringAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Plugins:
            return obj->GetPluginsAtIndex(index);
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGConfigPropertyAccessor::_ReadStringAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGConfigPropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring &value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::ProjectName:
            obj->SetProjectName(value);
            break;
        case VPGConfigProperty::ProjectNameDll:
            obj->SetProjectNameDll(value);
            break;
        case VPGConfigProperty::ProjectNameExe:
            obj->SetProjectNameExe(value);
            break;
        case VPGConfigProperty::ProjectPrefix:
            obj->SetProjectPrefix(value);
            break;
        case VPGConfigProperty::Version:
            obj->SetVersion(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigPropertyAccessor::_WriteStringAtIndex(const int64_t &objectProperty, const std::wstring &value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Plugins:
            if (index > -1)
                obj->SetPluginsAtIndex(index, value);
            else
                obj->InsertPlugins(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigPropertyAccessor::_WriteStringAtKey(const int64_t &objectProperty, const std::wstring &/*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigPropertyAccessor::_InsertStringAtIndex(const int64_t &objectProperty, const std::wstring &value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Plugins:
            if (index > -1)
                obj->InsertPluginsAtIndex(index, value);
            else
                obj->InsertPlugins(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

std::shared_ptr<IObject> VPGConfigPropertyAccessor::_ReadObject(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Behavior:
            return std::static_pointer_cast<IObject>(obj->GetBehavior());
        case VPGConfigProperty::Input:
            return std::static_pointer_cast<IObject>(obj->GetInput());
        case VPGConfigProperty::Ouput:
            return std::static_pointer_cast<IObject>(obj->GetOutput());
        case VPGConfigProperty::Template:
            return std::static_pointer_cast<IObject>(obj->GetTemplate());
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGConfigPropertyAccessor::_ReadObjectAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Exports:
            return std::static_pointer_cast<IObject>(obj->GetExportsAtIndex(index));
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGConfigPropertyAccessor::_ReadObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

void VPGConfigPropertyAccessor::_WriteObject(const int64_t &objectProperty, std::shared_ptr<IObject> value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Behavior:
            obj->SetBehavior(std::static_pointer_cast<VPGConfigBehavior>(value));
            break;
        case VPGConfigProperty::Input:
            obj->SetInput(std::static_pointer_cast<VPGConfigInput>(value));
            break;
        case VPGConfigProperty::Ouput:
            obj->SetOutput(std::static_pointer_cast<VPGConfigOutput>(value));
            break;
        case VPGConfigProperty::Template:
            obj->SetTemplate(std::static_pointer_cast<VPGConfigTemplate>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigPropertyAccessor::_WriteObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Exports:
            if (index > -1)
                obj->SetExportsAtIndex(index, std::static_pointer_cast<VPGConfigExport>(value));
            else
                obj->InsertExports(std::static_pointer_cast<VPGConfigExport>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigPropertyAccessor::_WriteObjectAtKey(const int64_t &objectProperty, std::shared_ptr<IObject> /*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigPropertyAccessor::_InsertObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Exports:
            if (index > -1)
                obj->InsertExportsAtIndex(index, std::static_pointer_cast<VPGConfigExport>(value));
            else
                obj->InsertExports(std::static_pointer_cast<VPGConfigExport>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

std::shared_ptr<IObject> VPGConfigPropertyAccessor::_CloneObject(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Behavior:
            return std::static_pointer_cast<IObject>(obj->GetBehavior()->Clone());
        case VPGConfigProperty::Input:
            return std::static_pointer_cast<IObject>(obj->GetInput()->Clone());
        case VPGConfigProperty::Ouput:
            return std::static_pointer_cast<IObject>(obj->GetOutput()->Clone());
        case VPGConfigProperty::Template:
            return std::static_pointer_cast<IObject>(obj->GetTemplate()->Clone());
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGConfigPropertyAccessor::_CloneObjectAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Exports:
            return std::static_pointer_cast<IObject>(obj->CloneExportsAtIndex(index));
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGConfigPropertyAccessor::_CloneObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

size_t VPGConfigPropertyAccessor::_GetCount(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Plugins:
            return obj->GetPlugins().size();
        case VPGConfigProperty::Exports:
            return obj->GetExports().size();
        default:
            assert(false);
        }
    CATCH
    return 0;
}

std::set<void *> VPGConfigPropertyAccessor::_GetMapKeys(const int64_t &objectProperty) const
{
    std::set<void *> result;
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return result;
}

bool VPGConfigPropertyAccessor::_IsContainKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGConfigPropertyAccessor::_Remove(const int64_t &objectProperty, const void *value) const
{
    TRY
        assert(value != nullptr);
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Plugins: {
            auto valuePtr = static_cast<const wchar_t *>(value);
            assert(valuePtr != nullptr);
            obj->RemovePlugins(valuePtr);
            break;
        }
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigPropertyAccessor::_RemoveObject(const int64_t &objectProperty, const IObject *value) const
{
    TRY
        assert(value != nullptr);
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Exports:
            obj->RemoveExports(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigPropertyAccessor::_RemoveAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Exports:
            obj->RemoveExportsAtIndex(index);
            break;
        case VPGConfigProperty::Plugins:
            obj->RemovePluginsAtIndex(index);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigPropertyAccessor::_RemoveAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigPropertyAccessor::_Clear(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Plugins:
            obj->ClearPlugins();
            break;
        case VPGConfigProperty::Exports:
            obj->ClearExports();
            break;
        default:
            assert(false);
        }
    CATCH
}

bool VPGConfigTemplatePropertyAccessor::_ReadBool(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigTemplate>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigTemplateProperty>(objectProperty))
        {
        case VPGConfigTemplateProperty::IsExcludeUnittest:
            return obj->GetIsExcludeUnittest();
        case VPGConfigTemplateProperty::IsExcludeVCCUnitTest:
            return obj->GetIsExcludeVCCUnitTest();
        default:
            assert(false);
        }
    CATCH
    return false;
}

bool VPGConfigTemplatePropertyAccessor::_ReadBoolAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

bool VPGConfigTemplatePropertyAccessor::_ReadBoolAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGConfigTemplatePropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool &value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigTemplate>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigTemplateProperty>(objectProperty))
        {
        case VPGConfigTemplateProperty::IsExcludeUnittest:
            obj->SetIsExcludeUnittest(value);
            break;
        case VPGConfigTemplateProperty::IsExcludeVCCUnitTest:
            obj->SetIsExcludeVCCUnitTest(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigTemplatePropertyAccessor::_WriteBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigTemplatePropertyAccessor::_WriteBoolAtKey(const int64_t &objectProperty, const bool &/*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigTemplatePropertyAccessor::_InsertBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGConfigTemplatePropertyAccessor::_ReadString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigTemplate>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigTemplateProperty>(objectProperty))
        {
        case VPGConfigTemplateProperty::Url:
            return obj->GetUrl();
        case VPGConfigTemplateProperty::Workspace:
            return obj->GetWorkspace();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGConfigTemplatePropertyAccessor::_ReadStringAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

std::wstring VPGConfigTemplatePropertyAccessor::_ReadStringAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGConfigTemplatePropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring &value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigTemplate>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigTemplateProperty>(objectProperty))
        {
        case VPGConfigTemplateProperty::Url:
            obj->SetUrl(value);
            break;
        case VPGConfigTemplateProperty::Workspace:
            obj->SetWorkspace(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigTemplatePropertyAccessor::_WriteStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigTemplatePropertyAccessor::_WriteStringAtKey(const int64_t &objectProperty, const std::wstring &/*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigTemplatePropertyAccessor::_InsertStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}
