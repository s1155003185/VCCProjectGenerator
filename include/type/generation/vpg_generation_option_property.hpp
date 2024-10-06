#pragma once

// @@json
enum class VPGGenerationOptionExportProperty
{
    Interface, // GETSET(VPGGenerationOptionInterfaceType, Interface, VPGGenerationOptionInterfaceType::Java)
    Workspace, // GETSET(std::wstring, Workspace, L"")

    IsExportExternalLib, // GETSET(bool, IsExportExternalLib, true)
    ExportDirectoryDll, // GETSET(std::wstring, ExportDirectoryDll, L"")
    ExportDirectoryExe, // GETSET(std::wstring, ExportDirectoryExe, L"")

    // Bridge Location
    DllBridgeDirectory, // GETSET(std::wstring, DllBridgeDirectory, L"")

    // Export Location
    ObjectDirectory, // GETSET(std::wstring, ObjectDirectory, L"")
    TypeDirectory // GETSET(std::wstring, TypeDirectory, L"")
};

// @@json { "Key.NamingStyle" : "PascalCase" }
enum class VPGGenerationOptionProperty
{
    Version, // GETSET(std::wstring, Version, L"v0.0.1")
    ProjectType,  // GETSET(VPGProjectType, ProjectType, VPGProjectType::VccModule)
    TemplateGitUrl, // GETSET(std::wstring, TemplateGitUrl, L"")
    TemplateWorkspace, // GETSET(std::wstring, TemplateWorkspace, L"")

    // --------------------------------------------------
    // Config
    // --------------------------------------------------
    // Project
    ProjectPrefix, // GETSET(std::wstring, ProjectPrefix, L"")

    ProjectName, // GETSET(std::wstring, ProjectName, L"VCCModule") @@Command Need to assign Default Name first to pass validation
    ProjectNameDll, // GETSET(std::wstring, ProjectNameDll, L"libVCCModule") @@Command Need to assign Default Name first to pass validation
    ProjectNameExe, // GETSET(std::wstring, ProjectNameExe, L"VCCModule") @@Command Need to assign Default Name first to pass validation
    IsGit, // GETSET(bool, IsGit, false)

    IsExcludeUnittest, // GETSET(bool, IsExcludeUnittest, false)
    IsExcludeVCCUnitTest, // GETSET(bool, IsExcludeVCCUnitTest, false)

    // Files
    TypeWorkspace, // GETSET(std::wstring, TypeWorkspace, L"include/type")

    ActionTypeDirectory, // GETSET(std::wstring, ActionTypeDirectory, L"include/type")
    ExceptionTypeDirectory, // GETSET(std::wstring, ExceptionTypeDirectory, L"include/type")
    ManagerTypeDirectory, // GETSET(std::wstring, ManagerTypeDirectory, L"include/type")
    ObjectTypeDirectory, // GETSET(std::wstring, ObjectTypeDirectory, L"include/type")

    ObjectDirectoryHpp, // GETSET(std::wstring, ObjectDirectoryHpp, L"include/model")
    ObjectDirectoryCpp, // GETSET(std::wstring, ObjectDirectoryCpp, L"src/model")
    PropertyAccessorDirectoryHpp, // GETSET(std::wstring, PropertyAccessorDirectoryHpp, L"include/propertyAccessor")
    PropertyAccessorDirectoryCpp, // GETSET(std::wstring, PropertyAccessorDirectoryCpp, L"src/propertyAccessor")

    ObjectFactoryDirectoryHpp, // GETSET(std::wstring, ObjectFactoryDirectoryHpp, L"include/factory")
    ObjectFactoryDirectoryCpp, // GETSET(std::wstring, ObjectFactoryDirectoryCpp, L"src/factory")
    PropertyAccessorFactoryDirectoryHpp, // GETSET(std::wstring, PropertyAccessorFactoryDirectoryHpp, L"include/factory")
    PropertyAccessorFactoryDirectoryCpp, // GETSET(std::wstring, PropertyAccessorFactoryDirectoryCpp, L"src/factory")
    
    // Plugins
    Plugins, // VECTOR(std::wstring, Plugins)

    // Export
    Exports // VECTOR_SPTR(VPGGenerationOptionExport, Exports)
};