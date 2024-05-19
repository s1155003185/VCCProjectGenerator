#pragma once

#include <memory>
#include <string>
#include <map>
#include <vector>

#include "log_property.hpp"
#include "vpg_enum_class_reader.hpp"

using namespace vcc;

class VPGObjectFileGenerationService
{
    private:
        VPGObjectFileGenerationService() = default;
        ~VPGObjectFileGenerationService() {}

    public:
        // get #include file name
        static std::wstring GetProjectClassIncludeFile(const std::map<std::wstring, std::wstring> &projectClassIncludeFiles, const std::map<std::wstring, std::wstring> &classFilesByEnumClass, const std::wstring &className);
        static std::wstring GetProjectEnumClassIncludeFile(const std::map<std::wstring, std::wstring> &projectClassIncludeFiles, const std::map<std::wstring, std::wstring> &enumClassFilesByEnumClass, const std::wstring &className);

        static void Generate(const LogProperty *logProperty, const std::wstring &classPrefix,
            const std::map<std::wstring, std::wstring> &projectClassIncludeFiles, const std::map<std::wstring, std::wstring> &classFilesByEnumClass, const std::map<std::wstring, std::wstring> &enumClassFilesByEnumClass,
            const std::wstring &hppFilePath, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList);
};