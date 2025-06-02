#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "log_config.hpp"
#include "string_helper.hpp"

#include "vpg_project_type.hpp"
#include "vpg_cpp_generation_manager.hpp"

class VPGBaseGenerationManagerTest : public testing::Test 
{
    GETSET_SPTR_NULL(vcc::LogConfig, LogConfig);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGBaseGenerationManagerTest/");
    GETSET(std::wstring, WorkspaceSource, L"");
    GETSET(std::wstring, WorkspaceTarget, L"");
    
    GETSET_SPTR_NULL(VPGConfig, Option)
    // Cannot use VPGBaseGenerationManager directly as it needs template
    MANAGER_SPTR_NULL(VPGCppGenerationManager, Manager);
    
    GETSET(std::wstring, FileContent, L"");
    
    private:
        void CreateFolderInSourceWorkspace(std::wstring folder)
        {
            if (folder.empty())
                vcc::CreateDirectory(this->GetWorkspaceSource());
            else
                vcc::CreateDirectory(vcc::ConcatPaths({this->GetWorkspaceSource(), folder}));
        }

        void CreateFolderInTargetWorkspace(std::wstring folder)
        {
            if (folder.empty())
                vcc::CreateDirectory(this->GetWorkspaceTarget());
            else
                vcc::CreateDirectory(vcc::ConcatPaths({this->GetWorkspaceTarget(), folder}));
        }

        void CreateFileInSourceWorkspace(std::wstring fileName, std::wstring content)
        {
            vcc::AppendFileOneLine(vcc::ConcatPaths({this->GetWorkspaceSource(), fileName}), content, true);
        }
        void CreateFileInTargetWorkspace(std::wstring fileName, std::wstring content)
        {
            vcc::AppendFileOneLine(vcc::ConcatPaths({this->GetWorkspaceTarget(), fileName}), content, true);
        }

    public:
        void SetUp() override
        {
            this->_LogConfig = std::make_shared<vcc::LogConfig>();
            this->_LogConfig->SetIsConsoleLog(false);
            this->_WorkspaceSource = this->_Workspace + L"Source";
            this->_WorkspaceTarget = this->_Workspace + L"Target";
            std::filesystem::remove_all(PATH(this->GetWorkspace()));

            _Option = std::make_shared<VPGConfig>();
            _Option->GetTemplate()->SetWorkspace(L"A");
            _Manager = std::make_shared<VPGCppGenerationManager>(_LogConfig, L"", _Option);
            _Manager->SetWorkspace(L"B");

            std::wstring makeFileStr = L"hi\r\n";
            makeFileStr += L"# <vcc:name sync=\"ALERT\" gen=\"ALERT\">\r\n";
            makeFileStr += L"#----------------------------------#\r\n";
            makeFileStr += L"#---------- Project Name ----------#\r\n";
            makeFileStr += L"#----------------------------------#\r\n";
            makeFileStr += L"PROJ_NAME := VCCProjGenerator\r\n";
            makeFileStr += L"PROJ_NAME_DLL := libvpg\r\n";
            makeFileStr += L"PROJ_NAME_EXE := vpg\r\n";
            makeFileStr += L"IS_EXCLUDE_UNITTEST := N\r\n";
            makeFileStr += L"# </vcc:name>\r\n";
            makeFileStr += L"HI";
            this->_FileContent = makeFileStr;
        }

        void TearDown() override
        {
            std::filesystem::remove_all(PATH(this->GetWorkspace()));
        }

