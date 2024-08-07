#pragma once

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "log_property.hpp"

#include "vpg_enum_class.hpp"
#include "vpg_generation_option.hpp"

using namespace vcc;

class VPGJavaGenerationService
{
    private:
        VPGJavaGenerationService() = default;
        ~VPGJavaGenerationService() {}

        static std::shared_ptr<VPGGenerationOptionExport> GetJavaOption(const VPGGenerationOption *option);
        static std::wstring GetJavaPactkage(const std::wstring &path, const std::wstring &filePathName);
        static std::wstring GetCppToJavaConvertedType(const std::wstring &cppType);
        static std::wstring GenerateJavaBridgeContent(const std::wstring &content, const VPGGenerationOption *option);

        static std::wstring GenerateEnumContent(const VPGEnumClass *enumClass, const VPGGenerationOptionExport *option);
        static std::wstring GenerateObjectContent(const VPGEnumClass *enumClass, const VPGGenerationOptionExport *option);
    public:
        static void GenerateJavaBridge(const LogProperty *logProperty, const std::wstring &dllInterfacehppFilePath, const VPGGenerationOption *option);
        static void GenerateEnumAndObject(const LogProperty *logProperty, const std::wstring &workspace, const VPGGenerationOption *option);
};