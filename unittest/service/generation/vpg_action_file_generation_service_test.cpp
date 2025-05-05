#include <gtest/gtest.h>

#include <filesystem>
#include <string>
#include <vector>

#include "file_helper.hpp"

#include "vpg_action_file_generation_service.hpp"
#include "vpg_enum_class_reader.hpp"
#include "vpg_file_generation_manager.hpp"
#include "vpg_global.hpp"

using namespace vcc;

class VPGActionFileGenerationServiceTest : public testing::Test 
{
    GETSET_SPTR_NULL(LogConfig, LogConfig);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGActionFileGenerationServiceTest/");

    public:
        void SetUp() override
        {
            this->_LogConfig = std::make_shared<LogConfig>();
            this->_LogConfig->SetIsConsoleLog(false);
            std::filesystem::remove_all(PATH(this->GetWorkspace()));
        }

        void TearDown() override
        {
            std::filesystem::remove_all(PATH(this->GetWorkspace()));
        }
};

std::wstring GetHppClass(const std::wstring &actionName, const std::vector<std::wstring> &properties, const std::wstring &assignment)
{
    TRY
        std::wstring propertyStr = L"";
        for (auto const &property : properties)
            propertyStr += INDENT + property + L"\r\n";

        std::wstring action = L"class VPGGitForm" + actionName + L" : public BaseAction\r\n"
            "{\r\n"
            + propertyStr
            + (!propertyStr.empty() ? L"\r\n" : L"")
            + L"    // <vcc:customVPGGitForm" + actionName + L"Properties sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
            "    // </vcc:customVPGGitForm" + actionName + L"Properties>\r\n"
            "\r\n"
            "    private:\r\n"
            "        // <vcc:customVPGGitForm" + actionName + L"PrivateFunctions sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
            "        // </vcc:customVPGGitForm" + actionName + L"PrivateFunctions>\r\n"
            "\r\n"
            "    protected:\r\n"
            "        virtual std::wstring GetRedoMessageStart() const override;\r\n"
            "        virtual std::wstring GetRedoMessageComplete() const override;\r\n"
            "        virtual std::wstring GetUndoMessageStart() const override;\r\n"
            "        virtual std::wstring GetUndoMessageComplete() const override;\r\n"
            "\r\n"
            "        virtual std::shared_ptr<IResult> OnRedo() override;\r\n"
            "        virtual std::shared_ptr<IResult> OnUndo() override;\r\n"
            "\r\n"
            "        // <vcc:customVPGGitForm" + actionName + L"ProtectedFunctions sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
            "        // </vcc:customVPGGitForm" + actionName + L"ProtectedFunctions>\r\n"
            "\r\n"
            "    public:\r\n"
            "        VPGGitForm" + actionName + L"() : BaseAction() {}\r\n"
            "        VPGGitForm" + actionName + L"(std::shared_ptr<LogConfig> logConfig, std::shared_ptr<IObject> parentForm);\r\n";
        if (!assignment.empty())
            action += L"        VPGGitForm" + actionName + L"(std::shared_ptr<LogConfig> logConfig, std::shared_ptr<IObject> parentForm, " + assignment + L");\r\n";
        action += L"        ~VPGGitForm" + actionName + L"() {}\r\n"
            "\r\n"
            "        // <vcc:customVPGGitForm" + actionName + L"PublicFunctions sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
            "        // </vcc:customVPGGitForm" + actionName + L"PublicFunctions>\r\n"
            "};\r\n";
        return action;
    CATCH
    return L"";
}

