#include <gtest/gtest.h>

#include <filesystem>
#include <string>
#include <vector>

#include "file_helper.hpp"

#include "vpg_action_file_generation_service.hpp"
#include "vpg_cpp_helper.hpp"
#include "vpg_enum_class_reader.hpp"
#include "vpg_file_generation_manager.hpp"
#include "vpg_global.hpp"

class VPGActionFileGenerationServiceTest : public testing::Test 
{
    GETSET_SPTR_NULL(vcc::LogConfig, LogConfig);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGActionFileGenerationServiceTest/");

    public:
        void SetUp() override
        {
            this->_LogConfig = std::make_shared<vcc::LogConfig>();
            this->_LogConfig->setIsConsoleLog(false);
            std::filesystem::remove_all(PATH(this->getWorkspace()));
        }

        void TearDown() override
        {
            std::filesystem::remove_all(PATH(this->getWorkspace()));
        }
};

std::wstring getHppClass(const std::wstring &np, const std::wstring &actionName, const std::vector<std::wstring> &properties, const std::wstring &assignment)
{
    TRY
        std::wstring propertyStr = L"";
        for (auto const &property : properties)
            propertyStr += INDENT + property + L"\r\n";

        std::wstring action = L"\r\n"
            "class VPGGitForm" + actionName + L" : public vcc::BaseAction\r\n"
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
            "        virtual std::wstring getRedoMessageStart() const override;\r\n"
            "        virtual std::wstring getRedoMessageComplete() const override;\r\n"
            "        virtual std::wstring getUndoMessageStart() const override;\r\n"
            "        virtual std::wstring getUndoMessageComplete() const override;\r\n"
            "\r\n"
            "        virtual std::shared_ptr<vcc::IResult> onRedo() override;\r\n"
            "        virtual std::shared_ptr<vcc::IResult> onUndo() override;\r\n"
            "\r\n"
            "        // <vcc:customVPGGitForm" + actionName + L"ProtectedFunctions sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
            "        // </vcc:customVPGGitForm" + actionName + L"ProtectedFunctions>\r\n"
            "\r\n"
            "    public:\r\n"
            "        VPGGitForm" + actionName + L"() : vcc::BaseAction() {}\r\n"
            "        VPGGitForm" + actionName + L"(std::shared_ptr<vcc::LogConfig> logConfig, std::shared_ptr<vcc::IObject> parentForm);\r\n";
        if (!assignment.empty())
            action += L"        VPGGitForm" + actionName + L"(std::shared_ptr<vcc::LogConfig> logConfig, std::shared_ptr<vcc::IObject> parentForm, " + assignment + L");\r\n";
        action += L"        ~VPGGitForm" + actionName + L"() {}\r\n"
            "\r\n"
            "        // <vcc:customVPGGitForm" + actionName + L"PublicFunctions sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
            "        // </vcc:customVPGGitForm" + actionName + L"PublicFunctions>\r\n"
            "};\r\n";
        std::map<std::wstring, std::vector<std::wstring>> namespaceClassMapping;
        namespaceClassMapping[np].push_back(action);
        return generateCodeWithNamespace(namespaceClassMapping);
    CATCH
    return L"";
}

