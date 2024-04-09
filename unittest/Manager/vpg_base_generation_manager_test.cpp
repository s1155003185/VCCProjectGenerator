#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "log_property.hpp"
#include "memory_macro.hpp"
#include "string_helper.hpp"

#include "vpg_project_type.hpp"
#include "vpg_cpp_generation_manager.hpp"

using namespace vcc;

class VPGBaseGenerationManagerTest : public testing::Test 
{
    GET_SPTR(LogProperty, LogProperty);
    GET(std::wstring, Workspace, L"bin/Debug/VPGBaseGenerationManagerTest/");
    GET(std::wstring, WorkspaceSource, L"");
    GET(std::wstring, WorkspaceTarget, L"");
    
    GET_SPTR(VPGGenerationOption, Option)
    // Cannot use VPGBaseGenerationManager directly as it needs template
    MANAGER(VPGCppGenerationManager, Manager, _LogProperty, _Option);
    
    GETSET(std::wstring, FileContent, L"");
    
    private:
        void CreateFolderInSourceWorkspace(std::wstring folder)
        {
            if (folder.empty())
                CreateDirectory(this->GetWorkspaceSource());
            else
                CreateDirectory(ConcatPaths({this->GetWorkspaceSource(), folder}));
        }

        void CreateFolderInTargetWorkspace(std::wstring folder)
        {
            if (folder.empty())
                CreateDirectory(this->GetWorkspaceTarget());
            else
                CreateDirectory(ConcatPaths({this->GetWorkspaceTarget(), folder}));
        }

        void CreateFileInSourceWorkspace(std::wstring fileName, std::wstring content)
        {
            AppendFileOneLine(ConcatPaths({this->GetWorkspaceSource(), fileName}), content, true);
        }
        void CreateFileInTargetWorkspace(std::wstring fileName, std::wstring content)
        {
            AppendFileOneLine(ConcatPaths({this->GetWorkspaceTarget(), fileName}), content, true);
        }

    public:
        void SetUp() override
        {
            this->_LogProperty->SetIsConsoleLog(false);
            this->_WorkspaceSource = this->_Workspace + L"Source";
            this->_WorkspaceTarget = this->_Workspace + L"Target";
            std::filesystem::remove_all(PATH(this->GetWorkspace()));

            _Option->SetWorkspaceSource(L"A");
            _Option->SetWorkspaceDestination(L"B");

            std::wstring makeFileStr = L"hi\r\n";
            makeFileStr += L"# <vcc:name sync=\"ALERT\">\r\n";
            makeFileStr += L"#----------------------------------#\r\n";
            makeFileStr += L"#---------- Project Name ----------#\r\n";
            makeFileStr += L"#----------------------------------#\r\n";
            makeFileStr += L"PROJ_NAME := VCCProjGenerator\r\n";
            makeFileStr += L"PROJ_NAME_DLL := libvpg\r\n";
            makeFileStr += L"PROJ_NAME_EXE := vpg\r\n";
            makeFileStr += L"PROJ_NAME_GTEST := unittest\r\n";
            makeFileStr += L"# </vcc:name>\r\n";
            makeFileStr += L"HI";
            this->_FileContent = makeFileStr;
        }

        void TearDown() override
        {
            std::filesystem::remove_all(PATH(this->GetWorkspace()));
        }