std::wstring GetCppClass(const std::wstring &actionName, const std::vector<std::wstring> &properties, const std::wstring &assignment)
{
    TRY
        std::wstring propertyStr = INDENT + L"_LogConfig = logConfig;\r\n"
            + INDENT + L"_ParentObject = parentForm;\r\n";
        std::wstring propertyStrSimple = propertyStr;
        
        for (auto const &property : properties)
            propertyStr += INDENT + property + L"\r\n";

        std::wstring action = L"VPGGitForm" + actionName + L"::VPGGitForm" + actionName + L"(std::shared_ptr<LogConfig> logConfig, std::shared_ptr<IObject> parentForm) : BaseAction()\r\n"
            "{\r\n"
            + propertyStrSimple
            + L"}\r\n";
        if (propertyStr != propertyStrSimple)
            action += L"\r\n"
                "VPGGitForm" + actionName + L"::VPGGitForm" + actionName + L"(std::shared_ptr<LogConfig> logConfig, std::shared_ptr<IObject> parentForm" + (!assignment.empty() ? L", " : L"") + assignment + L") : BaseAction()\r\n"
                "{\r\n"
                + propertyStr
                + L"}\r\n";
        action += L"\r\n"
            "std::wstring VPGGitForm" + actionName + L"::GetRedoMessageStart() const\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        // <vcc:VPGGitForm" + actionName + L"GetRedoMessageStart sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
            "        return L\"Execute VPGGitForm" + actionName + L" start\";\r\n"
            "        // </vcc:VPGGitForm" + actionName + L"GetRedoMessageStart>\r\n"
            "    CATCH\r\n"
            "    return L\"\";\r\n"
            "}\r\n"
            "\r\n"
            "std::wstring VPGGitForm" + actionName + L"::GetRedoMessageComplete() const\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        // <vcc:VPGGitForm" + actionName + L"GetRedoMessageComplete sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
            "        return L\"Execute VPGGitForm" + actionName + L" complete\";\r\n" 
            "        // </vcc:VPGGitForm" + actionName + L"GetRedoMessageComplete>\r\n"
            "    CATCH\r\n"
            "    return L\"\";\r\n"
            "}\r\n"
            "\r\n"
            "std::wstring VPGGitForm" + actionName + L"::GetUndoMessageStart() const\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        // <vcc:VPGGitForm" + actionName + L"GetUndoMessageStart sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
            "        return L\"Undo VPGGitForm" + actionName + L" start\";\r\n"
            "        // </vcc:VPGGitForm" + actionName + L"GetUndoMessageStart>\r\n"
            "    CATCH\r\n"
            "    return L\"\";\r\n"
            "}\r\n"
            "\r\n"
            "std::wstring VPGGitForm" + actionName + L"::GetUndoMessageComplete() const\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        // <vcc:VPGGitForm" + actionName + L"GetUndoMessageComplete sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
            "        return L\"Undo VPGGitForm" + actionName + L" complete\";\r\n" 
            "        // </vcc:VPGGitForm" + actionName + L"GetUndoMessageComplete>\r\n"
            "    CATCH\r\n"
            "    return L\"\";\r\n"
            "}\r\n"
            "\r\n"
            "std::shared_ptr<IResult> VPGGitForm" + actionName + L"::OnRedo()\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        // <vcc:VPGGitForm" + actionName + L"OnRedo sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
            "        // </vcc:VPGGitForm" + actionName + L"OnRedo>\r\n"
            "    CATCH_RETURN_RESULT(OperationResult)\r\n"
            "    return std::make_shared<OperationResult>();\r\n"
            "}\r\n"
            "\r\n"
            "std::shared_ptr<IResult> VPGGitForm" + actionName + L"::OnUndo()\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        // <vcc:VPGGitForm" + actionName + L"OnUndo sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
            "        // </vcc:VPGGitForm" + actionName + L"OnUndo>\r\n"
            "    CATCH_RETURN_RESULT(OperationResult)\r\n"
            "    return std::make_shared<OperationResult>();\r\n"
            "}\r\n";
        return action;
    CATCH
    return L"";
}

