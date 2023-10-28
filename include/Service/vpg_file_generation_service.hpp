#pragma once

#include <string>
#include <set>

#include "log_property.hpp"

using namespace vcc;

// Property: Generate Object Type, Object Class, PropertyAccessor, Interface // TODO: Interface
// Type: Generate Interface // TODO: Interface

class VPGEnumClass;
class VPGFileGenerationService
{   
    public:
        VPGFileGenerationService() = default;
        ~VPGFileGenerationService() {}

        // properties
        static std::set<std::wstring> GetClassMacroList(const std::wstring &projWorkspace);

        static void GenerateObjectTypeFile(LogProperty &logProperty, const std::wstring &classPrefix, const std::wstring &hppFilePath, const std::vector<std::wstring> &propertyTypeList);
        
        static void GeneratePropertyClassFile(LogProperty &logProperty, const std::wstring &classPrefix, const std::wstring &hppFilePath, const std::vector<VPGEnumClass> &enumClassList);
        static void GeneratePropertyPropertyAccessorFile(LogProperty &logProperty, const std::wstring &classPrefix, const std::wstring &hppFilePath, const std::wstring &cppFilePath, const std::vector<VPGEnumClass> &enumClassList);
        static void GernerateProperty(LogProperty &logProperty, const std::wstring &projPrefix, const std::wstring &projWorkspace, const std::wstring &typeWorkspace, 
            const std::wstring &objTypeDirectory, const std::wstring &objDirectory, const std::wstring &propertyAccessorDirectory);
        // TODO
        // void GenerateType(const std::wstring &srcWorkspace, const &destWorkspace);
};