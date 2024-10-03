#pragma once

#include <string>

#include "log_property.hpp"
#include "vpg_code_reader.hpp"

enum class VPGFileContentSyncMode
{
    NA,
    Force,
    Full,
    Demand,
    Skip
};

// TODO: PropertyReserve / PropertyAltert
enum class VPGFileContentSyncTag
{
    NA,
    Replace,
    Reserve
};

class VPGFileSyncService
{
    private:
        VPGFileSyncService() = default;
        virtual ~VPGFileSyncService() {}

        static bool IsSyncTag(const std::wstring &tag);

        static VPGFileContentSyncMode GetSyncMode(const Xml *codeElemet);
        static const Xml *GetTagFromCode(const Xml *code, const std::wstring tagName);
        static bool IsTagReplace(const Xml *child);
        static bool IsTagReserve(const Xml *child);

        static std::wstring GenerateForceCode(const VPGFileContentSyncMode srcMode, const VPGFileContentSyncMode destMode, const Xml *src, const Xml *dest);
        static std::wstring GenerateFullCode(const VPGFileContentSyncMode srcMode, const VPGFileContentSyncMode destMode, const Xml *src, const Xml *dest);
        static std::wstring GenerateDemandCode(const VPGFileContentSyncMode srcMode, const VPGFileContentSyncMode destMode, const Xml *src, const Xml *dest);
        static std::wstring GenerateSkipCode(const std::wstring &dest);
    public:
        static void CopyFile(const LogProperty *logProperty, const std::wstring &sourcePath, const std::wstring &destPath);
        static std::wstring SyncFileContent(const std::wstring &src, const std::wstring &dest, const VPGFileContentSyncMode defaultMode, const std::wstring &commandDelimiter);
};