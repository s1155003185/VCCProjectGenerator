#pragma once

#include <memory>
#include <string>

#include "base_json_object.hpp"
#include "base_object.hpp"
#include "class_macro.hpp"
#include "i_document.hpp"
#include "json.hpp"
#include "object_type.hpp"
#include "vpg_config_type.hpp"
#include "vpg_project_type.hpp"

class VPGConfigTemplate : public vcc::BaseObject, public vcc::BaseJsonObject
{
    GETSET(std::wstring, Url, L"")
    GETSET(std::wstring, Workspace, L"")
    GETSET(bool, IsExcludeUnittest, false)
    GETSET(bool, IsExcludeVCCUnitTest, false)

    public:
        VPGConfigTemplate() : vcc::BaseObject(ObjectType::ConfigTemplate) {}
        virtual ~VPGConfigTemplate() {}

        virtual std::shared_ptr<vcc::IObject> Clone() const override
        {
            return std::make_shared<VPGConfigTemplate>(*this);
        }

        virtual std::shared_ptr<vcc::Json> ToJson() const override;
        virtual void DeserializeJson(std::shared_ptr<vcc::IDocument> document) override;
};

class VPGConfigBehavior : public vcc::BaseObject, public vcc::BaseJsonObject
{
    GETSET(VPGConfigActionHistoryType, ActionHistoryType, VPGConfigActionHistoryType::NoHistory)
    GETSET(bool, IsActionResultThrowException, false)

    public:
        VPGConfigBehavior() : vcc::BaseObject(ObjectType::ConfigBehavior) {}
        virtual ~VPGConfigBehavior() {}

        virtual std::shared_ptr<vcc::IObject> Clone() const override
        {
            return std::make_shared<VPGConfigBehavior>(*this);
        }

        virtual std::shared_ptr<vcc::Json> ToJson() const override;
        virtual void DeserializeJson(std::shared_ptr<vcc::IDocument> document) override;
};

class VPGConfigInput : public vcc::BaseObject, public vcc::BaseJsonObject
{
    GETSET(std::wstring, TypeWorkspace, L"include/type")

    public:
        VPGConfigInput() : vcc::BaseObject(ObjectType::ConfigInput) {}
        virtual ~VPGConfigInput() {}

        virtual std::shared_ptr<vcc::IObject> Clone() const override
        {
            return std::make_shared<VPGConfigInput>(*this);
        }

        virtual std::shared_ptr<vcc::Json> ToJson() const override;
        virtual void DeserializeJson(std::shared_ptr<vcc::IDocument> document) override;
};

class VPGConfigOutput : public vcc::BaseObject, public vcc::BaseJsonObject
{
    GETSET(std::wstring, ExceptionTypeDirectory, L"include/type")
    GETSET(std::wstring, ObjectTypeDirectory, L"include/type")
    GETSET(std::wstring, ApplicationDirectoryHpp, L"include")
    GETSET(std::wstring, ApplicationDirectoryCpp, L"src")
    GETSET(std::wstring, ActionDirectoryHpp, L"")
    GETSET(std::wstring, ActionDirectoryCpp, L"")
    GETSET(std::wstring, FormDirectoryHpp, L"include/form")
    GETSET(std::wstring, FormDirectoryCpp, L"src/form")
    GETSET(std::wstring, ObjectDirectoryHpp, L"include/model")
    GETSET(std::wstring, ObjectDirectoryCpp, L"src/model")
    GETSET(std::wstring, PropertyAccessorDirectoryHpp, L"include/propertyaccessor")
    GETSET(std::wstring, PropertyAccessorDirectoryCpp, L"src/propertyaccessor")
    GETSET(std::wstring, ObjectFactoryDirectoryHpp, L"include/factory")
    GETSET(std::wstring, ObjectFactoryDirectoryCpp, L"src/factory")
    GETSET(std::wstring, PropertyAccessorFactoryDirectoryHpp, L"include/factory")
    GETSET(std::wstring, PropertyAccessorFactoryDirectoryCpp, L"src/factory")

    public:
        VPGConfigOutput() : vcc::BaseObject(ObjectType::ConfigOutput) {}
        virtual ~VPGConfigOutput() {}

        virtual std::shared_ptr<vcc::IObject> Clone() const override
        {
            return std::make_shared<VPGConfigOutput>(*this);
        }

