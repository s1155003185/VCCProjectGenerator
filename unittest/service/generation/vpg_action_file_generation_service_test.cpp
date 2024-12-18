#include <gtest/gtest.h>

#include <filesystem>
#include <string>
#include <vector>

#include "file_helper.hpp"

#include "vpg_action_file_generation_service.hpp"

using namespace vcc;

class VPGActionFileGenerationServiceTest : public testing::Test 
{
    GETSET_SPTR(LogConfig, LogConfig);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGActionFileGenerationServiceTest/");
    
    GETSET(std::wstring, FolderPathHpp, L"");
    GETSET(std::wstring, FolderPathCpp, L"");

    public:
        void SetUp() override
        {
            this->_LogConfig->SetIsConsoleLog(false);
            std::filesystem::remove_all(PATH(this->GetWorkspace()));

            // this->_FilePathHpp = ConcatPaths({this->GetWorkspace(), L"object_factory.hpp"});
            // this->_FilePathCpp = ConcatPaths({this->GetWorkspace(), L"object_factory.cpp"});

            // this->_ExpectedHpp = L""
            //     "#pragma once\r\n"
            //     "\r\n"
            //     "#include <memory>\r\n"
            //     "\r\n"
            //     "#include \"base_factory.hpp\"\r\n"
            //     "#include \"i_object.hpp\"\r\n"
            //     "#include \"object_type.hpp\"\r\n"
            //     "\r\n"
            //     "using namespace vcc;\r\n"
            //     "\r\n"
            //     "class ObjectFactory : public BaseFactory\r\n"
            //     "{\r\n"
            //     "    private:\r\n"
            //     "        ObjectFactory() = default;\r\n"
            //     "        virtual ~ObjectFactory() {}\r\n"
            //     "\r\n"
            //     "    public:\r\n"
            //     "        static std::shared_ptr<IObject> Create(const ObjectType &objectType, std::shared_ptr<IObject> parentObject = nullptr);\r\n"
            //     "};\r\n";
        }

        void TearDown() override
        {
            std::filesystem::remove_all(PATH(this->GetWorkspace()));
        }
};

