#pragma once

#include <string>

#include "log_config.hpp"
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

enum class VPGFileContentSyncTagMode
{
    Synchronization,
    Generation
};


class VPGFileSyncService
{
    private:
        VPGFileSyncService() = default;
        virtual ~VPGFileSyncService() {}

        static bool IsSyncTag(const VPGFileContentSyncTagMode &mode, const std::wstring &tag);

        static VPGFileContentSyncMode GetSyncMode(const VPGFileContentSyncTagMode &mode, const Xml *codeElemet);
        static const Xml *GetTagFromCode(const Xml *code, const std::wstring tagName);
        static bool IsTagReplace(const VPGFileContentSyncTagMode &mode, const Xml *child);
        static bool IsTagReserve(const VPGFileContentSyncTagMode &mode, const Xml *child);

        static std::wstring GenerateForceCode(const VPGFileContentSyncMode updatedCodeMode, const VPGFileContentSyncMode originalCodeMode, const Xml *updatedCode, const Xml *originalCode);
        static std::wstring GenerateFullCode(const VPGFileContentSyncTagMode &mode, const VPGFileContentSyncMode updatedCodeMode, const VPGFileContentSyncMode originalCodeMode, const Xml *updatedCode, const Xml *originalCode);
        static std::wstring GenerateDemandCode(const VPGFileContentSyncTagMode &mode, const VPGFileContentSyncMode updatedCodeMode, const VPGFileContentSyncMode originalCodeMode, const Xml *updatedCode, const Xml *originalCode);
        static std::wstring GenerateSkipCode(const std::wstring &originalCode);
    public:
        static void CopyFile(const LogConfig *logConfig, const VPGFileContentSyncTagMode &mode, const std::wstring &sourcePath, const std::wstring &originalCodePath);
        static std::wstring SyncFileContent(const VPGFileContentSyncTagMode &mode, const std::wstring &updatedCode, const std::wstring &originalCode, const VPGFileContentSyncMode defaultMode, const std::wstring &commandDelimiter);
};