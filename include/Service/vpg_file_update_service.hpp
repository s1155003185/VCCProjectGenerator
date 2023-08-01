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
        static std::wstring _GetProjectName(VPGDllType dllType);
        static std::wstring _GetProjectName(VPGInterfaceType interfaceType);

        static std::wstring _DownloadVCCResource(LogProperty &logProperty, std::wstring url, std::wstring branch, std::wstring directory);
        static std::wstring _UpdateVCCResource(LogProperty &logProperty, std::wstring workspace);
    public:
        VPGFileUpdateService() {}
        ~VPGFileUpdateService() {}

        static std::wstring DownloadVCCResource(LogProperty &logProperty, VPGDllType dllType, std::wstring directory);
        static std::wstring DownloadVCCResource(LogProperty &logProperty, VPGInterfaceType interfaceType, std::wstring directory);
        static std::wstring UpdateVCCResource(LogProperty &logProperty, VPGDllType dllType, std::wstring directory);
        static std::wstring UpdateVCCResource(LogProperty &logProperty, VPGInterfaceType interfaceType, std::wstring directory);
};