        virtual std::shared_ptr<vcc::Json> ToJson() const override;
        virtual void DeserializeJson(std::shared_ptr<vcc::IDocument> document) override;
};

class VPGConfigExport : public vcc::BaseObject, public vcc::BaseJsonObject
{
    GETSET(VPGConfigInterfaceType, Interface, VPGConfigInterfaceType::Java)
    GETSET(std::wstring, Workspace, L"../JavaProject")
    GETSET(bool, IsExportExternalLib, true)
    GETSET(std::wstring, ExportDirectoryDll, L"src/main/resources")
    GETSET(std::wstring, ExportDirectoryExe, L"")
    GETSET(std::wstring, DllBridgeDirectory, L"src/main/java/com/package")
    GETSET(std::wstring, FormDirectory, L"src/main/java/com/package/form")
    GETSET(std::wstring, ObjectDirectory, L"src/main/java/com/package/model")
    GETSET(std::wstring, TypeDirectory, L"src/main/java/com/package/type")

    public:
        VPGConfigExport() : vcc::BaseObject(ObjectType::ConfigExport) {}
        virtual ~VPGConfigExport() {}

        virtual std::shared_ptr<vcc::IObject> Clone() const override
        {
            return std::make_shared<VPGConfigExport>(*this);
        }

        virtual std::shared_ptr<vcc::Json> ToJson() const override;
        virtual void DeserializeJson(std::shared_ptr<vcc::IDocument> document) override;
};

