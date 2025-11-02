#include "vpg_unittest_file_generation_service.hpp"

#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include "base_object.hpp"
#include "class_macro.hpp"
#include "file_helper.hpp"
#include "log_config.hpp"
#include "vpg_enum_class.hpp"
#include "vpg_global.hpp"

class VPGUnittestFileGenerationServiceTest : public testing::Test, public vcc::BaseObject {
    GETSET_SPTR_NULL(vcc::LogConfig, LogConfig);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGUnittestFileGenerationServiceTest/");
    GETSET(std::wstring, EnumClass, L"");
    GETSET(std::wstring, EnumClassWithNamespace, L"");
    GETSET(std::wstring, OriginalPropertyFilePath, L"vpg_git_form_property.hpp");
    VECTOR_SPTR(VPGEnumClass, EnumClassList);
    VECTOR_SPTR(VPGEnumClass, EnumClassListWithNamespace);

   public:
    void SetUp() override {
        this->_LogConfig = std::make_shared<vcc::LogConfig>();
        this->_LogConfig->setIsConsoleLog(false);
        std::filesystem::remove_all(PATH(this->getWorkspace()));

        this->_EnumClass =
            L""
            "#pragma once\r\n"
            "\r\n"
            "//@@Form\r\n"
            "enum class VPGGitFormProperty\r\n"
            "{\r\n"
            "    AddWorkspace // ACTION(AddWorkspace)\r\n"
            "    , DeleteWorkspace // ACTION_WITH_ARG_SPTR(DeleteWorkspace, "
            "VPGGitFormDeleteWorkspaceArgument)\r\n"
            "};\r\n";
        VPGGlobal::getEnumClassReader()->parse(this->_EnumClass, this->_EnumClassList);

        this->_EnumClassWithNamespace =
            L""
            "#pragma once\r\n"
            "\r\n"
            "namespace Namespace {\r\n"
            "    //@@Form\r\n"
            "    enum class VPGGitFormProperty\r\n"
            "    {\r\n"
            "        AddWorkspace // ACTION(AddWorkspace)\r\n"
            "        , DeleteWorkspace // ACTION_WITH_ARG_SPTR(DeleteWorkspace, "
            "VPGGitFormDeleteWorkspaceArgument)\r\n"
            "    };\r\n"
            "};\r\n";
        VPGGlobal::getEnumClassReader()->parse(this->_EnumClass, this->_EnumClassListWithNamespace);
    }

    void TearDown() override { std::filesystem::remove_all(PATH(this->getWorkspace())); }

    std::shared_ptr<IObject> clone() const override { return nullptr; }
};

