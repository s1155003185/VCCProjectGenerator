#pragma once

#include <map>
#include <string>
#include <set>

#include "base_manager.hpp"
#include "class_macro.hpp"
#include "log_property.hpp"
#include "vpg_enum_class_reader.hpp"

using namespace vcc;

// Property: Generate Object Type, Object Class, PropertyAccessor, Interface // TODO: Interface
// Type: Generate Interface // TODO: Interface

class VPGEnumClass;
class VPGFileGenerationManager : public BaseManager<VPGFileGenerationManager>
{
    SET(std::wstring, ClassMacros);
    MAP(std::wstring, std::wstring, IncludeFiles);

    private:
        std::wstring GetConcatPath(const std::wstring &projWorkspace, const std::wstring &objWorkspace, const std::wstring &middlePath, const std::wstring &fileName) const;

    public:
        VPGFileGenerationManager(std::shared_ptr<LogProperty> logProperty) : BaseManager(logProperty) {}
        virtual ~VPGFileGenerationManager() {}

        // properties
        void GetClassMacroList(const std::wstring &projWorkspace);
        
        std::wstring GetClassNameFromEnumClassName(const std::wstring &enumClassName);
        std::wstring GetClassFilenameFromEnumClassFilename(const std::wstring &enumClassFileName);
        void GetFileList(const VPGEnumClassReader *reader, const std::wstring &directoryFullPath, const std::wstring &projectPrefix);
        
        // validation
        bool IsClassEnumFile(const std::wstring &filename, const std::wstring &projectPrefix);
        bool IsClassEnum(const std::wstring &enumClassName, const std::wstring &projectPrefix);
        
        void GernerateProperty(const LogProperty *logProperty, const std::wstring &projPrefix, const std::wstring &projWorkspace, const std::wstring &typeWorkspace, 
            const std::wstring &objTypeDirectoryHpp, const std::wstring &objDirectoryHpp, const std::wstring &propertyAccessorDirectoryHpp, const std::wstring &propertyAccessorDirectoryCpp);
};