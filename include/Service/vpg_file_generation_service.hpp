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

        static void GenerateObjectTypeFile(const LogProperty &logProperty, const std::wstring &classPrefix, const std::wstring &hppFilePath, const std::vector<std::wstring> &propertyTypeList);
        
        static void GeneratePropertyClassFile(const LogProperty &logProperty, const std::wstring &classPrefix, const std::wstring &hppFilePath, const std::vector<VPGEnumClass> &enumClassList);
        static void GeneratePropertyPropertyAccessorFile(const LogProperty &logProperty, const std::wstring &classPrefix, const std::wstring &hppFilePath, const std::wstring &cppFilePath, const std::vector<VPGEnumClass> &enumClassList);
        static void GernerateProperty(const LogProperty &logProperty, const std::wstring &projPrefix, const std::wstring &projWorkspace, const std::wstring &typeWorkspace, 
            const std::wstring &objTypeDirectoryHpp, const std::wstring &objDirectoryHpp, const std::wstring &propertyAccessorDirectoryHpp, const std::wstring &propertyAccessorDirectoryCpp);
        // TODO
        // void GenerateType(const std::wstring &srcWorkspace, const &destWorkspace);
};