TEST_F(VPGUnittestFileGenerationServiceTest, GenerateUnittest_Action) {
    VPGUnittestFileGenerationService::generateCppAction(
        this->getLogConfig().get(), L"VPG", this->getOriginalPropertyFilePath(),
        this->getWorkspace(), this->getEnumClassList(), {L"TestAction1", L"TestAction2"}, false);
    EXPECT_TRUE(vcc::isFilePresent(
        vcc::concatPaths({this->getWorkspace(), L"vpg_git_form_action_test.cpp"})));
    EXPECT_EQ(
        vcc::readFile(vcc::concatPaths({this->getWorkspace(), L"vpg_git_form_action_test.cpp"})),
        L""
        "// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n"
        "#include <gtest/gtest.h>\r\n"
        "\r\n"
        "// <vcc:customHeader sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
        "// </vcc:customHeader>\r\n"
        "\r\n"
        "class VPGGitFormActionTest : public testing::Test \r\n"
        "{\r\n"
        "    // <vcc:customVPGGitFormActionTestProperties sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
        "    // </vcc:customVPGGitFormActionTestProperties>\r\n"
        "\r\n"
        "    public:\r\n"
        "        VPGGitFormActionTest() {}\r\n"
        "        virtual ~VPGGitFormActionTest() {}\r\n"
        "\r\n"
        "        void SetUp() override\r\n"
        "        {\r\n"
        "            // <vcc:customVPGGitFormActionTestSetUp sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
        "            // </vcc:customVPGGitFormActionTestSetUp>\r\n"
        "        }\r\n"
        "\r\n"
        "        void TearDown() override\r\n"
        "        {\r\n"
        "            // <vcc:customVPGGitFormActionTestTearDown sync=\"RESERVE\" "
        "gen=\"RESERVE\">\r\n"
        "            // </vcc:customVPGGitFormActionTestTearDown>\r\n"
        "        }\r\n"
        "\r\n"
        "        // <vcc:customVPGGitFormActionTestFunctions sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
        "        // </vcc:customVPGGitFormActionTestFunctions>\r\n"
        "};\r\n"
        "\r\n"
        "TEST_F(VPGGitFormActionTest, VPGGitFormAddWorkspace_TestAction1) {\r\n"
        "    // <vcc:customVPGGitFormActionTestVPGGitFormAddWorkspace_TestAction1 sync=\"RESERVE\" "
        "gen=\"RESERVE\">\r\n"
        "    // TODO: Implement test case\r\n"
        "    // </vcc:customVPGGitFormActionTestVPGGitFormAddWorkspace_TestAction1>\r\n"
        "};\r\n"
        "\r\n"
        "TEST_F(VPGGitFormActionTest, VPGGitFormAddWorkspace_TestAction2) {\r\n"
        "    // <vcc:customVPGGitFormActionTestVPGGitFormAddWorkspace_TestAction2 sync=\"RESERVE\" "
        "gen=\"RESERVE\">\r\n"
        "    // TODO: Implement test case\r\n"
        "    // </vcc:customVPGGitFormActionTestVPGGitFormAddWorkspace_TestAction2>\r\n"
        "};\r\n"
        "\r\n"
        "TEST_F(VPGGitFormActionTest, VPGGitFormDeleteWorkspace_TestAction1) {\r\n"
        "    // <vcc:customVPGGitFormActionTestVPGGitFormDeleteWorkspace_TestAction1 "
        "sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
        "    // TODO: Implement test case\r\n"
        "    // </vcc:customVPGGitFormActionTestVPGGitFormDeleteWorkspace_TestAction1>\r\n"
        "};\r\n"
        "\r\n"
        "TEST_F(VPGGitFormActionTest, VPGGitFormDeleteWorkspace_TestAction2) {\r\n"
        "    // <vcc:customVPGGitFormActionTestVPGGitFormDeleteWorkspace_TestAction2 "
        "sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
        "    // TODO: Implement test case\r\n"
        "    // </vcc:customVPGGitFormActionTestVPGGitFormDeleteWorkspace_TestAction2>\r\n"
        "};\r\n");
}

