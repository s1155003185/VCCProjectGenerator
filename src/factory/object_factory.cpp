#include "object_factory.hpp"

#include <assert.h>
#include <memory>

#include "i_object.hpp"
#include "object_type.hpp"
#include "vpg_generation_option.hpp"
#include "vpg_git_form.hpp"
#include "vpg_git_log.hpp"

using namespace vcc;

std::shared_ptr<IObject> ObjectFactory::Create(const ObjectType &objectType)
{
    switch (objectType)
    {
    case ObjectType::GenerationOption:
        return std::make_shared<VPGGenerationOption>();
    case ObjectType::GenerationOptionExport:
        return std::make_shared<VPGGenerationOptionExport>();
    case ObjectType::GitForm:
        return std::make_shared<VPGGitForm>();
    case ObjectType::GitLog:
        return std::make_shared<VPGGitLog>();
    default:
        assert(false);
        break;
    }
    return nullptr;
}
