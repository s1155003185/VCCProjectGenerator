#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "log_property.hpp"
#include "vpg_file_converse_service.hpp"
#include "vpg_project_type.hpp"

using namespace std;
using namespace vcc;

class VPGDirectorySyncServiceTest : public testing::Test 
{
    GETOBJ(LogProperty, LogPropery);
    GET(wstring, Workspace, L"bin/Debug/DirectorySyncServiceTest/");
    GET(wstring, WorkspaceSource, L"bin/Debug/DirectorySyncServiceTestSource");
    GET(wstring, WorkspaceTarget, L"bin/Debug/DirectorySyncServiceTestTarget");

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

    public:

        void SetUp() override
        {
            this->_LogPropery.SetIsConsoleLog(false);
            filesystem::remove_all(PATH(this->GetWorkspace()));
            filesystem::remove_all(PATH(this->GetWorkspaceSource()));
            filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
        }

        void TearDown() override
        {

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
            VPGDirectorySyncService::_SyncWorkspace(*this->GetLogPropery(), 
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
            VPGDirectorySyncService::_SyncWorkspace(*this->GetLogPropery(), 
                this->GetWorkspaceSource(), this->GetWorkspaceTarget(),
                includeOnly, excludes);
        }
};

TEST_F(VPGDirectorySyncServiceTest, CheckAndCreateDirectory)
{
    VPGDirectorySyncService::CheckAndCreateDirectory(*this->GetLogPropery(), this->GetWorkspace());
    // bin
    EXPECT_TRUE(this->CheckFolderExists(L"bin"));
    EXPECT_TRUE(this->CheckFolderExists(L"bin/Debug"));
    EXPECT_TRUE(this->CheckFolderExists(L"bin/Release"));

    // include
    EXPECT_TRUE(this->CheckFolderExists(L"include"));

    // src
    EXPECT_TRUE(this->CheckFolderExists(L"src"));

    // unittest
    EXPECT_TRUE(this->CheckFolderExists(L"unittest"));
}

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