        std::wstring GetResultStr(const VPGGenerationOption *_Option) {
            std::wstring projName = !IsBlank(_Option->GetProjectName()) ? (L" " + _Option->GetProjectName()) : L"";
            std::wstring dllName = !IsBlank(_Option->GetProjectNameDLL()) ? (L" " + _Option->GetProjectNameDLL()) : L"";
            std::wstring exeName = !IsBlank(_Option->GetProjectNameEXE()) ? (L" " + _Option->GetProjectNameEXE()) : L"";
            std::wstring gtestName = !IsBlank(_Option->GetProjectNameGtest()) ? (L" " + _Option->GetProjectNameGtest()) : L"";
            std::wstring makeFileStr = L"hi\r\n";
            makeFileStr += L"# <vcc:name sync=\"ALERT\">\r\n";
            makeFileStr += L"#----------------------------------#\r\n";
            makeFileStr += L"#---------- Project Name ----------#\r\n";
            makeFileStr += L"#----------------------------------#\r\n";
            makeFileStr += L"PROJ_NAME :=" + projName + L"\r\n";
            makeFileStr += L"PROJ_NAME_DLL :=" + dllName + L"\r\n";
            makeFileStr += L"PROJ_NAME_EXE :=" + exeName + L"\r\n";
            makeFileStr += L"PROJ_NAME_GTEST :=" + gtestName + L"\r\n";
            makeFileStr += L"# </vcc:name>\r\n";
            makeFileStr += L"HI";
            return makeFileStr;
        }


        bool CheckFolderExists(std::wstring path)
        {
            return IsDirectoryExists(ConcatPaths({this->GetWorkspace(), path}));
        }

        void CreateSyncWorkspaceFolderWithExcludeTestCase() 
        {
            this->CreateFolderInSourceWorkspace(L"");
            this->CreateFolderInSourceWorkspace(L"FolderAdd");
            this->CreateFolderInSourceWorkspace(L"FolderUpdate");
            //this->CreateFolderInSourceWorkspace(L"FolderDelete");
            this->CreateFolderInSourceWorkspace(L"FolderAddFilterOut");
            this->CreateFolderInSourceWorkspace(L"FolderUpdateFilterOut");
            //this->CreateFolderInSourceWorkspace(L"FolderDeleteFilterOut");

            this->CreateFolderInTargetWorkspace(L"");
            //this->CreateFolderInTargetWorkspace(L"FolderAdd");
            this->CreateFolderInTargetWorkspace(L"FolderUpdate");
            this->CreateFolderInTargetWorkspace(L"FolderDelete");
            //this->CreateFolderInTargetWorkspace(L"FolderAddFilterOut");
            this->CreateFolderInTargetWorkspace(L"FolderUpdateFilterOut");
            this->CreateFolderInTargetWorkspace(L"FolderDeleteFilterOut");

            std::vector<std::wstring> includeOnly;
            std::vector<std::wstring> excludes;
            excludes.push_back(L"*FilterOut/");
            this->GetManager()->SyncWorkspace(this->GetLogProperty().get(), 
                this->GetWorkspaceSource(), this->GetWorkspaceTarget(),
                includeOnly, excludes);
        }

        void CreateSyncWorkspaceFolderIncludeOnlyTestCase()
        {
            this->CreateFolderInSourceWorkspace(L"");
            this->CreateFolderInSourceWorkspace(L"FolderAdd");
            this->CreateFolderInSourceWorkspace(L"FolderUpdate");
            //this->CreateFolderInSourceWorkspace(L"FolderDelete");
            this->CreateFolderInSourceWorkspace(L"FolderAddIncludeOnly");
            this->CreateFolderInSourceWorkspace(L"FolderUpdateIncludeOnly");
            //this->CreateFolderInSourceWorkspace(L"FolderDeleteIncludeOnly");

            this->CreateFolderInTargetWorkspace(L"");
            //this->CreateFolderInTargetWorkspace(L"FolderAdd");
            this->CreateFolderInTargetWorkspace(L"FolderUpdate");
            this->CreateFolderInTargetWorkspace(L"FolderDelete");
            //this->CreateFolderInTargetWorkspace(L"FolderAddIncludeOnly");
            this->CreateFolderInTargetWorkspace(L"FolderUpdateIncludeOnly");
            this->CreateFolderInTargetWorkspace(L"FolderDeleteIncludeOnly");

            std::vector<std::wstring> includeOnly;
            includeOnly.push_back(L"*IncludeOnly/");
            std::vector<std::wstring> excludes;
            this->GetManager()->SyncWorkspace(this->GetLogProperty().get(), 
                this->GetWorkspaceSource(), this->GetWorkspaceTarget(),
                includeOnly, excludes);
        }

