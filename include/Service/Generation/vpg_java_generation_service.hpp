#pragma once

#include <set>
#include <string>

#include "log_property.hpp"

#include "vpg_generation_option.hpp"

using namespace vcc;

class VPGJavaGenerationService
{
    private:
        VPGJavaGenerationService() = default;
        ~VPGJavaGenerationService() {}

        static std::shared_ptr<VPGGenerationOptionExport> GetJavaOption(const VPGGenerationOption *option);
        static std::wstring GetCppToJavaConvertedType(const std::wstring &cppType);
        static std::wstring GenerateJavaBridgeContent(const std::wstring &content, const VPGGenerationOption *option);

    public:
        static void GenerateJavaBridge(const LogProperty *logProperty, const std::wstring &dllInterfacehppFilePath, const VPGGenerationOption *option);
};