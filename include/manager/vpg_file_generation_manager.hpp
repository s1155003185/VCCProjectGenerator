#pragma once

#include <map>
#include <string>
#include <set>

#include "base_manager.hpp"
#include "class_macro.hpp"
#include "log_config.hpp"
#include "vpg_enum_class_reader.hpp"
#include "vpg_config.hpp"

// Property: Generate Object Type, Object Class, PropertyAccessor, Interface // TODO: Interface
// Type: Generate Interface // TODO: Interface

class VPGEnumClass;
class VPGFileGenerationManager : public vcc::BaseManager
{
    GETSET(std::wstring, Workspace, L"");
    SET(std::wstring, ClassMacros);
    MAP(std::wstring, std::wstring, IncludeFiles);
    MAP_SPTR_R(std::wstring, VPGEnumClass, EnumClasses);

    private:
        std::wstring GetConcatPath(const std::wstring &projWorkspace, const std::wstring &objWorkspace, const std::wstring &middlePath, const std::wstring &fileName) const;

    public:
        VPGFileGenerationManager(std::shared_ptr<vcc::LogConfig> logConfig, const std::wstring &workspace) : BaseManager(logConfig) { _Workspace = workspace; }
        virtual ~VPGFileGenerationManager() {}

        // properties
        void GetClassMacroList(const std::wstring &projWorkspace);
        
        std::wstring GetClassFilenameFromEnumClassFilename(const std::wstring &enumClassFileName);
        void GetFileList(const VPGEnumClassReader *reader, const std::wstring &directoryFullPath, const std::wstring &projectPrefix, const bool &isSeperateAction);
        
        void GernerateProperty(const vcc::LogConfig *logConfig, const VPGConfig *option);
};