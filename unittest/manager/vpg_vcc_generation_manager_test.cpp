#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "json.hpp"
#include "json_builder.hpp"
#include "log_config.hpp"
#include "process_service.hpp"
#include "vpg_vcc_generation_manager.hpp"

using namespace vcc;

class VPGVccGenerationManagerTest : public testing::Test 
{
    GETSET_SPTR_NULL(LogConfig, LogConfig);
    GETSET_SPTR_NULL(VPGConfig, Option);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGVccGenerationManagerTest/");
    GETSET(std::wstring, WorkspaceSource, L"");
    GETSET(std::wstring, WorkspaceTarget, L"");
    
    GETSET(std::wstring, TestFolder, L"../VPGVccGenerationManagerTest_VCCTestProject");
    GETSET(bool, IsCopyDebugFolderToTestFolder, false);

    MANAGER_SPTR_NULL(VPGVccGenerationManager, Manager);

    public:
        void SetUp() override
        {
            this->_WorkspaceSource = this->_Workspace + L"Source";
            this->_WorkspaceTarget = this->_Workspace + L"Target";

            this->_LogConfig = std::make_shared<vcc::LogConfig>();
            this->_LogConfig->setIsConsoleLog(false);
            std::filesystem::remove_all(PATH(this->getWorkspace()));

            createDirectory(this->getWorkspace());
            createDirectory(this->getWorkspaceSource());
            createDirectory(this->getWorkspaceTarget());

            _Option = std::make_shared<VPGConfig>();
            // option for initialize source
            this->_Manager = std::make_shared<VPGVccGenerationManager>(_LogConfig, L"", _Option);
            this->_Manager->setWorkspace(this->getWorkspaceSource());
            this->_Option->setProjectType(VPGProjectType::VccComplex);
            this->_Option->getTemplate()->setWorkspace(L".");
            this->_Option->setProjectName(L"VCCProjGenerator");
            this->_Option->setProjectNameDll(L"libvpg");
            this->_Option->setProjectNameExe(L"vpg");
            this->_Option->getTemplate()->setIsExcludeUnittest(false);
            this->_Manager->CreateBasicProject();

            // Cannot use current project as time consumming
            CopyDirectoryOption dirOption;
            dirOption.setIsRecursive(false);
            dirOption.setIsForce(true);
            copyDirectory(L".", this->getWorkspaceSource(), &dirOption);

            dirOption.setIsRecursive(true);
            dirOption.setIsForce(true);
            dirOption.insertIncludeFileFilters(L".vscode/*");
            dirOption.insertIncludeFileFilters(L"include/application.hpp");
            dirOption.insertIncludeFileFilters(L"include/external/vcc/*");
            dirOption.insertIncludeFileFilters(L"include/factory/*.hpp");
            dirOption.insertIncludeFileFilters(L"include/type/*_type.hpp");
            dirOption.insertIncludeFileFilters(L"src/application.cpp");
            dirOption.insertIncludeFileFilters(L"src/factory/*.cpp");
            dirOption.insertIncludeFileFilters(L"src/external/vcc/*");
            dirOption.insertIncludeFileFilters(L"unittest/external/vcc/*");
            dirOption.insertIncludeFileFilters(L"unittest/dllTest/*");
            dirOption.insertIncludeFileFilters(L"unittest/gtest_main.cpp");
            dirOption.insertIncludeFileFilters(L"*Makefile");
            dirOption.insertIncludeFileFilters(L"*README.md");

            dirOption.insertExcludeFileFilters(L"*.o");

            copyDirectory(L".", this->getWorkspaceSource(), &dirOption);

            // replace main so that the project can be compile
            std::wstring mainFilePath = concatPaths({this->getWorkspaceSource(), L"main.cpp"});
            std::wstring mainFileContent = L"int main() {}";
            std::filesystem::remove(PATH(mainFilePath));
            appendFileOneLine(mainFilePath, mainFileContent);
            
            // option for testing
            this->_Manager->setWorkspace(this->getWorkspaceTarget());
            this->_Option->getTemplate()->setWorkspace(this->getWorkspaceSource());
            this->_Option->setIsGit(true);
            this->_Option->setProjectNameExe(L"VCCProject");
            this->_Option->setProjectNameDll(L"VCCDllProject");
            this->_Option->setProjectName(L"ProjectName");
            this->_Option->getTemplate()->setIsExcludeUnittest(false);
        }

