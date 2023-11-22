#pragma once

#include <map>
#include <string>

#include "log_property.hpp"
#include "vpg_code_reader.hpp"

constexpr auto VCC_NAMESPACE = L"vcc";
constexpr auto VCC_NAME = L"vccproj";

constexpr auto SYNC_TOKEN = L"sync";

// xml
constexpr auto FULL_MODE = L"FULL";
constexpr auto DEMAND_MODE = L"DEMAND";
constexpr auto SKIP_MODE = L"SKIP";
constexpr auto FORCE_MODE = L"FORCE";

// tag
constexpr auto REPLACE_TAG = L"REPLACE";
constexpr auto RESERVE_TAG = L"RESERVE";
constexpr auto PROPERTY_RESERVE_TAG = L"PROPERTYRESERVE";

enum class VPGFileContentSyncMode
{
    NA,
    Force,
    Full,
    Demand,
    Skip
};

enum class VPGFileContentSyncTag
{
    NA,
    Replace,
    Reserve,
    PropertyReserve
};

class VPGFileSyncService
{
    private:
        static VPGFileContentSyncMode _GetSyncMode(const XMLElement &codeElemet);
        static VPGFileContentSyncTag _GetSyncTag(std::wstring tagValue);
        static const XMLElement *_GetTagFromCode(const XMLElement &code, const std::wstring tagName);
        static bool _IsTagReplace(const XMLElement &child);
        static bool _IsTagReserve(const XMLElement &child);
        static bool _IsTagPropertyReserve(const XMLElement &child);

        static std::wstring _GetNextToken(const std::wstring &str, size_t &pos);
        static std::wstring _GetPosOfStringUntilCodeEnd(const std::wstring &str, size_t &pos);
        static std::map<std::wstring, std::wstring> _GetProperyList(const std::wstring &str);
        static std::wstring _GeneratePropertyDemandTag(const XMLElement &src, const XMLElement &dest);

        static std::wstring _GenerateForceCode(const VPGFileContentSyncMode &srcMode, const VPGFileContentSyncMode &destMode, const XMLElement &src, const XMLElement &dest);
        static std::wstring _GenerateFullCode(const VPGFileContentSyncMode &srcMode, const VPGFileContentSyncMode &destMode, const XMLElement &src, const XMLElement &dest);
        static std::wstring _GenerateDemandCode(const VPGFileContentSyncMode &srcMode, const VPGFileContentSyncMode &destMode, const XMLElement &src, const XMLElement &dest);
        static std::wstring _GenerateSkipCode(const std::wstring &dest);
    public:
        VPGFileSyncService() {}
        ~VPGFileSyncService() {}
        static void CopyFile(LogProperty &logProperty, const std::wstring &sourcePath, const std::wstring &destgetPath);
        static std::wstring SyncFileContent(const std::wstring &src, const std::wstring &dest, const VPGFileContentSyncMode &defaultMode);
};