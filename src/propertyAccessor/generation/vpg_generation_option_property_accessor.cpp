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

bool VPGGenerationOptionExportPropertyAccessor::_ReadBool(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
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

bool VPGGenerationOptionExportPropertyAccessor::_ReadBool(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGGenerationOptionExportPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool &value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
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

void VPGGenerationOptionExportPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool & /*value*/, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGenerationOptionExportPropertyAccessor::_InsertBool(const int64_t &objectProperty, const bool & /*value*/, const int64_t & /*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

long VPGGenerationOptionExportPropertyAccessor::_ReadLong(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
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

long VPGGenerationOptionExportPropertyAccessor::_ReadLong(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

void VPGGenerationOptionExportPropertyAccessor::_WriteLong(const int64_t &objectProperty, const long &value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
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

void VPGGenerationOptionExportPropertyAccessor::_WriteLong(const int64_t &objectProperty, const long & /*value*/, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGenerationOptionExportPropertyAccessor::_InsertLong(const int64_t &objectProperty, const long & /*value*/, const int64_t & /*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGGenerationOptionExportPropertyAccessor::_ReadString(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGenerationOptionExport>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionExportProperty>(objectProperty))
        {
        case VPGGenerationOptionExportProperty::Workspace:
            return obj->GetWorkspace();
        case VPGGenerationOptionExportProperty::ExportDirectoryDll:
            return obj->GetExportDirectoryDll();
        case VPGGenerationOptionExportProperty::ExportDirectoryExe:
            return obj->GetExportDirectoryExe();
        case VPGGenerationOptionExportProperty::DllBridgeDirectory:
            return obj->GetDllBridgeDirectory();
        case VPGGenerationOptionExportProperty::FormDirectory:
            return obj->GetFormDirectory();
        case VPGGenerationOptionExportProperty::ObjectDirectory:
            return obj->GetObjectDirectory();
        case VPGGenerationOptionExportProperty::TypeDirectory:
            return obj->GetTypeDirectory();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGGenerationOptionExportPropertyAccessor::_ReadString(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGGenerationOptionExportPropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring &value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGenerationOptionExport>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionExportProperty>(objectProperty))
        {
        case VPGGenerationOptionExportProperty::Workspace:
            obj->SetWorkspace(value);
            break;
        case VPGGenerationOptionExportProperty::ExportDirectoryDll:
            obj->SetExportDirectoryDll(value);
            break;
        case VPGGenerationOptionExportProperty::ExportDirectoryExe:
            obj->SetExportDirectoryExe(value);
            break;
        case VPGGenerationOptionExportProperty::DllBridgeDirectory:
            obj->SetDllBridgeDirectory(value);
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
        default:
            assert(false);
        }
    CATCH
}

void VPGGenerationOptionExportPropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring & /*value*/, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGenerationOptionExportPropertyAccessor::_InsertString(const int64_t &objectProperty, const std::wstring & /*value*/, const int64_t & /*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

bool VPGGenerationOptionPropertyAccessor::_ReadBool(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::IsGit:
            return obj->GetIsGit();
        case VPGGenerationOptionProperty::IsExcludeUnittest:
            return obj->GetIsExcludeUnittest();
        case VPGGenerationOptionProperty::IsExcludeVCCUnitTest:
            return obj->GetIsExcludeVCCUnitTest();
        default:
            assert(false);
        }
    CATCH
    return false;
}

bool VPGGenerationOptionPropertyAccessor::_ReadBool(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGGenerationOptionPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool &value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::IsGit:
            obj->SetIsGit(value);
            break;
        case VPGGenerationOptionProperty::IsExcludeUnittest:
            obj->SetIsExcludeUnittest(value);
            break;
        case VPGGenerationOptionProperty::IsExcludeVCCUnitTest:
            obj->SetIsExcludeVCCUnitTest(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool & /*value*/, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_InsertBool(const int64_t &objectProperty, const bool & /*value*/, const int64_t & /*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

long VPGGenerationOptionPropertyAccessor::_ReadLong(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
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

long VPGGenerationOptionPropertyAccessor::_ReadLong(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

void VPGGenerationOptionPropertyAccessor::_WriteLong(const int64_t &objectProperty, const long &value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
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

void VPGGenerationOptionPropertyAccessor::_WriteLong(const int64_t &objectProperty, const long & /*value*/, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_InsertLong(const int64_t &objectProperty, const long & /*value*/, const int64_t & /*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGGenerationOptionPropertyAccessor::_ReadString(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::Version:
            return obj->GetVersion();
        case VPGGenerationOptionProperty::TemplateGitUrl:
            return obj->GetTemplateGitUrl();
        case VPGGenerationOptionProperty::TemplateWorkspace:
            return obj->GetTemplateWorkspace();
        case VPGGenerationOptionProperty::ProjectPrefix:
            return obj->GetProjectPrefix();
        case VPGGenerationOptionProperty::ProjectName:
            return obj->GetProjectName();
        case VPGGenerationOptionProperty::ProjectNameDll:
            return obj->GetProjectNameDll();
        case VPGGenerationOptionProperty::ProjectNameExe:
            return obj->GetProjectNameExe();
        case VPGGenerationOptionProperty::TypeWorkspace:
            return obj->GetTypeWorkspace();
        case VPGGenerationOptionProperty::ExceptionTypeDirectory:
            return obj->GetExceptionTypeDirectory();
        case VPGGenerationOptionProperty::ObjectTypeDirectory:
            return obj->GetObjectTypeDirectory();
        case VPGGenerationOptionProperty::ApplicationDirectoryHpp:
            return obj->GetApplicationDirectoryHpp();
        case VPGGenerationOptionProperty::ApplicationDirectoryCpp:
            return obj->GetApplicationDirectoryCpp();
        case VPGGenerationOptionProperty::ActionDirectoryHpp:
            return obj->GetActionDirectoryHpp();
        case VPGGenerationOptionProperty::ActionDirectoryCpp:
            return obj->GetActionDirectoryCpp();
        case VPGGenerationOptionProperty::FormDirectoryHpp:
            return obj->GetFormDirectoryHpp();
        case VPGGenerationOptionProperty::FormDirectoryCpp:
            return obj->GetFormDirectoryCpp();
        case VPGGenerationOptionProperty::ObjectDirectoryHpp:
            return obj->GetObjectDirectoryHpp();
        case VPGGenerationOptionProperty::ObjectDirectoryCpp:
            return obj->GetObjectDirectoryCpp();
        case VPGGenerationOptionProperty::PropertyAccessorDirectoryHpp:
            return obj->GetPropertyAccessorDirectoryHpp();
        case VPGGenerationOptionProperty::PropertyAccessorDirectoryCpp:
            return obj->GetPropertyAccessorDirectoryCpp();
        case VPGGenerationOptionProperty::ObjectFactoryDirectoryHpp:
            return obj->GetObjectFactoryDirectoryHpp();
        case VPGGenerationOptionProperty::ObjectFactoryDirectoryCpp:
            return obj->GetObjectFactoryDirectoryCpp();
        case VPGGenerationOptionProperty::PropertyAccessorFactoryDirectoryHpp:
            return obj->GetPropertyAccessorFactoryDirectoryHpp();
        case VPGGenerationOptionProperty::PropertyAccessorFactoryDirectoryCpp:
            return obj->GetPropertyAccessorFactoryDirectoryCpp();
        case VPGGenerationOptionProperty::Plugins:
            return obj->GetPlugins(index);
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGGenerationOptionPropertyAccessor::_ReadString(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGGenerationOptionPropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring &value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::Version:
            obj->SetVersion(value);
            break;
        case VPGGenerationOptionProperty::TemplateGitUrl:
            obj->SetTemplateGitUrl(value);
            break;
        case VPGGenerationOptionProperty::TemplateWorkspace:
            obj->SetTemplateWorkspace(value);
            break;
        case VPGGenerationOptionProperty::ProjectPrefix:
            obj->SetProjectPrefix(value);
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
        case VPGGenerationOptionProperty::TypeWorkspace:
            obj->SetTypeWorkspace(value);
            break;
        case VPGGenerationOptionProperty::ExceptionTypeDirectory:
            obj->SetExceptionTypeDirectory(value);
            break;
        case VPGGenerationOptionProperty::ObjectTypeDirectory:
            obj->SetObjectTypeDirectory(value);
            break;
        case VPGGenerationOptionProperty::ApplicationDirectoryHpp:
            obj->SetApplicationDirectoryHpp(value);
            break;
        case VPGGenerationOptionProperty::ApplicationDirectoryCpp:
            obj->SetApplicationDirectoryCpp(value);
            break;
        case VPGGenerationOptionProperty::ActionDirectoryHpp:
            obj->SetActionDirectoryHpp(value);
            break;
        case VPGGenerationOptionProperty::ActionDirectoryCpp:
            obj->SetActionDirectoryCpp(value);
            break;
        case VPGGenerationOptionProperty::FormDirectoryHpp:
            obj->SetFormDirectoryHpp(value);
            break;
        case VPGGenerationOptionProperty::FormDirectoryCpp:
            obj->SetFormDirectoryCpp(value);
            break;
        case VPGGenerationOptionProperty::ObjectDirectoryHpp:
            obj->SetObjectDirectoryHpp(value);
            break;
        case VPGGenerationOptionProperty::ObjectDirectoryCpp:
            obj->SetObjectDirectoryCpp(value);
            break;
        case VPGGenerationOptionProperty::PropertyAccessorDirectoryHpp:
            obj->SetPropertyAccessorDirectoryHpp(value);
            break;
        case VPGGenerationOptionProperty::PropertyAccessorDirectoryCpp:
            obj->SetPropertyAccessorDirectoryCpp(value);
            break;
        case VPGGenerationOptionProperty::ObjectFactoryDirectoryHpp:
            obj->SetObjectFactoryDirectoryHpp(value);
            break;
        case VPGGenerationOptionProperty::ObjectFactoryDirectoryCpp:
            obj->SetObjectFactoryDirectoryCpp(value);
            break;
        case VPGGenerationOptionProperty::PropertyAccessorFactoryDirectoryHpp:
            obj->SetPropertyAccessorFactoryDirectoryHpp(value);
            break;
        case VPGGenerationOptionProperty::PropertyAccessorFactoryDirectoryCpp:
            obj->SetPropertyAccessorFactoryDirectoryCpp(value);
            break;
        case VPGGenerationOptionProperty::Plugins:
            if (index > -1)
                obj->SetPlugins(index, value);
            else
                obj->InsertPlugins(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring & /*value*/, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_InsertString(const int64_t &objectProperty, const std::wstring &value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::Plugins:
            if (index > -1)
                obj->InsertPlugins(index, value);
            else
                obj->InsertPlugins(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

std::shared_ptr<IObject> VPGGenerationOptionPropertyAccessor::_ReadObject(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::Exports:
            return std::static_pointer_cast<IObject>(obj->GetExports(index));
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGGenerationOptionPropertyAccessor::_ReadObject(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

void VPGGenerationOptionPropertyAccessor::_WriteObject(const int64_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::Exports:
            if (index > -1)
                obj->SetExports(index, std::static_pointer_cast<VPGGenerationOptionExport>(value));
            else
                obj->InsertExports(std::static_pointer_cast<VPGGenerationOptionExport>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_WriteObject(const int64_t &objectProperty, std::shared_ptr<IObject> /*value*/, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_InsertObject(const int64_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::Exports:
            if (index > -1)
                obj->InsertExports(index, std::static_pointer_cast<VPGGenerationOptionExport>(value));
            else
                obj->InsertExports(std::static_pointer_cast<VPGGenerationOptionExport>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

std::shared_ptr<IObject> VPGGenerationOptionPropertyAccessor::_CloneObject(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::Exports:
            return std::static_pointer_cast<IObject>(obj->CloneExports(index));
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGGenerationOptionPropertyAccessor::_CloneObject(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

size_t VPGGenerationOptionPropertyAccessor::_GetContainerCount(const int64_t &objectProperty) const
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

bool VPGGenerationOptionPropertyAccessor::_IsContainKey(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGGenerationOptionPropertyAccessor::_RemoveContainerElement(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::Plugins:
            obj->RemovePluginsAtIndex(index);
            break;
        case VPGGenerationOptionProperty::Exports:
            obj->RemoveExportsAtIndex(index);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_RemoveContainerElement(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_ClearContainer(const int64_t &objectProperty) const
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