        void TearDown() override
        {
            // ensure compile main project will not compile this project
            std::filesystem::remove_all(PATH(this->getWorkspace()));
        }
};

TEST_F(VPGVccGenerationManagerTest, Add)
{
    // Complex
    this->_Option->setProjectNameExe(L"VCCProject");
    this->_Option->setProjectNameDll(L"VCCDllProject");
    this->_Option->setProjectName(L"ProjectName");
    this->_Option->getTemplate()->setIsExcludeUnittest(false);
    this->_Option->getTemplate()->setIsExcludeVCCUnitTest(false);
    this->getManager()->add();
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"main.cpp"})));

    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/application.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/type/exception_type.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/factory/object_factory.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/factory/property_accessor_factory.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/application.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/factory/object_factory.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/factory/property_accessor_factory.cpp"})));
    
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"include/external/vcc/core"})));
    EXPECT_FALSE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"include/external/vcc/versioning/Git"})));
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"unittest/external/vcc/core"})));
        
    if (this->getIsCopyDebugFolderToTestFolder()) {
        std::wstring path = concatPaths({this->getTestFolder(), L"VCCComplex"});
        removeDirectory(path);
        copyDirectory(this->getWorkspaceTarget(), path);        
    }

    // EXE only
    std::filesystem::remove_all(PATH(this->getWorkspaceTarget()));
    this->_Option->setProjectNameExe(L"VCCProject");
    this->_Option->setProjectNameDll(L"");
    this->_Option->setProjectName(L"ProjectName");
    this->_Option->getTemplate()->setIsExcludeUnittest(false);
    this->_Option->getTemplate()->setIsExcludeVCCUnitTest(false);
    this->getManager()->add();
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"Makefile"})));
    EXPECT_FALSE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_FALSE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_FALSE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"main.cpp"})));

    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/application.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/type/exception_type.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/factory/object_factory.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/factory/property_accessor_factory.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/application.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/factory/object_factory.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/factory/property_accessor_factory.cpp"})));
    
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"include/external/vcc/core"})));
    EXPECT_FALSE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"include/external/vcc/versioning/Git"})));
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"unittest/external/vcc/core"})));

    if (this->getIsCopyDebugFolderToTestFolder()) {
        std::wstring path = concatPaths({this->getTestFolder(), L"VCCEXE"});
        removeDirectory(path);
        copyDirectory(this->getWorkspaceTarget(), path);        
    }

    // DLL only
    std::filesystem::remove_all(PATH(this->getWorkspaceTarget()));
    this->_Option->setProjectNameExe(L"");
    this->_Option->setProjectNameDll(L"VCCDllProject");
    this->_Option->setProjectName(L"ProjectName");
    this->_Option->getTemplate()->setIsExcludeUnittest(false);
    this->_Option->getTemplate()->setIsExcludeVCCUnitTest(false);
    this->getManager()->add();
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_FALSE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"main.cpp"})));

    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/application.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/type/exception_type.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/factory/object_factory.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/factory/property_accessor_factory.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/application.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/factory/object_factory.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/factory/property_accessor_factory.cpp"})));
    
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"include/external/vcc/core"})));
    EXPECT_FALSE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"include/external/vcc/versioning/Git"})));
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"unittest/external/vcc/core"})));

    if (this->getIsCopyDebugFolderToTestFolder()) {
        std::wstring path = concatPaths({this->getTestFolder(), L"VCCDLL"});
        removeDirectory(path);
        copyDirectory(this->getWorkspaceTarget(), path);        
    }

    // No unittest
    std::filesystem::remove_all(PATH(this->getWorkspaceTarget()));
    this->_Option->setProjectNameExe(L"VCCProject");
    this->_Option->setProjectNameDll(L"VCCDllProject");
    this->_Option->setProjectName(L"ProjectName");
    this->_Option->getTemplate()->setIsExcludeUnittest(true);
    this->_Option->getTemplate()->setIsExcludeVCCUnitTest(false);
    this->getManager()->add();
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"main.cpp"})));
    
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/application.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/type/exception_type.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/factory/object_factory.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/factory/property_accessor_factory.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/application.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/factory/object_factory.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/factory/property_accessor_factory.cpp"})));
    
    EXPECT_FALSE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"include/external/vcc/core"})));
    EXPECT_FALSE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"include/external/vcc/versioning/Git"})));
    EXPECT_FALSE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"unittest/external/vcc/core"})));

    if (this->getIsCopyDebugFolderToTestFolder()) {
        std::wstring path = concatPaths({this->getTestFolder(), L"NoUnittest"});
        removeDirectory(path);
        copyDirectory(this->getWorkspaceTarget(), path);        
    }

    // Exclude VCC Unittest
    std::filesystem::remove_all(PATH(this->getWorkspaceTarget()));
    this->_Option->setProjectNameExe(L"VCCProject");
    this->_Option->setProjectNameDll(L"VCCDllProject");
    this->_Option->setProjectName(L"ProjectName");
    this->_Option->getTemplate()->setIsExcludeUnittest(false);
    this->_Option->getTemplate()->setIsExcludeVCCUnitTest(true);
    this->getManager()->add();
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"main.cpp"})));
    
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/application.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/type/exception_type.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/factory/object_factory.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/factory/property_accessor_factory.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/application.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/factory/object_factory.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/factory/property_accessor_factory.cpp"})));
    
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"include/external/vcc/core"})));
    EXPECT_FALSE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"include/external/vcc/versioning/Git"})));
    EXPECT_FALSE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"unittest/external/vcc/core"})));

    if (this->getIsCopyDebugFolderToTestFolder()) {
        std::wstring path = concatPaths({this->getTestFolder(), L"NoVCCUnittest"});
        removeDirectory(path);
        copyDirectory(this->getWorkspaceTarget(), path);        
    }

    // Plugins
    std::filesystem::remove_all(PATH(this->getWorkspaceTarget()));
    this->_Option->setProjectNameExe(L"VCCProject");
    this->_Option->setProjectNameDll(L"VCCDllProject");
    this->_Option->setProjectName(L"ProjectName");
    this->_Option->getTemplate()->setIsExcludeUnittest(false);
    this->_Option->insertPlugins(L"vcc/versioning/git");
    this->_Option->getTemplate()->setIsExcludeVCCUnitTest(false);
    this->getManager()->add();
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"main.cpp"})));
    
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/application.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/type/exception_type.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/factory/object_factory.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/factory/property_accessor_factory.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/application.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/factory/object_factory.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/factory/property_accessor_factory.cpp"})));
    
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"include/external/vcc/core"})));
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"include/external/vcc/versioning/git"})));
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"unittest/external/vcc/core"})));

    if (this->getIsCopyDebugFolderToTestFolder()) {
        std::wstring path = concatPaths({this->getTestFolder(), L"WithPlugins"});
        removeDirectory(path);
        copyDirectory(this->getWorkspaceTarget(), path);        
    }
}

