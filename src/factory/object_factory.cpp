#include "object_factory.hpp"

#include <assert.h>
#include <memory>

#include "exception_macro.hpp"
#include "i_object.hpp"
#include "object_type.hpp"
#include "vpg_config.hpp"
#include "vpg_enum_class.hpp"
#include "vpg_git_form.hpp"
#include "vpg_git_log.hpp"
#include "vpg_main_form.hpp"
#include "vpg_workspace_form.hpp"

std::shared_ptr<vcc::IObject> ObjectFactory::Create(const ObjectType &objectType, std::shared_ptr<vcc::IObject> parentObject)
{
    std::shared_ptr<vcc::IObject> result = nullptr;
    TRY
        switch (objectType)
        {
        case ObjectType::Config:
            result = std::make_shared<VPGConfig>();
            break;
        case ObjectType::ConfigBehavior:
            result = std::make_shared<VPGConfigBehavior>();
            break;
        case ObjectType::ConfigExport:
            result = std::make_shared<VPGConfigExport>();
            break;
        case ObjectType::ConfigInput:
            result = std::make_shared<VPGConfigInput>();
            break;
        case ObjectType::ConfigOutput:
            result = std::make_shared<VPGConfigOutput>();
            break;
        case ObjectType::ConfigTemplate:
            result = std::make_shared<VPGConfigTemplate>();
            break;
        case ObjectType::EnumClass:
            result = std::make_shared<VPGEnumClass>();
            break;
        case ObjectType::EnumClassAttribute:
            result = std::make_shared<VPGEnumClassAttribute>();
            break;
        case ObjectType::GitForm:
            result = std::make_shared<VPGGitForm>();
            break;
        case ObjectType::GitLog:
            result = std::make_shared<VPGGitLog>();
            break;
        case ObjectType::MainForm:
            result = std::make_shared<VPGMainForm>();
            break;
        case ObjectType::MainFormAddWorkspaceFormArgument:
            result = std::make_shared<VPGMainFormAddWorkspaceFormArgument>();
            break;
        case ObjectType::MainFormDeleteWorkspaceFormArgument:
            result = std::make_shared<VPGMainFormDeleteWorkspaceFormArgument>();
            break;
        case ObjectType::MainFormRenameWorkspaceFormArgument:
            result = std::make_shared<VPGMainFormRenameWorkspaceFormArgument>();
            break;
        case ObjectType::WorkspaceForm:
            result = std::make_shared<VPGWorkspaceForm>();
            break;
        default:
            assert(false);
            break;
        }
        if (result != nullptr)
            result->SetParentObject(parentObject);
    CATCH
    return result;
}
