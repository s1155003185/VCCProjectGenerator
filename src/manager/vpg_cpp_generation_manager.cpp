#include "vpg_cpp_generation_manager.hpp"

#include <assert.h>

#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "vpg_code_reader.hpp"
const std::wstring classId = L"VPGCppGenerationManager";

void VPGCppGenerationManager::add() const {
    TRY vcc::LogService::logInfo(
        this->getLogConfig().get(), classId,
        L"Copy Project to " +
            (!this->_Workspace.empty() ? this->_Workspace : vcc::getCurrentFolderPath()));
    this->CreateBasicProject();
    vcc::LogService::logInfo(this->getLogConfig().get(), classId, L"Done.");
    CATCH
}
