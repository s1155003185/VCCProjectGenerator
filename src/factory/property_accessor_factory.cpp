#include "property_accessor_factory.hpp"

#include <assert.h>
#include <memory>

#include "base_property_accessor.hpp"
#include "i_object.hpp"
#include "i_property_accessor.hpp"
#include "vpg_generation_option_property_accessor.hpp"
#include "vpg_git_form_property_accessor.hpp"
#include "vpg_git_log_property_accessor.hpp"
#include "vpg_main_form_property_accessor.hpp"
#include "vpg_workspace_form_property_accessor.hpp"

using namespace vcc;

std::shared_ptr<IPropertyAccessor> PropertyAccessorFactory::Create(std::shared_ptr<IObject> object)
{
    assert(object != nullptr);

    switch (object->GetObjectType())
    {
    case ObjectType::GenerationOption:
        return std::make_shared<VPGGenerationOptionPropertyAccessor>(object);
    case ObjectType::GenerationOptionExport:
        return std::make_shared<VPGGenerationOptionExportPropertyAccessor>(object);
    case ObjectType::GitForm:
        return std::make_shared<VPGGitFormPropertyAccessor>(object);
    case ObjectType::GitLog:
        return std::make_shared<VPGGitLogPropertyAccessor>(object);
    case ObjectType::MainForm:
        return std::make_shared<VPGMainFormPropertyAccessor>(object);
    case ObjectType::WorkspaceForm:
        return std::make_shared<VPGWorkspaceFormPropertyAccessor>(object);
    default:
        assert(false);
        break;
    }
    return nullptr;
}
