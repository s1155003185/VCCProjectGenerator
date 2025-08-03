#include "vpg_cpp_generation_manager.hpp"

#include <assert.h>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "vpg_code_reader.hpp"

#define CLASS_ID L"VPGCppGenerationManager"

void VPGCppGenerationManager::add() const
{
    TRY
        vcc::LogService::logInfo(this->getLogConfig().get(), CLASS_ID, L"Copy Project to " + (!this->_Workspace.empty() ? this->_Workspace : vcc::getCurrentFolderPath()));
        this->CreateBasicProject();
        vcc::LogService::logInfo(this->getLogConfig().get(), CLASS_ID, L"Done.");
    CATCH
}