        std::wstring GetResultStr(const VPGConfig *_Option) {
            std::wstring projName = !vcc::IsBlank(_Option->GetProjectName()) ? (L" " + _Option->GetProjectName()) : L"";
            std::wstring dllName = !vcc::IsBlank(_Option->GetProjectNameDll()) ? (L" " + _Option->GetProjectNameDll()) : L"";
            std::wstring exeName = !vcc::IsBlank(_Option->GetProjectNameExe()) ? (L" " + _Option->GetProjectNameExe()) : L"";
            std::wstring IsExcludeUnittest = _Option->GetTemplate()->GetIsExcludeUnittest() ? L" Y" : L" N";
            std::wstring makeFileStr = L"hi\r\n";
            makeFileStr += L"# <vcc:name sync=\"ALERT\" gen=\"ALERT\">\r\n";
            makeFileStr += L"#----------------------------------#\r\n";
            makeFileStr += L"#---------- Project Name ----------#\r\n";
            makeFileStr += L"#----------------------------------#\r\n";
            makeFileStr += L"PROJ_NAME :=" + projName + L"\r\n";
            makeFileStr += L"PROJ_NAME_DLL :=" + dllName + L"\r\n";
            makeFileStr += L"PROJ_NAME_EXE :=" + exeName + L"\r\n";
            makeFileStr += L"IS_EXCLUDE_UNITTEST :=" + IsExcludeUnittest + L"\r\n";
            makeFileStr += L"# </vcc:name>\r\n";
            makeFileStr += L"HI";
            return makeFileStr;
        }


