#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "log_property.hpp"

#include "vpg_vcc_generation_manager.hpp"
#include "vpg_directory_sync_service.hpp"

using namespace std;
using namespace vcc;

class VPGVccGenerationManagerTest : public testing::Test 
{
    GET_SPTR(LogProperty, LogProperty);
    GET_SPTR(VPGGenerationOption, Option);
    GET(wstring, Workspace, L"bin/Debug/VPGVccGenerationManagerTest/");
    GET(wstring, WorkspaceSource, L"");
    GET(wstring, WorkspaceTarget, L"");

    MANAGER(VPGVccGenerationManager, Manager, _LogProperty, _Option);

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
            this->_Option->SetProjectType(VPGProjectType::VccComplex);
            this->_Option->SetWorkspaceSource(L".");
            this->_Option->SetWorkspaceDestination(this->GetWorkspaceSource());
            this->_Option->SetProjectName(L"VCCProjGenerator");
            this->_Option->SetProjectNameDLL(L"libvpg");
            this->_Option->SetProjectNameEXE(L"vpg");
            this->_Option->SetProjectNameGtest(L"unittest"); // must be hardcode unittest
            this->_Manager->CreateBasicProject();

            // Cannot use current project as time consumming
            CopyDirectoryOption dirOption;
            dirOption.SetIsRecursive(false);
            dirOption.SetIsForce(true);
            CopyDirectory(L".", this->GetWorkspaceSource(), &dirOption);

            dirOption.SetIsRecursive(true);
            dirOption.SetIsForce(true);
            dirOption.InsertIncludeFileFilters(L".vscode/*");
            dirOption.InsertIncludeFileFilters(L"include/External/VCC/*");
            dirOption.InsertIncludeFileFilters(L"src/External/VCC/*");
            dirOption.InsertIncludeFileFilters(L"unittest/External/VCC/*");
            dirOption.InsertIncludeFileFilters(L"unittest/dllTest/*");
            dirOption.InsertIncludeFileFilters(L"unittest/gtest_main.cpp");
            dirOption.InsertIncludeFileFilters(L"*Makefile");
            dirOption.InsertIncludeFileFilters(L"*README.md");

            dirOption.InsertExcludeFileFilters(L"*.o");

            CopyDirectory(L".", this->GetWorkspaceSource(), &dirOption);

            // option for testing
            this->_Option->SetWorkspaceSource(this->GetWorkspaceSource());
            this->_Option->SetWorkspaceDestination(this->GetWorkspaceTarget());
            this->_Option->SetProjectNameEXE(L"VCCProject");
            this->_Option->SetProjectNameDLL(L"VCCDllProject");
            this->_Option->SetProjectName(L"ProjectName");
            this->_Option->SetProjectNameGtest(L"VCCUnitTest");
        }

        void TearDown() override
        {
            // ensure compile main project will not compile this project
            filesystem::remove_all(PATH(this->GetWorkspace()));
        }
};

TEST_F(VPGVccGenerationManagerTest, Add)
{
    // Complex
    this->_Option->SetProjectNameEXE(L"VCCProject");
    this->_Option->SetProjectNameDLL(L"VCCDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetProjectNameGtest(L"VCCUnitTest");
    this->GetManager()->Add();
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"Makefile")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllEntryPoint.cpp")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllFunctions.cpp")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllFunctions.h")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"main.cpp")));
    EXPECT_TRUE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"VCCUnitTest")));
    EXPECT_TRUE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"include/External/VCC/Core")));
    EXPECT_FALSE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"include/External/VCC/Version/Git")));

    // EXE only
    filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    this->_Option->SetProjectNameEXE(L"VCCProject");
    this->_Option->SetProjectNameDLL(L"");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetProjectNameGtest(L"VCCUnitTest");
    this->GetManager()->Add();
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"Makefile")));
    EXPECT_FALSE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllEntryPoint.cpp")));
    EXPECT_FALSE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllFunctions.cpp")));
    EXPECT_FALSE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllFunctions.h")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"main.cpp")));
    EXPECT_TRUE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"VCCUnitTest")));
    EXPECT_TRUE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"include/External/VCC/Core")));
    EXPECT_FALSE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"include/External/VCC/Version/Git")));

    // DLL only
    filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    this->_Option->SetProjectNameEXE(L"");
    this->_Option->SetProjectNameDLL(L"VCCDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetProjectNameGtest(L"VCCUnitTest");
    this->GetManager()->Add();
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"Makefile")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllEntryPoint.cpp")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllFunctions.cpp")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllFunctions.h")));
    EXPECT_FALSE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"main.cpp")));
    EXPECT_TRUE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"VCCUnitTest")));
    EXPECT_TRUE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"include/External/VCC/Core")));
    EXPECT_FALSE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"include/External/VCC/Version/Git")));

    // No unittest
    filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    this->_Option->SetProjectNameEXE(L"VCCProject");
    this->_Option->SetProjectNameDLL(L"VCCDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetProjectNameGtest(L"");
    this->GetManager()->Add();
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"Makefile")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllEntryPoint.cpp")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllFunctions.cpp")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllFunctions.h")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"main.cpp")));
    EXPECT_FALSE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"VCCUnitTest")));
    EXPECT_TRUE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"include/External/VCC/Core")));
    EXPECT_FALSE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"include/External/VCC/Version/Git")));

    // Plugins
    filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    this->_Option->SetProjectNameEXE(L"VCCProject");
    this->_Option->SetProjectNameDLL(L"VCCDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetProjectNameGtest(L"VCCUnitTest");
    this->_Option->InsertPlugins(L"VCC/Version/Git");
    //this->_Option
    this->GetManager()->Add();
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"Makefile")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllEntryPoint.cpp")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllFunctions.cpp")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"DllFunctions.h")));
    EXPECT_TRUE(IsFileExists(ConcatPath(this->GetWorkspaceTarget(), L"main.cpp")));
    EXPECT_TRUE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"VCCUnitTest")));
    EXPECT_TRUE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"include/External/VCC/Core")));
    EXPECT_FALSE(IsDirectoryExists(ConcatPath(this->GetWorkspaceTarget(), L"include/External/VCC/Version/Git")));
}

TEST_F(VPGVccGenerationManagerTest, Update)
{
    this->GetManager()->Update();
}

TEST_F(VPGVccGenerationManagerTest, Generate)
{
    this->GetManager()->Generate();
}