TEST_F(VPGVccGenerationManagerTest, Update)
{
    // cannot test really update case as cannot modify original project git resposne

    // no plugin to have plugin
    this->_Option->setProjectNameExe(L"VCCProject");
    this->_Option->setProjectNameDll(L"VCCDllProject");
    this->_Option->setProjectName(L"ProjectName");
    this->_Option->getTemplate()->setIsExcludeUnittest(false);
    this->getManager()->add();
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"main.cpp"})));
    
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/application.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/type/exception_type.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/factory/object_factory.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/factory/property_accessor_factory.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/application.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/factory/object_factory.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/factory/property_accessor_factory.cpp"})));
    
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"include/external/vcc/core"})));
    EXPECT_FALSE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"include/external/vcc/versioning/git"})));

    this->_Option->insertPlugins(L"vcc/versioning/git");
    auto jsonBuilder = std::make_unique<vcc::JsonBuilder>();
    jsonBuilder->setIsBeautify(true);
    writeFile(concatPaths({this->getWorkspaceTarget(), L".vcc/vcc.json"}), jsonBuilder->serialize(this->_Option->ToJson().get()));

    this->getManager()->update();
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"main.cpp"})));
    
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/application.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/type/exception_type.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/factory/object_factory.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/factory/property_accessor_factory.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/application.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/factory/object_factory.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/factory/property_accessor_factory.cpp"})));
    
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"include/external/vcc/core"})));
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"include/external/vcc/versioning/git"})));

    if (this->getIsCopyDebugFolderToTestFolder()) {
        std::wstring path = concatPaths({this->getTestFolder(), L"update"});
        removeDirectory(path);
        copyDirectory(this->getWorkspaceTarget(), path);
    }
}

