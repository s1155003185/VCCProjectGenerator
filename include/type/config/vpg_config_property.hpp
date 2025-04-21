#pragma once

// @@json
enum class VPGConfigTemplateProperty
{
    Url, // GETSET(std::wstring, Url, L"")
    Workspace, // GETSET(std::wstring, Workspace, L"")
    
    // Unittest
    IsExcludeUnittest, // GETSET(bool, IsExcludeUnittest, false)
    IsExcludeVCCUnitTest // GETSET(bool, IsExcludeVCCUnitTest, false)
};

// @@json
enum class VPGConfigBehaviorProperty
{
    IsResultThrowException // GETSET(bool, IsResultThrowException, false)
};

// @@json
enum class VPGConfigInputProperty
{
    TypeWorkspace // GETSET(std::wstring, TypeWorkspace, L"include/type")
};

// @@json
enum class VPGConfigOutputProperty
{
    ExceptionTypeDirectory, // GETSET(std::wstring, ExceptionTypeDirectory, L"include/type")
    ObjectTypeDirectory, // GETSET(std::wstring, ObjectTypeDirectory, L"include/type")

    ApplicationDirectoryHpp, // GETSET(std::wstring, ApplicationDirectoryHpp, L"include")
    ApplicationDirectoryCpp, // GETSET(std::wstring, ApplicationDirectoryCpp, L"src")
    ActionDirectoryHpp, // GETSET(std::wstring, ActionDirectoryHpp, L"")
    ActionDirectoryCpp, // GETSET(std::wstring, ActionDirectoryCpp, L"")
    FormDirectoryHpp, // GETSET(std::wstring, FormDirectoryHpp, L"include/form")
    FormDirectoryCpp, // GETSET(std::wstring, FormDirectoryCpp, L"src/form")
    ObjectDirectoryHpp, // GETSET(std::wstring, ObjectDirectoryHpp, L"include/model")
    ObjectDirectoryCpp, // GETSET(std::wstring, ObjectDirectoryCpp, L"src/model")
    PropertyAccessorDirectoryHpp, // GETSET(std::wstring, PropertyAccessorDirectoryHpp, L"include/propertyaccessor")
    PropertyAccessorDirectoryCpp, // GETSET(std::wstring, PropertyAccessorDirectoryCpp, L"src/propertyaccessor")

    ObjectFactoryDirectoryHpp, // GETSET(std::wstring, ObjectFactoryDirectoryHpp, L"include/factory")
    ObjectFactoryDirectoryCpp, // GETSET(std::wstring, ObjectFactoryDirectoryCpp, L"src/factory")
    PropertyAccessorFactoryDirectoryHpp, // GETSET(std::wstring, PropertyAccessorFactoryDirectoryHpp, L"include/factory")
    PropertyAccessorFactoryDirectoryCpp // GETSET(std::wstring, PropertyAccessorFactoryDirectoryCpp, L"src/factory")
};

// @@json
enum class VPGConfigExportProperty
{
    Interface, // GETSET(VPGConfigInterfaceType, Interface, VPGConfigInterfaceType::Java)
    Workspace, // GETSET(std::wstring, Workspace, L"../JavaProject")

    IsExportExternalLib, // GETSET(bool, IsExportExternalLib, true)
    ExportDirectoryDll, // GETSET(std::wstring, ExportDirectoryDll, L"src/main/resources")
    ExportDirectoryExe, // GETSET(std::wstring, ExportDirectoryExe, L"")

    // Bridge Location
    DllBridgeDirectory, // GETSET(std::wstring, DllBridgeDirectory, L"src/main/java/com/package")

    // Export Location
    FormDirectory, // GETSET(std::wstring, FormDirectory, L"src/main/java/com/package/form")
    ObjectDirectory, // GETSET(std::wstring, ObjectDirectory, L"src/main/java/com/package/model")
    TypeDirectory // GETSET(std::wstring, TypeDirectory, L"src/main/java/com/package/type")
};

// @@json { "Key.NamingStyle" : "PascalCase" }
enum class VPGConfigProperty
{
    Version, // GETSET(std::wstring, Version, L"v0.0.1")
    ProjectType,  // GETSET(VPGProjectType, ProjectType, VPGProjectType::VccModule)
    
    ProjectPrefix, // GETSET(std::wstring, ProjectPrefix, L"")
    ProjectName, // GETSET(std::wstring, ProjectName, L"VCCModule") @@Command Need to assign Default Name first to pass validation
    ProjectNameDll, // GETSET(std::wstring, ProjectNameDll, L"libVCCModule") @@Command Need to assign Default Name first to pass validation
    ProjectNameExe, // GETSET(std::wstring, ProjectNameExe, L"VCCModule") @@Command Need to assign Default Name first to pass validation
    
    IsGit, // GETSET(bool, IsGit, false)

    // --------------------------------------------------
    // Config
    // --------------------------------------------------
    Template, // GETSET_SPTR_NULL(VPGConfigTemplate, Template)
    
    Behavior, // GETSET_SPTR_NULL(VPGConfigBehavior, Behavior)

    Input, // GETSET_SPTR_NULL(VPGConfigInput, Input)

    Ouput, // GETSET_SPTR_NULL(VPGConfigOutput, Output)

    Plugins, // VECTOR(std::wstring, Plugins)

    Exports // VECTOR_SPTR(VPGConfigExport, Exports)
};