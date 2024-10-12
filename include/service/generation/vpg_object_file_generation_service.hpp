#pragma once

#include <memory>
#include <string>
#include <map>
#include <vector>

#include "json.hpp"
#include "log_config.hpp"
#include "vpg_enum_class.hpp"

using namespace vcc;

class VPGObjectFileGenerationService
{
    private:
        VPGObjectFileGenerationService() = default;
        ~VPGObjectFileGenerationService() {}

        // Json
        static bool IsJsonObject(const VPGEnumClass* enumClass);
        static std::vector<std::wstring> GetObjectToJsonEnumSwitch(const std::wstring &switchVariable, const std::wstring &returnVariable,
            const std::wstring &type, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping);
        static std::vector<std::wstring> GetJsonToObjectEnumSwitch(const std::wstring &propertyName, const bool isKey,
            const std::wstring &type, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping);
        static std::vector<std::wstring> GetObjectToJson(const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
            const std::wstring &parentName, const std::wstring &macro, const std::wstring &type, const std::wstring &propertyName, const bool &isArray, const bool &isMap);
        static std::wstring GetJsonToObjectMapKey(const std::wstring &type);
        static std::vector<std::wstring> GetJsonToObject(const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
            const std::wstring &parentName, const std::wstring &macro, const std::wstring &type, const std::wstring &mapKeyType, const std::wstring &propertyName, const bool &isArray, const bool &isMap);
    public:
        // Attribute
        static std::shared_ptr<Json> GetJsonAttributes(const std::wstring &command, const std::wstring &attributeName);

        // get #include file name
        static std::wstring GetProjectClassIncludeFile(const std::map<std::wstring, std::wstring> &projectClassIncludeFiles, const std::wstring &className);

        static void GenerateHpp(const LogConfig *logProperty, const std::wstring &classPrefix, const std::map<std::wstring, std::wstring> &projectClassIncludeFiles,
            const std::wstring &filePathHpp, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList);
        static void GenerateCpp(const LogConfig *logProperty,
            const std::map<std::wstring, std::wstring> &classPathMapping, const std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping,
            const std::wstring &filePathCpp, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList);
};