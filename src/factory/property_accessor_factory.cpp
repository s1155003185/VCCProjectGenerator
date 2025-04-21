#include "property_accessor_factory.hpp"

#include <assert.h>
#include <memory>

#include "base_property_accessor.hpp"
#include "i_object.hpp"
#include "i_property_accessor.hpp"
#include "vpg_config_property_accessor.hpp"
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
    case ObjectType::Config:
        return std::make_shared<VPGConfigPropertyAccessor>(object);
    case ObjectType::ConfigBehavior:
        return std::make_shared<VPGConfigBehaviorPropertyAccessor>(object);
    case ObjectType::ConfigExport:
        return std::make_shared<VPGConfigExportPropertyAccessor>(object);
    case ObjectType::ConfigInput:
        return std::make_shared<VPGConfigInputPropertyAccessor>(object);
    case ObjectType::ConfigOutput:
        return std::make_shared<VPGConfigOutputPropertyAccessor>(object);
    case ObjectType::ConfigTemplate:
        return std::make_shared<VPGConfigTemplatePropertyAccessor>(object);
    case ObjectType::GitForm:
        return std::make_shared<VPGGitFormPropertyAccessor>(object);
    case ObjectType::GitLog:
        return std::make_shared<VPGGitLogPropertyAccessor>(object);
    case ObjectType::MainForm:
        return std::make_shared<VPGMainFormPropertyAccessor>(object);
    case ObjectType::MainFormAddWorkspaceFormArgument:
        return std::make_shared<VPGMainFormAddWorkspaceFormArgumentPropertyAccessor>(object);
    case ObjectType::MainFormDeleteWorkspaceFormArgument:
        return std::make_shared<VPGMainFormDeleteWorkspaceFormArgumentPropertyAccessor>(object);
    case ObjectType::MainFormRenameWorkspaceFormArgument:
        return std::make_shared<VPGMainFormRenameWorkspaceFormArgumentPropertyAccessor>(object);
    case ObjectType::WorkspaceForm:
        return std::make_shared<VPGWorkspaceFormPropertyAccessor>(object);
    default:
        assert(false);
        break;
    }
    return nullptr;
}
