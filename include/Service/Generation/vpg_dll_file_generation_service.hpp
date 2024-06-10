#pragma once

#include <set>
#include <string>

#include "log_property.hpp"

using namespace vcc;

class VPGDllFileGenerationService
{
    private:
        VPGDllFileGenerationService() = default;
        ~VPGDllFileGenerationService() {}

    public:
        static void GenerateHpp(const LogProperty *logProperty, const std::wstring &hppFilePath);
        static void GenerateCpp(const LogProperty *logProperty, const std::wstring &cppFilePath);
};