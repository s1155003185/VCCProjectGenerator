#pragma once

#include <string>

#include "log_property.hpp"
#include "vpg_code_reader.hpp"

const std::wstring VCC_NAMESPACE = L"vcc";
const std::wstring VCC_NAME = VCC_NAMESPACE + L":vccproj";

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
        VPGFileSyncService() = default;
        ~VPGFileSyncService() {}
        static void CopyFile(const LogProperty *logProperty, const std::wstring &sourcePath, const std::wstring &destPath);
        static std::wstring SyncFileContent(const std::wstring &src, const std::wstring &dest, const VPGFileContentSyncMode defaultMode, const std::wstring &commandDelimiter);
};