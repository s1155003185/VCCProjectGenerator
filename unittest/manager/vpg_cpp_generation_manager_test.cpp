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

            this->_LogConfig = std::make_shared<vcc::LogConfig>();
            this->_LogConfig->setIsConsoleLog(false);
            std::filesystem::remove_all(PATH(this->getWorkspace()));
            std::filesystem::remove_all(PATH(this->getWorkspaceSource()));
            std::filesystem::remove_all(PATH(this->getWorkspaceTarget()));

            createDirectory(this->getWorkspace());
            createDirectory(this->getWorkspaceSource());
            createDirectory(this->getWorkspaceTarget());

            // option for initialize source
            _Option = std::make_shared<VPGConfig>();
            this->_Manager = std::make_shared<VPGCppGenerationManager>(_LogConfig, L"", _Option);
            this->_Manager->setWorkspace(this->getWorkspaceSource());
            this->_Option->setProjectType(VPGProjectType::CppComplex);
            this->_Option->getTemplate()->setWorkspace(L".");
            this->_Option->setProjectName(L"VCCProjGenerator");
            this->_Option->setProjectNameDll(L"libvpg");
            this->_Option->setProjectNameExe(L"vpg");
            this->_Option->getTemplate()->setIsExcludeUnittest(false);
            this->_Manager->CreateBasicProject();
            // replace main so that the project can be compile
            std::wstring mainFilePath = concatPaths({this->getWorkspaceSource(), L"main.cpp"});
            std::wstring mainFileContent = L"int main() {}";
            std::filesystem::remove(PATH(mainFilePath));
            appendFileOneLine(mainFilePath, mainFileContent);

            // option for testing
            this->_Manager->setWorkspace(this->getWorkspaceTarget());
            this->_Option->getTemplate()->setWorkspace(this->getWorkspaceSource());
            this->_Option->setProjectNameExe(L"CPPProject");
            this->_Option->setProjectNameDll(L"CPPDllProject");
            this->_Option->setProjectName(L"ProjectName");
            this->_Option->getTemplate()->setIsExcludeUnittest(false);
        }

        void TearDown() override
        {
            std::filesystem::remove_all(PATH(this->getWorkspace()));
        }
};

TEST_F(VPGCppGenerationManagerTest, Add)
{
    // Cannot test make as make process is async
    // Command make process as Window version will hang
    
    // Complex
    std::filesystem::remove_all(PATH(this->getWorkspaceTarget()));
    this->_Option->setProjectNameExe(L"CPPProject");
    this->_Option->setProjectNameDll(L"CPPDllProject");
    this->_Option->setProjectName(L"ProjectName");
    this->_Option->getTemplate()->setIsExcludeUnittest(false);
    this->_Option->setProjectType(VPGProjectType::CppComplex);
    this->getManager()->add();
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L".vscode/launch.json"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L".vscode/tasks.json"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"main.cpp"})));
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"unittest"})));
    // ProcessService::execute(this->getLogConfig().get(), L"", this->getWorkspaceTarget(), L"make");
    // EXPECT_TRUE(IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPProject.exe"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPProject")));
    // EXPECT_TRUE(IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPDllProject.dll"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPDllProject.so")));
    // EXPECT_TRUE(IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPUnitTest.exe"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPUnitTest")));
    if (this->getIsCopyDebugFolderToTestFolder()) {
        std::wstring path = concatPaths({this->getTestFolder(), L"CPPComplex"});
        removeDirectory(path);
        copyDirectory(this->getWorkspaceSource(), path);
    }
    
    // EXE only
    std::filesystem::remove_all(PATH(this->getWorkspaceTarget()));
    this->_Option->setProjectNameExe(L"CPPProject");
    this->_Option->setProjectNameDll(L"");
    this->_Option->setProjectName(L"ProjectName");
    this->_Option->getTemplate()->setIsExcludeUnittest(false);
    this->getManager()->add();
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L".vscode/launch.json"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L".vscode/tasks.json"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"Makefile"})));
    EXPECT_FALSE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_FALSE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_FALSE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"main.cpp"})));
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"unittest"})));
    // ProcessService::execute(this->getLogConfig().get(), L"", this->getWorkspaceTarget(), L"make");
    // EXPECT_TRUE(IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPProject.exe"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPProject")));
    // EXPECT_FALSE(IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPDllProject.dll"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPDllProject.so")));
    // EXPECT_TRUE(IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPUnitTest.exe"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPUnitTest")));
        
    if (this->getIsCopyDebugFolderToTestFolder()) {
        std::wstring path = concatPaths({this->getTestFolder(), L"CPPEXE"});
        removeDirectory(path);
        copyDirectory(this->getWorkspaceSource(), path);
    }

    // DLL only
    std::filesystem::remove_all(PATH(this->getWorkspaceTarget()));
    this->_Option->setProjectNameExe(L"");
    this->_Option->setProjectNameDll(L"CPPDllProject");
    this->_Option->setProjectName(L"ProjectName");
    this->_Option->getTemplate()->setIsExcludeUnittest(false);
    this->getManager()->add();
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L".vscode/launch.json"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L".vscode/tasks.json"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_FALSE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"main.cpp"})));
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"unittest"})));
    // ProcessService::execute(this->getLogConfig().get(), L"", this->getWorkspaceTarget(), L"make");
    // EXPECT_FALSE(IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPProject.exe"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPProject")));
    // EXPECT_TRUE(IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPDllProject.dll"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPDllProject.so")));
    // EXPECT_TRUE(IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPUnitTest.exe"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPUnitTest")));        
        
    if (this->getIsCopyDebugFolderToTestFolder()) {
        std::wstring path = concatPaths({this->getTestFolder(), L"CPPDLL"});
        removeDirectory(path);
        copyDirectory(this->getWorkspaceSource(), path);
    }

    // No unittest
    std::filesystem::remove_all(PATH(this->getWorkspaceTarget()));
    this->_Option->setProjectNameExe(L"CPPProject");
    this->_Option->setProjectNameDll(L"CPPDllProject");
    this->_Option->setProjectName(L"ProjectName");
    this->_Option->getTemplate()->setIsExcludeUnittest(true);
    this->getManager()->add();
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L".vscode/launch.json"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L".vscode/tasks.json"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"main.cpp"})));
    EXPECT_FALSE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"unittest"})));
    // ProcessService::execute(this->getLogConfig().get(), L"", this->getWorkspaceTarget(), L"make");
    // EXPECT_TRUE(IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPProject.exe"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPProject")));
    // EXPECT_TRUE(IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPDllProject.dll"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPDllProject.so")));
    // EXPECT_FALSE(IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPUnitTest.exe"))
    //     || IsFilePresent(ConcatPaths({ConcatPaths({this->getWorkspaceTarget(), L"bin/Debug/"), L"CPPUnitTest")));
        
    if (this->getIsCopyDebugFolderToTestFolder()) {
        std::wstring path = concatPaths({this->getTestFolder(), L"NoUnittest"});
        removeDirectory(path);
        copyDirectory(this->getWorkspaceSource(), path);
    }
}