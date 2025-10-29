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

        static bool isIncludeString(const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList);
        
        static std::vector<std::wstring> getPropertyAccessorTypeOrder();

        static void generateRead(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassPropertiesReadOnly, std::wstring &result);
        static void generateWrite(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassPropertiesWriteOnly, std::wstring &result);
        static void generateInsert(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassPropertiesWriteOnly, std::wstring &result);
        static void generateClone(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassPropertiesReadOnly, std::wstring &result);
        static void generateContainerCount(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassProperties, std::wstring &result);
        static void generateContainerMapKey(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassProperties, std::wstring &result);
        static void generateContainerIsContainKey(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassPropertiesReadOnly, std::wstring &result);
        static void generateContainerRemove(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassAttribute>> &enumClassPropertiesWriteOnly, std::wstring &result);

    public:
        static std::wstring getIncludeFile(const std::map<std::wstring, std::wstring> &projectClassIncludeFiles, const std::wstring &className);

        static void getPropertyAccessorTypeName(const std::wstring &originalType, std::wstring &convertedType, std::wstring &convertedName, std::wstring &returnResult);

        static void generateHpp(const vcc::LogConfig *logConfig, const std::wstring &projectPrefix,
            const std::wstring &filePathHpp, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList);
        static void generateCpp(const vcc::LogConfig *logConfig, const std::wstring &projectPrefix,
            const std::map<std::wstring, std::wstring> &projectClassIncludeFiles,
            const std::wstring &filePathCpp, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList);
};