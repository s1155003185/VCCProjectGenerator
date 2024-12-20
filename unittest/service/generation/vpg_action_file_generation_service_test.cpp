#include <gtest/gtest.h>

#include <filesystem>
#include <string>
#include <vector>

#include "file_helper.hpp"

#include "vpg_action_file_generation_service.hpp"
#include "vpg_enum_class_reader.hpp"

using namespace vcc;

class VPGActionFileGenerationServiceTest : public testing::Test 
{
    GETSET_SPTR(LogConfig, LogConfig);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGActionFileGenerationServiceTest/");

    MANAGER_SPTR(VPGEnumClassReader, Reader);

    public:
        void SetUp() override
        {
            this->_LogConfig->SetIsConsoleLog(false);
            std::filesystem::remove_all(PATH(this->GetWorkspace()));
        }

        void TearDown() override
        {
            std::filesystem::remove_all(PATH(this->GetWorkspace()));
        }
};

TEST_F(VPGActionFileGenerationServiceTest, NoFile)
{
    std::wstring enumClass = L""
        "#pragma once\r\n"
        "\r\n"
        "//@@Form\r\n"
        "enum class VPGGitFormProperty\r\n"
        "{\r\n"
        "    AddWorkspace // ACTION(AddWorkspace) @@Class { \"Properties\" : [ \"GETSET(std::wstring, Name, L\\\"\\\")\" ]\r\n"
        "    , WorkNormalProperty // ACTION(WorkNormalProperty) @@Class { \"Properties\" : [ \"GETSET(std::wstring, Name, L\\\"\\\")\", \"GETSET(State, State, State::Busy)\" ], \"Assignments\": [\"\", \"State::Busy\"] } }\r\n"
        "    , WorkObject // ACTION(WorkObject) @@Class { \"Properties\" : [ \"GETSET_SPTR_NULL(LogConfig, LogConfig)\" , \"GETSET(std::wstring, Name, L\\\"\\\")\"], \"Assignments\": [\"_LogConfig\", \"State::Busy\"] }\r\n"
        "    , WorkList // ACTION(WorkList) @@Class { \"Properties\" : [ \"VECTOR(double, VECTOR)\" , \"MAP(std::wstring, std::wstring, MAP)\", \"ORDERED_MAP(int64_t, int64_t, ORDERED_MAP)\"], \"Assignments\": [ \"{1, 2}\", \"{ std::make_shared(1,2), std::make_shared(2,3) }\" ]] }\r\n"
        "    , WorkListObject // ACTION(WorkListObject) @@Class { \"Properties\" : [ \"VECTOR_SPTR(LogConfig, VECTOR)\" , \"MAP_SPTR_R(std::wstring, LogConfig, MAP)\", \"ORDERED_MAP_SPTR_R(int64_t, LogConfig, ORDERED_MAP)\"], \"Assignments\": [ {}, {} ]] }\r\n"
        "    , DeleteWorkspace // ACTION(DeleteWorkspace)\r\n"
        "};\r\n"
        "\r\n";

    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
    this->GetReader()->Parse(enumClass, enumClassList);
    EXPECT_EQ(VPGActionFileGenerationService::GenerateHpp(nullptr, L"", enumClassList.at(0).get()),
        L"abc");
    EXPECT_EQ(VPGActionFileGenerationService::GenerateCpp(nullptr, L"", enumClassList.at(0).get()),
        L"def");
}

TEST_F(VPGActionFileGenerationServiceTest, SeperateFile)
{
    std::wstring enumClass = L""
        "#pragma once\r\n"
        "\r\n"
        "//@@Form\r\n"
        "enum class VPGGitFormProperty\r\n"
        "{\r\n"
        "    AddWorkspace // ACTION(AddWorkspace) @@Class { \"Properties\" : [ \"GETSET(std::wstring, Name, L\\\"\\\")\" ]\r\n"
        "    , WorkNormalProperty // ACTION(WorkNormalProperty) @@Class { \"Properties\" : [ \"GETSET(std::wstring, Name, L\\\"\\\")\", \"GETSET(State, State, State::Busy)\" ], \"Assignments\": [\"\", \"State::Busy\"] } }\r\n"
        "    , WorkObject // ACTION(WorkObject) @@Class { \"Properties\" : [ \"GETSET_SPTR_NULL(LogConfig, LogConfig)\" , \"GETSET(std::wstring, Name, L\\\"\\\")\"], \"Assignments\": [\"_LogConfig\", \"State::Busy\"] }\r\n"
        "    , WorkList // ACTION(WorkList) @@Class { \"Properties\" : [ \"VECTOR(double, VECTOR)\" , \"MAP(std::wstring, std::wstring, MAP)\", \"ORDERED_MAP(int64_t, int64_t, ORDERED_MAP)\"], \"Assignments\": [ \"{1, 2}\", \"{ std::make_shared(1,2), std::make_shared(2,3) }\" ]] }\r\n"
        "    , WorkListObject // ACTION(WorkListObject) @@Class { \"Properties\" : [ \"VECTOR_SPTR(LogConfig, VECTOR)\" , \"MAP_SPTR_R(std::wstring, LogConfig, MAP)\", \"ORDERED_MAP_SPTR_R(int64_t, LogConfig, ORDERED_MAP)\"], \"Assignments\": [ {}, {} ]] }\r\n"
        "    , DeleteWorkspace // ACTION(DeleteWorkspace)\r\n"
        "};\r\n"
        "\r\n";

    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
    this->GetReader()->Parse(enumClass, enumClassList);
    EXPECT_EQ(VPGActionFileGenerationService::GenerateHpp(nullptr, this->GetWorkspace(), enumClassList.at(0).get()), L"");
    EXPECT_EQ(VPGActionFileGenerationService::GenerateCpp(nullptr, this->GetWorkspace(), enumClassList.at(0).get()), L"");

    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_add_workspace.hpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_add_workspace.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_normal_property.hpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_normal_property.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_object.hpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_object.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_list.hpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_list.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_list_object.hpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_list_object.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_delete_workspace.hpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_delete_workspace.cpp"})));
    
    EXPECT_EQ(ReadFile(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_add_workspace.hpp"})), L"");
    EXPECT_EQ(ReadFile(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_add_workspace.cpp"})), L"");
    EXPECT_EQ(ReadFile(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_normal_property.hpp"})), L"");
    EXPECT_EQ(ReadFile(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_normal_property.cpp"})), L"");
    EXPECT_EQ(ReadFile(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_object.hpp"})), L"");
    EXPECT_EQ(ReadFile(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_object.cpp"})), L"");
    EXPECT_EQ(ReadFile(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_list.hpp"})), L"");
    EXPECT_EQ(ReadFile(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_list.cpp"})), L"");
    EXPECT_EQ(ReadFile(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_list_object.hpp"})), L"");
    EXPECT_EQ(ReadFile(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_list_object.cpp"})), L"");
    EXPECT_EQ(ReadFile(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_delete_workspace.hpp"})), L"");
    EXPECT_EQ(ReadFile(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_delete_workspace.cpp"})), L"");
}
