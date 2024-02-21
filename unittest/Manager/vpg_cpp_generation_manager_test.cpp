#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "log_property.hpp"

#include "vpg_cpp_generation_manager.hpp"

using namespace std;
using namespace vcc;

class VPGCppGenerationManagerTest : public testing::Test 
{
    GET_SPTR(LogProperty, LogProperty);
    GET(wstring, Workspace, L"bin/Debug/VPGCppGenerationManagerTest/");
    GET(wstring, WorkspaceSource, L"");
    GET(wstring, WorkspaceTarget, L"");

    MANAGER(VPGCppGenerationManager, Manager, _LogProperty, VPGProjectType::NA);

    public:
        void SetUp() override
        {
            this->_WorkspaceSource = this->_Workspace + L"Source";
            this->_WorkspaceTarget = this->_Workspace + L"Target";

            this->_LogProperty->SetIsConsoleLog(false);
            filesystem::remove_all(PATH(this->GetWorkspace()));
            filesystem::remove_all(PATH(this->GetWorkspaceSource()));
            filesystem::remove_all(PATH(this->GetWorkspaceTarget()));

            CreateDirectory(this->GetWorkspace());
            CreateDirectory(this->GetWorkspaceSource());
            CreateDirectory(this->GetWorkspaceTarget());

            CopyFile(L"Makefile", ConcatPath(this->_WorkspaceTarget, L"Makefile"));
        }

        void TearDown() override
        {

        }
};

TEST_F(VPGCppGenerationManagerTest, Normal)
{
    this->GetManager()->Add(this->GetWorkspaceSource(), this->GetWorkspaceTarget(), nullptr);
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"Makefile")));
}