std::wstring getCppClass(const std::wstring &np, const std::wstring &actionName, const std::vector<std::wstring> &properties, const std::wstring &assignment)
{
    TRY
        std::wstring propertyStr = INDENT + L"_LogConfig = logConfig;\r\n"
            + INDENT + L"_ParentObject = parentForm;\r\n";
        std::wstring propertyStrSimple = propertyStr;
        
        for (auto const &property : properties)
            propertyStr += INDENT + property + L"\r\n";

        std::wstring action = L"\r\n"
            "VPGGitForm" + actionName + L"::VPGGitForm" + actionName + L"(std::shared_ptr<vcc::LogConfig> logConfig, std::shared_ptr<vcc::IObject> parentForm) : vcc::BaseAction()\r\n"
            "{\r\n"
            + propertyStrSimple
            + L"}\r\n";
        if (propertyStr != propertyStrSimple)
            action += L"\r\n"
                "VPGGitForm" + actionName + L"::VPGGitForm" + actionName + L"(std::shared_ptr<vcc::LogConfig> logConfig, std::shared_ptr<vcc::IObject> parentForm" + (!assignment.empty() ? L", " : L"") + assignment + L") : vcc::BaseAction()\r\n"
                "{\r\n"
                + propertyStr
                + L"}\r\n";
        action += L"\r\n"
            "std::wstring VPGGitForm" + actionName + L"::getRedoMessageStart() const\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        // <vcc:VPGGitForm" + actionName + L"GetRedoMessageStart sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
            "        return L\"execute VPGGitForm" + actionName + L" start\";\r\n"
            "        // </vcc:VPGGitForm" + actionName + L"GetRedoMessageStart>\r\n"
            "    CATCH\r\n"
            "    return L\"\";\r\n"
            "}\r\n"
            "\r\n"
            "std::wstring VPGGitForm" + actionName + L"::getRedoMessageComplete() const\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        // <vcc:VPGGitForm" + actionName + L"GetRedoMessageComplete sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
            "        return L\"execute VPGGitForm" + actionName + L" complete\";\r\n" 
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
            "std::shared_ptr<vcc::IResult> VPGGitForm" + actionName + L"::onRedo()\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        // <vcc:VPGGitForm" + actionName + L"OnRedo sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
            "        // </vcc:VPGGitForm" + actionName + L"OnRedo>\r\n"
            "    CATCH_RETURN_RESULT(vcc::OperationResult)\r\n"
            "    return std::make_shared<vcc::OperationResult>();\r\n"
            "}\r\n"
            "\r\n"
            "std::shared_ptr<vcc::IResult> VPGGitForm" + actionName + L"::onUndo()\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        // <vcc:VPGGitForm" + actionName + L"OnUndo sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
            "        // </vcc:VPGGitForm" + actionName + L"OnUndo>\r\n"
            "    CATCH_RETURN_RESULT(vcc::OperationResult)\r\n"
            "    return std::make_shared<vcc::OperationResult>();\r\n"
            "}\r\n";
        std::map<std::wstring, std::vector<std::wstring>> namespaceClassMapping;
        namespaceClassMapping[np].push_back(action);
        return generateCodeWithNamespace(namespaceClassMapping);
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
    VPGGlobal::getEnumClassReader()->parse(enumClass, enumClassList);

    std::vector<std::wstring> hppResult = {
            getHppClass(L"", L"AddWorkspace", {  }, L""),
            getHppClass(L"", L"DeleteWorkspace" , { L"GETSET_SPTR_NULL(VPGGitFormDeleteWorkspaceArgument, Argument)" }, L"std::shared_ptr<VPGGitFormDeleteWorkspaceArgument> argument")
    };

    std::vector<std::wstring> cppResult = {
            getCppClass(L"", L"AddWorkspace", { }, L""),
            getCppClass(L"", L"DeleteWorkspace" , { L"_Argument = argument;" }, L"std::shared_ptr<VPGGitFormDeleteWorkspaceArgument> argument")
    };

    std::map<std::wstring, std::wstring> classPathMapping;
    classPathMapping.insert(std::make_pair(L"State", L"state.hpp"));
    classPathMapping.insert(std::make_pair(L"vcc::LogConfig", L"log_config.hpp"));
    classPathMapping.insert(std::make_pair(L"VPGGitFormDeleteWorkspaceArgument", L"vpg_git_form.hpp"));
    classPathMapping.insert(std::make_pair(L"VPGGitFormDeleteWorkspaceArgumentProperty", L"vpg_git_form.hpp"));
    classPathMapping.insert(std::make_pair(L"vcc::OperationResult", L"operation_result.hpp"));

    std::map<std::wstring, std::wstring> hppClass, cppClass;
    std::set<std::wstring> sytemIncludeFiles;
    std::set<std::wstring> customIncludeFiles;
    VPGActionFileGenerationService::GenerateHpp(nullptr, classPathMapping, enumClassList.at(0).get(), L"VPG", L"", hppClass, sytemIncludeFiles, customIncludeFiles);
    VPGActionFileGenerationService::GenerateCpp(nullptr, classPathMapping, enumClassList.at(0).get(), L"VPG", L"", cppClass, sytemIncludeFiles, customIncludeFiles);
    EXPECT_EQ(hppClass.size(), hppResult.size());
    EXPECT_EQ(cppClass.size(), cppResult.size());

    for (auto const &pair : hppClass) {
        for (auto const &hpp : hppResult) {
            if (vcc::isContain(hpp, pair.first)) {
                EXPECT_EQ(pair.second, hpp);
                break;
            }
        }
    }
    for (auto const &pair : cppClass) {
        for (auto const &cpp : cppResult) {
            if (vcc::isContain(cpp, pair.first)) {
                EXPECT_EQ(pair.second, cpp);
                break;
            }
        }
    }
}

