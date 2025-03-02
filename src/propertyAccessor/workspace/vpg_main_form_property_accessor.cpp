#include "vpg_main_form_property_accessor.hpp"

#include <memory>
#include <string>
#include <vector>

#include "exception_macro.hpp"
#include "i_object.hpp"
#include "property_accessor_macro.hpp"
#include "vpg_main_form.hpp"
#include "vpg_main_form_property.hpp"
#include "vpg_workspace_form.hpp"

using namespace vcc;

std::wstring VPGMainFormAddWorkspaceFormArgumentPropertyAccessor::_ReadString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainFormAddWorkspaceFormArgument>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormAddWorkspaceFormArgumentProperty>(objectProperty))
        {
        case VPGMainFormAddWorkspaceFormArgumentProperty::Name:
            return obj->GetName();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGMainFormAddWorkspaceFormArgumentPropertyAccessor::_ReadStringAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

std::wstring VPGMainFormAddWorkspaceFormArgumentPropertyAccessor::_ReadStringAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGMainFormAddWorkspaceFormArgumentPropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring &value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainFormAddWorkspaceFormArgument>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormAddWorkspaceFormArgumentProperty>(objectProperty))
        {
        case VPGMainFormAddWorkspaceFormArgumentProperty::Name:
            obj->SetName(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGMainFormAddWorkspaceFormArgumentPropertyAccessor::_WriteStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormAddWorkspaceFormArgumentPropertyAccessor::_WriteStringAtKey(const int64_t &objectProperty, const std::wstring &/*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormAddWorkspaceFormArgumentPropertyAccessor::_InsertStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::shared_ptr<IObject> VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor::_ReadObject(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainFormDeleteWorkspaceFormArgument>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormDeleteWorkspaceFormArgumentProperty>(objectProperty))
        {
        case VPGMainFormDeleteWorkspaceFormArgumentProperty::WorkspaceForm:
            return std::static_pointer_cast<IObject>(obj->GetWorkspaceForm());
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor::_ReadObjectAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor::_ReadObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

void VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor::_WriteObject(const int64_t &objectProperty, std::shared_ptr<IObject> value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainFormDeleteWorkspaceFormArgument>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormDeleteWorkspaceFormArgumentProperty>(objectProperty))
        {
        case VPGMainFormDeleteWorkspaceFormArgumentProperty::WorkspaceForm:
            obj->SetWorkspaceForm(std::static_pointer_cast<VPGWorkspaceForm>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor::_WriteObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<IObject> /*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor::_WriteObjectAtKey(const int64_t &objectProperty, std::shared_ptr<IObject> /*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor::_InsertObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<IObject> /*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::shared_ptr<IObject> VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor::_CloneObject(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainFormDeleteWorkspaceFormArgument>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormDeleteWorkspaceFormArgumentProperty>(objectProperty))
        {
        case VPGMainFormDeleteWorkspaceFormArgumentProperty::WorkspaceForm:
            return std::static_pointer_cast<IObject>(obj->GetWorkspaceForm()->Clone());
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor::_CloneObjectAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor::_CloneObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGMainFormPropertyAccessor::_ReadObject(const int64_t &objectProperty) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGMainFormPropertyAccessor::_ReadObjectAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGMainForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormProperty>(objectProperty))
        {
        case VPGMainFormProperty::WorkspaceForms:
            return std::static_pointer_cast<IObject>(obj->GetWorkspaceFormsAtIndex(index));
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGMainFormPropertyAccessor::_ReadObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

void VPGMainFormPropertyAccessor::_WriteObject(const int64_t &objectProperty, std::shared_ptr<IObject> /*value*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormPropertyAccessor::_WriteObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGMainForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormProperty>(objectProperty))
        {
        case VPGMainFormProperty::WorkspaceForms:
            if (index > -1)
                obj->SetWorkspaceFormsAtIndex(index, std::static_pointer_cast<VPGWorkspaceForm>(value));
            else
                obj->InsertWorkspaceForms(std::static_pointer_cast<VPGWorkspaceForm>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGMainFormPropertyAccessor::_WriteObjectAtKey(const int64_t &objectProperty, std::shared_ptr<IObject> /*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormPropertyAccessor::_InsertObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGMainForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormProperty>(objectProperty))
        {
        case VPGMainFormProperty::WorkspaceForms:
            if (index > -1)
                obj->InsertWorkspaceFormsAtIndex(index, std::static_pointer_cast<VPGWorkspaceForm>(value));
            else
                obj->InsertWorkspaceForms(std::static_pointer_cast<VPGWorkspaceForm>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

std::shared_ptr<IObject> VPGMainFormPropertyAccessor::_CloneObject(const int64_t &objectProperty) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGMainFormPropertyAccessor::_CloneObjectAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGMainForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormProperty>(objectProperty))
        {
        case VPGMainFormProperty::WorkspaceForms:
            return std::static_pointer_cast<IObject>(obj->CloneWorkspaceFormsAtIndex(index));
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGMainFormPropertyAccessor::_CloneObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

size_t VPGMainFormPropertyAccessor::_GetCount(const int64_t &objectProperty) const
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

bool VPGMainFormPropertyAccessor::_IsContainKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGMainFormPropertyAccessor::_Remove(const int64_t &objectProperty, const void */*value*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormPropertyAccessor::_RemoveObject(const int64_t &objectProperty, const IObject *value) const
{
    TRY
        assert(value != nullptr);
        auto obj = std::static_pointer_cast<VPGMainForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormProperty>(objectProperty))
        {
        case VPGMainFormProperty::WorkspaceForms:
            obj->RemoveWorkspaceForms(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGMainFormPropertyAccessor::_RemoveAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGMainForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormProperty>(objectProperty))
        {
        case VPGMainFormProperty::WorkspaceForms:
            obj->RemoveWorkspaceFormsAtIndex(index);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGMainFormPropertyAccessor::_RemoveAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormPropertyAccessor::_Clear(const int64_t &objectProperty) const
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

std::wstring VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_ReadString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainFormRenameWorkspaceFormArgument>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormRenameWorkspaceFormArgumentProperty>(objectProperty))
        {
        case VPGMainFormRenameWorkspaceFormArgumentProperty::NewName:
            return obj->GetNewName();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_ReadStringAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

std::wstring VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_ReadStringAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring &value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainFormRenameWorkspaceFormArgument>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormRenameWorkspaceFormArgumentProperty>(objectProperty))
        {
        case VPGMainFormRenameWorkspaceFormArgumentProperty::NewName:
            obj->SetNewName(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_WriteStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_WriteStringAtKey(const int64_t &objectProperty, const std::wstring &/*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_InsertStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::shared_ptr<IObject> VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_ReadObject(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainFormRenameWorkspaceFormArgument>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormRenameWorkspaceFormArgumentProperty>(objectProperty))
        {
        case VPGMainFormRenameWorkspaceFormArgumentProperty::WorkspaceForm:
            return std::static_pointer_cast<IObject>(obj->GetWorkspaceForm());
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_ReadObjectAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_ReadObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

void VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_WriteObject(const int64_t &objectProperty, std::shared_ptr<IObject> value) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainFormRenameWorkspaceFormArgument>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormRenameWorkspaceFormArgumentProperty>(objectProperty))
        {
        case VPGMainFormRenameWorkspaceFormArgumentProperty::WorkspaceForm:
            obj->SetWorkspaceForm(std::static_pointer_cast<VPGWorkspaceForm>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_WriteObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<IObject> /*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_WriteObjectAtKey(const int64_t &objectProperty, std::shared_ptr<IObject> /*value*/, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_InsertObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<IObject> /*value*/, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::shared_ptr<IObject> VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_CloneObject(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainFormRenameWorkspaceFormArgument>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormRenameWorkspaceFormArgumentProperty>(objectProperty))
        {
        case VPGMainFormRenameWorkspaceFormArgumentProperty::WorkspaceForm:
            return std::static_pointer_cast<IObject>(obj->GetWorkspaceForm()->Clone());
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_CloneObjectAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_CloneObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}
