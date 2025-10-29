#pragma once

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "log_config.hpp"

#include "vpg_enum_class.hpp"
#include "vpg_config.hpp"

class VPGJavaGenerationService
{
    private:
        VPGJavaGenerationService() = delete;
        ~VPGJavaGenerationService() {}

        static std::shared_ptr<VPGConfigExport> getJavaOption(const VPGConfig *option);
        static std::wstring getJavaPactkage(const std::wstring &path, const std::wstring &middlePath, const std::wstring &filePathName);
        static std::wstring getJavaPactkageObject(const VPGEnumClass *enumClass, const VPGConfigExport *option, const std::wstring &middlePath);
        
        static std::map<std::wstring, std::wstring> getImportFileMap(const std::wstring &projectPrefix, const VPGConfigExport *option, const std::map<std::wstring, std::wstring> &typeWorkspaceClassRelativePathMapObject, const std::map<std::wstring, std::wstring> &typeWorkspaceClassRelativePathMapForm);

        static std::wstring getOperationResultFilePath(const std::wstring &projectPrefix, const VPGConfigExport *option);

        static std::wstring getPropertyAccessorCppToJavaConvertedType(const std::wstring &cppType);
        static std::wstring getJavaGetterSetterCppToJavaConvertedType(const std::wstring &cppType);
        static std::wstring getJavaGetterSetterJavaTypeToJavaCaptialType(const std::wstring &javaType);
        
        static std::wstring generateJavaBridgeContent(const std::wstring &content, const VPGConfig *option);

        static std::wstring generateEnumContent(const std::wstring &projectPrefix, const VPGEnumClass *enumClass, const std::wstring &middlePath, const VPGConfigExport *option);
        
        // Property
        static std::wstring getGetterSetterMapKeyContent(const std::wstring &classPropertyEnum, const std::wstring &dllInstantPrefix, const std::wstring &javaType1,
            const std::wstring &orginalFunction, const bool &isReturnNeeded);
        static std::wstring generateObjectGetterSetterContainerCount(const VPGEnumClassAttribute *property, const std::wstring &projectPrefix, const std::wstring &objectProperty, bool isVector, bool isMap, bool isSet);
        static std::wstring generateObjectGetterSetterContainer(const VPGEnumClassAttribute *property, const std::wstring &projectPrefix, const std::wstring &objectProperty,
            const std::wstring &cppType1, const std::wstring &javaType1,
            bool isVector, bool isMap, bool isSet, std::set<std::wstring> &importFiles);
        static std::wstring generateObjectGetterSetterRead(const VPGEnumClassAttribute *property, const std::wstring &projectPrefix, const std::wstring &objectProperty,
            const std::wstring &macro, const std::wstring &cppType1, const std::wstring &javaType1, const std::wstring &cppType2, const std::wstring &javaType2,
            bool isVector, bool isMap, bool isSet,
            std::set<std::wstring> &importFiles);
        static std::wstring generateObjectGetterSetterWrite(const VPGEnumClassAttribute *property, const std::wstring &projectPrefix, const std::wstring &objectProperty,
            const std::wstring &macro, const std::wstring &cppType1, const std::wstring &javaType1, const std::wstring &cppType2, const std::wstring &javaType2,
            bool isVector, bool isMap, bool isSet,
            const std::map<std::wstring, std::wstring> &importFileMap, std::set<std::wstring> &importFiles);
        static std::wstring generateObjectGetterSetterInsert(const VPGEnumClassAttribute *property, const std::wstring &projectPrefix, const std::wstring &objectProperty,
            const std::wstring &cppType1, const std::wstring &javaType1,
            bool isVector, std::set<std::wstring> &importFiles);
        
        static std::wstring generateObjectGetterSetter(const std::wstring &projectPrefix, const std::wstring &objectProperty, const VPGEnumClassAttribute *property, const std::map<std::wstring, std::wstring> &importFileMap, std::set<std::wstring> &importFiles);
        static std::wstring generateObjectContent(const std::wstring &projectPrefix, const VPGEnumClass *enumClass, const std::wstring &middlePath, const std::map<std::wstring, std::wstring> &typeWorkspaceClassRelativePathMap, const VPGConfigExport *option);
    
        // Result
        static std::wstring generateOperationResultContent(const std::wstring &projectPrefix, const VPGConfigExport *option, const std::map<std::wstring, std::wstring> &importFileMap);
    
        // Form Action
        static std::wstring generateFormAction(const std::wstring &projectPrefix, const VPGEnumClass *enumClass);
        static std::wstring generateFormCustomAction(const std::wstring &projectPrefix, const VPGEnumClass *enumClass, const std::wstring &optionResultParent, const std::map<std::wstring, std::wstring> &importFileMap, std::set<std::wstring> &importFiles);
    public:
        static void generateJavaBridge(const vcc::LogConfig *logConfig, const std::wstring &targetWorkspace, const std::wstring &dllInterfacehppFilePath, const VPGConfig *option);
        
        static void generateEnum(const vcc::LogConfig *logConfig, const std::wstring &filePath, const std::wstring &cppMiddlePath, const VPGEnumClass *enumClass, const VPGConfig *option, const VPGConfigExport *javaOption);
        static void generateObject(const vcc::LogConfig *logConfig, const std::wstring &filePath, const std::wstring &cppMiddlePath, const VPGEnumClass *enumClass,
            const std::map<std::wstring, std::wstring> &typeWorkspaceClassRelativePathMapObject, const std::map<std::wstring, std::wstring> &typeWorkspaceClassRelativePathMapForm,
            const VPGConfig *option, const VPGConfigExport *javaOption);
        static void generateOperationResult(const vcc::LogConfig *logConfig, const std::wstring &projectPrefix, const VPGConfigExport *option,
            const std::map<std::wstring, std::wstring> &typeWorkspaceClassRelativePathMapObject, const std::map<std::wstring, std::wstring> &typeWorkspaceClassRelativePathMapForm);
};