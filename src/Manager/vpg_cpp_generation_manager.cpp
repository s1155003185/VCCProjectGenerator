#include "vpg_cpp_generation_manager.hpp"

#include <assert.h>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "vpg_code_reader.hpp"

using namespace vcc;

#define CLASS_ID L"VPGCppGenerationManager"

void VPGCppGenerationManager::Add() const
{
    TRY_CATCH(){
        LogService::LogInfo(this->GetLogProperty().get(), CLASS_ID, L"Copy Project to " + this->GetOption()->GetWorkspaceDestination());
        this->CreateBasicProject();
        LogService::LogInfo(this->GetLogProperty().get(), CLASS_ID, L"Done.");
    }
}