TEST_F(VPGActionFileGenerationServiceTest, NoFile_Namespace)
{
    std::wstring enumClass = L""
        "#pragma once\r\n"
        "\r\n"
        "namespace Namespace {\r\n"
        "    //@@Form\r\n"
        "    enum class VPGGitFormProperty\r\n"
        "    {\r\n"
        "        AddWorkspace // ACTION(AddWorkspace)\r\n"
        "        , DeleteWorkspace // ACTION_WITH_ARG_SPTR(DeleteWorkspace, VPGGitFormDeleteWorkspaceArgument)\r\n"
        "    };\r\n"
        "};\r\n";

    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
    VPGGlobal::getEnumClassReader()->parse(enumClass, enumClassList);

    std::vector<std::wstring> hppResult = {
            getHppClass(L"", L"AddWorkspace", {  }, L""),
            getHppClass(L"", L"DeleteWorkspace" , { L"GETSET_SPTR_NULL(VPGGitFormDeleteWorkspaceArgument, Argument)" }, L"std::shared_ptr<VPGGitFormDeleteWorkspaceArgument> argument")
    };

    std::vector<std::wstring> cppResult = {
            getCppClass(L"", L"AddWorkspace", { }, L""),
            getCppClass(L"", L"DeleteWorkspace" , { L"_Argument = argument;" }, L"std::shared_ptr<VPGGitFormDeleteWorkspaceArgument> argument")
    };

    std::map<std::wstring, std::wstring> classPathMapping;
    classPathMapping.insert(std::make_pair(L"State", L"state.hpp"));
    classPathMapping.insert(std::make_pair(L"LogConfig", L"log_config.hpp"));
    classPathMapping.insert(std::make_pair(L"VPGGitFormDeleteWorkspaceArgument", L"vpg_git_form.hpp"));
    classPathMapping.insert(std::make_pair(L"VPGGitFormDeleteWorkspaceArgumentProperty", L"vpg_git_form.hpp"));
    classPathMapping.insert(std::make_pair(L"vcc::OperationResult", L"operation_result.hpp"));

    std::map<std::wstring, std::wstring> hppClass, cppClass;
    std::set<std::wstring> sytemIncludeFiles;
    std::set<std::wstring> customIncludeFiles;
    VPGActionFileGenerationService::GenerateHpp(nullptr, classPathMapping, enumClassList.at(0).get(), L"VPG", L"", hppClass, sytemIncludeFiles, customIncludeFiles);
    VPGActionFileGenerationService::GenerateCpp(nullptr, classPathMapping, enumClassList.at(0).get(), L"VPG", L"", cppClass, sytemIncludeFiles, customIncludeFiles);
    EXPECT_EQ(hppClass.size(), hppResult.size());
    EXPECT_EQ(cppClass.size(), cppResult.size());

    for (auto const &pair : hppClass) {
        for (auto const &hpp : hppResult) {
            if (vcc::isContain(hpp, pair.first)) {
                EXPECT_EQ(pair.second, hpp);
                break;
            }
        }
    }
    for (auto const &pair : cppClass) {
        for (auto const &cpp : cppResult) {
            if (vcc::isContain(cpp, pair.first)) {
                EXPECT_EQ(pair.second, cpp);
                break;
            }
        }
    }
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
    VPGGlobal::getEnumClassReader()->parse(enumClass, enumClassList);
    
    std::map<std::wstring, std::wstring> classPathMapping;
    classPathMapping.insert(std::make_pair(L"State", L"state.hpp"));
    classPathMapping.insert(std::make_pair(L"vcc::LogConfig", L"log_config.hpp"));
    classPathMapping.insert(std::make_pair(L"vcc::OperationResult", L"operation_result.hpp"));
    classPathMapping.insert(std::make_pair(L"VPGGitFormDeleteWorkspaceArgument", L"vpg_git_form.hpp"));
    classPathMapping.insert(std::make_pair(L"VPGGitFormDeleteWorkspaceArgumentProperty", L"vpg_git_form.hpp"));

    std::map<std::wstring, std::wstring> hppClass, cppClass;
    std::set<std::wstring> sytemIncludeFiles;
    std::set<std::wstring> customIncludeFiles;
    VPGActionFileGenerationService::GenerateHpp(nullptr, classPathMapping, enumClassList.at(0).get(), L"VPG", this->getWorkspace(), hppClass, sytemIncludeFiles, customIncludeFiles);
    VPGActionFileGenerationService::GenerateCpp(nullptr, classPathMapping, enumClassList.at(0).get(), L"VPG", this->getWorkspace(), cppClass, sytemIncludeFiles, customIncludeFiles);
    EXPECT_TRUE(hppClass.empty());
    EXPECT_TRUE(cppClass.empty());

    EXPECT_TRUE(vcc::isFilePresent(vcc::concatPaths({this->getWorkspace(), L"vpg_git_form_add_workspace.hpp"})));
    EXPECT_TRUE(vcc::isFilePresent(vcc::concatPaths({this->getWorkspace(), L"vpg_git_form_add_workspace.cpp"})));
    EXPECT_TRUE(vcc::isFilePresent(vcc::concatPaths({this->getWorkspace(), L"vpg_git_form_delete_workspace.hpp"})));
    EXPECT_TRUE(vcc::isFilePresent(vcc::concatPaths({this->getWorkspace(), L"vpg_git_form_delete_workspace.cpp"})));

    std::wstring hppHeader = L"// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n"
        L"#pragma once\r\n"
        "\r\n";
    std::wstring cppHeader = L"// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n";

    std::wstring customPropertyHeader = L"\r\n"
        "// <vcc:customHeader sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
        "// </vcc:customHeader>\r\n";
    std::wstring customFooter = L"\r\n"
        "// <vcc:customFunctions sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
        "// </vcc:customFunctions>\r\n";

    EXPECT_EQ(vcc::readFile(vcc::concatPaths({this->getWorkspace(), L"vpg_git_form_add_workspace.hpp"})),
            hppHeader
            + L"#include <memory>\r\n"
            "#include <string>\r\n"
            "\r\n"
            "#include \"base_action.hpp\"\r\n"
            "#include \"i_object.hpp\"\r\n"
            "#include \"i_result.hpp\"\r\n"
            "#include \"log_config.hpp\"\r\n"
            + customPropertyHeader
            + getHppClass(L"", L"AddWorkspace", { }, L""));
    EXPECT_EQ(vcc::readFile(vcc::concatPaths({this->getWorkspace(), L"vpg_git_form_add_workspace.cpp"})),
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
            + getCppClass(L"", L"AddWorkspace", { }, L"")
            + customFooter);

    EXPECT_EQ(vcc::readFile(vcc::concatPaths({this->getWorkspace(), L"vpg_git_form_delete_workspace.hpp"})),
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
            + getHppClass(L"", L"DeleteWorkspace" , { L"GETSET_SPTR_NULL(VPGGitFormDeleteWorkspaceArgument, Argument)" }, L"std::shared_ptr<VPGGitFormDeleteWorkspaceArgument> argument"));
    EXPECT_EQ(vcc::readFile(vcc::concatPaths({this->getWorkspace(), L"vpg_git_form_delete_workspace.cpp"})),
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
            + getCppClass(L"", L"DeleteWorkspace" , { L"_Argument = argument;" }, L"std::shared_ptr<VPGGitFormDeleteWorkspaceArgument> argument")
            + customFooter);
}

