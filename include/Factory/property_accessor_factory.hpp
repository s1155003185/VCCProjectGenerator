#include "base_property_accessor_factory.hpp"

#include <memory>

#include "base_property_accessor.hpp"
#include "i_object.hpp"
#include "i_property_accessor.hpp"

using namespace vcc;

std::shared_ptr<IPropertyAccessor> PropertyAccessorFactory::Create(std::shared_ptr<IObject> object)
{
    assert(object != nullptr);

    switch (object->GetObjectType())
    {
    default:
        assert(false);
        break;
    }
    return nullptr;
}
