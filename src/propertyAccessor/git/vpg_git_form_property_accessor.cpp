#include "vpg_git_form_property_accessor.hpp"

#include <memory>

#include "exception_macro.hpp"
#include "i_object.hpp"
#include "property_accessor_macro.hpp"
#include "vpg_git_form.hpp"
#include "vpg_git_form_property.hpp"
#include "vpg_git_log.hpp"

using namespace vcc;

std::shared_ptr<IObject> VPGGitFormPropertyAccessor::_ReadObject(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        std::shared_ptr<VPGGitForm> obj = std::static_pointer_cast<VPGGitForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitFormProperty>(objectProperty))
        {
        case VPGGitFormProperty::Log:
            return std::static_pointer_cast<IObject>(obj->GetLog());
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGGitFormPropertyAccessor::_ReadObject(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

void VPGGitFormPropertyAccessor::_WriteObject(const int64_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        std::shared_ptr<VPGGitForm> obj = std::static_pointer_cast<VPGGitForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitFormProperty>(objectProperty))
        {
        case VPGGitFormProperty::Log:
            obj->SetLog(std::static_pointer_cast<VPGGitLog>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGitFormPropertyAccessor::_WriteObject(const int64_t &objectProperty, std::shared_ptr<IObject> /*value*/, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGitFormPropertyAccessor::_InsertObject(const int64_t &objectProperty, std::shared_ptr<IObject> /*value*/, const int64_t & /*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::shared_ptr<IObject> VPGGitFormPropertyAccessor::_CloneObject(const int64_t &objectProperty, const int64_t &index) const
{
    TRY
        assert(index >= -1);
        std::shared_ptr<VPGGitForm> obj = std::static_pointer_cast<VPGGitForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitFormProperty>(objectProperty))
        {
        case VPGGitFormProperty::Log:
            return std::static_pointer_cast<IObject>(obj->GetLog()->Clone());
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<IObject> VPGGitFormPropertyAccessor::_CloneObject(const int64_t &objectProperty, const void * /*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}