        void CreateSyncWorkspaceFileWithExcludeTestCase() 
        {
            this->CreateFolderInSourceWorkspace(L"");
            this->CreateFileInSourceWorkspace(L"FileAdd", L"FileAdd");
            this->CreateFileInSourceWorkspace(L"FileModify", L"FileModifyFrom");
            //this->CreateFileInSourceWorkspace(L"FileDelete", L"FileDelete");
            this->CreateFileInSourceWorkspace(L"FileAddFilterOut", L"FileAdd");
            this->CreateFileInSourceWorkspace(L"FileModifyFilterOut", L"FileModifyFrom");
            //this->CreateFileInSourceWorkspace(L"FileDeleteFilterOut", L"FileDelete");

            this->CreateFolderInTargetWorkspace(L"");
            //this->CreateFileInTargetWorkspace(L"FileAdd", L"FileAdd");
            this->CreateFileInTargetWorkspace(L"FileModify", L"FileModifyTo");
            this->CreateFileInTargetWorkspace(L"FileDelete", L"FileDelete");
            //this->CreateFileInTargetWorkspace(L"FileAddFilterOut", L"FileAdd");
            this->CreateFileInTargetWorkspace(L"FileModifyFilterOut", L"FileModifyTo");
            this->CreateFileInTargetWorkspace(L"FileDeleteFilterOut", L"FileDelete");


            std::vector<std::wstring> includeOnly;
            std::vector<std::wstring> excludes;
            excludes.push_back(L"*FilterOut");
            this->GetManager()->SyncWorkspace(this->GetLogProperty().get(), 
                this->GetWorkspaceSource(), this->GetWorkspaceTarget(),
                includeOnly, excludes);
        }
        
        void CreateSyncWorkspaceFileIncludeOnlyTestCase()
        {
            this->CreateFolderInSourceWorkspace(L"");
            this->CreateFileInSourceWorkspace(L"FileAdd", L"FileAdd");
            this->CreateFileInSourceWorkspace(L"FileModify", L"FileModifyFrom");
            //this->CreateFileInSourceWorkspace(L"FileDelete", L"FileDelete");
            this->CreateFileInSourceWorkspace(L"FileAddIncludeOnly", L"FileAdd");
            this->CreateFileInSourceWorkspace(L"FileModifyIncludeOnly", L"FileModifyFrom");
            //this->CreateFileInSourceWorkspace(L"FileDeleteIncludeOnly", L"FileDelete");

            this->CreateFolderInTargetWorkspace(L"");
            //this->CreateFileInTargetWorkspace(L"FileAdd", L"FileAdd");
            this->CreateFileInTargetWorkspace(L"FileModify", L"FileModifyTo");
            this->CreateFileInTargetWorkspace(L"FileDelete", L"FileDelete");
            //this->CreateFileInTargetWorkspace(L"FileAddIncludeOnly", L"FileAdd");
            this->CreateFileInTargetWorkspace(L"FileModifyIncludeOnly", L"FileModifyTo");
            this->CreateFileInTargetWorkspace(L"FileDeleteIncludeOnly", L"FileDelete");

            std::vector<std::wstring> includeOnly;
            includeOnly.push_back(L"*IncludeOnly");
            std::vector<std::wstring> excludes;
            this->GetManager()->SyncWorkspace(this->GetLogProperty().get(), 
                this->GetWorkspaceSource(), this->GetWorkspaceTarget(),
                includeOnly, excludes);
        }
};

