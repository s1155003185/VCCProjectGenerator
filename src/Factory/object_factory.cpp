#include "object_factory.hpp"

#include <assert.h>
#include <memory>

#include "i_object.hpp"
#include "object_type.hpp"
#include "vpg_generation_option.hpp"

using namespace vcc;

std::shared_ptr<IObject> ObjectFactory::Create(const ObjectType &objectType)
{
    switch (objectType)
    {
    case ObjectType::VPGGenerationOption:
        return std::make_shared<VPGGenerationOption>();
    case ObjectType::VPGGenerationOptionExport:
        return std::make_shared<VPGGenerationOptionExport>();
    default:
        assert(false);
        break;
    }
    return nullptr;
}