TEST_F(VPGVccGenerationManagerTest, Update_Makefile)
{
    // cannot test really update case as cannot modify original project git resposne

    // no plugin to have plugin
    this->_Option->setProjectNameExe(L"VCCProject");
    this->_Option->setProjectNameDll(L"VCCDllProject");
    this->_Option->setProjectName(L"ProjectName");
    this->_Option->getTemplate()->setIsExcludeUnittest(false);
    this->getManager()->add();
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"main.cpp"})));
    
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/application.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/type/exception_type.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/factory/object_factory.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/factory/property_accessor_factory.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/application.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/factory/object_factory.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/factory/property_accessor_factory.cpp"})));
    
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"include/external/vcc/core"})));
    EXPECT_FALSE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"include/external/vcc/versioning/Git"})));

    // Update project name
    std::wstring projectName = L"NewProjectName";
    std::wstring projectNameDll = L"NewProjectNameDll";
    std::wstring projectNameExe = L"NewProjectNameExe";
    std::wstring projectNameGtest = L"unittest";

    auto jsonBuilder = std::make_unique<vcc::JsonBuilder>();
    jsonBuilder->setIsBeautify(true);

    std::wstring vccJsonFileContent = readFile(concatPaths({this->getWorkspaceTarget(), L".vcc/vcc.json"}));
    auto json = std::make_shared<vcc::Json>();
    jsonBuilder->deserialize(vccJsonFileContent, json);
    json->setString(L"ProjectName", projectName);
    json->setString(L"ProjectNameDll", projectNameDll);
    json->setString(L"ProjectNameExe", projectNameExe);
    json->getObject(L"Template")->setBool(L"IsExcludeUnittest", false);
    writeFile(concatPaths({this->getWorkspaceTarget(), L".vcc/vcc.json"}), jsonBuilder->serialize(json.get()));

    this->getManager()->update();
    std::wstring makefileFileContent = readFile(concatPaths({this->getWorkspaceTarget(), L"Makefile"}));
    EXPECT_TRUE(countSubstring(makefileFileContent, L"PROJ_NAME := NewProjectName") == 1);
    EXPECT_TRUE(countSubstring(makefileFileContent, L"PROJ_NAME_DLL := NewProjectNameDll") == 1);
    EXPECT_TRUE(countSubstring(makefileFileContent, L"PROJ_NAME_EXE := NewProjectNameExe") == 1);
    EXPECT_TRUE(countSubstring(makefileFileContent, L"IS_EXCLUDE_UNITTEST := N") == 1);
    EXPECT_TRUE(isDirectoryExists(concatPaths({this->getWorkspaceTarget(), L"unittest"})));

    if (this->getIsCopyDebugFolderToTestFolder()) {
        std::wstring path = concatPaths({this->getTestFolder(), L"Update_Makefile"});
        removeDirectory(path);
        copyDirectory(this->getWorkspaceTarget(), path);
    }
}