TEST_F(VPGBaseGenerationManagerTest, DllTestFileContent)
{
    _Option->SetProjectName(L"libvpg");
    _Option->SetProjectNameDLL(L"libvpg");
    _Option->SetProjectNameEXE(L"libvpg");
    _Option->SetProjectNameGtest(L"libvpg");
    std::wstring originalText  = ReadFile(L"unittest/Dll/dll_test.cpp");
    std::wstring result = originalText;
    this->GetManager()->GetDLLTestFileContent(result);
    ReplaceAll(originalText, L"\r\n", L"\n");
    ReplaceAll(result, L"\r\n", L"\n");
    EXPECT_EQ(originalText, result);
}

TEST_F(VPGBaseGenerationManagerTest, Complex)
{
    _Option->SetProjectName(L"ProjectName");
    _Option->SetProjectNameDLL(L"DllName");
    _Option->SetProjectNameEXE(L"ExeName");
    _Option->SetProjectNameGtest(L"GTestName");
    std::wstring result = this->GetManager()->AdjustMakefile(this->GetFileContent());
    EXPECT_EQ(result, GetResultStr(_Option.get()));
}

TEST_F(VPGBaseGenerationManagerTest, DLLOnly)
{
    _Option->SetProjectName(L"ProjectName");
    _Option->SetProjectNameDLL(L"DllName");
    _Option->SetProjectNameEXE(L"");
    _Option->SetProjectNameGtest(L"GTestName");
    std::wstring result = this->GetManager()->AdjustMakefile(this->GetFileContent());
    EXPECT_EQ(result, GetResultStr(_Option.get()));
}

TEST_F(VPGBaseGenerationManagerTest, EXEOnly)
{
    _Option->SetProjectName(L"ProjectName");
    _Option->SetProjectNameDLL(L"");
    _Option->SetProjectNameEXE(L"ExeName");
    _Option->SetProjectNameGtest(L"GTestName");
    std::wstring result = this->GetManager()->AdjustMakefile(this->GetFileContent());
    EXPECT_EQ(result, GetResultStr(_Option.get()));
}

TEST_F(VPGBaseGenerationManagerTest, NoGtest)
{
    _Option->SetProjectName(L"ProjectName");
    _Option->SetProjectNameDLL(L"DllName");
    _Option->SetProjectNameEXE(L"ExeName");
    _Option->SetProjectNameGtest(L"");
    std::wstring result = this->GetManager()->AdjustMakefile(this->GetFileContent());
    EXPECT_EQ(result, GetResultStr(_Option.get()));
}

TEST_F(VPGBaseGenerationManagerTest, SyncWorkspaceFolderWithExclude)
{
    this->CreateSyncWorkspaceFolderWithExcludeTestCase();
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"FolderAdd"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"FolderUpdate"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"FolderDelete"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"FolderAddFilterOut"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"FolderUpdateFilterOut"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"FolderDeleteFilterOut"})));
}

TEST_F(VPGBaseGenerationManagerTest, SyncWorkspaceFolderIncludeOnly)
{
    this->CreateSyncWorkspaceFolderIncludeOnlyTestCase();
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"FolderAdd"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"FolderUpdate"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"FolderDelete"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"FolderAddIncludeOnly"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"FolderUpdateIncludeOnly"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"FolderDeleteIncludeOnly"})));
}

TEST_F(VPGBaseGenerationManagerTest, SyncWorkspaceFileWithExclude)
{
    this->CreateSyncWorkspaceFileWithExcludeTestCase();
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"FileAdd"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"FileModify"})));
    EXPECT_FALSE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"FileDelete"})));
    EXPECT_FALSE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"FileAddFilterOut"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"FileModifyFilterOut"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"FileDeleteFilterOut"})));
}

TEST_F(VPGBaseGenerationManagerTest, SyncWorkspaceFileIncludeOnly)
{
    this->CreateSyncWorkspaceFileIncludeOnlyTestCase();
    EXPECT_FALSE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"FileAdd"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"FileModify"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"FileDelete"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"FileAddIncludeOnly"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"FileModifyIncludeOnly"})));
    EXPECT_FALSE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"FileDeleteIncludeOnly"})));
}
