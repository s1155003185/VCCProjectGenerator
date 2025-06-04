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
    ActionHistoryType, // GETSET(VPGConfigActionHistoryType, ActionHistoryType, VPGConfigActionHistoryType::NoHistory)
    IsActionResultThrowException // GETSET(bool, IsActionResultThrowException, false)
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
    // Template
    Template, // GETSET_SPTR(VPGConfigTemplate, Template)

    // Template - Getter
    TemplateUrl, // GETCUSTOM(std::wstring, TemplateUrl, return this->GetTemplate() != nullptr ? this->GetTemplate()->GetUrl() : std::make_shared<VPGConfigTemplate>()->GetUrl();) @@NoJson
    TemplateWorkspace, // GETCUSTOM(std::wstring, TemplateWorkspace, return this->GetTemplate() != nullptr ? this->GetTemplate()->GetWorkspace() : std::make_shared<VPGConfigTemplate>()->GetWorkspace();) @@NoJson
    TemplateIsExcludeUnittest, // GETCUSTOM(bool, TemplateIsExcludeUnittest, return this->GetTemplate() != nullptr ? this->GetTemplate()->GetIsExcludeUnittest() : std::make_shared<VPGConfigTemplate>()->GetIsExcludeUnittest();) @@NoJson
    TemplateIsExcludeVCCUnitTest, // GETCUSTOM(bool, TemplateIsExcludeVCCUnitTest, return this->GetTemplate() != nullptr ? this->GetTemplate()->GetIsExcludeVCCUnitTest() : std::make_shared<VPGConfigTemplate>()->GetIsExcludeVCCUnitTest();) @@NoJson
    
    // Behavior
    Behavior, // GETSET_SPTR(VPGConfigBehavior, Behavior)

    // Behavior - Getter
    BehaviorActionHistoryType, // GETCUSTOM(VPGConfigActionHistoryType, BehaviorActionHistoryType, return this->GetBehavior() != nullptr ? this->GetBehavior()->GetActionHistoryType() : std::make_shared<VPGConfigBehavior>()->GetActionHistoryType();) @@NoJson
    BehaviorIsActionResultThrowException, // GETCUSTOM(bool, BehaviorIsActionResultThrowException, return this->GetBehavior() != nullptr ? this->GetBehavior()->GetIsActionResultThrowException() : std::make_shared<VPGConfigBehavior>()->GetIsActionResultThrowException();) @@NoJson

    // Input
    Input, // GETSET_SPTR(VPGConfigInput, Input)

    // Input - Getter
    InputTypeWorkspace, // GETCUSTOM(std::wstring, InputTypeWorkspace, return this->GetInput() != nullptr ? this->GetInput()->GetTypeWorkspace() : std::make_shared<VPGConfigInput>()->GetTypeWorkspace();) @@NoJson

    // Output
    Ouput, // GETSET_SPTR(VPGConfigOutput, Output)

    // Output - Getter
    OuputExceptionTypeDirectory, // GETCUSTOM(std::wstring, OuputExceptionTypeDirectory, return this->GetOutput() != nullptr ? this->GetOutput()->GetExceptionTypeDirectory() : std::make_shared<VPGConfigOutput>()->GetExceptionTypeDirectory();) @@NoJson
    OuputObjectTypeDirectory, // GETCUSTOM(std::wstring, OuputObjectTypeDirectory, return this->GetOutput() != nullptr ? this->GetOutput()->GetObjectTypeDirectory() : std::make_shared<VPGConfigOutput>()->GetObjectTypeDirectory();) @@NoJson
    OuputApplicationDirectoryHpp, // GETCUSTOM(std::wstring, OuputApplicationDirectoryHpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetApplicationDirectoryHpp() : std::make_shared<VPGConfigOutput>()->GetApplicationDirectoryHpp();) @@NoJson
    OuputApplicationDirectoryCpp, // GETCUSTOM(std::wstring, OuputApplicationDirectoryCpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetApplicationDirectoryCpp() : std::make_shared<VPGConfigOutput>()->GetApplicationDirectoryCpp();) @@NoJson
    OuputActionDirectoryHpp, // GETCUSTOM(std::wstring, OuputActionDirectoryHpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetActionDirectoryHpp() : std::make_shared<VPGConfigOutput>()->GetActionDirectoryHpp();) @@NoJson
    OuputActionDirectoryCpp, // GETCUSTOM(std::wstring, OuputActionDirectoryCpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetActionDirectoryCpp() : std::make_shared<VPGConfigOutput>()->GetActionDirectoryCpp();) @@NoJson
    OuputFormDirectoryHpp, // GETCUSTOM(std::wstring, OuputFormDirectoryHpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetFormDirectoryHpp() : std::make_shared<VPGConfigOutput>()->GetFormDirectoryHpp();) @@NoJson
    OuputFormDirectoryCpp, // GETCUSTOM(std::wstring, OuputFormDirectoryCpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetFormDirectoryCpp() : std::make_shared<VPGConfigOutput>()->GetFormDirectoryCpp();) @@NoJson
    OuputObjectDirectoryHpp, // GETCUSTOM(std::wstring, OuputObjectDirectoryHpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetObjectDirectoryHpp() : std::make_shared<VPGConfigOutput>()->GetObjectDirectoryHpp();) @@NoJson
    OuputObjectDirectoryCpp, // GETCUSTOM(std::wstring, OuputObjectDirectoryCpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetObjectDirectoryCpp() : std::make_shared<VPGConfigOutput>()->GetObjectDirectoryCpp();) @@NoJson
    OuputPropertyAccessorDirectoryHpp, // GETCUSTOM(std::wstring, OuputPropertyAccessorDirectoryHpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetPropertyAccessorDirectoryHpp() : std::make_shared<VPGConfigOutput>()->GetPropertyAccessorDirectoryHpp();) @@NoJson
    OuputPropertyAccessorDirectoryCpp, // GETCUSTOM(std::wstring, OuputPropertyAccessorDirectoryCpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetPropertyAccessorDirectoryCpp() : std::make_shared<VPGConfigOutput>()->GetPropertyAccessorDirectoryCpp();) @@NoJson
    OuputObjectFactoryDirectoryHpp, // GETCUSTOM(std::wstring, OuputObjectFactoryDirectoryHpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetObjectFactoryDirectoryHpp() : std::make_shared<VPGConfigOutput>()->GetObjectFactoryDirectoryHpp();) @@NoJson
    OuputObjectFactoryDirectoryCpp, // GETCUSTOM(std::wstring, OuputObjectFactoryDirectoryCpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetObjectFactoryDirectoryCpp() : std::make_shared<VPGConfigOutput>()->GetObjectFactoryDirectoryCpp();) @@NoJson
    OuputPropertyAccessorFactoryDirectoryHpp, // GETCUSTOM(std::wstring, OuputPropertyAccessorFactoryDirectoryHpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetPropertyAccessorFactoryDirectoryHpp() : std::make_shared<VPGConfigOutput>()->GetPropertyAccessorFactoryDirectoryHpp();) @@NoJson
    OuputPropertyAccessorFactoryDirectoryCpp, // GETCUSTOM(std::wstring, OuputPropertyAccessorFactoryDirectoryCpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetPropertyAccessorFactoryDirectoryCpp() : std::make_shared<VPGConfigOutput>()->GetPropertyAccessorFactoryDirectoryCpp();) @@NoJson

    Plugins, // VECTOR(std::wstring, Plugins)

    Exports // VECTOR_SPTR(VPGConfigExport, Exports)
};