#pragma once

#include <memory>
#include <string>
#include <map>
#include <vector>

#include "log_config.hpp"
#include "vpg_enum_class.hpp"

using namespace vcc;

class VPGObjectFileGenerationService
{
    private:
        VPGObjectFileGenerationService() = default;
        ~VPGObjectFileGenerationService() {}

        // Json
        static std::vector<std::wstring> GetObjectToJsonEnumSwitch(const std::wstring &switchVariable, const std::wstring &returnVariable,
            const std::wstring &type, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping);
        static std::vector<std::wstring> GetJsonToObjectEnumSwitch(const std::wstring &propertyName, const bool isKey,
            const std::wstring &type, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping);
        static std::vector<std::wstring> GetObjectToJson(const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
            const std::wstring &parentName, const std::wstring &macro, const std::wstring &type, const std::wstring &propertyName, const bool &isArray, const bool &isMap);
        static std::wstring GetJsonToObjectMapKey(const std::wstring &type);
        static std::vector<std::wstring> GetJsonToObject(const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
            const std::wstring &parentName, const std::wstring &macro, const std::wstring &type, const std::wstring &mapKeyType, const std::wstring &propertyName, const bool &isArray, const bool &isMap);
    
        // Hpp
        static void GetHppIncludeFiles(const std::map<std::wstring, std::wstring> &projectClassIncludeFiles,
            const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList,
            bool &isContainForm,
            std::set<std::wstring> &systemFileList,
            std::set<std::wstring> &projectFileList,
            std::set<std::wstring> &abstractClassList,
            std::set<std::wstring> &abstractEnumClassList,
            std::set<std::wstring> &classInCurrentFileList);
        
        // Cpp
        static void GetCppIncludeFiles(
            const std::map<std::wstring, std::wstring> &classPathMapping,
            const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList,
            const bool &isContainForm,
            const bool &isContainJson,
            std::set<std::wstring> &systemIncludeFiles,
            std::set<std::wstring> &customIncludeFiles);

            
        static std::wstring GetCppExternalFunctionForm(const std::wstring &className,
            const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
            const std::shared_ptr<VPGEnumClass> enumClass);

        static std::wstring GetCppExternalFunctionJson(const std::wstring &className,
            const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
            const std::shared_ptr<VPGEnumClass> enumClass);

    public:
        // get #include file name
        static std::wstring GetProjectClassIncludeFile(const std::map<std::wstring, std::wstring> &projectClassIncludeFiles, const std::wstring &className);

        static void GenerateHpp(const LogConfig *logConfig,
            const std::wstring &classPrefix,
            const std::map<std::wstring, std::wstring> &projectClassIncludeFiles,
            const std::wstring &objectFilePathHpp,
            const std::wstring &formFilePathHpp,
            const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList);

        static void GenerateCpp(const LogConfig *logConfig,
            const std::map<std::wstring, std::wstring> &classPathMapping,
            const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
            const std::wstring &objectFilePathCpp,
            const std::wstring &formFilePathCpp,
            const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList);
};