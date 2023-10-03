#pragma once

#include <string>

#include "log_property.hpp"

using namespace vcc;

enum class VPGFileContentSyncMode
{
    Full,
    Demand,
    Skip
};

enum class VPGFileContentSyncTag
{
    Replace,
    Reserve
};

class VPGFileSyncService
{
    public:
        VPGFileSyncService() {}
        ~VPGFileSyncService() {}
        static void CopyFile(LogProperty &logProperty, const std::wstring &sourcePath, const std::wstring &targetPath);
        static std::wstring SyncFileContent(const std::wstring &src, const std::wstring &dest, 
            const VPGFileContentSyncMode &defaultMode, const std::wstring &commandDelimiter);
};