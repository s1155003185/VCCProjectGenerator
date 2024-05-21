#pragma once

#include <set>
#include <string>
#include <vector>

#include "log_property.hpp"
#include "vpg_enum_class_reader.hpp"

using namespace vcc;

class VPGPropertyAccessorGenerationSerive
{
    private:
        VPGPropertyAccessorGenerationSerive() = default;
        ~VPGPropertyAccessorGenerationSerive() {}

        static bool IsIncludeString(const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList);
        static void GetPropertyAccessorTypeName(const std::wstring &originalType, std::wstring &convertedType, std::wstring &convertedName, std::wstring &returnResult);

        static std::vector<std::wstring> GetPropertyAccessorTypeOrder();

        static void GetIsHavingGenerateTypeMapType(const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassProperties, bool &isHavingGeneralType, bool &isHavingVectorType, bool &isHavingMapType);
        static void GenerateRead(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassProperties, std::wstring &result);
        static void GenerateWrite(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassProperties, std::wstring &result);
        static void GenerateClone(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassProperties, std::wstring &result);
        static void GenerateContainer(const std::wstring &propertyName, const std::wstring &type, const std::vector<std::shared_ptr<VPGEnumClassProperty>> &enumClassProperties, std::wstring &result);

    public:
        static std::wstring GetIncludeFile(const std::map<std::wstring, std::wstring> &projectClassIncludeFiles, const std::wstring &className);

        static void GenerateHpp(const LogProperty *logProperty, const std::wstring &hppFilePath, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList);
        static void GenerateCpp(const LogProperty *logProperty, const std::map<std::wstring, std::wstring> &projectClassIncludeFiles,
            const std::wstring &cppFilePath, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList);
};