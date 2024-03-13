#pragma once

#include <string>

#include "log_property.hpp"
#include "vpg_code_reader.hpp"

constexpr auto VCC_NAMESPACE = L"vcc";
constexpr auto VCC_NAME = L"vccproj";

constexpr auto SYNC_TOKEN_SHORT = L"sync";
constexpr auto SYNC_TOKEN_LONG = L"synchronization";

// constexpr auto GEN_TOKEN_SHORT = L"gen";
// constexpr auto GEN_TOKEN_LONG = L"generation";

// xml
constexpr auto FULL_MODE = L"FULL";
constexpr auto DEMAND_MODE = L"DEMAND";
constexpr auto SKIP_MODE = L"SKIP";
constexpr auto FORCE_MODE = L"FORCE";

// tag
constexpr auto REPLACE_TAG = L"REPLACE";
constexpr auto RESERVE_TAG = L"RESERVE";

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
        static bool _IsSyncTag(const std::wstring &tag);

        static VPGFileContentSyncMode _GetSyncMode(const XMLElement *codeElemet);
        static VPGFileContentSyncTag _GetSyncTag(std::wstring tagValue);
        static const XMLElement *_GetTagFromCode(const XMLElement *code, const std::wstring tagName);
        static bool _IsTagReplace(const XMLElement *child);
        static bool _IsTagReserve(const XMLElement *child);

        static std::wstring _GenerateForceCode(const VPGFileContentSyncMode srcMode, const VPGFileContentSyncMode destMode, const XMLElement *src, const XMLElement *dest);
        static std::wstring _GenerateFullCode(const VPGFileContentSyncMode srcMode, const VPGFileContentSyncMode destMode, const XMLElement *src, const XMLElement *dest);
        static std::wstring _GenerateDemandCode(const VPGFileContentSyncMode srcMode, const VPGFileContentSyncMode destMode, const XMLElement *src, const XMLElement *dest);
        static std::wstring _GenerateSkipCode(const std::wstring &dest);
    public:
        VPGFileSyncService() = default;
        ~VPGFileSyncService() {}
        static void CopyFile(const LogProperty *logProperty, const std::wstring &sourcePath, const std::wstring &destPath);
        static std::wstring SyncFileContent(const std::wstring &src, const std::wstring &dest, const VPGFileContentSyncMode defaultMode, const std::wstring &commandDelimiter);
};