#pragma once

#include <set>
#include <string>
#include <vector>

#include "log_config.hpp"
#include "vpg_enum_class.hpp"

class VPGPropertyAccessorGenerationService
{
    private:
        VPGPropertyAccessorGenerationService() = default;
        ~VPGPropertyAccessorGenerationService() {}

        static bool IsIncludeString(const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList);
        
        static std::vector<std::wstring> GetPropertyAccessorTypeOrder();

        static void GenerateRead(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassPropertiesReadOnly, std::wstring &result);
        static void GenerateWrite(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassPropertiesWriteOnly, std::wstring &result);
        static void GenerateInsert(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassPropertiesWriteOnly, std::wstring &result);
        static void GenerateClone(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassPropertiesReadOnly, std::wstring &result);
        static void GenerateContainerCount(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassProperties, std::wstring &result);
        static void GenerateContainerMapKey(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassProperties, std::wstring &result);
        static void GenerateContainerIsContainKey(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassPropertiesReadOnly, std::wstring &result);
        static void GenerateContainerRemove(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassPropertiesWriteOnly, std::wstring &result);

    public:
        static std::wstring GetIncludeFile(const std::map<std::wstring, std::wstring> &projectClassIncludeFiles, const std::wstring &className);

        static void GetPropertyAccessorTypeName(const std::wstring &originalType, std::wstring &convertedType, std::wstring &convertedName, std::wstring &returnResult);

        static void GenerateHpp(const vcc::LogConfig *logConfig, const std::wstring &projectPrefix,
            const std::wstring &filePathHpp, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList);
        static void GenerateCpp(const vcc::LogConfig *logConfig, const std::wstring &projectPrefix,
            const std::map<std::wstring, std::wstring> &projectClassIncludeFiles,
            const std::wstring &filePathCpp, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList);
};