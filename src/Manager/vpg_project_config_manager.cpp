#include "vpg_project_config_manager.hpp"

#include <memory>

#include "file_helper.hpp"
#include "json.hpp"
#include "json_builder.hpp"
#include "memory_macro.hpp"

#include "vpg_base_generation_manager.hpp"

using namespace vcc;

void VPGProjectConfigManager::Serialize(const std::wstring workspace, const VPGGenerationOption *option)
{
    unique_ptr<JsonBuilder> builder = make_unique<JsonBuilder>();


}

void VPGProjectConfigManager::Deserialize(const std::wstring workspace, shared_ptr<VPGGenerationOption> option)
{
    //TRY_CATCH(
        std::wstring json = ReadFile(workspace);
        unique_ptr<JsonBuilder> builder = make_unique<JsonBuilder>();
        DECLARE_SPTR(JsonObject, jsonObj);
        builder->Deserialize(json, jsonObj);

        option->SetVersion(jsonObj->GetString(L"Version"));
        option->SetIsGit(jsonObj->GetBool(L"IsGit"));

        option->SetProjectPrefix(jsonObj->GetString(L"ProjectPrefix"));

        option->SetProjectName(jsonObj->GetString(L"ProjectName"));
        option->SetProjectNameDLL(jsonObj->GetString(L"ProjectNameDLL"));
        option->SetProjectNameEXE(jsonObj->GetString(L"ProjectNameEXE"));
        option->SetProjectNameGtest(jsonObj->GetString(L"ProjectNameGtest"));

        option->SetIsExcludeVCCUnitTest(jsonObj->GetBool(L"IsExcludeVCCUnitTest"));


    //)
}
