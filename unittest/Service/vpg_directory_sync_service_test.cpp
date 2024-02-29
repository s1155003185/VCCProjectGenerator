#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "log_property.hpp"
#include "vpg_directory_sync_service.hpp"
#include "vpg_project_type.hpp"

using namespace std;
using namespace vcc;

class VPGDirectorySyncServiceTest : public testing::Test 
{
    GET_SPTR(LogProperty, LogProperty);
    GET(wstring, Workspace, L"bin/Debug/VPGDirectorySyncServiceTest/");
    GET(wstring, WorkspaceSource, L"");
    GET(wstring, WorkspaceTarget, L"");

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
            this->_LogProperty->SetIsConsoleLog(false);
            this->_WorkspaceSource = this->_Workspace + L"Source";
            this->_WorkspaceTarget = this->_Workspace + L"Target";

            filesystem::remove_all(PATH(this->GetWorkspace()));
            filesystem::remove_all(PATH(this->GetWorkspaceSource()));
            filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
        }

        void TearDown() override
        {
            filesystem::remove_all(PATH(this->GetWorkspace()));
        }

        bool CheckFolderExists(std::wstring path)
        {
            return IsDirectoryExists(ConcatPath(this->GetWorkspace(), path));
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
            VPGDirectorySyncService::_SyncWorkspace(this->GetLogProperty().get(), 
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
            VPGDirectorySyncService::_SyncWorkspace(this->GetLogProperty().get(), 
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
            VPGDirectorySyncService::_SyncWorkspace(this->GetLogProperty().get(), 
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
            VPGDirectorySyncService::_SyncWorkspace(this->GetLogProperty().get(), 
                this->GetWorkspaceSource(), this->GetWorkspaceTarget(),
                includeOnly, excludes);
        }
};

TEST_F(VPGDirectorySyncServiceTest, SyncWorkspaceFolderWithExclude)
{
    this->CreateSyncWorkspaceFolderWithExcludeTestCase();
    EXPECT_TRUE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"FolderAdd")));
    EXPECT_TRUE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"FolderUpdate")));
    EXPECT_FALSE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"FolderDelete")));
    EXPECT_FALSE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"FolderAddFilterOut")));
    EXPECT_TRUE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"FolderUpdateFilterOut")));
    EXPECT_TRUE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"FolderDeleteFilterOut")));
}

TEST_F(VPGDirectorySyncServiceTest, SyncWorkspaceFolderIncludeOnly)
{
    this->CreateSyncWorkspaceFolderIncludeOnlyTestCase();
    EXPECT_FALSE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"FolderAdd")));
    EXPECT_TRUE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"FolderUpdate")));
    EXPECT_TRUE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"FolderDelete")));
    EXPECT_TRUE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"FolderAddIncludeOnly")));
    EXPECT_TRUE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"FolderUpdateIncludeOnly")));
    EXPECT_FALSE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"FolderDeleteIncludeOnly")));
}

TEST_F(VPGDirectorySyncServiceTest, SyncWorkspaceFileWithExclude)
{
    this->CreateSyncWorkspaceFileWithExcludeTestCase();
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"FileAdd")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"FileModify")));
    EXPECT_FALSE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"FileDelete")));
    EXPECT_FALSE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"FileAddFilterOut")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"FileModifyFilterOut")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"FileDeleteFilterOut")));
}

TEST_F(VPGDirectorySyncServiceTest, SyncWorkspaceFileIncludeOnly)
{
    this->CreateSyncWorkspaceFileIncludeOnlyTestCase();
    EXPECT_FALSE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"FileAdd")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"FileModify")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"FileDelete")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"FileAddIncludeOnly")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"FileModifyIncludeOnly")));
    EXPECT_FALSE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"FileDeleteIncludeOnly")));
}
