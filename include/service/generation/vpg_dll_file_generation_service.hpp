#pragma once

#include <set>
#include <string>

#include "base_object.hpp"
#include "class_macro.hpp"
#include "log_config.hpp"

class VPGDllFileGenerationServiceOption : public vcc::BaseObject
{
    GETSET(bool, IsGeneratePropertyAccessor, false);

    public:
        VPGDllFileGenerationServiceOption() = default;
        virtual ~VPGDllFileGenerationServiceOption() {}

        bool GetIsGenerateApplication() const
        {
            // Generate if one of flags is true
            return _IsGeneratePropertyAccessor;
        }
        
        virtual std::shared_ptr<vcc::IObject> Clone() const override
        {
            return std::make_shared<VPGDllFileGenerationServiceOption>(*this);
        }
};

class VPGDllFileGenerationService
{
    private:
        VPGDllFileGenerationService() = default;
        ~VPGDllFileGenerationService() {}

        static std::wstring GenerateApplicationHpp(const VPGDllFileGenerationServiceOption *option);
        static std::wstring GenerateApplicationCpp(const VPGDllFileGenerationServiceOption *option, std::set<std::wstring> &customIncludeFiles);

        static std::wstring GeneratePropertyAccessorHpp(const VPGDllFileGenerationServiceOption *option, std::set<std::wstring> &customIncludeFiles);
        static std::wstring GeneratePropertyAccessorCpp(const VPGDllFileGenerationServiceOption *option, std::set<std::wstring> &customIncludeFiles);

    public:
        static void GenerateHpp(const vcc::LogConfig *logConfig, const std::wstring &filePathHpp, const VPGDllFileGenerationServiceOption *option);
        static void GenerateCpp(const vcc::LogConfig *logConfig, const std::wstring &filePathCpp, const VPGDllFileGenerationServiceOption *option);
};