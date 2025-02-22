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
    GETSET_SPTR(LogConfig, LogConfig);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGActionFileGenerationServiceTest/");

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
            "        virtual void OnRedo() override;\r\n"
            "        virtual void OnUndo() override;\r\n"
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
            "void VPGGitForm" + actionName + L"::OnRedo()\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        // <vcc:VPGGitForm" + actionName + L"OnRedo sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
            "        // </vcc:VPGGitForm" + actionName + L"OnRedo>\r\n"
            "    CATCH\r\n"
            "}\r\n"
            "\r\n"
            "void VPGGitForm" + actionName + L"::OnUndo()\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        // <vcc:VPGGitForm" + actionName + L"OnUndo sync=\"RESERVE\" gen=\"RESERVE\">\r\n"
            "        // </vcc:VPGGitForm" + actionName + L"OnUndo>\r\n"
            "    CATCH\r\n"
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
        "    AddWorkspace // ACTION(AddWorkspace) @@Class { \"Properties\" : [ \"GETSET(std::wstring, Name, L\\\"\\\")\" ]\r\n"
        "    , WorkNormalProperty // ACTION(WorkNormalProperty) @@Class { \"Properties\" : [ \"GETSET(std::wstring, Name, L\\\"\\\")\", \"GETSET(State, State, State::Busy)\" ], \"Assignments\": [\"\", \"State::Busy\"] } }\r\n"
        "    , WorkObject // ACTION(WorkObject) @@Class { \"Properties\" : [ \"GETSET_SPTR_NULL(LogConfig, LogConfig)\" , \"GETSET(std::wstring, Name, L\\\"\\\")\"], \"Assignments\": [\"_LogConfig\", \"State::Busy\"] }\r\n"
        "    , WorkList // ACTION(WorkList) @@Class { \"Properties\" : [ \"VECTOR(double, Vector)\" , \"MAP(std::wstring, std::wstring, Map)\", \"ORDERED_MAP(int64_t, int64_t, OrderedMap)\"], \"Assignments\": [ \"{1, 2}\", \"{ std::make_shared(1,2), std::make_shared(2,3) }\" ]] }\r\n"
        "    , WorkListObject // ACTION(WorkListObject) @@Class { \"Properties\" : [ \"VECTOR_SPTR(LogConfig, Vector)\" , \"MAP_SPTR_R(std::wstring, LogConfig, Map)\", \"ORDERED_MAP_SPTR_R(int64_t, LogConfig, OrderedMap)\"], \"Assignments\": [ \"{}\", \"{}\" ]] }\r\n"
        "    , DeleteWorkspace // ACTION(DeleteWorkspace)\r\n"
        "};\r\n"
        "\r\n";

    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
    VPGGlobal::GetEnumClassReader()->Parse(enumClass, enumClassList);

    std::vector<std::wstring> hppResult = {
            GetHppClass(L"AddWorkspace", { L"GETSET(std::wstring, Name, L\"\")" }, L"std::wstring name"),
            GetHppClass(L"WorkNormalProperty", { L"GETSET(std::wstring, Name, L\"\")", L"GETSET(State, State, State::Busy)" }, L"std::wstring name, State state"),
            GetHppClass(L"WorkObject", { L"GETSET_SPTR_NULL(LogConfig, LogConfig)" , L"GETSET(std::wstring, Name, L\"\")" }, L"std::shared_ptr<LogConfig> logConfig, std::wstring name"),
            GetHppClass(L"WorkList", { L"VECTOR(double, Vector)" , L"MAP(std::wstring, std::wstring, Map)", L"ORDERED_MAP(int64_t, int64_t, OrderedMap)" }, L"std::vector<double> vector, std::map<std::wstring, std::wstring> map, std::vector<std::pair<int64_t, int64_t>> orderedMap"),
            GetHppClass(L"WorkListObject", { L"VECTOR_SPTR(LogConfig, Vector)" , L"MAP_SPTR_R(std::wstring, LogConfig, Map)", L"ORDERED_MAP_SPTR_R(int64_t, LogConfig, OrderedMap)" }, L"std::vector<std::shared_ptr<LogConfig>> vector, std::map<std::wstring, std::shared_ptr<LogConfig>> map, std::vector<std::pair<int64_t, std::shared_ptr<LogConfig>>> orderedMap"),
            GetHppClass(L"DeleteWorkspace" , {}, L"")
    };

    std::vector<std::wstring> cppResult = {
            GetCppClass(L"AddWorkspace", { L"_Name = name;" }, L"std::wstring name"),
            GetCppClass(L"WorkNormalProperty", { L"_Name = name;", L"_State = state;" }, L"std::wstring name, State state"),
            GetCppClass(L"WorkObject", { L"_LogConfig = logConfig;" , L"_Name = name;" }, L"std::shared_ptr<LogConfig> logConfig, std::wstring name"),
            GetCppClass(L"WorkList", { L"_Vector = vector;" , L"_Map = map;", L"_OrderedMap = orderedMap;" }, L"std::vector<double> vector, std::map<std::wstring, std::wstring> map, std::vector<std::pair<int64_t, int64_t>> orderedMap"),
            GetCppClass(L"WorkListObject", { L"_Vector = vector;" , L"_Map = map;", L"_OrderedMap = orderedMap;" }, L"std::vector<std::shared_ptr<LogConfig>> vector, std::map<std::wstring, std::shared_ptr<LogConfig>> map, std::vector<std::pair<int64_t, std::shared_ptr<LogConfig>>> orderedMap"),
            GetCppClass(L"DeleteWorkspace" , {}, L"")
    };

    std::map<std::wstring, std::wstring> classPathMapping;
    classPathMapping.insert(std::make_pair(L"State", L"state.hpp"));
    classPathMapping.insert(std::make_pair(L"LogConfig", L"log_config.hpp"));

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
        "\r\n"
        "//@@Form\r\n"
        "enum class VPGGitFormProperty\r\n"
        "{\r\n"
        "    AddWorkspace // ACTION(AddWorkspace) @@Class { \"Properties\" : [ \"GETSET(std::wstring, Name, L\\\"\\\")\" ]\r\n"
        "    , WorkObject // ACTION(WorkObject) @@Class { \"Properties\" : [ \"GETSET_SPTR_NULL(LogConfig, LogConfig)\" , \"GETSET(State, State, State::Busy)\" ], \"Assignments\": [\"_LogConfig\", \"State::Busy\"] }\r\n"
        "    , WorkListObject // ACTION(WorkListObject) @@Class { \"Properties\" : [ \"VECTOR_SPTR(LogConfig, Vector)\" , \"MAP_SPTR_R(std::wstring, LogConfig, Map)\", \"ORDERED_MAP_SPTR_R(int64_t, LogConfig, OrderedMap)\"], \"Assignments\": [ \"{}\", \"{}\" ]] }\r\n"
        "};\r\n"
        "\r\n";

    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
    VPGGlobal::GetEnumClassReader()->Parse(enumClass, enumClassList);
    std::vector<std::wstring> emptyVector;
    
    std::map<std::wstring, std::wstring> classPathMapping;
    classPathMapping.insert(std::make_pair(L"State", L"state.hpp"));
    classPathMapping.insert(std::make_pair(L"LogConfig", L"log_config.hpp"));

    std::vector<std::wstring> hppClass, cppClass;
    std::set<std::wstring> sytemIncludeFiles;
    std::set<std::wstring> customIncludeFiles;
    VPGActionFileGenerationService::GenerateHpp(nullptr, classPathMapping, enumClassList.at(0).get(), L"VPG", this->GetWorkspace(), hppClass, sytemIncludeFiles, customIncludeFiles);
    VPGActionFileGenerationService::GenerateCpp(nullptr, classPathMapping, enumClassList.at(0).get(), L"VPG", this->GetWorkspace(), cppClass, sytemIncludeFiles, customIncludeFiles);
    EXPECT_EQ(hppClass, emptyVector);
    EXPECT_EQ(cppClass, emptyVector);

    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_add_workspace.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_add_workspace.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_object.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_object.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_list_object.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_list_object.cpp"})));

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
            "#include \"log_config.hpp\"\r\n"
            + customPropertyHeader
            + GetHppClass(L"AddWorkspace", { L"GETSET(std::wstring, Name, L\"\")" }, L"std::wstring name"));
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
            "#include \"log_config.hpp\"\r\n"
            + customPropertyHeader
            + GetCppClass(L"AddWorkspace", { L"_Name = name;" }, L"std::wstring name")
            + customFooter);

    EXPECT_EQ(ReadFile(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_object.hpp"})),
            hppHeader
            + L"#include <memory>\r\n"
            "#include <string>\r\n"
            "\r\n"
            "#include \"base_action.hpp\"\r\n"
            "#include \"i_object.hpp\"\r\n"
            "#include \"log_config.hpp\"\r\n"
            "#include \"state.hpp\"\r\n"
            + customPropertyHeader
            + GetHppClass(L"WorkObject", { L"GETSET_SPTR_NULL(LogConfig, LogConfig)" , L"GETSET(State, State, State::Busy)" }, L"std::shared_ptr<LogConfig> logConfig, State state"));
    EXPECT_EQ(ReadFile(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_object.cpp"})),
            cppHeader
            + L"#include \"vpg_git_form_work_object.hpp\"\r\n"
            "\r\n"
            "#include <memory>\r\n"
            "#include <string>\r\n"
            "\r\n"
            "#include \"base_action.hpp\"\r\n"
            "#include \"exception_macro.hpp\"\r\n"
            "#include \"i_object.hpp\"\r\n"
            "#include \"log_config.hpp\"\r\n"
            "#include \"state.hpp\"\r\n"
            + customPropertyHeader
            + GetCppClass(L"WorkObject", { L"_LogConfig = logConfig;" , L"_State = state;" }, L"std::shared_ptr<LogConfig> logConfig, State state")
            + customFooter);

    EXPECT_EQ(ReadFile(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_list_object.hpp"})),
            hppHeader
            + L"#include <map>\r\n"
            "#include <memory>\r\n"
            "#include <string>\r\n"
            "#include <vector>\r\n"
            "\r\n"
            "#include \"base_action.hpp\"\r\n"
            "#include \"i_object.hpp\"\r\n"
            "#include \"log_config.hpp\"\r\n"
            + customPropertyHeader
            + GetHppClass(L"WorkListObject", { L"VECTOR_SPTR(LogConfig, Vector)" , L"MAP_SPTR_R(std::wstring, LogConfig, Map)", L"ORDERED_MAP_SPTR_R(int64_t, LogConfig, OrderedMap)" }, L"std::vector<std::shared_ptr<LogConfig>> vector, std::map<std::wstring, std::shared_ptr<LogConfig>> map, std::vector<std::pair<int64_t, std::shared_ptr<LogConfig>>> orderedMap"));
    EXPECT_EQ(ReadFile(ConcatPaths({this->GetWorkspace(), L"vpg_git_form_work_list_object.cpp"})),
            cppHeader
            + L"#include \"vpg_git_form_work_list_object.hpp\"\r\n"
            "\r\n"
            "#include <map>\r\n"
            "#include <memory>\r\n"
            "#include <string>\r\n"
            "#include <vector>\r\n"
            "\r\n"
            "#include \"base_action.hpp\"\r\n"
            "#include \"exception_macro.hpp\"\r\n"
            "#include \"i_object.hpp\"\r\n"
            "#include \"log_config.hpp\"\r\n"
            + customPropertyHeader
            + GetCppClass(L"WorkListObject", { L"_Vector = vector;" , L"_Map = map;", L"_OrderedMap = orderedMap;" }, L"std::vector<std::shared_ptr<LogConfig>> vector, std::map<std::wstring, std::shared_ptr<LogConfig>> map, std::vector<std::pair<int64_t, std::shared_ptr<LogConfig>>> orderedMap")
            + customFooter);
}
