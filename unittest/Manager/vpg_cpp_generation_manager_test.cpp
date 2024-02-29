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
    GET_SPTR(VPGGenerationOption, Option);
    GET(wstring, Workspace, L"bin/Debug/VPGCppGenerationManagerTest/");
    GET(wstring, WorkspaceSource, L"");
    GET(wstring, WorkspaceTarget, L"");

    MANAGER(VPGCppGenerationManager, Manager, _LogProperty, _Option);

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

            // option for initialize source
            this->_Option->SetProjectType(VPGProjectType::CppComplex);
            this->_Option->SetWorkspaceSource(L".");
            this->_Option->SetWorkspaceDestination(this->GetWorkspaceSource());
            this->_Option->SetProjectName(L"VCCProjGenerator");
            this->_Option->SetProjectNameDLL(L"libvpg");
            this->_Option->SetProjectNameEXE(L"vpg");
            this->_Option->SetProjectNameGtest(L"unittest"); // must be hardcode unittest
            this->_Manager->CreateBasicProject();
            // replace main so that the project can be compile
            std::wstring mainFilePath = ConcatPath(this->GetWorkspaceSource(), L"main.cpp");
            std::wstring mainFileContent = L"int main() {}";
            filesystem::remove(PATH(mainFilePath));
            AppendFileOneLine(mainFilePath, mainFileContent);

            // option for testing
            this->_Option->SetWorkspaceSource(this->GetWorkspaceSource());
            this->_Option->SetWorkspaceDestination(this->GetWorkspaceTarget());
            this->_Option->SetProjectNameEXE(L"CPPProject");
            this->_Option->SetProjectNameDLL(L"CPPDllProject");
            this->_Option->SetProjectName(L"ProjectName");
            this->_Option->SetProjectNameGtest(L"CPPUnitTest");
        }

        void TearDown() override
        {
            filesystem::remove_all(PATH(this->GetWorkspace()));
        }
};

TEST_F(VPGCppGenerationManagerTest, Add)
{
    filesystem::remove_all(PATH(this->GetWorkspaceTarget()));

    // Complex
    this->_Option->SetProjectNameEXE(L"CPPProject");
    this->_Option->SetProjectNameDLL(L"CPPDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetProjectNameGtest(L"CPPUnitTest");
    this->_Option->SetProjectType(VPGProjectType::CppComplex);
    this->GetManager()->Add();
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"Makefile")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllEntryPoint.cpp")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllFunctions.cpp")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllFunctions.h")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"main.cpp")));
    EXPECT_TRUE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"CPPUnitTest")));

    // EXE only
    filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    this->_Option->SetProjectNameEXE(L"CPPProject");
    this->_Option->SetProjectNameDLL(L"");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetProjectNameGtest(L"CPPUnitTest");
    this->GetManager()->Add();
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"Makefile")));
    EXPECT_FALSE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllEntryPoint.cpp")));
    EXPECT_FALSE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllFunctions.cpp")));
    EXPECT_FALSE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllFunctions.h")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"main.cpp")));
    EXPECT_TRUE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"CPPUnitTest")));

    // DLL only
    filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    this->_Option->SetProjectNameEXE(L"");
    this->_Option->SetProjectNameDLL(L"CPPDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetProjectNameGtest(L"CPPUnitTest");
    this->GetManager()->Add();
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"Makefile")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllEntryPoint.cpp")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllFunctions.cpp")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllFunctions.h")));
    EXPECT_FALSE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"main.cpp")));
    EXPECT_TRUE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"CPPUnitTest")));

    // No unittest
    filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    this->_Option->SetProjectNameEXE(L"CPPProject");
    this->_Option->SetProjectNameDLL(L"CPPDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetProjectNameGtest(L"");
    this->GetManager()->Add();
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"Makefile")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllEntryPoint.cpp")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllFunctions.cpp")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllFunctions.h")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"main.cpp")));
    EXPECT_FALSE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"CPPUnitTest")));

}