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
        static std::wstring GetJavaPactkage(const std::wstring &path, const std::wstring &middlePath, const std::wstring &filePathName);
        
        static std::wstring GetPropertyAccessorCppToJavaConvertedType(const std::wstring &cppType);
        static std::wstring GetJavaGetterSetterCppToJavaConvertedType(const std::wstring &cppType);
        
        static std::wstring GenerateJavaBridgeContent(const std::wstring &content, const VPGGenerationOption *option);

        static std::wstring GenerateEnumContent(const std::wstring &projectPrefix, const VPGEnumClass *enumClass, const std::wstring &middlePath, const VPGGenerationOptionExport *option);
        static std::wstring GenerateObjectGetterSetter(const std::wstring &projectPrefix, const std::wstring &objectProperty, const VPGEnumClassProperty *property, const std::map<std::wstring, std::wstring> &importFileMap, std::set<std::wstring> &importFiles);
        static std::wstring GenerateObjectContent(const std::wstring &projectPrefix, const VPGEnumClass *enumClass, const std::wstring &middlePath, const std::map<std::wstring, std::wstring> &typeWorkspaceClassRelativePathMap, const VPGGenerationOptionExport *option);
    public:
        static void GenerateJavaBridge(const LogProperty *logProperty, const std::wstring &dllInterfacehppFilePath, const VPGGenerationOption *option);
        
        static void GenerateEnum(const LogProperty *logProperty, const std::wstring &filePath, const std::wstring &cppMiddlePath, const VPGEnumClass *enumClass, const VPGGenerationOption *option, const VPGGenerationOptionExport *javaOption);
        static void GenerateObject(const LogProperty *logProperty, const std::wstring &filePath, const std::wstring &cppMiddlePath, const VPGEnumClass *enumClass,
            const std::map<std::wstring, std::wstring> &typeWorkspaceClassRelativePathMap,
            const VPGGenerationOption *option, const VPGGenerationOptionExport *javaOption);
};