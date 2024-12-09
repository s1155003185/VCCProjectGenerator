#include "vpg_workspace_form_property_accessor.hpp"

#include <memory>
#include <vector>

#include "exception_macro.hpp"
#include "i_object.hpp"
#include "property_accessor_macro.hpp"
#include "vpg_git_form.hpp"
#include "vpg_workspace_form.hpp"
#include "vpg_workspace_form_property.hpp"

using namespace vcc;

std::shared_ptr<IObject> VPGWorkspaceFormPropertyAccessor::_ReadObject(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::GitForms:
            return std::static_pointer_cast<IObject>(obj->GetGitForms(index));
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGWorkspaceFormPropertyAccessor::_ReadObject(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

void VPGWorkspaceFormPropertyAccessor::_WriteObject(const int64_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::GitForms:
            if (index > -1)
                obj->SetGitForms(index, std::static_pointer_cast<VPGGitForm>(value));
            else
                obj->InsertGitForms(std::static_pointer_cast<VPGGitForm>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_WriteObject(const int64_t &objectProperty, std::shared_ptr<IObject> /*value*/, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_InsertObject(const int64_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::GitForms:
            if (index > -1)
                obj->InsertGitForms(index, std::static_pointer_cast<VPGGitForm>(value));
            else
                obj->InsertGitForms(std::static_pointer_cast<VPGGitForm>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

std::shared_ptr<IObject> VPGWorkspaceFormPropertyAccessor::_CloneObject(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::GitForms:
            return std::static_pointer_cast<IObject>(obj->CloneGitForms(index));
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGWorkspaceFormPropertyAccessor::_CloneObject(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

size_t VPGWorkspaceFormPropertyAccessor::_GetContainerCount(const int64_t &objectProperty) const
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

bool VPGWorkspaceFormPropertyAccessor::_IsContainKey(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGWorkspaceFormPropertyAccessor::_RemoveContainerElement(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGWorkspaceForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGWorkspaceFormProperty>(objectProperty))
        {
        case VPGWorkspaceFormProperty::GitForms:
            obj->RemoveGitForms(index);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_RemoveContainerElement(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGWorkspaceFormPropertyAccessor::_ClearContainer(const int64_t &objectProperty) const
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
