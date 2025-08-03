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
    TemplateUrl, // GETCUSTOM(std::wstring, TemplateUrl, return this->getTemplate() != nullptr ? this->getTemplate()->getUrl() : std::make_shared<VPGConfigTemplate>()->getUrl();) @@NoJson
    TemplateWorkspace, // GETCUSTOM(std::wstring, TemplateWorkspace, return this->getTemplate() != nullptr ? this->getTemplate()->getWorkspace() : std::make_shared<VPGConfigTemplate>()->getWorkspace();) @@NoJson
    TemplateIsExcludeUnittest, // GETCUSTOM(bool, TemplateIsExcludeUnittest, return this->getTemplate() != nullptr ? this->getTemplate()->getIsExcludeUnittest() : std::make_shared<VPGConfigTemplate>()->getIsExcludeUnittest();) @@NoJson
    TemplateIsExcludeVCCUnitTest, // GETCUSTOM(bool, TemplateIsExcludeVCCUnitTest, return this->getTemplate() != nullptr ? this->getTemplate()->getIsExcludeVCCUnitTest() : std::make_shared<VPGConfigTemplate>()->getIsExcludeVCCUnitTest();) @@NoJson
    
    // Behavior
    Behavior, // GETSET_SPTR(VPGConfigBehavior, Behavior)

    // Behavior - Getter
    BehaviorActionHistoryType, // GETCUSTOM(VPGConfigActionHistoryType, BehaviorActionHistoryType, return this->getBehavior() != nullptr ? this->getBehavior()->getActionHistoryType() : std::make_shared<VPGConfigBehavior>()->getActionHistoryType();) @@NoJson
    BehaviorIsActionResultThrowException, // GETCUSTOM(bool, BehaviorIsActionResultThrowException, return this->getBehavior() != nullptr ? this->getBehavior()->getIsActionResultThrowException() : std::make_shared<VPGConfigBehavior>()->getIsActionResultThrowException();) @@NoJson

    // Input
    Input, // GETSET_SPTR(VPGConfigInput, Input)

    // Input - Getter
    InputTypeWorkspace, // GETCUSTOM(std::wstring, InputTypeWorkspace, return this->getInput() != nullptr ? this->getInput()->getTypeWorkspace() : std::make_shared<VPGConfigInput>()->getTypeWorkspace();) @@NoJson

    // Output
    Output, // GETSET_SPTR(VPGConfigOutput, Output)

    // Output - Getter
    OutputExceptionTypeDirectory, // GETCUSTOM(std::wstring, OutputExceptionTypeDirectory, return this->getOutput() != nullptr ? this->getOutput()->getExceptionTypeDirectory() : std::make_shared<VPGConfigOutput>()->getExceptionTypeDirectory();) @@NoJson
    OutputObjectTypeDirectory, // GETCUSTOM(std::wstring, OutputObjectTypeDirectory, return this->getOutput() != nullptr ? this->getOutput()->getObjectTypeDirectory() : std::make_shared<VPGConfigOutput>()->getObjectTypeDirectory();) @@NoJson
    OutputApplicationDirectoryHpp, // GETCUSTOM(std::wstring, OutputApplicationDirectoryHpp, return this->getOutput() != nullptr ? this->getOutput()->getApplicationDirectoryHpp() : std::make_shared<VPGConfigOutput>()->getApplicationDirectoryHpp();) @@NoJson
    OutputApplicationDirectoryCpp, // GETCUSTOM(std::wstring, OutputApplicationDirectoryCpp, return this->getOutput() != nullptr ? this->getOutput()->getApplicationDirectoryCpp() : std::make_shared<VPGConfigOutput>()->getApplicationDirectoryCpp();) @@NoJson
    OutputActionDirectoryHpp, // GETCUSTOM(std::wstring, OutputActionDirectoryHpp, return this->getOutput() != nullptr ? this->getOutput()->getActionDirectoryHpp() : std::make_shared<VPGConfigOutput>()->getActionDirectoryHpp();) @@NoJson
    OutputActionDirectoryCpp, // GETCUSTOM(std::wstring, OutputActionDirectoryCpp, return this->getOutput() != nullptr ? this->getOutput()->getActionDirectoryCpp() : std::make_shared<VPGConfigOutput>()->getActionDirectoryCpp();) @@NoJson
    OutputFormDirectoryHpp, // GETCUSTOM(std::wstring, OutputFormDirectoryHpp, return this->getOutput() != nullptr ? this->getOutput()->getFormDirectoryHpp() : std::make_shared<VPGConfigOutput>()->getFormDirectoryHpp();) @@NoJson
    OutputFormDirectoryCpp, // GETCUSTOM(std::wstring, OutputFormDirectoryCpp, return this->getOutput() != nullptr ? this->getOutput()->getFormDirectoryCpp() : std::make_shared<VPGConfigOutput>()->getFormDirectoryCpp();) @@NoJson
    OutputObjectDirectoryHpp, // GETCUSTOM(std::wstring, OutputObjectDirectoryHpp, return this->getOutput() != nullptr ? this->getOutput()->getObjectDirectoryHpp() : std::make_shared<VPGConfigOutput>()->getObjectDirectoryHpp();) @@NoJson
    OutputObjectDirectoryCpp, // GETCUSTOM(std::wstring, OutputObjectDirectoryCpp, return this->getOutput() != nullptr ? this->getOutput()->getObjectDirectoryCpp() : std::make_shared<VPGConfigOutput>()->getObjectDirectoryCpp();) @@NoJson
    OutputPropertyAccessorDirectoryHpp, // GETCUSTOM(std::wstring, OutputPropertyAccessorDirectoryHpp, return this->getOutput() != nullptr ? this->getOutput()->getPropertyAccessorDirectoryHpp() : std::make_shared<VPGConfigOutput>()->getPropertyAccessorDirectoryHpp();) @@NoJson
    OutputPropertyAccessorDirectoryCpp, // GETCUSTOM(std::wstring, OutputPropertyAccessorDirectoryCpp, return this->getOutput() != nullptr ? this->getOutput()->getPropertyAccessorDirectoryCpp() : std::make_shared<VPGConfigOutput>()->getPropertyAccessorDirectoryCpp();) @@NoJson
    OutputObjectFactoryDirectoryHpp, // GETCUSTOM(std::wstring, OutputObjectFactoryDirectoryHpp, return this->getOutput() != nullptr ? this->getOutput()->getObjectFactoryDirectoryHpp() : std::make_shared<VPGConfigOutput>()->getObjectFactoryDirectoryHpp();) @@NoJson
    OutputObjectFactoryDirectoryCpp, // GETCUSTOM(std::wstring, OutputObjectFactoryDirectoryCpp, return this->getOutput() != nullptr ? this->getOutput()->getObjectFactoryDirectoryCpp() : std::make_shared<VPGConfigOutput>()->getObjectFactoryDirectoryCpp();) @@NoJson
    OutputPropertyAccessorFactoryDirectoryHpp, // GETCUSTOM(std::wstring, OutputPropertyAccessorFactoryDirectoryHpp, return this->getOutput() != nullptr ? this->getOutput()->getPropertyAccessorFactoryDirectoryHpp() : std::make_shared<VPGConfigOutput>()->getPropertyAccessorFactoryDirectoryHpp();) @@NoJson
    OutputPropertyAccessorFactoryDirectoryCpp, // GETCUSTOM(std::wstring, OutputPropertyAccessorFactoryDirectoryCpp, return this->getOutput() != nullptr ? this->getOutput()->getPropertyAccessorFactoryDirectoryCpp() : std::make_shared<VPGConfigOutput>()->getPropertyAccessorFactoryDirectoryCpp();) @@NoJson

    Plugins, // VECTOR(std::wstring, Plugins)

    Exports // VECTOR_SPTR(VPGConfigExport, Exports)
};