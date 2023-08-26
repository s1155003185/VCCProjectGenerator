#include "vpg_file_sync_service.hpp"

#include <filesystem>
#include <string>

#include "exception.hpp"
#include "exception_macro.hpp"
#include "exception_type.hpp"
#include "file_helper.hpp"
#include "log_property.hpp"

using namespace vcc;

void VPGFileSyncService::CopyFile(LogProperty &logProperty, const std::wstring &sourcePath, const std::wstring &targetPath)
{
    try
    {
        if (!IsFileExists(sourcePath))
            THROW_EXCEPTION_M(ExceptionType::FILE_NOT_FOUND, sourcePath + L": File not found.");

        bool isAdd = IsFileExists(targetPath);
        std::filesystem::copy_file(PATH(sourcePath), PATH(targetPath), std::filesystem::copy_options::overwrite_existing);

        if (isAdd)
            LogService::LogInfo(logProperty, L"", L"Added File: " + targetPath);
        else
            LogService::LogInfo(logProperty, L"", L"Updated File: " + targetPath);
    }
    catch(std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
}