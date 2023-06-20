#pragma once

#include "log_property.hpp"

#include <string>

enum class VPGDllType;
enum class VPGInterfaceType;

using namespace vcc;
class VPGFileUpdateService
{
    private:
        static std::wstring _DefaultFolder(LogProperty &logProperty);
        static std::wstring _DownloadVCCResource(LogProperty &logProperty, std::wstring url, std::wstring branch, std::wstring directory, bool forceUpdate, int64_t logLevel);

    public:
        VPGFileUpdateService() {}
        ~VPGFileUpdateService() {}

        static std::wstring DownloadVCCResource(LogProperty &logProperty, VPGDllType dllType, std::wstring directory, bool forceUpdate, int64_t logLevel);
        static std::wstring DownloadVCCResource(LogProperty &logProperty, VPGInterfaceType interfaceType, std::wstring directory, bool forceUpdate, int64_t logLevel);
};