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

std::wstring VPGMainFormAddWorkspaceFormArgumentPropertyAccessor::_readString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainFormAddWorkspaceFormArgument>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormAddWorkspaceFormArgumentProperty>(objectProperty))
        {
        case VPGMainFormAddWorkspaceFormArgumentProperty::Name:
            return obj->getName();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGMainFormAddWorkspaceFormArgumentPropertyAccessor::_readStringAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

std::wstring VPGMainFormAddWorkspaceFormArgumentPropertyAccessor::_readStringAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGMainFormAddWorkspaceFormArgumentPropertyAccessor::_writeString(const int64_t &objectProperty, const std::wstring &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainFormAddWorkspaceFormArgument>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormAddWorkspaceFormArgumentProperty>(objectProperty))
        {
        case VPGMainFormAddWorkspaceFormArgumentProperty::Name:
            obj->setName(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGMainFormAddWorkspaceFormArgumentPropertyAccessor::_writeStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormAddWorkspaceFormArgumentPropertyAccessor::_writeStringAtKey(const int64_t &objectProperty, const std::wstring &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormAddWorkspaceFormArgumentPropertyAccessor::_insertStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::shared_ptr<vcc::IObject> VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor::_readObject(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainFormDeleteWorkspaceFormArgument>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormDeleteWorkspaceFormArgumentProperty>(objectProperty))
        {
        case VPGMainFormDeleteWorkspaceFormArgumentProperty::WorkspaceForm:
            return std::static_pointer_cast<vcc::IObject>(obj->getWorkspaceForm());
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor::_readObjectAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor::_readObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

void VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor::_writeObject(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainFormDeleteWorkspaceFormArgument>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormDeleteWorkspaceFormArgumentProperty>(objectProperty))
        {
        case VPGMainFormDeleteWorkspaceFormArgumentProperty::WorkspaceForm:
            obj->setWorkspaceForm(std::static_pointer_cast<VPGWorkspaceForm>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor::_writeObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> /*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor::_writeObjectAtKey(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> /*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor::_insertObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> /*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::shared_ptr<vcc::IObject> VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor::_cloneObject(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainFormDeleteWorkspaceFormArgument>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormDeleteWorkspaceFormArgumentProperty>(objectProperty))
        {
        case VPGMainFormDeleteWorkspaceFormArgumentProperty::WorkspaceForm:
            return std::static_pointer_cast<vcc::IObject>(obj->getWorkspaceForm()->clone());
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor::_cloneObjectAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor::_cloneObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGMainFormPropertyAccessor::_readObject(const int64_t &objectProperty) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGMainFormPropertyAccessor::_readObjectAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGMainForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormProperty>(objectProperty))
        {
        case VPGMainFormProperty::WorkspaceForms:
            return std::static_pointer_cast<vcc::IObject>(obj->getWorkspaceFormsAtIndex(index));
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGMainFormPropertyAccessor::_readObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

void VPGMainFormPropertyAccessor::_writeObject(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> /*value*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormPropertyAccessor::_writeObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> value, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGMainForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormProperty>(objectProperty))
        {
        case VPGMainFormProperty::WorkspaceForms:
            if (index > -1)
                obj->setWorkspaceFormsAtIndex(index, std::static_pointer_cast<VPGWorkspaceForm>(value));
            else
                obj->insertWorkspaceForms(std::static_pointer_cast<VPGWorkspaceForm>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGMainFormPropertyAccessor::_writeObjectAtKey(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> /*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormPropertyAccessor::_insertObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> value, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGMainForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormProperty>(objectProperty))
        {
        case VPGMainFormProperty::WorkspaceForms:
            if (index > -1)
                obj->insertWorkspaceFormsAtIndex(index, std::static_pointer_cast<VPGWorkspaceForm>(value));
            else
                obj->insertWorkspaceForms(std::static_pointer_cast<VPGWorkspaceForm>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

std::shared_ptr<vcc::IObject> VPGMainFormPropertyAccessor::_cloneObject(const int64_t &objectProperty) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGMainFormPropertyAccessor::_cloneObjectAtIndex(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGMainForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormProperty>(objectProperty))
        {
        case VPGMainFormProperty::WorkspaceForms:
            return std::static_pointer_cast<vcc::IObject>(obj->cloneWorkspaceFormsAtIndex(index));
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGMainFormPropertyAccessor::_cloneObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

size_t VPGMainFormPropertyAccessor::_getCount(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormProperty>(objectProperty))
        {
        case VPGMainFormProperty::WorkspaceForms:
            return obj->getWorkspaceForms().size();
        default:
            assert(false);
        }
    CATCH
    return 0;
}

std::set<void *> VPGMainFormPropertyAccessor::_getMapKeys(const int64_t &objectProperty) const
{
    std::set<void *> result;
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return result;
}

bool VPGMainFormPropertyAccessor::_isContainKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return false;
}

void VPGMainFormPropertyAccessor::_remove(const int64_t &objectProperty, const void */*value*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormPropertyAccessor::_removeObject(const int64_t &objectProperty, const vcc::IObject *value)
{
    TRY
        assert(value != nullptr);
        auto obj = std::static_pointer_cast<VPGMainForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormProperty>(objectProperty))
        {
        case VPGMainFormProperty::WorkspaceForms:
            obj->removeWorkspaceForms(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGMainFormPropertyAccessor::_removeAtIndex(const int64_t &objectProperty, const int64_t &index)
{
    TRY
        assert(index >= -1);
        auto obj = std::static_pointer_cast<VPGMainForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormProperty>(objectProperty))
        {
        case VPGMainFormProperty::WorkspaceForms:
            obj->removeWorkspaceFormsAtIndex(index);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGMainFormPropertyAccessor::_removeAtKey(const int64_t &objectProperty, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormPropertyAccessor::_clear(const int64_t &objectProperty)
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormProperty>(objectProperty))
        {
        case VPGMainFormProperty::WorkspaceForms:
            obj->clearWorkspaceForms();
            break;
        default:
            assert(false);
        }
    CATCH
}

std::wstring VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_readString(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainFormRenameWorkspaceFormArgument>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormRenameWorkspaceFormArgumentProperty>(objectProperty))
        {
        case VPGMainFormRenameWorkspaceFormArgumentProperty::NewName:
            return obj->getNewName();
        default:
            assert(false);
        }
    CATCH
    return L"";
}

std::wstring VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_readStringAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

std::wstring VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_readStringAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return L"";
}

void VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_writeString(const int64_t &objectProperty, const std::wstring &value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainFormRenameWorkspaceFormArgument>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormRenameWorkspaceFormArgumentProperty>(objectProperty))
        {
        case VPGMainFormRenameWorkspaceFormArgumentProperty::NewName:
            obj->setNewName(value);
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_writeStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_writeStringAtKey(const int64_t &objectProperty, const std::wstring &/*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_insertStringAtIndex(const int64_t &objectProperty, const std::wstring &/*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::shared_ptr<vcc::IObject> VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_readObject(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainFormRenameWorkspaceFormArgument>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormRenameWorkspaceFormArgumentProperty>(objectProperty))
        {
        case VPGMainFormRenameWorkspaceFormArgumentProperty::WorkspaceForm:
            return std::static_pointer_cast<vcc::IObject>(obj->getWorkspaceForm());
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_readObjectAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_readObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

void VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_writeObject(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainFormRenameWorkspaceFormArgument>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormRenameWorkspaceFormArgumentProperty>(objectProperty))
        {
        case VPGMainFormRenameWorkspaceFormArgumentProperty::WorkspaceForm:
            obj->setWorkspaceForm(std::static_pointer_cast<VPGWorkspaceForm>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_writeObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> /*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_writeObjectAtKey(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> /*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_insertObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> /*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::shared_ptr<vcc::IObject> VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_cloneObject(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGMainFormRenameWorkspaceFormArgument>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGMainFormRenameWorkspaceFormArgumentProperty>(objectProperty))
        {
        case VPGMainFormRenameWorkspaceFormArgumentProperty::WorkspaceForm:
            return std::static_pointer_cast<vcc::IObject>(obj->getWorkspaceForm()->clone());
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_cloneObjectAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor::_cloneObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}
