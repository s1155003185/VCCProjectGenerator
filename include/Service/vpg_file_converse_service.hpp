#pragma once

#include <string>

#include "log_property.hpp"

using namespace vcc;

class VPGFileConverseService
{
    public:
        VPGFileConverseService() {}
        ~VPGFileConverseService() {}

        static void CheckAndCreateDirectory(LogProperty &logproperty, std::wstring workspace);

        //static std::wstring convertTypeToCode(std::wstring typeSource, std::wstring );
};