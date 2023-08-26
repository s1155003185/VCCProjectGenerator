#pragma once

#include <string>
#include <vector>

#include "log_property.hpp"

using namespace vcc;

enum class VPGProjectType;

class VPGFileConverseService
{
    friend class VPGFileConverseServiceTest;
    
    private:
        static bool _ShouldInclude(const std::wstring &path, const std::vector<std::wstring> &includeOnlyRegex);
        static bool _ShouldExclude(const std::wstring &path, const std::vector<std::wstring> &excludeRegex);
        static void _SyncWorkspace(LogProperty &logProperty, std::wstring sourceWorkspace, std::wstring targetWorkspace,
            const std::vector<std::wstring> &includeOnlyRegex, const std::vector<std::wstring> &excludeRegex);

    public:
        VPGFileConverseService() {}
        ~VPGFileConverseService() {}

        // create project
        static void CheckAndCreateDirectory(LogProperty &logProperty, std::wstring workspace);

        // file sync
        static void SyncWorkspace(LogProperty &logProperty, VPGProjectType projectType, std::wstring projectLocalDirectoryParent, std::wstring workspace,
            const std::vector<std::wstring> &includeOnlyRegex, const std::vector<std::wstring> &excludeRegex);
};