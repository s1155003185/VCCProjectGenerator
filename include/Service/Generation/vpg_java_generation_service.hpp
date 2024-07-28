#pragma once

#include <set>
#include <string>

#include "log_property.hpp"

using namespace vcc;

class VPGJavaGenerationService
{
    private:
        VPGJavaGenerationService() = default;
        ~VPGJavaGenerationService() {}

    public:
        static void Generate(const LogProperty *logProperty, const std::wstring &javaFilePath);
};