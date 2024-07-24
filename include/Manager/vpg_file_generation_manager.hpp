#pragma once

#include <map>
#include <string>
#include <set>

#include "base_manager.hpp"
#include "class_macro.hpp"
#include "log_property.hpp"
#include "vpg_enum_class_reader.hpp"
#include "vpg_generation_option.hpp"

using namespace vcc;

// Property: Generate Object Type, Object Class, PropertyAccessor, Interface // TODO: Interface
// Type: Generate Interface // TODO: Interface

class VPGEnumClass;
class VPGFileGenerationManager : public BaseManager<VPGFileGenerationManager>
{
    GETSET(std::wstring, Workspace, L"");
    SET(std::wstring, ClassMacros);
    MAP(std::wstring, std::wstring, IncludeFiles);
    MAP_SPTR_R(std::wstring, VPGEnumClass, EnumClasses);

    private:
        std::wstring GetConcatPath(const std::wstring &projWorkspace, const std::wstring &objWorkspace, const std::wstring &middlePath, const std::wstring &fileName) const;

    public:
        VPGFileGenerationManager(std::shared_ptr<LogProperty> logProperty, std::wstring workspace) : BaseManager(logProperty) { _Workspace = workspace; }
        virtual ~VPGFileGenerationManager() {}

        // properties
        void GetClassMacroList(const std::wstring &projWorkspace);
        
        std::wstring GetClassNameFromEnumClassName(const std::wstring &enumClassName);
        std::wstring GetClassFilenameFromEnumClassFilename(const std::wstring &enumClassFileName);
        void GetFileList(const VPGEnumClassReader *reader, const std::wstring &directoryFullPath, const std::wstring &projectPrefix);
        
        // validation
        bool IsClassEnumFile(const std::wstring &filename, const std::wstring &projectPrefix);
        bool IsClassEnum(const std::wstring &enumClassName, const std::wstring &projectPrefix);
        
        void GernerateProperty(const LogProperty *logProperty, const VPGGenerationOption *option);
};