#pragma once

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "log_config.hpp"

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
        static std::wstring GetJavaPactkageObject(const VPGEnumClass *enumClass, const VPGGenerationOptionExport *option, const std::wstring &middlePath);
        
        static std::wstring GetPropertyAccessorCppToJavaConvertedType(const std::wstring &cppType);
        static std::wstring GetJavaGetterSetterCppToJavaConvertedType(const std::wstring &cppType);
        static std::wstring GetJavaGetterSetterJavaTypeToJavaCaptialType(const std::wstring &javaType);
        
        static std::wstring GenerateJavaBridgeContent(const std::wstring &content, const VPGGenerationOption *option);

        static std::wstring GenerateEnumContent(const std::wstring &projectPrefix, const VPGEnumClass *enumClass, const std::wstring &middlePath, const VPGGenerationOptionExport *option);
        
        // Property
        static std::wstring GetGetterSetterMapKeyContent(const std::wstring &classPropertyEnum, const std::wstring &dllInstantPrefix, const std::wstring &javaType1,
            const std::wstring &orginalFunction, const bool &isReturnNeeded);
        static std::wstring GenerateObjectGetterSetterContainerCount(const VPGEnumClassProperty *property, const std::wstring &projectPrefix, const std::wstring &objectProperty, bool isVector, bool isMap, bool isSet);
        static std::wstring GenerateObjectGetterSetterContainer(const VPGEnumClassProperty *property, const std::wstring &projectPrefix, const std::wstring &objectProperty,
            const std::wstring &cppType1, const std::wstring &javaType1,
            bool isVector, bool isMap, bool isSet, std::set<std::wstring> &importFiles);
        static std::wstring GenerateObjectGetterSetterRead(const VPGEnumClassProperty *property, const std::wstring &projectPrefix, const std::wstring &objectProperty,
            const std::wstring &macro, const std::wstring &cppType1, const std::wstring &javaType1, const std::wstring &cppType2, const std::wstring &javaType2,
            bool isVector, bool isMap, bool isSet,
            std::set<std::wstring> &importFiles);
        static std::wstring GenerateObjectGetterSetterWrite(const VPGEnumClassProperty *property, const std::wstring &projectPrefix, const std::wstring &objectProperty,
            const std::wstring &macro, const std::wstring &cppType1, const std::wstring &javaType1, const std::wstring &cppType2, const std::wstring &javaType2,
            bool isVector, bool isMap, bool isSet,
            const std::map<std::wstring, std::wstring> &importFileMap, std::set<std::wstring> &importFiles);
        static std::wstring GenerateObjectGetterSetterInsert(const VPGEnumClassProperty *property, const std::wstring &projectPrefix, const std::wstring &objectProperty,
            const std::wstring &macro, const std::wstring &cppType1, const std::wstring &javaType1,
            bool isVector, std::set<std::wstring> &importFiles);
        
        static std::wstring GenerateObjectGetterSetter(const std::wstring &projectPrefix, const std::wstring &objectProperty, const VPGEnumClassProperty *property, const std::map<std::wstring, std::wstring> &importFileMap, std::set<std::wstring> &importFiles);
        static std::wstring GenerateObjectContent(const std::wstring &projectPrefix, const VPGEnumClass *enumClass, const std::wstring &middlePath, const std::map<std::wstring, std::wstring> &typeWorkspaceClassRelativePathMap, const VPGGenerationOptionExport *option);
    
        // Form Action
        static std::wstring GenerateFormAction(const std::wstring &projectPrefix, const VPGEnumClass *enumClass);
    public:
        static void GenerateJavaBridge(const LogConfig *logConfig, const std::wstring &targetWorkspace, const std::wstring &dllInterfacehppFilePath, const VPGGenerationOption *option);
        
        static void GenerateEnum(const LogConfig *logConfig, const std::wstring &filePath, const std::wstring &cppMiddlePath, const VPGEnumClass *enumClass, const VPGGenerationOption *option, const VPGGenerationOptionExport *javaOption);
        static void GenerateObject(const LogConfig *logConfig, const std::wstring &filePath, const std::wstring &cppMiddlePath, const VPGEnumClass *enumClass,
            const std::map<std::wstring, std::wstring> &typeWorkspaceClassRelativePathMapObject, const std::map<std::wstring, std::wstring> &typeWorkspaceClassRelativePathMapForm,
            const VPGGenerationOption *option, const VPGGenerationOptionExport *javaOption);
};