#pragma once

#include <map>
#include <string>

#include "platform_type.hpp"
#include "vpg_enum_class.hpp"

using namespace vcc;

class VPGIncludePathService
{
    private:
        VPGIncludePathService() = default;
        ~VPGIncludePathService() {}
        
    public:
        static void GetWorkspaceIncludePath(const std::wstring &workspace, const std::set<std::wstring> &classMacroList, std::map<std::wstring, std::wstring> &classPathMapping, std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping);
        //static void GetSystemIncludePath(const PlatformType &platformType, const std::wstring &workspace, std::map<std::wstring, std::wstring> &classPathMapping);
};