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

        bool getIsGenerateApplication() const
        {
            // Generate if one of flags is true
            return _IsGeneratePropertyAccessor;
        }
        
        virtual std::shared_ptr<vcc::IObject> clone() const override
        {
            return std::make_shared<VPGDllFileGenerationServiceOption>(*this);
        }
};

class VPGDllFileGenerationService
{
    private:
        VPGDllFileGenerationService() = default;
        ~VPGDllFileGenerationService() {}

        static std::wstring generateApplicationHpp(const VPGDllFileGenerationServiceOption *option);
        static std::wstring generateApplicationCpp(const VPGDllFileGenerationServiceOption *option, std::set<std::wstring> &customIncludeFiles);

        static std::wstring generatePropertyAccessorHpp(const VPGDllFileGenerationServiceOption *option, std::set<std::wstring> &customIncludeFiles);
        static std::wstring generatePropertyAccessorCpp(const VPGDllFileGenerationServiceOption *option, std::set<std::wstring> &customIncludeFiles);

    public:
        static void generateHpp(const vcc::LogConfig *logConfig, const std::wstring &filePathHpp, const VPGDllFileGenerationServiceOption *option);
        static void generateCpp(const vcc::LogConfig *logConfig, const std::wstring &filePathCpp, const VPGDllFileGenerationServiceOption *option);
};