TEST_F(VPGActionFileGenerationServiceTest, NoFile)
{
    std::wstring enumClass = L""
        "#pragma once\r\n"
        "\r\n"
        "//@@Form\r\n"
        "enum class VPGGitFormProperty\r\n"
        "{\r\n"
        "    AddWorkspace // ACTION(AddWorkspace)\r\n"
        "    , DeleteWorkspace // ACTION_WITH_ARG_SPTR(DeleteWorkspace, VPGGitFormDeleteWorkspaceArgument)\r\n"
        "};\r\n"
        "\r\n";

    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
    VPGGlobal::GetEnumClassReader()->Parse(enumClass, enumClassList);

    std::vector<std::wstring> hppResult = {
            GetHppClass(L"AddWorkspace", {  }, L""),
            GetHppClass(L"DeleteWorkspace" , { L"GETSET_SPTR_NULL(VPGGitFormDeleteWorkspaceArgument, Argument)" }, L"std::shared_ptr<VPGGitFormDeleteWorkspaceArgument> argument")
    };

    std::vector<std::wstring> cppResult = {
            GetCppClass(L"AddWorkspace", { }, L""),
            GetCppClass(L"DeleteWorkspace" , { L"_Argument = argument;" }, L"std::shared_ptr<VPGGitFormDeleteWorkspaceArgument> argument")
    };

    std::map<std::wstring, std::wstring> classPathMapping;
    classPathMapping.insert(std::make_pair(L"State", L"state.hpp"));
    classPathMapping.insert(std::make_pair(L"LogConfig", L"log_config.hpp"));
    classPathMapping.insert(std::make_pair(L"VPGGitFormDeleteWorkspaceArgument", L"vpg_git_form.hpp"));
    classPathMapping.insert(std::make_pair(L"VPGGitFormDeleteWorkspaceArgumentProperty", L"vpg_git_form.hpp"));

    std::vector<std::wstring> hppClass, cppClass;
    std::set<std::wstring> sytemIncludeFiles;
    std::set<std::wstring> customIncludeFiles;
    VPGActionFileGenerationService::GenerateHpp(nullptr, classPathMapping, enumClassList.at(0).get(), L"VPG", L"", hppClass, sytemIncludeFiles, customIncludeFiles);
    VPGActionFileGenerationService::GenerateCpp(nullptr, classPathMapping, enumClassList.at(0).get(), L"VPG", L"", cppClass, sytemIncludeFiles, customIncludeFiles);
    EXPECT_EQ(hppClass, hppResult);
    EXPECT_EQ(cppClass, cppResult);
}

