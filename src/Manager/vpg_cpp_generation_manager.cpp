#include "vpg_cpp_generation_manager.hpp"

#include <assert.h>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"

#include "vpg_code_reader.hpp"
#include "vpg_directory_sync_service.hpp"

using namespace vcc;

void VPGCppGenerationManager::Add(const std::wstring &srcWorkspace, const std::wstring &destWorkspace, const VPGGenerationOption *option)
{
    TRY_CATCH(
        assert(!srcWorkspace.empty());
        assert(!destWorkspace.empty());

        VPGDirectorySyncService::CheckAndCreateDirectory(this->GetLogProperty().get(), destWorkspace);
        if (IsFileExists(ConcatPath(srcWorkspace, MakeFileName))) {
            std::wstring destFilePath = ConcatPath(destWorkspace, MakeFileName);

            LogService::LogInfo(this->GetLogProperty().get(), L"CPPGenerator", L"Copy Makefile to " + destWorkspace);
            CopyFile(ConcatPath(srcWorkspace, MakeFileName), destFilePath);
            LogService::LogInfo(this->GetLogProperty().get(), L"CPPGenerator", L"Done.");

            std::wstring fileContent = ReadFile(destFilePath);
            WriteFile(destFilePath, this->AdjustMakefile(fileContent, option), true);
        }
    )
}