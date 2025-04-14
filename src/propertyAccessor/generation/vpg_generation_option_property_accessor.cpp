#include "vpg_generation_option_property_accessor.hpp"

#include <memory>
#include <string>
#include <vector>

#include "exception_macro.hpp"
#include "i_object.hpp"
#include "property_accessor_macro.hpp"
#include "vpg_generation_option.hpp"
#include "vpg_generation_option_interface_type.hpp"
#include "vpg_generation_option_property.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

bool VPGGenerationOptionExportPropertyAccessor::_ReadBool(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGenerationOptionExport>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionExportProperty>(objectProperty))
        {
        case VPGGenerationOptionExportProperty::IsExportExternalLib:
            return obj->GetIsExportExternalLib();
        default:
            assert(false);
        }
    CATCH
    return false;
}

bool VPGGenerationOptionExportPropertyAccessor::_ReadBoolAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

bool VPGGenerationOptionExportPropertyAccessor::_ReadBoolAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGGenerationOptionExportPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool &value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGenerationOptionExport>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionExportProperty>(objectProperty))
        {
        case VPGGenerationOptionExportProperty::IsExportExternalLib:
            obj->SetIsExportExternalLib(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGenerationOptionExportPropertyAccessor::_WriteBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGenerationOptionExportPropertyAccessor::_WriteBoolAtKey(const int64_t &objectProperty, const bool &/*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGenerationOptionExportPropertyAccessor::_InsertBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

long VPGGenerationOptionExportPropertyAccessor::_ReadLong(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGenerationOptionExport>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionExportProperty>(objectProperty))
        {
        case VPGGenerationOptionExportProperty::Interface:
            return static_cast<long>(obj->GetInterface());
        default:
            assert(false);
        }
    CATCH
    return 0L;
}

long VPGGenerationOptionExportPropertyAccessor::_ReadLongAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

long VPGGenerationOptionExportPropertyAccessor::_ReadLongAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

void VPGGenerationOptionExportPropertyAccessor::_WriteLong(const int64_t &objectProperty, const long &value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGenerationOptionExport>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionExportProperty>(objectProperty))
        {
        case VPGGenerationOptionExportProperty::Interface:
            obj->SetInterface(static_cast<VPGGenerationOptionInterfaceType>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGenerationOptionExportPropertyAccessor::_WriteLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGenerationOptionExportPropertyAccessor::_WriteLongAtKey(const int64_t &objectProperty, const long &/*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGenerationOptionExportPropertyAccessor::_InsertLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGGenerationOptionExportPropertyAccessor::_ReadString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGenerationOptionExport>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionExportProperty>(objectProperty))
        {
        case VPGGenerationOptionExportProperty::DllBridgeDirectory:
            return obj->GetDllBridgeDirectory();
        case VPGGenerationOptionExportProperty::ExportDirectoryDll:
            return obj->GetExportDirectoryDll();
        case VPGGenerationOptionExportProperty::ExportDirectoryExe:
            return obj->GetExportDirectoryExe();
        case VPGGenerationOptionExportProperty::FormDirectory:
            return obj->GetFormDirectory();
        case VPGGenerationOptionExportProperty::ObjectDirectory:
            return obj->GetObjectDirectory();
        case VPGGenerationOptionExportProperty::TypeDirectory:
            return obj->GetTypeDirectory();
        case VPGGenerationOptionExportProperty::Workspace:
            return obj->GetWorkspace();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGGenerationOptionExportPropertyAccessor::_ReadStringAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

std::wstring VPGGenerationOptionExportPropertyAccessor::_ReadStringAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGGenerationOptionExportPropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring &value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGenerationOptionExport>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionExportProperty>(objectProperty))
        {
        case VPGGenerationOptionExportProperty::DllBridgeDirectory:
            obj->SetDllBridgeDirectory(value);
            break;
        case VPGGenerationOptionExportProperty::ExportDirectoryDll:
            obj->SetExportDirectoryDll(value);
            break;
        case VPGGenerationOptionExportProperty::ExportDirectoryExe:
            obj->SetExportDirectoryExe(value);
            break;
        case VPGGenerationOptionExportProperty::FormDirectory:
            obj->SetFormDirectory(value);
            break;
        case VPGGenerationOptionExportProperty::ObjectDirectory:
            obj->SetObjectDirectory(value);
            break;
        case VPGGenerationOptionExportProperty::TypeDirectory:
            obj->SetTypeDirectory(value);
            break;
        case VPGGenerationOptionExportProperty::Workspace:
            obj->SetWorkspace(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGenerationOptionExportPropertyAccessor::_WriteStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGenerationOptionExportPropertyAccessor::_WriteStringAtKey(const int64_t &objectProperty, const std::wstring &/*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGenerationOptionExportPropertyAccessor::_InsertStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

bool VPGGenerationOptionPropertyAccessor::_ReadBool(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::IsExcludeUnittest:
            return obj->GetIsExcludeUnittest();
        case VPGGenerationOptionProperty::IsExcludeVCCUnitTest:
            return obj->GetIsExcludeVCCUnitTest();
        case VPGGenerationOptionProperty::IsGit:
            return obj->GetIsGit();
        case VPGGenerationOptionProperty::IsResultThrowException:
            return obj->GetIsResultThrowException();
        default:
            assert(false);
        }
    CATCH
    return false;
}

bool VPGGenerationOptionPropertyAccessor::_ReadBoolAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

bool VPGGenerationOptionPropertyAccessor::_ReadBoolAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGGenerationOptionPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool &value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::IsExcludeUnittest:
            obj->SetIsExcludeUnittest(value);
            break;
        case VPGGenerationOptionProperty::IsExcludeVCCUnitTest:
            obj->SetIsExcludeVCCUnitTest(value);
            break;
        case VPGGenerationOptionProperty::IsGit:
            obj->SetIsGit(value);
            break;
        case VPGGenerationOptionProperty::IsResultThrowException:
            obj->SetIsResultThrowException(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_WriteBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_WriteBoolAtKey(const int64_t &objectProperty, const bool &/*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_InsertBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

long VPGGenerationOptionPropertyAccessor::_ReadLong(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::ProjectType:
            return static_cast<long>(obj->GetProjectType());
        default:
            assert(false);
        }
    CATCH
    return 0L;
}

long VPGGenerationOptionPropertyAccessor::_ReadLongAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

long VPGGenerationOptionPropertyAccessor::_ReadLongAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

void VPGGenerationOptionPropertyAccessor::_WriteLong(const int64_t &objectProperty, const long &value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::ProjectType:
            obj->SetProjectType(static_cast<VPGProjectType>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_WriteLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_WriteLongAtKey(const int64_t &objectProperty, const long &/*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_InsertLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGGenerationOptionPropertyAccessor::_ReadString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::ActionDirectoryCpp:
            return obj->GetActionDirectoryCpp();
        case VPGGenerationOptionProperty::ActionDirectoryHpp:
            return obj->GetActionDirectoryHpp();
        case VPGGenerationOptionProperty::ApplicationDirectoryCpp:
            return obj->GetApplicationDirectoryCpp();
        case VPGGenerationOptionProperty::ApplicationDirectoryHpp:
            return obj->GetApplicationDirectoryHpp();
        case VPGGenerationOptionProperty::ExceptionTypeDirectory:
            return obj->GetExceptionTypeDirectory();
        case VPGGenerationOptionProperty::FormDirectoryCpp:
            return obj->GetFormDirectoryCpp();
        case VPGGenerationOptionProperty::FormDirectoryHpp:
            return obj->GetFormDirectoryHpp();
        case VPGGenerationOptionProperty::ObjectDirectoryCpp:
            return obj->GetObjectDirectoryCpp();
        case VPGGenerationOptionProperty::ObjectDirectoryHpp:
            return obj->GetObjectDirectoryHpp();
        case VPGGenerationOptionProperty::ObjectFactoryDirectoryCpp:
            return obj->GetObjectFactoryDirectoryCpp();
        case VPGGenerationOptionProperty::ObjectFactoryDirectoryHpp:
            return obj->GetObjectFactoryDirectoryHpp();
        case VPGGenerationOptionProperty::ObjectTypeDirectory:
            return obj->GetObjectTypeDirectory();
        case VPGGenerationOptionProperty::ProjectName:
            return obj->GetProjectName();
        case VPGGenerationOptionProperty::ProjectNameDll:
            return obj->GetProjectNameDll();
        case VPGGenerationOptionProperty::ProjectNameExe:
            return obj->GetProjectNameExe();
        case VPGGenerationOptionProperty::ProjectPrefix:
            return obj->GetProjectPrefix();
        case VPGGenerationOptionProperty::PropertyAccessorDirectoryCpp:
            return obj->GetPropertyAccessorDirectoryCpp();
        case VPGGenerationOptionProperty::PropertyAccessorDirectoryHpp:
            return obj->GetPropertyAccessorDirectoryHpp();
        case VPGGenerationOptionProperty::PropertyAccessorFactoryDirectoryCpp:
            return obj->GetPropertyAccessorFactoryDirectoryCpp();
        case VPGGenerationOptionProperty::PropertyAccessorFactoryDirectoryHpp:
            return obj->GetPropertyAccessorFactoryDirectoryHpp();
        case VPGGenerationOptionProperty::TemplateGitUrl:
            return obj->GetTemplateGitUrl();
        case VPGGenerationOptionProperty::TemplateWorkspace:
            return obj->GetTemplateWorkspace();
        case VPGGenerationOptionProperty::TypeWorkspace:
            return obj->GetTypeWorkspace();
        case VPGGenerationOptionProperty::Version:
            return obj->GetVersion();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGGenerationOptionPropertyAccessor::_ReadStringAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::Plugins:
            return obj->GetPluginsAtIndex(index);
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGGenerationOptionPropertyAccessor::_ReadStringAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGGenerationOptionPropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring &value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::ActionDirectoryCpp:
            obj->SetActionDirectoryCpp(value);
            break;
        case VPGGenerationOptionProperty::ActionDirectoryHpp:
            obj->SetActionDirectoryHpp(value);
            break;
        case VPGGenerationOptionProperty::ApplicationDirectoryCpp:
            obj->SetApplicationDirectoryCpp(value);
            break;
        case VPGGenerationOptionProperty::ApplicationDirectoryHpp:
            obj->SetApplicationDirectoryHpp(value);
            break;
        case VPGGenerationOptionProperty::ExceptionTypeDirectory:
            obj->SetExceptionTypeDirectory(value);
            break;
        case VPGGenerationOptionProperty::FormDirectoryCpp:
            obj->SetFormDirectoryCpp(value);
            break;
        case VPGGenerationOptionProperty::FormDirectoryHpp:
            obj->SetFormDirectoryHpp(value);
            break;
        case VPGGenerationOptionProperty::ObjectDirectoryCpp:
            obj->SetObjectDirectoryCpp(value);
            break;
        case VPGGenerationOptionProperty::ObjectDirectoryHpp:
            obj->SetObjectDirectoryHpp(value);
            break;
        case VPGGenerationOptionProperty::ObjectFactoryDirectoryCpp:
            obj->SetObjectFactoryDirectoryCpp(value);
            break;
        case VPGGenerationOptionProperty::ObjectFactoryDirectoryHpp:
            obj->SetObjectFactoryDirectoryHpp(value);
            break;
        case VPGGenerationOptionProperty::ObjectTypeDirectory:
            obj->SetObjectTypeDirectory(value);
            break;
        case VPGGenerationOptionProperty::ProjectName:
            obj->SetProjectName(value);
            break;
        case VPGGenerationOptionProperty::ProjectNameDll:
            obj->SetProjectNameDll(value);
            break;
        case VPGGenerationOptionProperty::ProjectNameExe:
            obj->SetProjectNameExe(value);
            break;
        case VPGGenerationOptionProperty::ProjectPrefix:
            obj->SetProjectPrefix(value);
            break;
        case VPGGenerationOptionProperty::PropertyAccessorDirectoryCpp:
            obj->SetPropertyAccessorDirectoryCpp(value);
            break;
        case VPGGenerationOptionProperty::PropertyAccessorDirectoryHpp:
            obj->SetPropertyAccessorDirectoryHpp(value);
            break;
        case VPGGenerationOptionProperty::PropertyAccessorFactoryDirectoryCpp:
            obj->SetPropertyAccessorFactoryDirectoryCpp(value);
            break;
        case VPGGenerationOptionProperty::PropertyAccessorFactoryDirectoryHpp:
            obj->SetPropertyAccessorFactoryDirectoryHpp(value);
            break;
        case VPGGenerationOptionProperty::TemplateGitUrl:
            obj->SetTemplateGitUrl(value);
            break;
        case VPGGenerationOptionProperty::TemplateWorkspace:
            obj->SetTemplateWorkspace(value);
            break;
        case VPGGenerationOptionProperty::TypeWorkspace:
            obj->SetTypeWorkspace(value);
            break;
        case VPGGenerationOptionProperty::Version:
            obj->SetVersion(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_WriteStringAtIndex(const int64_t &objectProperty, const std::wstring &value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::Plugins:
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

void VPGGenerationOptionPropertyAccessor::_WriteStringAtKey(const int64_t &objectProperty, const std::wstring &/*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_InsertStringAtIndex(const int64_t &objectProperty, const std::wstring &value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::Plugins:
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

std::shared_ptr<IObject> VPGGenerationOptionPropertyAccessor::_ReadObject(const int64_t &objectProperty) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGGenerationOptionPropertyAccessor::_ReadObjectAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::Exports:
            return std::static_pointer_cast<IObject>(obj->GetExportsAtIndex(index));
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGGenerationOptionPropertyAccessor::_ReadObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

void VPGGenerationOptionPropertyAccessor::_WriteObject(const int64_t &objectProperty, std::shared_ptr<IObject> /*value*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_WriteObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::Exports:
            if (index > -1)
                obj->SetExportsAtIndex(index, std::static_pointer_cast<VPGGenerationOptionExport>(value));
            else
                obj->InsertExports(std::static_pointer_cast<VPGGenerationOptionExport>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_WriteObjectAtKey(const int64_t &objectProperty, std::shared_ptr<IObject> /*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_InsertObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::Exports:
            if (index > -1)
                obj->InsertExportsAtIndex(index, std::static_pointer_cast<VPGGenerationOptionExport>(value));
            else
                obj->InsertExports(std::static_pointer_cast<VPGGenerationOptionExport>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

std::shared_ptr<IObject> VPGGenerationOptionPropertyAccessor::_CloneObject(const int64_t &objectProperty) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGGenerationOptionPropertyAccessor::_CloneObjectAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::Exports:
            return std::static_pointer_cast<IObject>(obj->CloneExportsAtIndex(index));
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGGenerationOptionPropertyAccessor::_CloneObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

size_t VPGGenerationOptionPropertyAccessor::_GetCount(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::Plugins:
            return obj->GetPlugins().size();
        case VPGGenerationOptionProperty::Exports:
            return obj->GetExports().size();
        default:
            assert(false);
        }
    CATCH
    return 0;
}

std::set<void *> VPGGenerationOptionPropertyAccessor::_GetMapKeys(const int64_t &objectProperty) const
{
    std::set<void *> result;
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return result;
}

bool VPGGenerationOptionPropertyAccessor::_IsContainKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGGenerationOptionPropertyAccessor::_Remove(const int64_t &objectProperty, const void *value) const
{
    TRY
        assert(value != nullptr);
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::Plugins: {
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

void VPGGenerationOptionPropertyAccessor::_RemoveObject(const int64_t &objectProperty, const IObject *value) const
{
    TRY
        assert(value != nullptr);
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::Exports:
            obj->RemoveExports(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_RemoveAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::Exports:
            obj->RemoveExportsAtIndex(index);
            break;
        case VPGGenerationOptionProperty::Plugins:
            obj->RemovePluginsAtIndex(index);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_RemoveAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_Clear(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::Plugins:
            obj->ClearPlugins();
            break;
        case VPGGenerationOptionProperty::Exports:
            obj->ClearExports();
            break;
        default:
            assert(false);
        }
    CATCH
}
