#include "object_factory.hpp"

#include <assert.h>
#include <memory>

#include "exception_macro.hpp"
#include "i_object.hpp"
#include "object_type.hpp"
#include "vpg_generation_option.hpp"
#include "vpg_git_form.hpp"
#include "vpg_git_log.hpp"
#include "vpg_main_form.hpp"
#include "vpg_workspace_form.hpp"

using namespace vcc;

std::shared_ptr<IObject> ObjectFactory::Create(const ObjectType &objectType, std::shared_ptr<IObject> parentObject)
{
    std::shared_ptr<IObject> result = nullptr;
    TRY
        switch (objectType)
        {
        case ObjectType::GenerationOption:
            result = std::make_shared<VPGGenerationOption>();
            break;
        case ObjectType::GenerationOptionExport:
            result = std::make_shared<VPGGenerationOptionExport>();
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
