#pragma once

#include <string>

enum class VPGDllType;
enum class VPGInterfaceType;
class VPGFileUpdateService
{
    private:
        static std::wstring _DefaultFolder();
        static std::wstring _DownloadVCCResource(std::wstring url, std::wstring branch, std::wstring directory, bool forceUpdate, int64_t logLevel);

    public:
        VPGFileUpdateService() {}
        ~VPGFileUpdateService() {}

        static std::wstring DownloadVCCResource(VPGDllType dllType, std::wstring directory, bool forceUpdate, int64_t logLevel);
        static std::wstring DownloadVCCResource(VPGInterfaceType interfaceType, std::wstring directory, bool forceUpdate, int64_t logLevel);
};