class VPGConfig : public vcc::BaseObject, public vcc::BaseJsonObject
{
    GETSET(std::wstring, Version, L"v0.0.1")
    GETSET(VPGProjectType, ProjectType, VPGProjectType::VccModule)
    GETSET(std::wstring, ProjectPrefix, L"")
    GETSET(std::wstring, ProjectName, L"VCCModule")
    GETSET(std::wstring, ProjectNameDll, L"libVCCModule")
    GETSET(std::wstring, ProjectNameExe, L"VCCModule")
    GETSET(bool, IsGit, false)
    GETSET_SPTR(VPGConfigTemplate, Template)
    GETCUSTOM(std::wstring, TemplateUrl, return this->GetTemplate() != nullptr ? this->GetTemplate()->GetUrl() : std::make_shared<VPGConfigTemplate>()->GetUrl();)
    GETCUSTOM(std::wstring, TemplateWorkspace, return this->GetTemplate() != nullptr ? this->GetTemplate()->GetWorkspace() : std::make_shared<VPGConfigTemplate>()->GetWorkspace();)
    GETCUSTOM(bool, TemplateIsExcludeUnittest, return this->GetTemplate() != nullptr ? this->GetTemplate()->GetIsExcludeUnittest() : std::make_shared<VPGConfigTemplate>()->GetIsExcludeUnittest();)
    GETCUSTOM(bool, TemplateIsExcludeVCCUnitTest, return this->GetTemplate() != nullptr ? this->GetTemplate()->GetIsExcludeVCCUnitTest() : std::make_shared<VPGConfigTemplate>()->GetIsExcludeVCCUnitTest();)
    GETSET_SPTR(VPGConfigBehavior, Behavior)
    GETCUSTOM(VPGConfigActionHistoryType, BehaviorActionHistoryType, return this->GetBehavior() != nullptr ? this->GetBehavior()->GetActionHistoryType() : std::make_shared<VPGConfigBehavior>()->GetActionHistoryType();)
    GETCUSTOM(bool, BehaviorIsActionResultThrowException, return this->GetBehavior() != nullptr ? this->GetBehavior()->GetIsActionResultThrowException() : std::make_shared<VPGConfigBehavior>()->GetIsActionResultThrowException();)
    GETSET_SPTR(VPGConfigInput, Input)
    GETCUSTOM(std::wstring, InputTypeWorkspace, return this->GetInput() != nullptr ? this->GetInput()->GetTypeWorkspace() : std::make_shared<VPGConfigInput>()->GetTypeWorkspace();)
    GETSET_SPTR(VPGConfigOutput, Output)
    GETCUSTOM(std::wstring, OutputExceptionTypeDirectory, return this->GetOutput() != nullptr ? this->GetOutput()->GetExceptionTypeDirectory() : std::make_shared<VPGConfigOutput>()->GetExceptionTypeDirectory();)
    GETCUSTOM(std::wstring, OutputObjectTypeDirectory, return this->GetOutput() != nullptr ? this->GetOutput()->GetObjectTypeDirectory() : std::make_shared<VPGConfigOutput>()->GetObjectTypeDirectory();)
    GETCUSTOM(std::wstring, OutputApplicationDirectoryHpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetApplicationDirectoryHpp() : std::make_shared<VPGConfigOutput>()->GetApplicationDirectoryHpp();)
    GETCUSTOM(std::wstring, OutputApplicationDirectoryCpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetApplicationDirectoryCpp() : std::make_shared<VPGConfigOutput>()->GetApplicationDirectoryCpp();)
    GETCUSTOM(std::wstring, OutputActionDirectoryHpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetActionDirectoryHpp() : std::make_shared<VPGConfigOutput>()->GetActionDirectoryHpp();)
    GETCUSTOM(std::wstring, OutputActionDirectoryCpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetActionDirectoryCpp() : std::make_shared<VPGConfigOutput>()->GetActionDirectoryCpp();)
    GETCUSTOM(std::wstring, OutputFormDirectoryHpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetFormDirectoryHpp() : std::make_shared<VPGConfigOutput>()->GetFormDirectoryHpp();)
    GETCUSTOM(std::wstring, OutputFormDirectoryCpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetFormDirectoryCpp() : std::make_shared<VPGConfigOutput>()->GetFormDirectoryCpp();)
    GETCUSTOM(std::wstring, OutputObjectDirectoryHpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetObjectDirectoryHpp() : std::make_shared<VPGConfigOutput>()->GetObjectDirectoryHpp();)
    GETCUSTOM(std::wstring, OutputObjectDirectoryCpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetObjectDirectoryCpp() : std::make_shared<VPGConfigOutput>()->GetObjectDirectoryCpp();)
    GETCUSTOM(std::wstring, OutputPropertyAccessorDirectoryHpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetPropertyAccessorDirectoryHpp() : std::make_shared<VPGConfigOutput>()->GetPropertyAccessorDirectoryHpp();)
    GETCUSTOM(std::wstring, OutputPropertyAccessorDirectoryCpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetPropertyAccessorDirectoryCpp() : std::make_shared<VPGConfigOutput>()->GetPropertyAccessorDirectoryCpp();)
    GETCUSTOM(std::wstring, OutputObjectFactoryDirectoryHpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetObjectFactoryDirectoryHpp() : std::make_shared<VPGConfigOutput>()->GetObjectFactoryDirectoryHpp();)
    GETCUSTOM(std::wstring, OutputObjectFactoryDirectoryCpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetObjectFactoryDirectoryCpp() : std::make_shared<VPGConfigOutput>()->GetObjectFactoryDirectoryCpp();)
    GETCUSTOM(std::wstring, OutputPropertyAccessorFactoryDirectoryHpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetPropertyAccessorFactoryDirectoryHpp() : std::make_shared<VPGConfigOutput>()->GetPropertyAccessorFactoryDirectoryHpp();)
    GETCUSTOM(std::wstring, OutputPropertyAccessorFactoryDirectoryCpp, return this->GetOutput() != nullptr ? this->GetOutput()->GetPropertyAccessorFactoryDirectoryCpp() : std::make_shared<VPGConfigOutput>()->GetPropertyAccessorFactoryDirectoryCpp();)
    VECTOR(std::wstring, Plugins)
    VECTOR_SPTR(VPGConfigExport, Exports)

    public:
        VPGConfig() : vcc::BaseObject(ObjectType::Config)
        {
            _Template = std::make_shared<VPGConfigTemplate>();
            _Behavior = std::make_shared<VPGConfigBehavior>();
            _Input = std::make_shared<VPGConfigInput>();
            _Output = std::make_shared<VPGConfigOutput>();
        }

        virtual ~VPGConfig() {}

        virtual std::shared_ptr<vcc::IObject> Clone() const override
        {
            auto obj = std::make_shared<VPGConfig>(*this);
            obj->CloneBehavior(this->_Behavior.get());
            obj->CloneExports(this->_Exports);
            obj->CloneInput(this->_Input.get());
            obj->CloneOutput(this->_Output.get());
            obj->CloneTemplate(this->_Template.get());
            return obj;
        }

        virtual std::shared_ptr<vcc::Json> ToJson() const override;
        virtual void DeserializeJson(std::shared_ptr<vcc::IDocument> document) override;
};
