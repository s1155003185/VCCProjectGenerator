#pragma once

#include <memory>
#include <string>
#include <map>
#include <vector>

#include "log_property.hpp"
#include "vpg_enum_class.hpp"

using namespace vcc;

class VPGObjectFileGenerationService
{
    private:
        VPGObjectFileGenerationService() = default;
        ~VPGObjectFileGenerationService() {}

        // Json
        static bool IsJsonObject(const VPGEnumClass* enumClass);
        static std::wstring GetJsonAddType(const std::wstring &type);
    public:
        // get #include file name
        static std::wstring GetProjectClassIncludeFile(const std::map<std::wstring, std::wstring> &projectClassIncludeFiles, const std::wstring &className);

        static void GenerateHpp(const LogProperty *logProperty, const std::wstring &classPrefix, const std::map<std::wstring, std::wstring> &projectClassIncludeFiles,
            const std::wstring &filePathHpp, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList);
        static void GenerateCpp(const LogProperty *logProperty, const std::wstring &classPrefix, const std::map<std::wstring, std::wstring> &projectClassIncludeFiles,
            const std::wstring &filePathCpp, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList);
};