        bool CheckFolderExists(std::wstring path)
        {
            return vcc::IsDirectoryExists(vcc::ConcatPaths({this->GetWorkspace(), path}));
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
            this->GetManager()->SyncWorkspace(this->GetLogConfig().get(), 
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
            this->GetManager()->SyncWorkspace(this->GetLogConfig().get(), 
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
            this->GetManager()->SyncWorkspace(this->GetLogConfig().get(), 
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
            this->GetManager()->SyncWorkspace(this->GetLogConfig().get(), 
                this->GetWorkspaceSource(), this->GetWorkspaceTarget(),
                includeOnly, excludes);
        }
};

TEST_F(VPGBaseGenerationManagerTest, DllTestFileContent)
{
    _Option->SetProjectName(L"libvpg");
    _Option->SetProjectNameDll(L"libvpg");
    _Option->SetProjectNameExe(L"libvpg");
    _Option->GetTemplate()->SetIsExcludeUnittest(false);
    std::wstring originalText  = vcc::ReadFile(L"unittest/Dll/dll_test.cpp");
    std::wstring result = originalText;
    this->GetManager()->GetDLLTestFileContent(result);
    vcc::ReplaceAll(originalText, L"\r\n", L"\n");
    vcc::ReplaceAll(result, L"\r\n", L"\n");
    EXPECT_EQ(originalText, result);
}

TEST_F(VPGBaseGenerationManagerTest, AdjustMakefile_Complex)
{
    _Option->SetProjectName(L"ProjectName");
    _Option->SetProjectNameDll(L"DllName");
    _Option->SetProjectNameExe(L"ExeName");
    _Option->GetTemplate()->SetIsExcludeUnittest(false);
    std::wstring result = this->GetManager()->AdjustMakefile(this->GetFileContent());
    EXPECT_EQ(result, GetResultStr(_Option.get()));
}

TEST_F(VPGBaseGenerationManagerTest, AdjustMakefile_DLLOnly)
{
    _Option->SetProjectName(L"ProjectName");
    _Option->SetProjectNameDll(L"DllName");
    _Option->SetProjectNameExe(L"");
    _Option->GetTemplate()->SetIsExcludeUnittest(false);
    std::wstring result = this->GetManager()->AdjustMakefile(this->GetFileContent());
    EXPECT_EQ(result, GetResultStr(_Option.get()));
}

TEST_F(VPGBaseGenerationManagerTest, AdjustMakefile_EXEOnly)
{
    _Option->SetProjectName(L"ProjectName");
    _Option->SetProjectNameDll(L"");
    _Option->SetProjectNameExe(L"ExeName");
    _Option->GetTemplate()->SetIsExcludeUnittest(false);
    std::wstring result = this->GetManager()->AdjustMakefile(this->GetFileContent());
    EXPECT_EQ(result, GetResultStr(_Option.get()));
}

TEST_F(VPGBaseGenerationManagerTest, AdjustMakefile_NoGtest)
{
    _Option->SetProjectName(L"ProjectName");
    _Option->SetProjectNameDll(L"DllName");
    _Option->SetProjectNameExe(L"ExeName");
    _Option->GetTemplate()->SetIsExcludeUnittest(true);
    std::wstring result = this->GetManager()->AdjustMakefile(this->GetFileContent());
    EXPECT_EQ(result, GetResultStr(_Option.get()));
}

TEST_F(VPGBaseGenerationManagerTest, AdjustMakefile_SyncWorkspaceFolderWithExclude)
{
    this->CreateSyncWorkspaceFolderWithExcludeTestCase();
    EXPECT_TRUE(vcc::IsDirectoryExists(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FolderAdd"})));
    EXPECT_TRUE(vcc::IsDirectoryExists(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FolderUpdate"})));
    EXPECT_FALSE(vcc::IsDirectoryExists(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FolderDelete"})));
    EXPECT_FALSE(vcc::IsDirectoryExists(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FolderAddFilterOut"})));
    EXPECT_TRUE(vcc::IsDirectoryExists(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FolderUpdateFilterOut"})));
    EXPECT_TRUE(vcc::IsDirectoryExists(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FolderDeleteFilterOut"})));
}

TEST_F(VPGBaseGenerationManagerTest, AdjustMakefile_SyncWorkspaceFolderIncludeOnly)
{
    this->CreateSyncWorkspaceFolderIncludeOnlyTestCase();
    EXPECT_FALSE(vcc::IsDirectoryExists(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FolderAdd"})));
    EXPECT_TRUE(vcc::IsDirectoryExists(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FolderUpdate"})));
    EXPECT_TRUE(vcc::IsDirectoryExists(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FolderDelete"})));
    EXPECT_TRUE(vcc::IsDirectoryExists(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FolderAddIncludeOnly"})));
    EXPECT_TRUE(vcc::IsDirectoryExists(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FolderUpdateIncludeOnly"})));
    EXPECT_FALSE(vcc::IsDirectoryExists(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FolderDeleteIncludeOnly"})));
}

TEST_F(VPGBaseGenerationManagerTest, AdjustMakefile_SyncWorkspaceFileWithExclude)
{
    this->CreateSyncWorkspaceFileWithExcludeTestCase();
    EXPECT_TRUE(vcc::IsFilePresent(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FileAdd"})));
    EXPECT_TRUE(vcc::IsFilePresent(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FileModify"})));
    EXPECT_FALSE(vcc::IsFilePresent(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FileDelete"})));
    EXPECT_FALSE(vcc::IsFilePresent(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FileAddFilterOut"})));
    EXPECT_TRUE(vcc::IsFilePresent(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FileModifyFilterOut"})));
    EXPECT_TRUE(vcc::IsFilePresent(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FileDeleteFilterOut"})));
}

TEST_F(VPGBaseGenerationManagerTest, AdjustMakefile_SyncWorkspaceFileIncludeOnly)
{
    this->CreateSyncWorkspaceFileIncludeOnlyTestCase();
    EXPECT_FALSE(vcc::IsFilePresent(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FileAdd"})));
    EXPECT_TRUE(vcc::IsFilePresent(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FileModify"})));
    EXPECT_TRUE(vcc::IsFilePresent(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FileDelete"})));
    EXPECT_TRUE(vcc::IsFilePresent(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FileAddIncludeOnly"})));
    EXPECT_TRUE(vcc::IsFilePresent(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FileModifyIncludeOnly"})));
    EXPECT_FALSE(vcc::IsFilePresent(vcc::ConcatPaths({this->GetWorkspaceTarget(), L"FileDeleteIncludeOnly"})));
}
