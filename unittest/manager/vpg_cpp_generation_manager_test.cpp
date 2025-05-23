#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "log_config.hpp"
#include "process_service.hpp"

#include "vpg_cpp_generation_manager.hpp"

using namespace vcc;

class VPGCppGenerationManagerTest : public testing::Test 
{
    GETSET_SPTR_NULL(LogConfig, LogConfig);
    GETSET_SPTR_NULL(VPGConfig, Option);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGCppGenerationManagerTest/");
    GETSET(std::wstring, WorkspaceSource, L"");
    GETSET(std::wstring, WorkspaceTarget, L"");

    GETSET(std::wstring, TestFolder, L"../VPGVccGenerationManagerTest_CPPTestProject");
    GETSET(bool, IsCopyDebugFolderToTestFolder, false);

    MANAGER_SPTR_NULL(VPGCppGenerationManager, Manager);

    public:
        void SetUp() override
        {
            this->_WorkspaceSource = this->_Workspace + L"Source";
            this->_WorkspaceTarget = this->_Workspace + L"Target";

            this->_LogConfig = std::make_shared<LogConfig>();
            this->_LogConfig->SetIsConsoleLog(false);
            std::filesystem::remove_all(PATH(this->GetWorkspace()));
            std::filesystem::remove_all(PATH(this->GetWorkspaceSource()));
            std::filesystem::remove_all(PATH(this->GetWorkspaceTarget()));

            CreateDirectory(this->GetWorkspace());
            CreateDirectory(this->GetWorkspaceSource());
            CreateDirectory(this->GetWorkspaceTarget());

            // option for initialize source
            _Option = std::make_shared<VPGConfig>();
            this->_Manager = std::make_shared<VPGCppGenerationManager>(_LogConfig, L"", _Option);
            this->_Manager->SetWorkspace(this->GetWorkspaceSource());
            this->_Option->SetProjectType(VPGProjectType::CppComplex);
            this->_Option->GetTemplate()->SetWorkspace(L".");
            this->_Option->SetProjectName(L"VCCProjGenerator");
            this->_Option->SetProjectNameDll(L"libvpg");
            this->_Option->SetProjectNameExe(L"vpg");
            this->_Option->GetTemplate()->SetIsExcludeUnittest(false);
            this->_Manager->CreateBasicProject();
            // replace main so that the project can be compile
            std::wstring mainFilePath = ConcatPaths({this->GetWorkspaceSource(), L"main.cpp"});
            std::wstring mainFileContent = L"int main() {}";
            std::filesystem::remove(PATH(mainFilePath));
            AppendFileOneLine(mainFilePath, mainFileContent);

            // option for testing
            this->_Manager->SetWorkspace(this->GetWorkspaceTarget());
            this->_Option->GetTemplate()->SetWorkspace(this->GetWorkspaceSource());
            this->_Option->SetProjectNameExe(L"CPPProject");
            this->_Option->SetProjectNameDll(L"CPPDllProject");
            this->_Option->SetProjectName(L"ProjectName");
            this->_Option->GetTemplate()->SetIsExcludeUnittest(false);
        }

        void TearDown() override
        {
            std::filesystem::remove_all(PATH(this->GetWorkspace()));
        }
};

TEST_F(VPGCppGenerationManagerTest, Add)
{
    // Cannot test make as make process is async
    // Command make process as Window version will hang
    
    // Complex
    std::filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    this->_Option->SetProjectNameExe(L"CPPProject");
    this->_Option->SetProjectNameDll(L"CPPDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->GetTemplate()->SetIsExcludeUnittest(false);
    this->_Option->SetProjectType(VPGProjectType::CppComplex);
    this->GetManager()->Add();
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L".vscode/launch.json"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L".vscode/tasks.json"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest"})));
    // ProcessService::Execute(this->GetLogConfig().get(), L"", this->GetWorkspaceTarget(), L"make");
    // EXPECT_TRUE(IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPProject.exe"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPProject")));
    // EXPECT_TRUE(IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPDllProject.dll"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPDllProject.so")));
    // EXPECT_TRUE(IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPUnitTest.exe"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPUnitTest")));
    if (this->GetIsCopyDebugFolderToTestFolder()) {
        std::wstring path = ConcatPaths({this->GetTestFolder(), L"CPPComplex"});
        RemoveDirectory(path);
        CopyDirectory(this->GetWorkspaceSource(), path);
    }
    
    // EXE only
    std::filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    this->_Option->SetProjectNameExe(L"CPPProject");
    this->_Option->SetProjectNameDll(L"");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->GetTemplate()->SetIsExcludeUnittest(false);
    this->GetManager()->Add();
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L".vscode/launch.json"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L".vscode/tasks.json"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_FALSE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_FALSE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_FALSE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest"})));
    // ProcessService::Execute(this->GetLogConfig().get(), L"", this->GetWorkspaceTarget(), L"make");
    // EXPECT_TRUE(IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPProject.exe"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPProject")));
    // EXPECT_FALSE(IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPDllProject.dll"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPDllProject.so")));
    // EXPECT_TRUE(IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPUnitTest.exe"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPUnitTest")));
        
    if (this->GetIsCopyDebugFolderToTestFolder()) {
        std::wstring path = ConcatPaths({this->GetTestFolder(), L"CPPEXE"});
        RemoveDirectory(path);
        CopyDirectory(this->GetWorkspaceSource(), path);
    }

    // DLL only
    std::filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    this->_Option->SetProjectNameExe(L"");
    this->_Option->SetProjectNameDll(L"CPPDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->GetTemplate()->SetIsExcludeUnittest(false);
    this->GetManager()->Add();
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L".vscode/launch.json"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L".vscode/tasks.json"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_FALSE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest"})));
    // ProcessService::Execute(this->GetLogConfig().get(), L"", this->GetWorkspaceTarget(), L"make");
    // EXPECT_FALSE(IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPProject.exe"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPProject")));
    // EXPECT_TRUE(IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPDllProject.dll"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPDllProject.so")));
    // EXPECT_TRUE(IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPUnitTest.exe"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPUnitTest")));        
        
    if (this->GetIsCopyDebugFolderToTestFolder()) {
        std::wstring path = ConcatPaths({this->GetTestFolder(), L"CPPDLL"});
        RemoveDirectory(path);
        CopyDirectory(this->GetWorkspaceSource(), path);
    }

    // No unittest
    std::filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    this->_Option->SetProjectNameExe(L"CPPProject");
    this->_Option->SetProjectNameDll(L"CPPDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->GetTemplate()->SetIsExcludeUnittest(true);
    this->GetManager()->Add();
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L".vscode/launch.json"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L".vscode/tasks.json"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest"})));
    // ProcessService::Execute(this->GetLogConfig().get(), L"", this->GetWorkspaceTarget(), L"make");
    // EXPECT_TRUE(IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPProject.exe"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPProject")));
    // EXPECT_TRUE(IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPDllProject.dll"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPDllProject.so")));
    // EXPECT_FALSE(IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPUnitTest.exe"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->GetWorkspaceTarget(), L"bin/Debug/"), L"CPPUnitTest")));
        
    if (this->GetIsCopyDebugFolderToTestFolder()) {
        std::wstring path = ConcatPaths({this->GetTestFolder(), L"NoUnittest"});
        RemoveDirectory(path);
        CopyDirectory(this->GetWorkspaceSource(), path);
    }
}