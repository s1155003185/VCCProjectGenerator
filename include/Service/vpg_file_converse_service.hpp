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

        // file compare



        //static std::wstring convertTypeToCode(std::wstring typeSource, std::wstring );
};