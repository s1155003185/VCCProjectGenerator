#include "vpg_git_form_property_accessor.hpp"

#include <memory>

#include "exception_macro.hpp"
#include "i_object.hpp"
#include "property_accessor_macro.hpp"
#include "vpg_git_form.hpp"
#include "vpg_git_form_property.hpp"
#include "vpg_git_log.hpp"

std::shared_ptr<vcc::IObject> VPGGitFormPropertyAccessor::_readObject(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGitForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitFormProperty>(objectProperty))
        {
        case VPGGitFormProperty::Log:
            return std::static_pointer_cast<vcc::IObject>(obj->getLog());
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGGitFormPropertyAccessor::_readObjectAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGGitFormPropertyAccessor::_readObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

void VPGGitFormPropertyAccessor::_writeObject(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> value)
{
    TRY
        auto obj = std::static_pointer_cast<VPGGitForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitFormProperty>(objectProperty))
        {
        case VPGGitFormProperty::Log:
            obj->setLog(std::static_pointer_cast<VPGGitLog>(value));
            break;
        default:
            assert(false);
        }
    CATCH
}

void VPGGitFormPropertyAccessor::_writeObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> /*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGitFormPropertyAccessor::_writeObjectAtKey(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> /*value*/, const void */*key*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

void VPGGitFormPropertyAccessor::_insertObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> /*value*/, const int64_t &/*index*/)
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
}

std::shared_ptr<vcc::IObject> VPGGitFormPropertyAccessor::_cloneObject(const int64_t &objectProperty) const
{
    TRY
        auto obj = std::static_pointer_cast<VPGGitForm>(_Object);
        assert(obj != nullptr);
        switch(static_cast<VPGGitFormProperty>(objectProperty))
        {
        case VPGGitFormProperty::Log:
            return std::static_pointer_cast<vcc::IObject>(obj->getLog()->clone());
        default:
            assert(false);
        }
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGGitFormPropertyAccessor::_cloneObjectAtIndex(const int64_t &objectProperty, const int64_t &/*index*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}

std::shared_ptr<vcc::IObject> VPGGitFormPropertyAccessor::_cloneObjectAtKey(const int64_t &objectProperty, const void */*key*/) const
{
    TRY
        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND
    CATCH
    return nullptr;
}
