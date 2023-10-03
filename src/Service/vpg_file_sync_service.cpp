#include "vpg_file_sync_service.hpp"

#include <filesystem>
#include <memory>
#include <string>

#include "exception.hpp"
#include "exception_macro.hpp"
#include "exception_type.hpp"
#include "file_helper.hpp"
#include "log_property.hpp"
#include "xml_reader.hpp"

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

std::wstring VPGFileSyncService::SyncFileContent(const std::wstring &src, const std::wstring &dest, 
    const VPGFileContentSyncMode &defaultMode, const std::wstring &commandDelimiter)
{
    std::wstring result = L"";
    try
    {
        VPGFileContentSyncMode mode = defaultMode;
        std::unique_ptr<XMLReader> reader = std::make_unique<XMLReader>();

        size_t srcPos = 0;
        size_t destPos = 0;

        GetNextCharPos(src, srcPos, true);
        GetNextCharPos(dest, destPos, true);

        if (dest.starts_with(commandDelimiter)) {
            
        }


        // 1. read first line to determine sync mode. Dest first, then src. If not exists, then default
        // 2. replace
        switch (mode)
        {
        case VPGFileContentSyncMode::Full:
            result = src;
            break;
        default:
            result = dest;
            break;
        }
    }
    catch(std::exception& e)
    {
        THROW_EXCEPTION(e);
    }
    return result;
}