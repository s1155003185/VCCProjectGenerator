#pragma once

// @@json
enum class VPGGenerationOptionExportProperty
{
    InterfaceType // GETSET(VPGGenerationOptionInterfaceType, Interface, VPGGenerationOptionInterfaceType::Java)
};

// @@json { "Key.NamingStyle" : "PascalCase" }
enum class VPGGenerationOptionProperty
{
    Version, // GETSET(std::wstring, Version, L"v0.0.1")
    ProjectType,  // GETSET(VPGProjectType, ProjectType, VPGProjectType::VccModule)
    WorkspaceSourceGitUrl, // GETSET(std::wstring, WorkspaceSourceGitUrl, L"")
    WorkspaceSource, // GETSET(std::wstring, WorkspaceSource, L"")

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
    TypeWorkspace, // GETSET(std::wstring, TypeWorkspace, L"include/Type")

    ActionTypeDirectory, // GETSET(std::wstring, ActionTypeDirectory, L"include/Type")
    ExceptionTypeDirectory, // GETSET(std::wstring, ExceptionTypeDirectory, L"include/Type")
    ManagerTypeDirectory, // GETSET(std::wstring, ManagerTypeDirectory, L"include/Type")
    ObjectTypeDirectory, // GETSET(std::wstring, ObjectTypeDirectory, L"include/Type")

    ObjectDirectoryHpp, // GETSET(std::wstring, ObjectDirectoryHpp, L"include/Model")
    ObjectDirectoryCpp, // GETSET(std::wstring, ObjectDirectoryCpp, L"src/Model")
    PropertyAccessorDirectoryHpp, // GETSET(std::wstring, PropertyAccessorDirectoryHpp, L"include/PropertyAccessor")
    PropertyAccessorDirectoryCpp, // GETSET(std::wstring, PropertyAccessorDirectoryCpp, L"src/PropertyAccessor")

    ObjectFactoryDirectoryHpp, // GETSET(std::wstring, ObjectFactoryDirectoryHpp, L"include/Factory")
    ObjectFactoryDirectoryCpp, // GETSET(std::wstring, ObjectFactoryDirectoryCpp, L"src/Factory")
    PropertyAccessorFactoryDirectoryHpp, // GETSET(std::wstring, PropertyAccessorFactoryDirectoryHpp, L"include/Factory")
    PropertyAccessorFactoryDirectoryCpp, // GETSET(std::wstring, PropertyAccessorFactoryDirectoryCpp, L"src/Factory")
    
    // Plugins
    Plugins, // VECTOR(std::wstring, Plugins)

    // Export
    Exports // VECTOR_SPTR(VPGGenerationOptionExport, Exports)
};