TEST_F(VPGVccGenerationManagerTest, Generate)
{
    // Create file for generation
    this->_Option->setProjectNameExe(L"VCCProject");
    this->_Option->setProjectNameDll(L"VCCDllProject");
    this->_Option->setProjectName(L"ProjectName");
    this->_Option->getTemplate()->setIsExcludeUnittest(false);
    this->getManager()->add();

    // Empty class
    std::wstring codeEmpty =  L""
    L"#pragma once\r\n"
    L"\r\n"
    L"enum class VCCObjectEmptyProperty\r\n"
    L"{\r\n"
    L"};\r\n"
    L"\r\n";
    appendFileOneLine(concatPaths({this->getWorkspaceTarget(), L"include/type/ClassA", L"vcc_object_empty_property.hpp"}), codeEmpty, true);

    // Dependent class
    std::wstring codeDependent =  L""
    "#pragma once\r\n"
    "\r\n"
    "enum class VCCObjectDependentProperty\r\n"
    "{\r\n"
    "    EnumA // GETSET_SPTR(VCCObjectEmpty, EnumA) \r\n"
    "};\r\n";
    appendFileOneLine(concatPaths({this->getWorkspaceTarget(), L"include/type/ClassA/Dependent", L"vcc_object_dependent_property.hpp"}), codeDependent, true);
    
    // copied from TEST_F(VPGPropertyAccessorFileGenerationServiceTest, Single)
    std::wstring codeSingle =  L""
    "#pragma once\r\n"
    "\r\n"
    "enum class VCCObjectSingleProperty\r\n"
    "{\r\n"
    "    Bool, // GETSET(bool, Bool, false) \r\n"
    "    String, // GETSET(std::wstring, String, L\"\") \r\n"
    "    Enum // GETSET(ExceptionType, Enum, ExceptionType::NoError) \r\n"
    "};\r\n";
    appendFileOneLine(concatPaths({this->getWorkspaceTarget(), L"include/type/ClassA", L"vcc_object_single_property.hpp"}), codeSingle, true);

    // copied from TEST_F(VPGPropertyAccessorFileGenerationServiceTest, NoAccess)
    std::wstring codeNoAccess =  L""
    "#pragma once\r\n"
    "\r\n"
    "enum class VCCObjectNoAccessProperty\r\n"
    "{\r\n"
    "    ReadWrite, // GETSET(bool, ReadWrite, false) @@NoAccess \r\n"
    "    Read, // GETSET(bool, Read, false) @@NoAccess \r\n"
    "    Write, // GETSET(bool, Write, false) @@NoAccess \r\n"
    "    NoAccess // GETSET(bool, NoAccess, false) @@NoAccess \r\n"
    "};\r\n";
    appendFileOneLine(concatPaths({this->getWorkspaceTarget(), L"include/type/ClassA", L"vcc_object_no_access_property.hpp"}), codeNoAccess, true);
 
    // copied from TEST_F(VPGPropertyAccessorFileGenerationServiceTest, AccessMode_Normal)
    std::wstring codeNoAccessModeNormal =  L""
    "#pragma once\r\n"
    "\r\n"
    "enum class VCCObjectAccessModeNormalProperty\r\n"
    "{\r\n"
    "    ReadWrite, // GETSET(bool, ReadWrite, false) @@ReadWrite\r\n"
    "    Read, // GETSET(bool, Read, false) @@Read \r\n"
    "    Write, // GETSET(bool, Write, false) @@Write \r\n"
    "    NoAccess // GETSET(bool, NoAccess, false) @@NoAccess \r\n"
    "};\r\n";
    appendFileOneLine(concatPaths({this->getWorkspaceTarget(), L"include/type/ClassA", L"vcc_object_access_mode_normal_property.hpp"}), codeNoAccessModeNormal, true);
 
    // copied from TEST_F(VPGPropertyAccessorFileGenerationServiceTest, AccessMode_Vector)
    std::wstring codeNoAccessVector =  L""
    "#pragma once\r\n"
    "\r\n"
    "enum class VCCObjectAccessModeVectorProperty\r\n"
    "{\r\n"
    "    ReadWrite, // VECTOR(bool, ReadWrite) @@ReadWrite\r\n"
    "    Read, // VECTOR(bool, Read) @@Read \r\n"
    "    Write, // VECTOR(bool, Write) @@Write \r\n"
    "    NoAccess // VECTOR(bool, NoAccess) @@NoAccess \r\n"
    "};\r\n";
    appendFileOneLine(concatPaths({this->getWorkspaceTarget(), L"include/type/ClassA", L"vcc_object_access_mode_vector_property.hpp"}), codeNoAccessVector, true);
 
    // copied from TEST_F(VPGPropertyAccessorFileGenerationServiceTest, Multi)
    std::wstring codeMulti =  L""
    "#pragma once\r\n"
    "\r\n"
    "enum class VCCObjectMultiAProperty\r\n"
    "{\r\n"
    "    Bool // GETSET(bool, Bool, false)\r\n"
    "};\r\n"
    "\r\n"
    "enum class VCCObjectMultiBProperty\r\n"
    "{\r\n"
    "    Bool // GETSET(bool, Bool, false)\r\n"
    "};\r\n";
    appendFileOneLine(concatPaths({this->getWorkspaceTarget(), L"include/type/ClassA", L"vcc_object_multi_property.hpp"}), codeMulti, true);
 
    // copied from TEST_F(VPGPropertyAccessorFileGenerationServiceTest, Object)
    std::wstring codeObject =  L""
    "#pragma once\r\n"
    "\r\n"
    "enum class VCCObjectObjectProperty\r\n"
    "{\r\n"
    "    Object // GETSET_SPTR_NULL(VCCObjectEmpty, Object)\r\n"
    "};\r\n";
    appendFileOneLine(concatPaths({this->getWorkspaceTarget(), L"include/type/ClassA", L"vcc_object_object_property.hpp"}), codeObject, true);
 
    // copied from TEST_F(VPGPropertyAccessorFileGenerationServiceTest, Container)
    std::wstring codeContainer =  L""
    "#pragma once\r\n"
    "\r\n"
    "enum class VCCObjectContainerProperty\r\n"
    "{\r\n"
    "    VectorInt, // VECTOR(int, VectorInt)\r\n"
    "    VectorEnum, // VECTOR(ExceptionType, VectorEnum)\r\n"
    "    VectorObj, // VECTOR_SPTR(VCCObjectEmpty, VectorObj)\r\n"
    "    MapInt, // MAP(int, int, MapInt)\r\n"
    "    MapEnum, // MAP(char, ExceptionType, MapEnum)\r\n"
    "    MapObj // MAP_SPTR(double, VCCObjectEmpty, MapObj)\r\n"
    "};\r\n";
    appendFileOneLine(concatPaths({this->getWorkspaceTarget(), L"include/type/ClassA", L"vcc_object_container_property.hpp"}), codeContainer, true);
 
    // copied from TEST_F(VPGPropertyAccessorFileGenerationServiceTest, Mix)
    std::wstring codeMIx =  L""
    "#pragma once\r\n"
    "\r\n"
    "enum class VCCObjectMixProperty\r\n"
    "{\r\n"
    "    Bool, // GETSET(bool, Bool, false) \r\n"
    "    String, // GETSET(std::wstring, String, L\"\") \r\n"
    "    Object, // GETSET_SPTR_NULL(VCCObjectEmpty, Object) \r\n"
    "    Vector, // VECTOR(std::wstring, Vector) \r\n"
    "    Map // MAP(int, double, Map) \r\n"
    "};\r\n";
    appendFileOneLine(concatPaths({this->getWorkspaceTarget(), L"include/type/ClassA", L"vcc_object_mix_property.hpp"}), codeMIx, true);

    std::wstring codeJson =  L""
    "#pragma once\r\n"
    "\r\n"
    "// @@Json\r\n"
    "enum class VCCObjectJsonProperty\r\n"
    "{\r\n"
    "    Bool, // GETSET(bool, Bool, false) \r\n"
    "    String, // GETSET(std::wstring, String, L\"\") \r\n"
    "    Object, // GETSET_SPTR_NULL(VCCObjectJson, Object) \r\n"
    "    Vector, // VECTOR(std::wstring, Vector) \r\n"
    "    Map // MAP(int, double, Map) \r\n"
    "};\r\n";
    appendFileOneLine(concatPaths({this->getWorkspaceTarget(), L"include/type/ClassA", L"vcc_object_json_property.hpp"}), codeJson, true);

    this->_Option->getTemplate()->setWorkspace(this->getWorkspaceSource());
    this->_Option->setProjectPrefix(L"VCC");
    this->getManager()->setWorkspace(this->getWorkspaceTarget());
    this->getManager()->generate();
    
    // Empty Class
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/model/ClassA", L"vcc_object_empty.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/propertyAccessor/ClassA", L"vcc_object_empty_property_accessor.hpp"})));
    EXPECT_FALSE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/propertyAccessor/ClassA", L"vcc_object_empty_property_accessor.cpp"})));

    // Dependent Class
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/model/ClassA/Dependent", L"vcc_object_dependent.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/propertyAccessor/ClassA/Dependent", L"vcc_object_dependent_property_accessor.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/propertyAccessor/ClassA/Dependent", L"vcc_object_dependent_property_accessor.cpp"})));

    // Single Class
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/model/ClassA", L"vcc_object_single.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/propertyAccessor/ClassA", L"vcc_object_single_property_accessor.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/propertyAccessor/ClassA", L"vcc_object_single_property_accessor.cpp"})));

    // NoAccess Class
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/model/ClassA", L"vcc_object_no_access.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/propertyAccessor/ClassA", L"vcc_object_no_access_property_accessor.hpp"})));
    EXPECT_FALSE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/propertyAccessor/ClassA", L"vcc_object_no_access_property_accessor.cpp"})));

    // Access Normal Class
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/model/ClassA", L"vcc_object_access_mode_normal.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/propertyAccessor/ClassA", L"vcc_object_access_mode_normal_property_accessor.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/propertyAccessor/ClassA", L"vcc_object_access_mode_normal_property_accessor.cpp"})));

    // Access Vector Class
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/model/ClassA", L"vcc_object_access_mode_vector.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/propertyAccessor/ClassA", L"vcc_object_access_mode_vector_property_accessor.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/propertyAccessor/ClassA", L"vcc_object_access_mode_vector_property_accessor.cpp"})));

    // Multi Class
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/model/ClassA", L"vcc_object_multi.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/propertyAccessor/ClassA", L"vcc_object_multi_property_accessor.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/propertyAccessor/ClassA", L"vcc_object_multi_property_accessor.cpp"})));

    // Object Class
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/model/ClassA", L"vcc_object_object.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/propertyAccessor/ClassA", L"vcc_object_object_property_accessor.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/propertyAccessor/ClassA", L"vcc_object_object_property_accessor.cpp"})));

    // Container Class
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/model/ClassA", L"vcc_object_container.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/propertyAccessor/ClassA", L"vcc_object_container_property_accessor.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/propertyAccessor/ClassA", L"vcc_object_container_property_accessor.cpp"})));

    // Mix Class
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/model/ClassA", L"vcc_object_mix.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/propertyAccessor/ClassA", L"vcc_object_mix_property_accessor.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/propertyAccessor/ClassA", L"vcc_object_mix_property_accessor.cpp"})));

    // Mix Class
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/model/ClassA", L"vcc_object_json.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/model/ClassA", L"vcc_object_json.cpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"include/propertyAccessor/ClassA", L"vcc_object_mix_property_accessor.hpp"})));
    EXPECT_TRUE(isFilePresent(concatPaths({this->getWorkspaceTarget(), L"src/propertyAccessor/ClassA", L"vcc_object_mix_property_accessor.cpp"})));

    std::wstring unittest = L""
    "#include <gtest/gtest.h>\r\n"
    "\r\n"
    "#include \"vcc_object_empty.hpp\"\r\n"
    "#include \"vcc_object_dependent.hpp\"\r\n"
    "#include \"vcc_object_single.hpp\"\r\n"
    "#include \"vcc_object_no_access.hpp\"\r\n"
    "#include \"vcc_object_access_mode_normal.hpp\"\r\n"
    "#include \"vcc_object_access_mode_vector.hpp\"\r\n"
    "#include \"vcc_object_multi.hpp\"\r\n"
    "#include \"vcc_object_container.hpp\"\r\n"
    "#include \"vcc_object_object.hpp\"\r\n"
    "#include \"vcc_object_mix.hpp\"\r\n"
    "#include \"vcc_object_json.hpp\"\r\n"
    "\r\n"
    "TEST(ClassTest, Normal)\r\n"
    "{\r\n"
    "    DECLARE_SPTR(VCCObjectEmpty, objEmpty);\r\n"
    "    DECLARE_SPTR(VCCObjectDependent, objDependent);\r\n"
    "    DECLARE_SPTR(VCCObjectSingle, objSingle);\r\n"
    "    DECLARE_SPTR(VCCObjectNoAccess, objectNoAccess);\r\n"
    "    DECLARE_SPTR(VCCObjectAccessModeVector, objectAccessModeVector);\r\n"
    "    DECLARE_SPTR(VCCObjectMultiA, objectMultiA);\r\n"
    "    DECLARE_SPTR(VCCObjectMultiB, objectMultiB);\r\n"
    "    DECLARE_SPTR(VCCObjectContainer, objectContainer);\r\n"
    "    DECLARE_SPTR(VCCObjectObject, objectObject);\r\n"
    "    DECLARE_SPTR(VCCObjectMix, objectMix);\r\n"
    "    DECLARE_SPTR(VCCObjectJson, objectJson);\r\n"
    "}\r\n";
    appendFileOneLine(concatPaths({this->getWorkspaceTarget(), L"unittest/module/ClassA", L"class_test.cpp"}), unittest, true);

    if (this->getIsCopyDebugFolderToTestFolder()) {
        std::wstring path = concatPaths({this->getTestFolder(), L"Generate"});
        removeDirectory(path);
        copyDirectory(this->getWorkspaceTarget(), path);
    }
}