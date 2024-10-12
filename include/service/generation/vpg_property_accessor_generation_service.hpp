#pragma once

#include <set>
#include <string>
#include <vector>

#include "log_config.hpp"
#include "vpg_enum_class.hpp"

using namespace vcc;

class VPGPropertyAccessorGenerationService
{
    private:
        VPGPropertyAccessorGenerationService() = default;
        ~VPGPropertyAccessorGenerationService() {}

        static bool IsIncludeString(const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList);
        
        static std::vector<std::wstring> GetPropertyAccessorTypeOrder();

        static void GetIsHavingGenerateTypeMapType(const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassProperties, bool &isHavingGeneralType, bool &isHavingVectorType, bool &isHavingMapType);
        static void GenerateRead(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassPropertiesReadOnly, std::wstring &result);
        static void GenerateWrite(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassPropertiesWriteOnly, std::wstring &result);
        static void GenerateInsert(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassPropertiesWriteOnly, std::wstring &result);
        static void GenerateClone(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassPropertiesReadOnly, std::wstring &result);
        static void GenerateContainerCount(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassProperties, std::wstring &result);
        static void GenerateContainerMapKey(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassProperties, std::wstring &result);
        static void GenerateContainerIsContainKey(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassPropertiesReadOnly, std::wstring &result);
        static void GenerateContainerRemove(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassPropertiesWriteOnly, std::wstring &result);

    public:
        static std::wstring GetIncludeFile(const std::map<std::wstring, std::wstring> &projectClassIncludeFiles, const std::wstring &className);

        static void GetPropertyAccessorTypeName(const std::wstring &originalType, std::wstring &convertedType, std::wstring &convertedName, std::wstring &returnResult);

        static void GenerateHpp(const LogConfig *logProperty, const std::wstring &filePathHpp, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList);
        static void GenerateCpp(const LogConfig *logProperty, const std::map<std::wstring, std::wstring> &projectClassIncludeFiles,
            const std::wstring &filePathCpp, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList);
};