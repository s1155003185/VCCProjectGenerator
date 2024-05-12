#pragma once

#include <map>
#include <string>

#include "platform_type.hpp"

using namespace vcc;

class VPGIncludePathService
{
    private:
        VPGIncludePathService() = default;
        ~VPGIncludePathService() {}
        
    public:
        static void GetWorkspaceIncludePath(const std::wstring &workspace, std::map<std::wstring, std::wstring> &classPathMapping);
        //static void GetSystemIncludePath(const PlatformType &platformType, const std::wstring &workspace, std::map<std::wstring, std::wstring> &classPathMapping);
};