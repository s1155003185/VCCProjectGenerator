#include "vpg_main_form_property_accessor.hpp"

#include <memory>
#include <vector>

#include "exception_macro.hpp"
#include "i_object.hpp"
#include "property_accessor_macro.hpp"
#include "vpg_main_form.hpp"
#include "vpg_main_form_property.hpp"
#include "vpg_workspace_form.hpp"

using namespace vcc;

std::shared_ptr<IObject> VPGMainFormPropertyAccessor::_ReadObject(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGMainForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormProperty>(objectProperty))
        {
        case VPGMainFormProperty::WorkspaceForms:
            return std::static_pointer_cast<IObject>(obj->GetWorkspaceForms(index));
        case VPGMainFormProperty::CurrentWorkspaceForm:
            return std::static_pointer_cast<IObject>(obj->GetCurrentWorkspaceForm());
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGMainFormPropertyAccessor::_ReadObject(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

void VPGMainFormPropertyAccessor::_WriteObject(const int64_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGMainForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormProperty>(objectProperty))
        {
        case VPGMainFormProperty::WorkspaceForms:
            if (index > -1)
                obj->SetWorkspaceForms(index, std::static_pointer_cast<VPGWorkspaceForm>(value));
            else
                obj->InsertWorkspaceForms(std::static_pointer_cast<VPGWorkspaceForm>(value));
            break;
        case VPGMainFormProperty::CurrentWorkspaceForm:
            obj->SetCurrentWorkspaceForm(std::static_pointer_cast<VPGWorkspaceForm>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGMainFormPropertyAccessor::_WriteObject(const int64_t &objectProperty, std::shared_ptr<IObject> /*value*/, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormPropertyAccessor::_InsertObject(const int64_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGMainForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormProperty>(objectProperty))
        {
        case VPGMainFormProperty::WorkspaceForms:
            if (index > -1)
                obj->InsertWorkspaceForms(index, std::static_pointer_cast<VPGWorkspaceForm>(value));
            else
                obj->InsertWorkspaceForms(std::static_pointer_cast<VPGWorkspaceForm>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

std::shared_ptr<IObject> VPGMainFormPropertyAccessor::_CloneObject(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGMainForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormProperty>(objectProperty))
        {
        case VPGMainFormProperty::WorkspaceForms:
            return std::static_pointer_cast<IObject>(obj->CloneWorkspaceForms(index));
        case VPGMainFormProperty::CurrentWorkspaceForm:
            return std::static_pointer_cast<IObject>(obj->GetCurrentWorkspaceForm()->Clone());
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGMainFormPropertyAccessor::_CloneObject(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

size_t VPGMainFormPropertyAccessor::_GetContainerCount(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormProperty>(objectProperty))
        {
        case VPGMainFormProperty::WorkspaceForms:
            return obj->GetWorkspaceForms().size();
        default:
            assert(false);
        }
    CATCH
    return 0;
}

std::set<void *> VPGMainFormPropertyAccessor::_GetMapKeys(const int64_t &objectProperty) const
{
    std::set<void *> result;
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return result;
}

bool VPGMainFormPropertyAccessor::_IsContainKey(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGMainFormPropertyAccessor::_RemoveContainerElement(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGMainForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormProperty>(objectProperty))
        {
        case VPGMainFormProperty::WorkspaceForms:
            obj->RemoveWorkspaceFormsByIndex(index);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGMainFormPropertyAccessor::_RemoveContainerElement(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormPropertyAccessor::_ClearContainer(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormProperty>(objectProperty))
        {
        case VPGMainFormProperty::WorkspaceForms:
            obj->ClearWorkspaceForms();
            break;
        default:
            assert(false);
        }
    CATCH
}
