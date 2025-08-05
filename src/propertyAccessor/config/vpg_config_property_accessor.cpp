#include "vpg_config_property_accessor.hpp"

#include <memory>
#include <string>
#include <vector>

#include "exception_macro.hpp"
#include "i_object.hpp"
#include "property_accessor_macro.hpp"
#include "vpg_config.hpp"
#include "vpg_config_property.hpp"
#include "vpg_config_type.hpp"
#include "vpg_project_type.hpp"

bool VPGConfigBehaviorPropertyAccessor::_readBool(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigBehavior>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigBehaviorProperty>(objectProperty))
        {
        case VPGConfigBehaviorProperty::IsActionResultThrowException:
            return obj->getIsActionResultThrowException();
        default:
            assert(false);
        }
    CATCH
    return false;
}

bool VPGConfigBehaviorPropertyAccessor::_readBoolAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

bool VPGConfigBehaviorPropertyAccessor::_readBoolAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGConfigBehaviorPropertyAccessor::_writeBool(const int64_t &objectProperty, const bool &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigBehavior>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigBehaviorProperty>(objectProperty))
        {
        case VPGConfigBehaviorProperty::IsActionResultThrowException:
            obj->setIsActionResultThrowException(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigBehaviorPropertyAccessor::_writeBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigBehaviorPropertyAccessor::_writeBoolAtKey(const int64_t &objectProperty, const bool &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigBehaviorPropertyAccessor::_insertBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

long VPGConfigBehaviorPropertyAccessor::_readLong(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigBehavior>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigBehaviorProperty>(objectProperty))
        {
        case VPGConfigBehaviorProperty::ActionHistoryType:
            return static_cast<long>(obj->getActionHistoryType());
        default:
            assert(false);
        }
    CATCH
    return 0L;
}

long VPGConfigBehaviorPropertyAccessor::_readLongAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

long VPGConfigBehaviorPropertyAccessor::_readLongAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

void VPGConfigBehaviorPropertyAccessor::_writeLong(const int64_t &objectProperty, const long &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigBehavior>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigBehaviorProperty>(objectProperty))
        {
        case VPGConfigBehaviorProperty::ActionHistoryType:
            obj->setActionHistoryType(static_cast<VPGConfigActionHistoryType>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigBehaviorPropertyAccessor::_writeLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigBehaviorPropertyAccessor::_writeLongAtKey(const int64_t &objectProperty, const long &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigBehaviorPropertyAccessor::_insertLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

bool VPGConfigExportPropertyAccessor::_readBool(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigExport>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigExportProperty>(objectProperty))
        {
        case VPGConfigExportProperty::IsExportExternalLib:
            return obj->getIsExportExternalLib();
        default:
            assert(false);
        }
    CATCH
    return false;
}

bool VPGConfigExportPropertyAccessor::_readBoolAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

bool VPGConfigExportPropertyAccessor::_readBoolAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGConfigExportPropertyAccessor::_writeBool(const int64_t &objectProperty, const bool &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigExport>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigExportProperty>(objectProperty))
        {
        case VPGConfigExportProperty::IsExportExternalLib:
            obj->setIsExportExternalLib(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigExportPropertyAccessor::_writeBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigExportPropertyAccessor::_writeBoolAtKey(const int64_t &objectProperty, const bool &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigExportPropertyAccessor::_insertBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

long VPGConfigExportPropertyAccessor::_readLong(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigExport>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigExportProperty>(objectProperty))
        {
        case VPGConfigExportProperty::Interface:
            return static_cast<long>(obj->getInterface());
        default:
            assert(false);
        }
    CATCH
    return 0L;
}

long VPGConfigExportPropertyAccessor::_readLongAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

long VPGConfigExportPropertyAccessor::_readLongAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

void VPGConfigExportPropertyAccessor::_writeLong(const int64_t &objectProperty, const long &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigExport>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigExportProperty>(objectProperty))
        {
        case VPGConfigExportProperty::Interface:
            obj->setInterface(static_cast<VPGConfigInterfaceType>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigExportPropertyAccessor::_writeLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigExportPropertyAccessor::_writeLongAtKey(const int64_t &objectProperty, const long &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigExportPropertyAccessor::_insertLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGConfigExportPropertyAccessor::_readString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigExport>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigExportProperty>(objectProperty))
        {
        case VPGConfigExportProperty::DllBridgeDirectory:
            return obj->getDllBridgeDirectory();
        case VPGConfigExportProperty::ExportDirectoryDll:
            return obj->getExportDirectoryDll();
        case VPGConfigExportProperty::ExportDirectoryExe:
            return obj->getExportDirectoryExe();
        case VPGConfigExportProperty::FormDirectory:
            return obj->getFormDirectory();
        case VPGConfigExportProperty::ObjectDirectory:
            return obj->getObjectDirectory();
        case VPGConfigExportProperty::TypeDirectory:
            return obj->getTypeDirectory();
        case VPGConfigExportProperty::Workspace:
            return obj->getWorkspace();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGConfigExportPropertyAccessor::_readStringAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

std::wstring VPGConfigExportPropertyAccessor::_readStringAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGConfigExportPropertyAccessor::_writeString(const int64_t &objectProperty, const std::wstring &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigExport>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigExportProperty>(objectProperty))
        {
        case VPGConfigExportProperty::DllBridgeDirectory:
            obj->setDllBridgeDirectory(value);
            break;
        case VPGConfigExportProperty::ExportDirectoryDll:
            obj->setExportDirectoryDll(value);
            break;
        case VPGConfigExportProperty::ExportDirectoryExe:
            obj->setExportDirectoryExe(value);
            break;
        case VPGConfigExportProperty::FormDirectory:
            obj->setFormDirectory(value);
            break;
        case VPGConfigExportProperty::ObjectDirectory:
            obj->setObjectDirectory(value);
            break;
        case VPGConfigExportProperty::TypeDirectory:
            obj->setTypeDirectory(value);
            break;
        case VPGConfigExportProperty::Workspace:
            obj->setWorkspace(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigExportPropertyAccessor::_writeStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigExportPropertyAccessor::_writeStringAtKey(const int64_t &objectProperty, const std::wstring &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigExportPropertyAccessor::_insertStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGConfigInputPropertyAccessor::_readString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigInput>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigInputProperty>(objectProperty))
        {
        case VPGConfigInputProperty::TypeWorkspace:
            return obj->getTypeWorkspace();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGConfigInputPropertyAccessor::_readStringAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

std::wstring VPGConfigInputPropertyAccessor::_readStringAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGConfigInputPropertyAccessor::_writeString(const int64_t &objectProperty, const std::wstring &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigInput>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigInputProperty>(objectProperty))
        {
        case VPGConfigInputProperty::TypeWorkspace:
            obj->setTypeWorkspace(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigInputPropertyAccessor::_writeStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigInputPropertyAccessor::_writeStringAtKey(const int64_t &objectProperty, const std::wstring &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigInputPropertyAccessor::_insertStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGConfigOutputPropertyAccessor::_readString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigOutput>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigOutputProperty>(objectProperty))
        {
        case VPGConfigOutputProperty::ActionDirectoryCpp:
            return obj->getActionDirectoryCpp();
        case VPGConfigOutputProperty::ActionDirectoryHpp:
            return obj->getActionDirectoryHpp();
        case VPGConfigOutputProperty::ApplicationDirectoryCpp:
            return obj->getApplicationDirectoryCpp();
        case VPGConfigOutputProperty::ApplicationDirectoryHpp:
            return obj->getApplicationDirectoryHpp();
        case VPGConfigOutputProperty::ExceptionTypeDirectory:
            return obj->getExceptionTypeDirectory();
        case VPGConfigOutputProperty::FormDirectoryCpp:
            return obj->getFormDirectoryCpp();
        case VPGConfigOutputProperty::FormDirectoryHpp:
            return obj->getFormDirectoryHpp();
        case VPGConfigOutputProperty::ObjectDirectoryCpp:
            return obj->getObjectDirectoryCpp();
        case VPGConfigOutputProperty::ObjectDirectoryHpp:
            return obj->getObjectDirectoryHpp();
        case VPGConfigOutputProperty::ObjectFactoryDirectoryCpp:
            return obj->getObjectFactoryDirectoryCpp();
        case VPGConfigOutputProperty::ObjectFactoryDirectoryHpp:
            return obj->getObjectFactoryDirectoryHpp();
        case VPGConfigOutputProperty::ObjectTypeDirectory:
            return obj->getObjectTypeDirectory();
        case VPGConfigOutputProperty::PropertyAccessorDirectoryCpp:
            return obj->getPropertyAccessorDirectoryCpp();
        case VPGConfigOutputProperty::PropertyAccessorDirectoryHpp:
            return obj->getPropertyAccessorDirectoryHpp();
        case VPGConfigOutputProperty::PropertyAccessorFactoryDirectoryCpp:
            return obj->getPropertyAccessorFactoryDirectoryCpp();
        case VPGConfigOutputProperty::PropertyAccessorFactoryDirectoryHpp:
            return obj->getPropertyAccessorFactoryDirectoryHpp();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGConfigOutputPropertyAccessor::_readStringAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

std::wstring VPGConfigOutputPropertyAccessor::_readStringAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGConfigOutputPropertyAccessor::_writeString(const int64_t &objectProperty, const std::wstring &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigOutput>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigOutputProperty>(objectProperty))
        {
        case VPGConfigOutputProperty::ActionDirectoryCpp:
            obj->setActionDirectoryCpp(value);
            break;
        case VPGConfigOutputProperty::ActionDirectoryHpp:
            obj->setActionDirectoryHpp(value);
            break;
        case VPGConfigOutputProperty::ApplicationDirectoryCpp:
            obj->setApplicationDirectoryCpp(value);
            break;
        case VPGConfigOutputProperty::ApplicationDirectoryHpp:
            obj->setApplicationDirectoryHpp(value);
            break;
        case VPGConfigOutputProperty::ExceptionTypeDirectory:
            obj->setExceptionTypeDirectory(value);
            break;
        case VPGConfigOutputProperty::FormDirectoryCpp:
            obj->setFormDirectoryCpp(value);
            break;
        case VPGConfigOutputProperty::FormDirectoryHpp:
            obj->setFormDirectoryHpp(value);
            break;
        case VPGConfigOutputProperty::ObjectDirectoryCpp:
            obj->setObjectDirectoryCpp(value);
            break;
        case VPGConfigOutputProperty::ObjectDirectoryHpp:
            obj->setObjectDirectoryHpp(value);
            break;
        case VPGConfigOutputProperty::ObjectFactoryDirectoryCpp:
            obj->setObjectFactoryDirectoryCpp(value);
            break;
        case VPGConfigOutputProperty::ObjectFactoryDirectoryHpp:
            obj->setObjectFactoryDirectoryHpp(value);
            break;
        case VPGConfigOutputProperty::ObjectTypeDirectory:
            obj->setObjectTypeDirectory(value);
            break;
        case VPGConfigOutputProperty::PropertyAccessorDirectoryCpp:
            obj->setPropertyAccessorDirectoryCpp(value);
            break;
        case VPGConfigOutputProperty::PropertyAccessorDirectoryHpp:
            obj->setPropertyAccessorDirectoryHpp(value);
            break;
        case VPGConfigOutputProperty::PropertyAccessorFactoryDirectoryCpp:
            obj->setPropertyAccessorFactoryDirectoryCpp(value);
            break;
        case VPGConfigOutputProperty::PropertyAccessorFactoryDirectoryHpp:
            obj->setPropertyAccessorFactoryDirectoryHpp(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigOutputPropertyAccessor::_writeStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigOutputPropertyAccessor::_writeStringAtKey(const int64_t &objectProperty, const std::wstring &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigOutputPropertyAccessor::_insertStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

bool VPGConfigPropertyAccessor::_readBool(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::BehaviorIsActionResultThrowException:
            return obj->getBehaviorIsActionResultThrowException();
        case VPGConfigProperty::IsGit:
            return obj->getIsGit();
        case VPGConfigProperty::TemplateIsExcludeUnittest:
            return obj->getTemplateIsExcludeUnittest();
        case VPGConfigProperty::TemplateIsExcludeVCCUnitTest:
            return obj->getTemplateIsExcludeVCCUnitTest();
        default:
            assert(false);
        }
    CATCH
    return false;
}

bool VPGConfigPropertyAccessor::_readBoolAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

bool VPGConfigPropertyAccessor::_readBoolAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGConfigPropertyAccessor::_writeBool(const int64_t &objectProperty, const bool &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::IsGit:
            obj->setIsGit(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigPropertyAccessor::_writeBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigPropertyAccessor::_writeBoolAtKey(const int64_t &objectProperty, const bool &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigPropertyAccessor::_insertBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

long VPGConfigPropertyAccessor::_readLong(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::BehaviorActionHistoryType:
            return static_cast<long>(obj->getBehaviorActionHistoryType());
        case VPGConfigProperty::ProjectType:
            return static_cast<long>(obj->getProjectType());
        default:
            assert(false);
        }
    CATCH
    return 0L;
}

long VPGConfigPropertyAccessor::_readLongAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

long VPGConfigPropertyAccessor::_readLongAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

void VPGConfigPropertyAccessor::_writeLong(const int64_t &objectProperty, const long &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::ProjectType:
            obj->setProjectType(static_cast<VPGProjectType>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigPropertyAccessor::_writeLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigPropertyAccessor::_writeLongAtKey(const int64_t &objectProperty, const long &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigPropertyAccessor::_insertLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGConfigPropertyAccessor::_readString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::InputTypeWorkspace:
            return obj->getInputTypeWorkspace();
        case VPGConfigProperty::OutputActionDirectoryCpp:
            return obj->getOutputActionDirectoryCpp();
        case VPGConfigProperty::OutputActionDirectoryHpp:
            return obj->getOutputActionDirectoryHpp();
        case VPGConfigProperty::OutputApplicationDirectoryCpp:
            return obj->getOutputApplicationDirectoryCpp();
        case VPGConfigProperty::OutputApplicationDirectoryHpp:
            return obj->getOutputApplicationDirectoryHpp();
        case VPGConfigProperty::OutputExceptionTypeDirectory:
            return obj->getOutputExceptionTypeDirectory();
        case VPGConfigProperty::OutputFormDirectoryCpp:
            return obj->getOutputFormDirectoryCpp();
        case VPGConfigProperty::OutputFormDirectoryHpp:
            return obj->getOutputFormDirectoryHpp();
        case VPGConfigProperty::OutputObjectDirectoryCpp:
            return obj->getOutputObjectDirectoryCpp();
        case VPGConfigProperty::OutputObjectDirectoryHpp:
            return obj->getOutputObjectDirectoryHpp();
        case VPGConfigProperty::OutputObjectFactoryDirectoryCpp:
            return obj->getOutputObjectFactoryDirectoryCpp();
        case VPGConfigProperty::OutputObjectFactoryDirectoryHpp:
            return obj->getOutputObjectFactoryDirectoryHpp();
        case VPGConfigProperty::OutputObjectTypeDirectory:
            return obj->getOutputObjectTypeDirectory();
        case VPGConfigProperty::OutputPropertyAccessorDirectoryCpp:
            return obj->getOutputPropertyAccessorDirectoryCpp();
        case VPGConfigProperty::OutputPropertyAccessorDirectoryHpp:
            return obj->getOutputPropertyAccessorDirectoryHpp();
        case VPGConfigProperty::OutputPropertyAccessorFactoryDirectoryCpp:
            return obj->getOutputPropertyAccessorFactoryDirectoryCpp();
        case VPGConfigProperty::OutputPropertyAccessorFactoryDirectoryHpp:
            return obj->getOutputPropertyAccessorFactoryDirectoryHpp();
        case VPGConfigProperty::ProjectName:
            return obj->getProjectName();
        case VPGConfigProperty::ProjectNameDll:
            return obj->getProjectNameDll();
        case VPGConfigProperty::ProjectNameExe:
            return obj->getProjectNameExe();
        case VPGConfigProperty::ProjectPrefix:
            return obj->getProjectPrefix();
        case VPGConfigProperty::TemplateUrl:
            return obj->getTemplateUrl();
        case VPGConfigProperty::TemplateWorkspace:
            return obj->getTemplateWorkspace();
        case VPGConfigProperty::Version:
            return obj->getVersion();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGConfigPropertyAccessor::_readStringAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Plugins:
            return obj->getPluginsAtIndex(index);
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGConfigPropertyAccessor::_readStringAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGConfigPropertyAccessor::_writeString(const int64_t &objectProperty, const std::wstring &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::ProjectName:
            obj->setProjectName(value);
            break;
        case VPGConfigProperty::ProjectNameDll:
            obj->setProjectNameDll(value);
            break;
        case VPGConfigProperty::ProjectNameExe:
            obj->setProjectNameExe(value);
            break;
        case VPGConfigProperty::ProjectPrefix:
            obj->setProjectPrefix(value);
            break;
        case VPGConfigProperty::Version:
            obj->setVersion(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigPropertyAccessor::_writeStringAtIndex(const int64_t &objectProperty, const std::wstring &value, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Plugins:
            if (index > -1)
                obj->setPluginsAtIndex(index, value);
            else
                obj->insertPlugins(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigPropertyAccessor::_writeStringAtKey(const int64_t &objectProperty, const std::wstring &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigPropertyAccessor::_insertStringAtIndex(const int64_t &objectProperty, const std::wstring &value, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Plugins:
            if (index > -1)
                obj->insertPluginsAtIndex(index, value);
            else
                obj->insertPlugins(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

std::shared_ptr<vcc::IObject> VPGConfigPropertyAccessor::_readObject(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Behavior:
            return std::static_pointer_cast<vcc::IObject>(obj->getBehavior());
        case VPGConfigProperty::Input:
            return std::static_pointer_cast<vcc::IObject>(obj->getInput());
        case VPGConfigProperty::Output:
            return std::static_pointer_cast<vcc::IObject>(obj->getOutput());
        case VPGConfigProperty::Template:
            return std::static_pointer_cast<vcc::IObject>(obj->getTemplate());
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGConfigPropertyAccessor::_readObjectAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Exports:
            return std::static_pointer_cast<vcc::IObject>(obj->getExportsAtIndex(index));
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGConfigPropertyAccessor::_readObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

void VPGConfigPropertyAccessor::_writeObject(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Behavior:
            obj->setBehavior(std::static_pointer_cast<VPGConfigBehavior>(value));
            break;
        case VPGConfigProperty::Input:
            obj->setInput(std::static_pointer_cast<VPGConfigInput>(value));
            break;
        case VPGConfigProperty::Output:
            obj->setOutput(std::static_pointer_cast<VPGConfigOutput>(value));
            break;
        case VPGConfigProperty::Template:
            obj->setTemplate(std::static_pointer_cast<VPGConfigTemplate>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigPropertyAccessor::_writeObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> value, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Exports:
            if (index > -1)
                obj->setExportsAtIndex(index, std::static_pointer_cast<VPGConfigExport>(value));
            else
                obj->insertExports(std::static_pointer_cast<VPGConfigExport>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigPropertyAccessor::_writeObjectAtKey(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> /*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigPropertyAccessor::_insertObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> value, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Exports:
            if (index > -1)
                obj->insertExportsAtIndex(index, std::static_pointer_cast<VPGConfigExport>(value));
            else
                obj->insertExports(std::static_pointer_cast<VPGConfigExport>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

std::shared_ptr<vcc::IObject> VPGConfigPropertyAccessor::_cloneObject(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Behavior:
            return std::static_pointer_cast<vcc::IObject>(obj->getBehavior()->clone());
        case VPGConfigProperty::Input:
            return std::static_pointer_cast<vcc::IObject>(obj->getInput()->clone());
        case VPGConfigProperty::Output:
            return std::static_pointer_cast<vcc::IObject>(obj->getOutput()->clone());
        case VPGConfigProperty::Template:
            return std::static_pointer_cast<vcc::IObject>(obj->getTemplate()->clone());
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGConfigPropertyAccessor::_cloneObjectAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Exports:
            return std::static_pointer_cast<vcc::IObject>(obj->cloneExportsAtIndex(index));
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGConfigPropertyAccessor::_cloneObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

size_t VPGConfigPropertyAccessor::_getCount(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Plugins:
            return obj->getPlugins().size();
        case VPGConfigProperty::Exports:
            return obj->getExports().size();
        default:
            assert(false);
        }
    CATCH
    return 0;
}

std::set<void *> VPGConfigPropertyAccessor::_getMapKeys(const int64_t &objectProperty) const
{
    std::set<void *> result;
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return result;
}

bool VPGConfigPropertyAccessor::_isContainKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGConfigPropertyAccessor::_remove(const int64_t &objectProperty, const void *value)
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
            obj->removePlugins(valuePtr);
            break;
        }
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigPropertyAccessor::_removeObject(const int64_t &objectProperty, const vcc::IObject *value)
{
    TRY
        assert(value != nullptr);
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Exports:
            obj->removeExports(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigPropertyAccessor::_removeAtIndex(const int64_t &objectProperty, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Exports:
            obj->removeExportsAtIndex(index);
            break;
        case VPGConfigProperty::Plugins:
            obj->removePluginsAtIndex(index);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigPropertyAccessor::_removeAtKey(const int64_t &objectProperty, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigPropertyAccessor::_clear(const int64_t &objectProperty)
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfig>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigProperty>(objectProperty))
        {
        case VPGConfigProperty::Plugins:
            obj->clearPlugins();
            break;
        case VPGConfigProperty::Exports:
            obj->clearExports();
            break;
        default:
            assert(false);
        }
    CATCH
}

bool VPGConfigTemplatePropertyAccessor::_readBool(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigTemplate>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigTemplateProperty>(objectProperty))
        {
        case VPGConfigTemplateProperty::IsExcludeUnittest:
            return obj->getIsExcludeUnittest();
        case VPGConfigTemplateProperty::IsExcludeVCCUnitTest:
            return obj->getIsExcludeVCCUnitTest();
        default:
            assert(false);
        }
    CATCH
    return false;
}

bool VPGConfigTemplatePropertyAccessor::_readBoolAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

bool VPGConfigTemplatePropertyAccessor::_readBoolAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGConfigTemplatePropertyAccessor::_writeBool(const int64_t &objectProperty, const bool &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigTemplate>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigTemplateProperty>(objectProperty))
        {
        case VPGConfigTemplateProperty::IsExcludeUnittest:
            obj->setIsExcludeUnittest(value);
            break;
        case VPGConfigTemplateProperty::IsExcludeVCCUnitTest:
            obj->setIsExcludeVCCUnitTest(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigTemplatePropertyAccessor::_writeBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigTemplatePropertyAccessor::_writeBoolAtKey(const int64_t &objectProperty, const bool &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigTemplatePropertyAccessor::_insertBoolAtIndex(const int64_t &objectProperty, const bool &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGConfigTemplatePropertyAccessor::_readString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigTemplate>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigTemplateProperty>(objectProperty))
        {
        case VPGConfigTemplateProperty::Url:
            return obj->getUrl();
        case VPGConfigTemplateProperty::Workspace:
            return obj->getWorkspace();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGConfigTemplatePropertyAccessor::_readStringAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

std::wstring VPGConfigTemplatePropertyAccessor::_readStringAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGConfigTemplatePropertyAccessor::_writeString(const int64_t &objectProperty, const std::wstring &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGConfigTemplate>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGConfigTemplateProperty>(objectProperty))
        {
        case VPGConfigTemplateProperty::Url:
            obj->setUrl(value);
            break;
        case VPGConfigTemplateProperty::Workspace:
            obj->setWorkspace(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGConfigTemplatePropertyAccessor::_writeStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigTemplatePropertyAccessor::_writeStringAtKey(const int64_t &objectProperty, const std::wstring &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGConfigTemplatePropertyAccessor::_insertStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}