TEST_F(VPGActionFileGenerationServiceTest, SeperateFile)
{
    std::wstring enumClass = L""
        "#pragma once\r\n"
        "\r\n"
        "//@@Form\r\n"
        "enum class VPGGitFormProperty\r\n"
        "{\r\n"
        "    AddWorkspace // ACTION(AddWorkspace) @@Class { \"Properties\" : [ \"GETSET(std::wstring, Name, L\\\"\\\")\" ]\r\n"
        "    , DoWorkNormalProperty // ACTION(DoWorkNormalProperty) @@Class { \"Properties\" : [ \"GETSET(std::wstring, Name, L\\\"\\\")\", \"GETSET(State, State, State::Busy)\" ], \"Assignments\": [\"\", \"State::Busy\"] } }\r\n"
        "    , DoWorkObject // ACTION(DoWorkObject) @@Class { \"Properties\" : [ \"GETSET_SPTR_NULL(LogConfig, LogConfig)\" , \"GETSET(std::wstring, Name, L\\\"\\\")\"], \"Assignments\": [\"_LogConfig\", \"State::Busy\"] }\r\n"
        "    , DoWorkList // ACTION(DoWorkList) @@Class { \"Properties\" : [ \"VECTOR(double, VECTOR)\" , \"MAP(std::wstring, std::wstring, MAP)\", \"ORDERED_MAP(int64_t, int64_t, ORDERED_MAP)\"], \"Assignments\": [ \"{1, 2}\", \"{ std::make_shared(1,2), std::make_shared(2,3) }\" ]] }\r\n"
        "    , DoWorkListObject // ACTION(DoWorkListObject) @@Class { \"Properties\" : [ \"VECTOR_SPTR(LogConfig, VECTOR)\" , \"MAP_SPTR_R(std::wstring, LogConfig, MAP)\", \"ORDERED_MAP_SPTR_R(int64_t, LogConfig, ORDERED_MAP)\"], \"Assignments\": [ {}, {} ]] }\r\n"
        "    , DeleteWorkspace // ACTION(DeleteWorkspace)\r\n"
        "};\r\n"
        "\r\n";

        EXPECT_TRUE(false);
    // WriteFile(ConcatPaths({this->_Workspace, L"vcc_object_property.hpp"}), enumClass, true);

    // std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
    // this->GetReader()->Parse(enumClass, enumClassList);

    // std::wstring classPrefix = L"VPG";
    // std::map<std::wstring, std::wstring> projectClassIncludeFiles;
    // VPGObjectFileGenerationService::GenerateHpp(this->GetLogConfig().get(), classPrefix, projectClassIncludeFiles,
    //     this->GetFilePathHpp(), this->GetFilePathHpp(), this->GetActionFolderPathHpp(), enumClassList);
    // VPGObjectFileGenerationService::GenerateCpp(this->GetLogConfig().get(), classPrefix, projectClassIncludeFiles, this->GetEnumClasses(),
    //     this->GetFilePathCpp(), this->GetFilePathCpp(), this->GetActionFolderPathCpp(), enumClassList);

    // EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));
    // EXPECT_TRUE(IsFileExists(this->GetFilePathCpp()));

    // EXPECT_EQ(ReadFile(this->GetFilePathHpp()), 
    //     L"// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n"
    //     "#pragma once\r\n"
    //     "\r\n"
    //     "#include \"base_form.hpp\"\r\n"
    //     "#include \"class_macro.hpp\"\r\n"
    //     "#include \"object_type.hpp\"\r\n"
    //     "\r\n"
    //     "// <vcc:customHeader sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
    //     "// </vcc:customHeader>\r\n"
    //     "\r\n"
    //     "using namespace vcc;\r\n"
    //     "\r\n"
    //     "class VPGGitForm : public BaseForm\r\n"
    //     "{\r\n"
    //     "    ACTION(AddWorkspace)\r\n"
    //     "    ACTION(DeleteWorkspace)\r\n"
    //     "\r\n"
    //     "    // <vcc:customVPGGitFormProperties sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
    //     "    // </vcc:customVPGGitFormProperties>\r\n"
    //     "\r\n"
    //     "    private:\r\n"
    //     "        // <vcc:customVPGGitFormPrivateFunctions sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
    //     "        // </vcc:customVPGGitFormPrivateFunctions>\r\n"
    //     "\r\n"
    //     "    protected:\r\n"
    //     "        // <vcc:customVPGGitFormProtectedFunctions sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
    //     "        // </vcc:customVPGGitFormProtectedFunctions>\r\n"
    //     "\r\n"
    //     "    public:\r\n"
    //     "        VPGGitForm();\r\n"
    //     "        virtual ~VPGGitForm() {}\r\n"
    //     "\r\n"
    //     "        virtual std::shared_ptr<IObject> Clone() const override;\r\n"
    //     "\r\n"
    //     "        virtual void InitializeComponents() const override;\r\n"
    //     "\r\n"
    //     "        virtual void DoAction(const int64_t &formProperty) const override;\r\n"
    //     "\r\n"
    //     "        // <vcc:customVPGGitFormPublicFunctions sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
    //     "        // </vcc:customVPGGitFormPublicFunctions>\r\n"
    //     "};\r\n");
    // EXPECT_EQ(ReadFile(this->GetFilePathCpp()),
    //     L"// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n"
    //     "#include \"vcc_object.hpp\"\r\n"
    //     "\r\n"
    //     "#include <assert.h>\r\n"
    //     "#include <memory>\r\n"
    //     "\r\n"
    //     "#include \"\"\r\n"
    //     "#include \"base_form.hpp\"\r\n"
    //     "#include \"exception_macro.hpp\"\r\n"
    //     "\r\n"
    //     "// <vcc:customHeader sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
    //     "// </vcc:customHeader>\r\n"
    //     "\r\n"
    //     "using namespace vcc;\r\n"
    //     "\r\n"
    //     "VPGGitForm::VPGGitForm() : BaseForm()\r\n"
    //     "{\r\n"
    //     "    TRY\r\n"
    //     "        _ObjectType = ObjectType::GitForm;\r\n"
    //     "        Initialize();\r\n"
    //     "    CATCH\r\n"
    //     "}\r\n"
    //     "\r\n"
    //     "std::shared_ptr<IObject> VPGGitForm::Clone() const\r\n"
    //     "{\r\n"
    //     "    return std::make_shared<VPGGitForm>(*this);\r\n"
    //     "}\r\n"
    //     "\r\n"
    //     "void VPGGitForm::InitializeComponents() const\r\n"
    //     "{\r\n"
    //     "    TRY\r\n"
    //     "        BaseForm::InitializeComponents();\r\n"
    //     "        _LogConfig = nullptr;\r\n"
    //     "        _ActionManager = nullptr;\r\n"
    //     "        _ThreadManager = nullptr;\r\n"
    //     "        OnInitializeComponents();\r\n"
    //     "    CATCH\r\n"
    //     "}\r\n"
    //     "\r\n"
    //     "void VPGGitForm::DoAction(const int64_t &formProperty) const\r\n"
    //     "{\r\n"
    //     "    TRY\r\n"
    //     "        switch(static_cast<VPGGitFormProperty>(formProperty))\r\n"
    //     "        {\r\n"
    //     "        case VPGGitFormProperty::AddWorkspace:\r\n"
    //     "            DoAddWorkspace();\r\n"
    //     "            break;\r\n"
    //     "        case VPGGitFormProperty::DeleteWorkspace:\r\n"
    //     "            DoDeleteWorkspace();\r\n"
    //     "            break;\r\n"
    //     "        default:\r\n"
    //     "            assert(false);\r\n"
    //     "            break;\r\n"
    //     "        }\r\n"
    //     "    CATCH\r\n"
    //     "}\r\n"
    //     "\r\n"
    //     "void VPGGitForm::DoAddWorkspace() const\r\n"
    //     "{\r\n"
    //     "    TRY\r\n"
    //     "    CATCH\r\n"
    //     "}\r\n"
    //     "\r\n"
    //     "void VPGGitForm::DoDeleteWorkspace() const\r\n"
    //     "{\r\n"
    //     "    TRY\r\n"
    //     "    CATCH\r\n"
    //     "}\r\n"
    //     "\r\n"
    //     "// <vcc:customFunctions sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
    //     "// </vcc:customFunctions>\r\n");
}
