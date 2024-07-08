#include "property_accessor_factory.hpp"

#include <assert.h>
#include <memory>

#include "base_property_accessor.hpp"
#include "i_object.hpp"
#include "i_property_accessor.hpp"
#include "vpg_generation_option_property_accessor.hpp"

using namespace vcc;

std::shared_ptr<IPropertyAccessor> PropertyAccessorFactory::Create(std::shared_ptr<IObject> object)
{
    assert(object != nullptr);

    switch (object->GetObjectType())
    {
    case ObjectType::VPGGenerationOption:
        return std::make_shared<VPGGenerationOptionPropertyAccessor>(object);
    case ObjectType::VPGGenerationOptionExport:
        return std::make_shared<VPGGenerationOptionExportPropertyAccessor>(object);
    default:
        assert(false);
        break;
    }
    return nullptr;
}