std::map<std::wstring, std::vector<std::wstring>> getNamespaceClassContentMap(const std::wstring &ns, const std::vector<std::wstring> &code)
{
    std::map<std::wstring, std::vector<std::wstring>> result;
    result.insert(std::make_pair(ns, code));
    return result;
}

TEST_F(VPGActionFileGenerationServiceTest, SeperateFile_Namespace)
{
    std::wstring enumClass = L""
        "#pragma once\r\n"
        "\r\n"
        "namespace Namespace {\r\n"
        "    //@@Form\r\n"
        "    enum class VPGGitFormProperty\r\n"
        "    {\r\n"
        "        AddWorkspace // ACTION(AddWorkspace)\r\n"
        "        , DeleteWorkspace // ACTION_WITH_ARG_SPTR(DeleteWorkspace, VPGGitFormDeleteWorkspaceArgument)\r\n"
        "    };\r\n"
        "};\r\n";

    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
    VPGGlobal::getEnumClassReader()->parse(enumClass, enumClassList);
    std::vector<std::wstring> emptyVector;
    
    std::map<std::wstring, std::wstring> classPathMapping;
    classPathMapping.insert(std::make_pair(L"vcc::OperationResult", L"operation_result.hpp"));
    classPathMapping.insert(std::make_pair(L"VPGGitFormDeleteWorkspaceArgument", L"vpg_git_form.hpp"));
    classPathMapping.insert(std::make_pair(L"VPGGitFormDeleteWorkspaceArgumentProperty", L"vpg_git_form.hpp"));

    std::map<std::wstring, std::wstring> hppClass, cppClass;
    std::set<std::wstring> sytemIncludeFiles;
    std::set<std::wstring> customIncludeFiles;
    VPGActionFileGenerationService::GenerateHpp(nullptr, classPathMapping, enumClassList.at(0).get(), L"VPG", this->getWorkspace(), hppClass, sytemIncludeFiles, customIncludeFiles);
    VPGActionFileGenerationService::GenerateCpp(nullptr, classPathMapping, enumClassList.at(0).get(), L"VPG", this->getWorkspace(), cppClass, sytemIncludeFiles, customIncludeFiles);
    EXPECT_TRUE(hppClass.empty());
    EXPECT_TRUE(cppClass.empty());

    EXPECT_TRUE(vcc::isFilePresent(vcc::concatPaths({this->getWorkspace(), L"vpg_git_form_add_workspace.hpp"})));
    EXPECT_TRUE(vcc::isFilePresent(vcc::concatPaths({this->getWorkspace(), L"vpg_git_form_add_workspace.cpp"})));
    EXPECT_TRUE(vcc::isFilePresent(vcc::concatPaths({this->getWorkspace(), L"vpg_git_form_delete_workspace.hpp"})));
    EXPECT_TRUE(vcc::isFilePresent(vcc::concatPaths({this->getWorkspace(), L"vpg_git_form_delete_workspace.cpp"})));

    std::wstring hppHeader = L"// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n"
        L"#pragma once\r\n"
        "\r\n";
    std::wstring cppHeader = L"// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n";

    std::wstring customPropertyHeader = L"\r\n"
        "// <vcc:customHeader sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
        "// </vcc:customHeader>\r\n";
    std::wstring customFooter = L"\r\n"
        "// <vcc:customFunctions sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
        "// </vcc:customFunctions>\r\n";

    EXPECT_EQ(vcc::readFile(vcc::concatPaths({this->getWorkspace(), L"vpg_git_form_add_workspace.hpp"})),
            hppHeader
            + L"#include <memory>\r\n"
            "#include <string>\r\n"
            "\r\n"
            "#include \"base_action.hpp\"\r\n"
            "#include \"i_object.hpp\"\r\n"
            "#include \"i_result.hpp\"\r\n"
            "#include \"log_config.hpp\"\r\n"
            + customPropertyHeader
            + generateCodeWithNamespace(getNamespaceClassContentMap(L"Namespace", { getHppClass(L"", L"AddWorkspace", { }, L"")})));
    EXPECT_EQ(vcc::readFile(vcc::concatPaths({this->getWorkspace(), L"vpg_git_form_add_workspace.cpp"})),
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
            + generateCodeWithNamespace(getNamespaceClassContentMap(L"Namespace", { getCppClass(L"", L"AddWorkspace", { }, L"")}))
            + customFooter);


    EXPECT_EQ(vcc::readFile(vcc::concatPaths({this->getWorkspace(), L"vpg_git_form_delete_workspace.hpp"})),
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
            + generateCodeWithNamespace(getNamespaceClassContentMap(L"Namespace", { getHppClass(L"", L"DeleteWorkspace", { L"GETSET_SPTR_NULL(VPGGitFormDeleteWorkspaceArgument, Argument)" }, L"std::shared_ptr<VPGGitFormDeleteWorkspaceArgument> argument")})));
    EXPECT_EQ(vcc::readFile(vcc::concatPaths({this->getWorkspace(), L"vpg_git_form_delete_workspace.cpp"})),
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
            + generateCodeWithNamespace(getNamespaceClassContentMap(L"Namespace", { getCppClass(L"", L"DeleteWorkspace", { L"_Argument = argument;" }, L"std::shared_ptr<VPGGitFormDeleteWorkspaceArgument> argument")}))
            + customFooter);
}