TEST_F(VPGUnittestFileGenerationServiceTest, GenerateUnittest_Action_SeperatedFile) {
    VPGUnittestFileGenerationService::generateCppAction(
        this->getLogConfig().get(), L"VPG", this->getOriginalPropertyFilePath(),
        this->getWorkspace(), this->getEnumClassList(), {L"TestAction1", L"TestAction2"}, true);
    EXPECT_TRUE(vcc::isFilePresent(
        vcc::concatPaths({this->getWorkspace(), L"vpg_git_form_add_workspace_test.cpp"})));
    EXPECT_TRUE(vcc::isFilePresent(
        vcc::concatPaths({this->getWorkspace(), L"vpg_git_form_delete_workspace_test.cpp"})));

    EXPECT_EQ(vcc::readFile(
                  vcc::concatPaths({this->getWorkspace(), L"vpg_git_form_add_workspace_test.cpp"})),
              L""
              "// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n"
              "#include <gtest/gtest.h>\r\n"
              "\r\n"
              "// <vcc:customHeader sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
              "// </vcc:customHeader>\r\n"
              "\r\n"
              "class VPGGitFormAddWorkspaceTest : public testing::Test \r\n"
              "{\r\n"
              "    // <vcc:customVPGGitFormAddWorkspaceTestProperties sync=\"RESERVE\" "
              "gen=\"RESERVE\">\r\n"
              "    // </vcc:customVPGGitFormAddWorkspaceTestProperties>\r\n"
              "\r\n"
              "    public:\r\n"
              "        VPGGitFormAddWorkspaceTest() {}\r\n"
              "        virtual ~VPGGitFormAddWorkspaceTest() {}\r\n"
              "\r\n"
              "        void SetUp() override {\r\n"
              "            // <vcc:customVPGGitFormAddWorkspaceTestSetUp sync=\"RESERVE\" "
              "gen=\"RESERVE\">\r\n"
              "            // </vcc:customVPGGitFormAddWorkspaceTestSetUp>\r\n"
              "        }\r\n"
              "\r\n"
              "        void TearDown() override {\r\n"
              "            // <vcc:customVPGGitFormAddWorkspaceTestTearDown sync=\"RESERVE\" "
              "gen=\"RESERVE\">\r\n"
              "            // </vcc:customVPGGitFormAddWorkspaceTestTearDown>\r\n"
              "        }\r\n"
              "\r\n"
              "        // <vcc:customVPGGitFormAddWorkspaceTestFunctions sync=\"RESERVE\" "
              "gen=\"RESERVE\">\r\n"
              "        // </vcc:customVPGGitFormAddWorkspaceTestFunctions>\r\n"
              "};\r\n"
              "\r\n"
              "TEST_F(VPGGitFormAddWorkspaceTest, TestAction1) {\r\n"
              "    // <vcc:customVPGGitFormAddWorkspaceTestTestAction1 sync=\"RESERVE\" "
              "gen=\"RESERVE\">\r\n"
              "    // TODO: Implement test case\r\n"
              "    // </vcc:customVPGGitFormAddWorkspaceTestTestAction1>\r\n"
              "};\r\n"
              "\r\n"
              "TEST_F(VPGGitFormAddWorkspaceTest, TestAction2) {\r\n"
              "    // <vcc:customVPGGitFormAddWorkspaceTestTestAction2 sync=\"RESERVE\" "
              "gen=\"RESERVE\">\r\n"
              "    // TODO: Implement test case\r\n"
              "    // </vcc:customVPGGitFormAddWorkspaceTestTestAction2>\r\n"
              "};\r\n");
    EXPECT_EQ(vcc::readFile(vcc::concatPaths(
                  {this->getWorkspace(), L"vpg_git_form_delete_workspace_test.cpp"})),
              L""
              "// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n"
              "#include <gtest/gtest.h>\r\n"
              "\r\n"
              "// <vcc:customHeader sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
              "// </vcc:customHeader>\r\n"
              "\r\n"
              "class VPGGitFormDeleteWorkspaceTest : public testing::Test \r\n"
              "{\r\n"
              "    // <vcc:customVPGGitFormDeleteWorkspaceTestProperties sync=\"RESERVE\" "
              "gen=\"RESERVE\">\r\n"
              "    // </vcc:customVPGGitFormDeleteWorkspaceTestProperties>\r\n"
              "\r\n"
              "    public:\r\n"
              "        VPGGitFormDeleteWorkspaceTest() {}\r\n"
              "        virtual ~VPGGitFormDeleteWorkspaceTest() {}\r\n"
              "\r\n"
              "        void SetUp() override {\r\n"
              "            // <vcc:customVPGGitFormDeleteWorkspaceTestSetUp sync=\"RESERVE\" "
              "gen=\"RESERVE\">\r\n"
              "            // </vcc:customVPGGitFormDeleteWorkspaceTestSetUp>\r\n"
              "        }\r\n"
              "\r\n"
              "        void TearDown() override {\r\n"
              "            // <vcc:customVPGGitFormDeleteWorkspaceTestTearDown sync=\"RESERVE\" "
              "gen=\"RESERVE\">\r\n"
              "            // </vcc:customVPGGitFormDeleteWorkspaceTestTearDown>\r\n"
              "        }\r\n"
              "\r\n"
              "        // <vcc:customVPGGitFormDeleteWorkspaceTestFunctions sync=\"RESERVE\" "
              "gen=\"RESERVE\">\r\n"
              "        // </vcc:customVPGGitFormDeleteWorkspaceTestFunctions>\r\n"
              "};\r\n"
              "\r\n"
              "TEST_F(VPGGitFormDeleteWorkspaceTest, TestAction1) {\r\n"
              "    // <vcc:customVPGGitFormDeleteWorkspaceTestTestAction1 sync=\"RESERVE\" "
              "gen=\"RESERVE\">\r\n"
              "    // TODO: Implement test case\r\n"
              "    // </vcc:customVPGGitFormDeleteWorkspaceTestTestAction1>\r\n"
              "};\r\n"
              "\r\n"
              "TEST_F(VPGGitFormDeleteWorkspaceTest, TestAction2) {\r\n"
              "    // <vcc:customVPGGitFormDeleteWorkspaceTestTestAction2 sync=\"RESERVE\" "
              "gen=\"RESERVE\">\r\n"
              "    // TODO: Implement test case\r\n"
              "    // </vcc:customVPGGitFormDeleteWorkspaceTestTestAction2>\r\n"
              "};\r\n");
}

