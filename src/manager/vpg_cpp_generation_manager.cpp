#include "vpg_cpp_generation_manager.hpp"

#include <assert.h>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "vpg_code_reader.hpp"

#define CLASS_ID L"VPGCppGenerationManager"

void VPGCppGenerationManager::Add() const
{
    TRY
        vcc::LogService::LogInfo(this->GetLogConfig().get(), CLASS_ID, L"Copy Project to " + (!this->_Workspace.empty() ? this->_Workspace : vcc::GetCurrentFolderPath()));
        this->CreateBasicProject();
        vcc::LogService::LogInfo(this->GetLogConfig().get(), CLASS_ID, L"Done.");
    CATCH
}