TEST_F(VPGActionFileGenerationServiceTest, SeperateFile)
{
    std::wstring enumClass = L""
        "#pragma once\r\n"
        "//@@Form\r\n"
        "enum class VPGGitFormProperty\r\n"
        "{\r\n"
        "    AddWorkspace // ACTION(AddWorkspace)\r\n"
        "    , DeleteWorkspace // ACTION_WITH_ARG_SPTR(DeleteWorkspace, VPGGitFormDeleteWorkspaceArgument)\r\n"
        "};\r\n"
        "\r\n";

    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
    VPGGlobal::GetEnumClassReader()->Parse(enumClass, enumClassList);
    std::vector<std::wstring> emptyVector;
    
    std::map<std::wstring, std::wstring> classPathMapping;
    classPathMapping.insert(std::make_pair(L"State", L"state.hpp"));
    classPathMapping.insert(std::make_pair(L"LogConfig", L"log_config.hpp"));
    classPathMapping.insert(std::make_pair(L"OperationResult", L"operation_result.hpp"));
    classPathMapping.insert(std::make_pair(L"VPGGitFormDeleteWorkspaceArgument", L"vpg_git_form.hpp"));
    classPathMapping.insert(std::make_pair(L"VPGGitFormDeleteWorkspaceArgumentProperty", L"vpg_git_form.hpp"));

    std::vector<std::wstring> hppClass, cppClass;
    std::set<std::wstring> sytemIncludeFiles;
    std::set<std::wstring> customIncludeFiles;
    VPGActionFileGenerationService::GenerateHpp(nullptr, classPathMapping, enumClassList.at(0).get(), L"VPG", this->GetWorkspace(), hppClass, sytemIncludeFiles, customIncludeFiles);
    VPGActionFileGenerationService::GenerateCpp(nullptr, classPathMapping, enumClassList.at(0).get(), L"VPG", this->GetWorkspace(), cppClass, sytemIncludeFiles, customIncludeFiles);
    EXPECT_EQ(hppClass, emptyVector);
    EXPECT_EQ(cppClass, emptyVector);

    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_add_workspace.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_add_workspace.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_delete_workspace.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_delete_workspace.cpp"})));

    std::wstring hppHeader = L"// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n"
        L"#pragma once\r\n"
        "\r\n";
    std::wstring cppHeader = L"// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n";

    std::wstring customPropertyHeader = L"\r\n"
        "// <vcc:customHeader sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
        "// </vcc:customHeader>\r\n"
        "\r\n";
    std::wstring customFooter = L"\r\n"
        "// <vcc:customFunctions sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
        "// </vcc:customFunctions>\r\n";

    EXPECT_EQ(ReadFile(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_add_workspace.hpp"})),
            hppHeader
            + L"#include <memory>\r\n"
            "#include <string>\r\n"
            "\r\n"
            "#include \"base_action.hpp\"\r\n"
            "#include \"i_object.hpp\"\r\n"
            "#include \"i_result.hpp\"\r\n"
            "#include \"log_config.hpp\"\r\n"
            + customPropertyHeader
            + GetHppClass(L"AddWorkspace", { }, L""));
    EXPECT_EQ(ReadFile(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_add_workspace.cpp"})),
            cppHeader
            + L"#include \"vpg_git_form_add_workspace.hpp\"\r\n"
            "\r\n"
            "#include <memory>\r\n"
            "#include <string>\r\n"
            "\r\n"
            "#include \"base_action.hpp\"\r\n"
            "#include \"exception_macro.hpp\"\r\n"
            "#include \"i_object.hpp\"\r\n"
            "#include \"i_result.hpp\"\r\n"
            "#include \"log_config.hpp\"\r\n"
            "#include \"operation_result.hpp\"\r\n"
            + customPropertyHeader
            + GetCppClass(L"AddWorkspace", { }, L"")
            + customFooter);

    EXPECT_EQ(ReadFile(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_delete_workspace.hpp"})),
            hppHeader
            + L"#include <memory>\r\n"
            "#include <string>\r\n"
            "\r\n"
            "#include \"base_action.hpp\"\r\n"
            "#include \"i_object.hpp\"\r\n"
            "#include \"i_result.hpp\"\r\n"
            "#include \"log_config.hpp\"\r\n"
            "#include \"vpg_git_form.hpp\"\r\n"
            + customPropertyHeader
            + GetHppClass(L"DeleteWorkspace" , { L"GETSET_SPTR_NULL(VPGGitFormDeleteWorkspaceArgument, Argument)" }, L"std::shared_ptr<VPGGitFormDeleteWorkspaceArgument> argument"));
    EXPECT_EQ(ReadFile(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_delete_workspace.cpp"})),
            cppHeader
            + L"#include \"vpg_git_form_delete_workspace.hpp\"\r\n"
            "\r\n"
            "#include <memory>\r\n"
            "#include <string>\r\n"
            "\r\n"
            "#include \"base_action.hpp\"\r\n"
            "#include \"exception_macro.hpp\"\r\n"
            "#include \"i_object.hpp\"\r\n"
            "#include \"i_result.hpp\"\r\n"
            "#include \"log_config.hpp\"\r\n"
            "#include \"operation_result.hpp\"\r\n"
            "#include \"vpg_git_form.hpp\"\r\n"
            + customPropertyHeader
            + GetCppClass(L"DeleteWorkspace" , { L"_Argument = argument;" }, L"std::shared_ptr<VPGGitFormDeleteWorkspaceArgument> argument")
            + customFooter);
}
