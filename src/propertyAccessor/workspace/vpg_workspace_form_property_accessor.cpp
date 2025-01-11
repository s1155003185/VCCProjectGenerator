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

using namespace vcc;

std::wstring VPGWorkspaceFormPropertyAccessor::_ReadString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::Name:
            return obj->GetName();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGWorkspaceFormPropertyAccessor::_ReadStringAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

std::wstring VPGWorkspaceFormPropertyAccessor::_ReadStringAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGWorkspaceFormPropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring &value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::Name:
            obj->SetName(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_WriteStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_WriteStringAtKey(const int64_t &objectProperty, const std::wstring &/*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_InsertStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::shared_ptr<IObject> VPGWorkspaceFormPropertyAccessor::_ReadObject(const int64_t &objectProperty) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGWorkspaceFormPropertyAccessor::_ReadObjectAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::GitForms:
            return std::static_pointer_cast<IObject>(obj->GetGitFormsAtIndex(index));
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGWorkspaceFormPropertyAccessor::_ReadObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

void VPGWorkspaceFormPropertyAccessor::_WriteObject(const int64_t &objectProperty, std::shared_ptr<IObject> /*value*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_WriteObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::GitForms:
            if (index > -1)
                obj->SetGitFormsAtIndex(index, std::static_pointer_cast<VPGGitForm>(value));
            else
                obj->InsertGitForms(std::static_pointer_cast<VPGGitForm>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_WriteObjectAtKey(const int64_t &objectProperty, std::shared_ptr<IObject> /*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_InsertObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::GitForms:
            if (index > -1)
                obj->InsertGitFormsAtIndex(index, std::static_pointer_cast<VPGGitForm>(value));
            else
                obj->InsertGitForms(std::static_pointer_cast<VPGGitForm>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

std::shared_ptr<IObject> VPGWorkspaceFormPropertyAccessor::_CloneObject(const int64_t &objectProperty) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGWorkspaceFormPropertyAccessor::_CloneObjectAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::GitForms:
            return std::static_pointer_cast<IObject>(obj->CloneGitFormsAtIndex(index));
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGWorkspaceFormPropertyAccessor::_CloneObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

size_t VPGWorkspaceFormPropertyAccessor::_GetCount(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::GitForms:
            return obj->GetGitForms().size();
        default:
            assert(false);
        }
    CATCH
    return 0;
}

std::set<void *> VPGWorkspaceFormPropertyAccessor::_GetMapKeys(const int64_t &objectProperty) const
{
    std::set<void *> result;
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return result;
}

bool VPGWorkspaceFormPropertyAccessor::_IsContainKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGWorkspaceFormPropertyAccessor::_Remove(const int64_t &objectProperty, const void *value) const
{
    TRY
        assert(value != nullptr);
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::GitForms: {
            auto valuePtr = static_cast<const IObject *>(value);
            assert(valuePtr != nullptr);
            obj->RemoveGitForms(valuePtr);
            break;
        }
        default:
            assert(false);
        }
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_RemoveAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::GitForms:
            obj->RemoveGitFormsAtIndex(index);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_RemoveAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_Clear(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::GitForms:
            obj->ClearGitForms();
            break;
        default:
            assert(false);
        }
    CATCH
}