TEST_F(VPGUnittestFileGenerationServiceTest, GenerateUnittest_Action_Namespace) {
    VPGUnittestFileGenerationService::generateCppAction(
        this->getLogConfig().get(), L"VPG", this->getOriginalPropertyFilePath(),
        this->getWorkspace(), this->getEnumClassListWithNamespace(),
        {L"TestAction1", L"TestAction2"}, false);
    EXPECT_TRUE(vcc::isFilePresent(
        vcc::concatPaths({this->getWorkspace(), L"vpg_git_form_action_test.cpp"})));
    EXPECT_EQ(
        vcc::readFile(vcc::concatPaths({this->getWorkspace(), L"vpg_git_form_action_test.cpp"})),
        L""
        "// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n"
        "#include <gtest/gtest.h>\r\n"
        "\r\n"
        "// <vcc:customHeader sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
        "// </vcc:customHeader>\r\n"
        "\r\n"
        "class VPGGitFormActionTest : public testing::Test \r\n"
        "{\r\n"
        "    // <vcc:customVPGGitFormActionTestProperties sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
        "    // </vcc:customVPGGitFormActionTestProperties>\r\n"
        "\r\n"
        "    public:\r\n"
        "        VPGGitFormActionTest() {}\r\n"
        "        virtual ~VPGGitFormActionTest() {}\r\n"
        "\r\n"
        "        void SetUp() override\r\n"
        "        {\r\n"
        "            // <vcc:customVPGGitFormActionTestSetUp sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
        "            // </vcc:customVPGGitFormActionTestSetUp>\r\n"
        "        }\r\n"
        "\r\n"
        "        void TearDown() override\r\n"
        "        {\r\n"
        "            // <vcc:customVPGGitFormActionTestTearDown sync=\"RESERVE\" "
        "gen=\"RESERVE\">\r\n"
        "            // </vcc:customVPGGitFormActionTestTearDown>\r\n"
        "        }\r\n"
        "\r\n"
        "        // <vcc:customVPGGitFormActionTestFunctions sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
        "        // </vcc:customVPGGitFormActionTestFunctions>\r\n"
        "};\r\n"
        "\r\n"
        "TEST_F(VPGGitFormActionTest, VPGGitFormAddWorkspace_TestAction1) {\r\n"
        "    // <vcc:customVPGGitFormActionTestVPGGitFormAddWorkspace_TestAction1 sync=\"RESERVE\" "
        "gen=\"RESERVE\">\r\n"
        "    // TODO: Implement test case\r\n"
        "    // </vcc:customVPGGitFormActionTestVPGGitFormAddWorkspace_TestAction1>\r\n"
        "};\r\n"
        "\r\n"
        "TEST_F(VPGGitFormActionTest, VPGGitFormAddWorkspace_TestAction2) {\r\n"
        "    // <vcc:customVPGGitFormActionTestVPGGitFormAddWorkspace_TestAction2 sync=\"RESERVE\" "
        "gen=\"RESERVE\">\r\n"
        "    // TODO: Implement test case\r\n"
        "    // </vcc:customVPGGitFormActionTestVPGGitFormAddWorkspace_TestAction2>\r\n"
        "};\r\n"
        "\r\n"
        "TEST_F(VPGGitFormActionTest, VPGGitFormDeleteWorkspace_TestAction1) {\r\n"
        "    // <vcc:customVPGGitFormActionTestVPGGitFormDeleteWorkspace_TestAction1 "
        "sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
        "    // TODO: Implement test case\r\n"
        "    // </vcc:customVPGGitFormActionTestVPGGitFormDeleteWorkspace_TestAction1>\r\n"
        "};\r\n"
        "\r\n"
        "TEST_F(VPGGitFormActionTest, VPGGitFormDeleteWorkspace_TestAction2) {\r\n"
        "    // <vcc:customVPGGitFormActionTestVPGGitFormDeleteWorkspace_TestAction2 "
        "sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
        "    // TODO: Implement test case\r\n"
        "    // </vcc:customVPGGitFormActionTestVPGGitFormDeleteWorkspace_TestAction2>\r\n"
        "};\r\n");
}
