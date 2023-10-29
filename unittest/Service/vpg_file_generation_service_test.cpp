#include <gtest/gtest.h>

#include <filesystem>
#include <set>
#include <string>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "log_service.hpp"
#include "vpg_file_generation_service.hpp"

using namespace std;

class VPGFileGenerationServiceTest : public testing::Test 
{
    

    GETOBJU(LogProperty, LogProperty, LogPropertyType::None);
    GET(wstring, Workspace, L"bin/Debug/FileGenerationServiceTest");
    GET(wstring, WorkspaceSource, L"bin/Debug/FileGenerationServiceTestSource");
    GET(wstring, WorkspaceTarget, L"bin/Debug/FileGenerationServiceTestTarget");
    
    private:
        void CreateFolderInSourceWorkspace(std::wstring folder)
        {
            if (folder.empty())
                CreateDirectory(this->GetWorkspaceSource());
            else
                CreateDirectory(ConcatPath(this->GetWorkspaceSource(), folder));
        }

        void CreateFolderInTargetWorkspace(std::wstring folder)
        {
            if (folder.empty())
                CreateDirectory(this->GetWorkspaceTarget());
            else
                CreateDirectory(ConcatPath(this->GetWorkspaceTarget(), folder));
        }

        void CreateFileInSourceWorkspace(std::wstring fileName, std::wstring content)
        {
            AppendFileOneLine(ConcatPath(this->GetWorkspaceSource(), fileName), content, true);
        }
        void CreateFileInTargetWorkspace(std::wstring fileName, std::wstring content)
        {
            AppendFileOneLine(ConcatPath(this->GetWorkspaceTarget(), fileName), content, true);
        }
    public:

        void SetUp() override
        {
            filesystem::remove_all(PATH(this->GetWorkspace()));
            filesystem::remove_all(PATH(this->GetWorkspaceSource()));
            filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
            
            std::wstring code =  L"";
            code += L"#pragma once\r\n";
            code += L"\r\n";
            code += L"enum class VCCObjectProperty // Class Command\r\n";
            code += L"{\r\n";
            code += L"    EnumA, // GETSET(std::wstring, EnumA, L\"Default\") \r\n";
            code += L"    EnumB, // GET(int64_t, EnumB, 0) \r\n";
            code += L"    EnumC, // GETOBJ(ClassA, EnumC) \r\n";
            code += L"    EnumD, // GETUPTR(ClassB, EnumD, 1, 2, 3) \r\n";
            code += L"    EnumE\r\n";
            code += L"};";
            this->CreateFileInSourceWorkspace(L"vcc_a_property.hpp", code);
        }

        void TearDown() override
        {

        }

        bool CheckFolderExists(std::wstring path)
        {
            return IsDirectoryExists(ConcatPath(this->GetWorkspace(), path));
        }
};

TEST_F(VPGFileGenerationServiceTest, GetClassMacroList)
{
    std::set<std::wstring> expectedResult = {
        L"GETSET",
        L"GET",
        L"STATICGET",
        L"GETOBJ",
        L"GETUPTR",
        L"GETOBJU",
        L"GETOBJS",
        L"GETSPTR",
        L"VECTOR",
        L"SET",
        L"MAP",
    };

    std::set<std::wstring> results = VPGFileGenerationService::GetClassMacroList(L"");
    EXPECT_EQ(results, expectedResult);
}

TEST_F(VPGFileGenerationServiceTest, GenerateProperty)
{
    VPGFileGenerationService::GernerateProperty(*this->GetLogProperty(), L"VCC", L"", this->GetWorkspaceSource(), 
        this->GetWorkspaceTarget(), this->GetWorkspaceTarget(), this->GetWorkspaceTarget());
    EXPECT_TRUE(true);
}