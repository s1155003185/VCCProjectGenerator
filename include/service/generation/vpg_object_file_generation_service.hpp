#pragma once

#include <memory>
#include <string>
#include <map>
#include <vector>

#include "log_config.hpp"
#include "vpg_enum_class.hpp"
#include "vpg_config.hpp"

class VPGObjectFileGenerationService
{
    friend class VPGObjectFileGenerationServiceTest;
    
    private:
        VPGObjectFileGenerationService() = delete;
        ~VPGObjectFileGenerationService() {}

        // Clone
        static std::wstring getCloneFunction(const VPGEnumClass *enumClass, const std::wstring &className, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping, const bool &isCpp);
        static std::wstring getConstructorContent(const VPGEnumClass *enumClass, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping, const bool &isHeader);

        // Json
        static std::vector<std::wstring> getObjectToJsonEnumSwitch(const std::wstring &switchVariable, const std::wstring &returnVariable,
            const std::wstring &type, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping);
        static std::vector<std::wstring> getJsonToObjectEnumSwitch(const std::wstring &propertyName, const bool isKey,
            const std::wstring &type, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping);
        static std::vector<std::wstring> getObjectToJson(const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
            const std::wstring &parentName, const std::wstring &macro, const std::wstring &type, const std::wstring &propertyName, const bool &isArray, const bool &isMap);
        static std::wstring getJsonToObjectMapKey(const std::wstring &type);
        static std::vector<std::wstring> getJsonToObject(const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
            const std::wstring &parentName, const std::wstring &macro, const std::wstring &type, const std::wstring &mapKeyType, const std::wstring &propertyName, const bool &isArray, const bool &isMap);
    
        // Hpp
        static void getHppIncludeFiles(const std::map<std::wstring, std::wstring> &projectClassIncludeFiles,
            const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
            const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList,
            bool &isContainForm,
            std::set<std::wstring> &systemFileList,
            std::set<std::wstring> &projectFileList,
            std::set<std::wstring> &abstractClassList,
            std::set<std::wstring> &abstractEnumClassList,
            std::set<std::wstring> &classInCurrentFileList);

        static std::wstring getHppConstructor(const VPGEnumClass *enumClass, const std::wstring &classPrefix, const std::wstring &className, const std::wstring &baseClassNameWithoutQuote, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping);
        static std::wstring getHppProperties(const VPGEnumClass *enumClass, const std::wstring &className, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping);
        static std::wstring getHppPrivateFunctions(const VPGEnumClass *enumClass, const std::wstring &className);
        static std::wstring getHppProtectedFunctions(const VPGEnumClass *enumClass, const std::wstring &className);
        static std::wstring getHppPublicCloneFunctions(const VPGEnumClass *enumClass, const std::wstring &className, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping);
        static std::wstring getHppPublicJsonFunctions(const VPGEnumClass *enumClass);
        static std::wstring getHppPublicFunctions(const VPGEnumClass *enumClass, const VPGConfig *option);
        static std::wstring getHppPublicCustomFunctions(const VPGEnumClass *enumClass, const std::wstring &className);
        
        // Cpp
        static void getCppIncludeFiles(
            const std::map<std::wstring, std::wstring> &classPathMapping,
            const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList,
            const bool &isContainForm,
            const bool &isContainJson,
            std::set<std::wstring> &systemIncludeFiles,
            std::set<std::wstring> &customIncludeFiles);

        static std::wstring getCppCustomHeader(const bool &isContainForm);
        static std::wstring getCppConstructor(const VPGEnumClass *enumClass, const std::wstring &classPrefix, const std::wstring &className, const std::wstring &baseClassNameWithoutQuote, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping);
        static std::wstring getCppCloneFunctions(const VPGEnumClass *enumClass, const std::wstring &className, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping);
        static std::wstring getCppJsonFunction(const std::wstring &className,
            const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
            const std::shared_ptr<VPGEnumClass> enumClass);
        static std::wstring getCppInitialize(const VPGEnumClass *enumClass, const std::wstring &className, const std::wstring &baseClassName);
        static std::wstring getCppAction(const VPGEnumClass *enumClass, const std::wstring &className);
        static std::wstring getCppCustomFunction(const bool &isContainForm);

    public:
        // get #include file name
        static std::wstring getProjectClassIncludeFile(const std::map<std::wstring, std::wstring> &projectClassIncludeFiles, const std::wstring &className);

        static std::wstring GenerateHppClass(const VPGEnumClass* enumClass, const VPGConfig *option, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping);
        static void GenerateHpp(const vcc::LogConfig *logConfig,
            const VPGConfig *option,
            const std::map<std::wstring, std::wstring> &projectClassIncludeFiles,
            const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
            const std::wstring &objectFilePathHpp,
            const std::wstring &formFilePathHpp,
            const std::wstring &actionFolderPathHpp,
            const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList);

        static void GenerateCpp(const vcc::LogConfig *logConfig,
            const std::wstring &classPrefix,
            const std::map<std::wstring, std::wstring> &classPathMapping,
            const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
            const std::wstring &objectFilePathCpp,
            const std::wstring &formFilePathCpp,
            const std::wstring &actionFolderPathCpp,
            const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList);
};