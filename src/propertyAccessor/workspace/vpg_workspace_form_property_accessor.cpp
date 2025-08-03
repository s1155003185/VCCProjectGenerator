#include "vpg_workspace_form_property_accessor.hpp"

#include <memory>
#include <string>
#include <vector>

#include "exception_macro.hpp"
#include "i_object.hpp"
#include "property_accessor_macro.hpp"
#include "vpg_git_form.hpp"
#include "vpg_workspace_form.hpp"
#include "vpg_workspace_form_property.hpp"

long VPGWorkspaceFormPropertyAccessor::_readLong(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::TabOrder:
            return obj->getTabOrder();
        default:
            assert(false);
        }
    CATCH
    return 0L;
}

long VPGWorkspaceFormPropertyAccessor::_readLongAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

long VPGWorkspaceFormPropertyAccessor::_readLongAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return 0L;
}

void VPGWorkspaceFormPropertyAccessor::_writeLong(const int64_t &objectProperty, const long &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::TabOrder:
            obj->setTabOrder(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_writeLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_writeLongAtKey(const int64_t &objectProperty, const long &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_insertLongAtIndex(const int64_t &objectProperty, const long &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::wstring VPGWorkspaceFormPropertyAccessor::_readString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::Name:
            return obj->getName();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGWorkspaceFormPropertyAccessor::_readStringAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

std::wstring VPGWorkspaceFormPropertyAccessor::_readStringAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGWorkspaceFormPropertyAccessor::_writeString(const int64_t &objectProperty, const std::wstring &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::Name:
            obj->setName(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_writeStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_writeStringAtKey(const int64_t &objectProperty, const std::wstring &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_insertStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::shared_ptr<vcc::IObject> VPGWorkspaceFormPropertyAccessor::_readObject(const int64_t &objectProperty) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGWorkspaceFormPropertyAccessor::_readObjectAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::GitForms:
            return std::static_pointer_cast<vcc::IObject>(obj->getGitFormsAtIndex(index));
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGWorkspaceFormPropertyAccessor::_readObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

void VPGWorkspaceFormPropertyAccessor::_writeObject(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> /*value*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_writeObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> value, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::GitForms:
            if (index > -1)
                obj->setGitFormsAtIndex(index, std::static_pointer_cast<VPGGitForm>(value));
            else
                obj->insertGitForms(std::static_pointer_cast<VPGGitForm>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_writeObjectAtKey(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> /*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_insertObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> value, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::GitForms:
            if (index > -1)
                obj->insertGitFormsAtIndex(index, std::static_pointer_cast<VPGGitForm>(value));
            else
                obj->insertGitForms(std::static_pointer_cast<VPGGitForm>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

std::shared_ptr<vcc::IObject> VPGWorkspaceFormPropertyAccessor::_cloneObject(const int64_t &objectProperty) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGWorkspaceFormPropertyAccessor::_cloneObjectAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::GitForms:
            return std::static_pointer_cast<vcc::IObject>(obj->cloneGitFormsAtIndex(index));
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGWorkspaceFormPropertyAccessor::_cloneObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

size_t VPGWorkspaceFormPropertyAccessor::_getCount(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::GitForms:
            return obj->getGitForms().size();
        default:
            assert(false);
        }
    CATCH
    return 0;
}

std::set<void *> VPGWorkspaceFormPropertyAccessor::_getMapKeys(const int64_t &objectProperty) const
{
    std::set<void *> result;
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return result;
}

bool VPGWorkspaceFormPropertyAccessor::_isContainKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGWorkspaceFormPropertyAccessor::_remove(const int64_t &objectProperty, const void */*value*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_removeObject(const int64_t &objectProperty, const vcc::IObject *value)
{
    TRY
        assert(value != nullptr);
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::GitForms:
            obj->removeGitForms(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_removeAtIndex(const int64_t &objectProperty, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::GitForms:
            obj->removeGitFormsAtIndex(index);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_removeAtKey(const int64_t &objectProperty, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_clear(const int64_t &objectProperty)
{
    TRY
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::GitForms:
            obj->clearGitForms();
            break;
        default:
            assert(false);
        }
    CATCH
}
