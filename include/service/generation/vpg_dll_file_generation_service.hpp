#pragma once

#include <set>
#include <string>

#include "base_object.hpp"
#include "class_macro.hpp"
#include "log_config.hpp"

using namespace vcc;

class VPGDllFileGenerationServiceOption : public BaseObject<VPGDllFileGenerationServiceOption>
{
    GETSET(bool, IsGeneratePropertyAccessor, false);

    public:
        VPGDllFileGenerationServiceOption() = default;
        virtual ~VPGDllFileGenerationServiceOption() {}
};

class VPGDllFileGenerationService
{
    private:
        VPGDllFileGenerationService() = default;
        ~VPGDllFileGenerationService() {}

    public:
        static void GenerateHpp(const LogConfig *logProperty, const std::wstring &filePathHpp, const VPGDllFileGenerationServiceOption *option);
        static void GenerateCpp(const LogConfig *logProperty, const std::wstring &filePathCpp, const VPGDllFileGenerationServiceOption *option);
};