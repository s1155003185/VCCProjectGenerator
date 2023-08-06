#pragma once

#include <string>
#include <vector>

#include "log_property.hpp"


using namespace vcc;

class VPGFileConverseService
{
    public:
        VPGFileConverseService() {}
        ~VPGFileConverseService() {}

        // create project
        static void CheckAndCreateDirectory(LogProperty &logProperty, std::wstring workspace);
        static void GetFileDifferenceInWorkspaces(std::wstring sourceWorkspace, std::wstring targetWorkspace,
            std::vector<std::wstring> &needToAdd, std::vector<std::wstring> &needToDelete, std::vector<std::wstring> &needToModify);

        // file compare



        //static std::wstring convertTypeToCode(std::wstring typeSource, std::wstring );
};