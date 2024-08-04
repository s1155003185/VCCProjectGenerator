#include "vpg_generation_option_property_accessor.hpp"

#include <memory>
#include <string>
#include <vector>

#include "exception_macro.hpp"
#include "i_object.hpp"
#include "i_type_union.hpp"
#include "property_accessor_key.hpp"
#include "property_accessor_macro.hpp"
#include "vpg_generation_option.hpp"
#include "vpg_generation_option_interface_type.hpp"
#include "vpg_generation_option_property.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

long VPGGenerationOptionExportPropertyAccessor::_ReadLong(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        std::shared_ptr<VPGGenerationOptionExport> obj = std::static_pointer_cast<VPGGenerationOptionExport>(_Object);
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

long VPGGenerationOptionExportPropertyAccessor::_ReadLong(const int64_t &objectProperty, const ITypeUnion * /*key*/) const
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
        std::shared_ptr<VPGGenerationOptionExport> obj = std::static_pointer_cast<VPGGenerationOptionExport>(_Object);
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

void VPGGenerationOptionExportPropertyAccessor::_WriteLong(const int64_t &objectProperty, const long & /*value*/, const ITypeUnion * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGGenerationOptionExportPropertyAccessor::_ReadString(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        std::shared_ptr<VPGGenerationOptionExport> obj = std::static_pointer_cast<VPGGenerationOptionExport>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionExportProperty>(objectProperty))
        {
        case VPGGenerationOptionExportProperty::Workspace:
            return obj->GetWorkspace();
        case VPGGenerationOptionExportProperty::DllBridgeDirectory:
            return obj->GetDllBridgeDirectory();
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

std::wstring VPGGenerationOptionExportPropertyAccessor::_ReadString(const int64_t &objectProperty, const ITypeUnion * /*key*/) const
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
        std::shared_ptr<VPGGenerationOptionExport> obj = std::static_pointer_cast<VPGGenerationOptionExport>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionExportProperty>(objectProperty))
        {
        case VPGGenerationOptionExportProperty::Workspace:
            obj->SetWorkspace(value);
            break;
        case VPGGenerationOptionExportProperty::DllBridgeDirectory:
            obj->SetDllBridgeDirectory(value);
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

void VPGGenerationOptionExportPropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring & /*value*/, const ITypeUnion * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

bool VPGGenerationOptionPropertyAccessor::_ReadBool(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        std::shared_ptr<VPGGenerationOption> obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
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

bool VPGGenerationOptionPropertyAccessor::_ReadBool(const int64_t &objectProperty, const ITypeUnion * /*key*/) const
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
        std::shared_ptr<VPGGenerationOption> obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
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

void VPGGenerationOptionPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool & /*value*/, const ITypeUnion * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

long VPGGenerationOptionPropertyAccessor::_ReadLong(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        std::shared_ptr<VPGGenerationOption> obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
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

long VPGGenerationOptionPropertyAccessor::_ReadLong(const int64_t &objectProperty, const ITypeUnion * /*key*/) const
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
        std::shared_ptr<VPGGenerationOption> obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
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

void VPGGenerationOptionPropertyAccessor::_WriteLong(const int64_t &objectProperty, const long & /*value*/, const ITypeUnion * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGGenerationOptionPropertyAccessor::_ReadString(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        std::shared_ptr<VPGGenerationOption> obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
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
        case VPGGenerationOptionProperty::ActionTypeDirectory:
            return obj->GetActionTypeDirectory();
        case VPGGenerationOptionProperty::ExceptionTypeDirectory:
            return obj->GetExceptionTypeDirectory();
        case VPGGenerationOptionProperty::ManagerTypeDirectory:
            return obj->GetManagerTypeDirectory();
        case VPGGenerationOptionProperty::ObjectTypeDirectory:
            return obj->GetObjectTypeDirectory();
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

std::wstring VPGGenerationOptionPropertyAccessor::_ReadString(const int64_t &objectProperty, const ITypeUnion * /*key*/) const
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
        std::shared_ptr<VPGGenerationOption> obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
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
        case VPGGenerationOptionProperty::ActionTypeDirectory:
            obj->SetActionTypeDirectory(value);
            break;
        case VPGGenerationOptionProperty::ExceptionTypeDirectory:
            obj->SetExceptionTypeDirectory(value);
            break;
        case VPGGenerationOptionProperty::ManagerTypeDirectory:
            obj->SetManagerTypeDirectory(value);
            break;
        case VPGGenerationOptionProperty::ObjectTypeDirectory:
            obj->SetObjectTypeDirectory(value);
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

void VPGGenerationOptionPropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring & /*value*/, const ITypeUnion * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::shared_ptr<IObject> VPGGenerationOptionPropertyAccessor::_ReadObject(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        std::shared_ptr<VPGGenerationOption> obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
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

std::shared_ptr<IObject> VPGGenerationOptionPropertyAccessor::_ReadObject(const int64_t &objectProperty, const ITypeUnion * /*key*/) const
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
        std::shared_ptr<VPGGenerationOption> obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
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

void VPGGenerationOptionPropertyAccessor::_WriteObject(const int64_t &objectProperty, std::shared_ptr<IObject> /*value*/, const ITypeUnion * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::shared_ptr<IObject> VPGGenerationOptionPropertyAccessor::_CloneObject(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        std::shared_ptr<VPGGenerationOption> obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
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

std::shared_ptr<IObject> VPGGenerationOptionPropertyAccessor::_CloneObject(const int64_t &objectProperty, const ITypeUnion * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

size_t VPGGenerationOptionPropertyAccessor::_GetContainerCount(const int64_t &objectProperty) const
{
    TRY
        std::shared_ptr<VPGGenerationOption> obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
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

bool VPGGenerationOptionPropertyAccessor::_IsContainKey(const int64_t &objectProperty, const ITypeUnion * /*key*/) const
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
        std::shared_ptr<VPGGenerationOption> obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGenerationOptionProperty>(objectProperty))
        {
        case VPGGenerationOptionProperty::Plugins:
            obj->RemovePlugins(index);
            break;
        case VPGGenerationOptionProperty::Exports:
            obj->RemoveExports(index);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_RemoveContainerElement(const int64_t &objectProperty, const ITypeUnion * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGenerationOptionPropertyAccessor::_ClearContainer(const int64_t &objectProperty) const
{
    TRY
        std::shared_ptr<VPGGenerationOption> obj = std::static_pointer_cast<VPGGenerationOption>(_Object);
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
