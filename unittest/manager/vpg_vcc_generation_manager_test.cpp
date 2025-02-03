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
    GETSET_SPTR(LogConfig, LogConfig);
    GETSET_SPTR(VPGGenerationOption, Option);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGVccGenerationManagerTest/");
    GETSET(std::wstring, WorkspaceSource, L"");
    GETSET(std::wstring, WorkspaceTarget, L"");
    
    GETSET(std::wstring, TestFolder, L"../VPGVccGenerationManagerTest_VCCTestProject");
    GETSET(bool, IsCopyDebugFolderToTestFolder, false);

    MANAGER_SPTR(VPGVccGenerationManager, Manager, _LogConfig, L"", _Option);

    public:
        void SetUp() override
        {
            this->_WorkspaceSource = this->_Workspace + L"Source";
            this->_WorkspaceTarget = this->_Workspace + L"Target";

            this->_LogConfig->SetIsConsoleLog(false);
            std::filesystem::remove_all(PATH(this->GetWorkspace()));

            CreateDirectory(this->GetWorkspace());
            CreateDirectory(this->GetWorkspaceSource());
            CreateDirectory(this->GetWorkspaceTarget());

            // option for initialize source
            this->_Manager->SetWorkspace(this->GetWorkspaceSource());
            this->_Option->SetProjectType(VPGProjectType::VccComplex);
            this->_Option->SetTemplateWorkspace(L".");
            this->_Option->SetProjectName(L"VCCProjGenerator");
            this->_Option->SetProjectNameDll(L"libvpg");
            this->_Option->SetProjectNameExe(L"vpg");
            this->_Option->SetIsExcludeUnittest(false);
            this->_Manager->CreateBasicProject();

            // Cannot use current project as time consumming
            CopyDirectoryOption dirOption;
            dirOption.SetIsRecursive(false);
            dirOption.SetIsForce(true);
            CopyDirectory(L".", this->GetWorkspaceSource(), &dirOption);

            dirOption.SetIsRecursive(true);
            dirOption.SetIsForce(true);
            dirOption.InsertIncludeFileFilters(L".vscode/*");
            dirOption.InsertIncludeFileFilters(L"include/application.hpp");
            dirOption.InsertIncludeFileFilters(L"include/external/vcc/*");
            dirOption.InsertIncludeFileFilters(L"include/factory/*.hpp");
            dirOption.InsertIncludeFileFilters(L"include/type/*_type.hpp");
            dirOption.InsertIncludeFileFilters(L"src/application.cpp");
            dirOption.InsertIncludeFileFilters(L"src/factory/*.cpp");
            dirOption.InsertIncludeFileFilters(L"src/external/vcc/*");
            dirOption.InsertIncludeFileFilters(L"unittest/external/vcc/*");
            dirOption.InsertIncludeFileFilters(L"unittest/dllTest/*");
            dirOption.InsertIncludeFileFilters(L"unittest/gtest_main.cpp");
            dirOption.InsertIncludeFileFilters(L"*Makefile");
            dirOption.InsertIncludeFileFilters(L"*README.md");

            dirOption.InsertExcludeFileFilters(L"*.o");

            CopyDirectory(L".", this->GetWorkspaceSource(), &dirOption);

            // replace main so that the project can be compile
            std::wstring mainFilePath = ConcatPaths({this->GetWorkspaceSource(), L"main.cpp"});
            std::wstring mainFileContent = L"int main() {}";
            std::filesystem::remove(PATH(mainFilePath));
            AppendFileOneLine(mainFilePath, mainFileContent);
            
            // option for testing
            this->_Manager->SetWorkspace(this->GetWorkspaceTarget());
            this->_Option->SetTemplateWorkspace(this->GetWorkspaceSource());
            this->_Option->SetIsGit(true);
            this->_Option->SetProjectNameExe(L"VCCProject");
            this->_Option->SetProjectNameDll(L"VCCDllProject");
            this->_Option->SetProjectName(L"ProjectName");
            this->_Option->SetIsExcludeUnittest(false);
        }

        void TearDown() override
        {
            // ensure compile main project will not compile this project
            std::filesystem::remove_all(PATH(this->GetWorkspace()));
        }
};

TEST_F(VPGVccGenerationManagerTest, Add)
{
    // Complex
    this->_Option->SetProjectNameExe(L"VCCProject");
    this->_Option->SetProjectNameDll(L"VCCDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetIsExcludeUnittest(false);
    this->_Option->SetIsExcludeVCCUnitTest(false);
    this->GetManager()->Add();
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));

    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/application.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/type/exception_type.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/factory/object_factory.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/factory/property_accessor_factory.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/application.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/factory/object_factory.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/factory/property_accessor_factory.cpp"})));
    
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/external/vcc/core"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/external/vcc/versioning/Git"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest/external/vcc/core"})));
        
    if (this->GetIsCopyDebugFolderToTestFolder()) {
        std::wstring path = ConcatPaths({this->GetTestFolder(), L"VCCComplex"});
        RemoveDirectory(path);
        CopyDirectory(this->GetWorkspaceTarget(), path);        
    }

    // EXE only
    std::filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    this->_Option->SetProjectNameExe(L"VCCProject");
    this->_Option->SetProjectNameDll(L"");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetIsExcludeUnittest(false);
    this->_Option->SetIsExcludeVCCUnitTest(false);
    this->GetManager()->Add();
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_FALSE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_FALSE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_FALSE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));

    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/application.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/type/exception_type.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/factory/object_factory.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/factory/property_accessor_factory.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/application.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/factory/object_factory.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/factory/property_accessor_factory.cpp"})));
    
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/external/vcc/core"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/external/vcc/versioning/Git"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest/external/vcc/core"})));

    if (this->GetIsCopyDebugFolderToTestFolder()) {
        std::wstring path = ConcatPaths({this->GetTestFolder(), L"VCCEXE"});
        RemoveDirectory(path);
        CopyDirectory(this->GetWorkspaceTarget(), path);        
    }

    // DLL only
    std::filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    this->_Option->SetProjectNameExe(L"");
    this->_Option->SetProjectNameDll(L"VCCDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetIsExcludeUnittest(false);
    this->_Option->SetIsExcludeVCCUnitTest(false);
    this->GetManager()->Add();
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_FALSE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));

    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/application.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/type/exception_type.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/factory/object_factory.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/factory/property_accessor_factory.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/application.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/factory/object_factory.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/factory/property_accessor_factory.cpp"})));
    
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/external/vcc/core"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/external/vcc/versioning/Git"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest/external/vcc/core"})));

    if (this->GetIsCopyDebugFolderToTestFolder()) {
        std::wstring path = ConcatPaths({this->GetTestFolder(), L"VCCDLL"});
        RemoveDirectory(path);
        CopyDirectory(this->GetWorkspaceTarget(), path);        
    }

    // No unittest
    std::filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    this->_Option->SetProjectNameExe(L"VCCProject");
    this->_Option->SetProjectNameDll(L"VCCDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetIsExcludeUnittest(true);
    this->_Option->SetIsExcludeVCCUnitTest(false);
    this->GetManager()->Add();
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/application.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/type/exception_type.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/factory/object_factory.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/factory/property_accessor_factory.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/application.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/factory/object_factory.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/factory/property_accessor_factory.cpp"})));
    
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/external/vcc/core"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/external/vcc/versioning/Git"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest/external/vcc/core"})));

    if (this->GetIsCopyDebugFolderToTestFolder()) {
        std::wstring path = ConcatPaths({this->GetTestFolder(), L"NoUnittest"});
        RemoveDirectory(path);
        CopyDirectory(this->GetWorkspaceTarget(), path);        
    }

    // Exclude VCC Unittest
    std::filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    this->_Option->SetProjectNameExe(L"VCCProject");
    this->_Option->SetProjectNameDll(L"VCCDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetIsExcludeUnittest(false);
    this->_Option->SetIsExcludeVCCUnitTest(true);
    this->GetManager()->Add();
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/application.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/type/exception_type.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/factory/object_factory.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/factory/property_accessor_factory.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/application.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/factory/object_factory.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/factory/property_accessor_factory.cpp"})));
    
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/external/vcc/core"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/external/vcc/versioning/Git"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest/external/vcc/core"})));

    if (this->GetIsCopyDebugFolderToTestFolder()) {
        std::wstring path = ConcatPaths({this->GetTestFolder(), L"NoVCCUnittest"});
        RemoveDirectory(path);
        CopyDirectory(this->GetWorkspaceTarget(), path);        
    }

    // Plugins
    std::filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    this->_Option->SetProjectNameExe(L"VCCProject");
    this->_Option->SetProjectNameDll(L"VCCDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetIsExcludeUnittest(false);
    this->_Option->InsertPlugins(L"vcc/versioning/git");
    this->_Option->SetIsExcludeVCCUnitTest(false);
    this->GetManager()->Add();
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/application.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/type/exception_type.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/factory/object_factory.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/factory/property_accessor_factory.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/application.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/factory/object_factory.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/factory/property_accessor_factory.cpp"})));
    
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/external/vcc/core"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/external/vcc/versioning/git"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest/external/vcc/core"})));

    if (this->GetIsCopyDebugFolderToTestFolder()) {
        std::wstring path = ConcatPaths({this->GetTestFolder(), L"WithPlugins"});
        RemoveDirectory(path);
        CopyDirectory(this->GetWorkspaceTarget(), path);        
    }
}

TEST_F(VPGVccGenerationManagerTest, Update)
{
    // cannot test really update case as cannot modify original project git resposne

    // no plugin to have plugin
    this->_Option->SetProjectNameExe(L"VCCProject");
    this->_Option->SetProjectNameDll(L"VCCDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetIsExcludeUnittest(false);
    this->GetManager()->Add();
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/application.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/type/exception_type.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/factory/object_factory.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/factory/property_accessor_factory.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/application.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/factory/object_factory.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/factory/property_accessor_factory.cpp"})));
    
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/external/vcc/core"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/external/vcc/versioning/git"})));

    this->_Option->InsertPlugins(L"vcc/versioning/git");
    auto jsonBuilder = std::make_unique<JsonBuilder>();
    jsonBuilder->SetIsBeautify(true);
    WriteFile(ConcatPaths({this->GetWorkspaceTarget(), L".vcc/vcc.json"}), jsonBuilder->Serialize(this->_Option->ToJson().get()));

    this->GetManager()->Update();
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/application.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/type/exception_type.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/factory/object_factory.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/factory/property_accessor_factory.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/application.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/factory/object_factory.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/factory/property_accessor_factory.cpp"})));
    
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/external/vcc/core"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/external/vcc/versioning/git"})));

    if (this->GetIsCopyDebugFolderToTestFolder()) {
        std::wstring path = ConcatPaths({this->GetTestFolder(), L"Update"});
        RemoveDirectory(path);
        CopyDirectory(this->GetWorkspaceTarget(), path);
    }
}

TEST_F(VPGVccGenerationManagerTest, Update_Makefile)
{
    // cannot test really update case as cannot modify original project git resposne

    // no plugin to have plugin
    this->_Option->SetProjectNameExe(L"VCCProject");
    this->_Option->SetProjectNameDll(L"VCCDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetIsExcludeUnittest(false);
    this->GetManager()->Add();
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/application.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/type/exception_type.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/factory/object_factory.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/factory/property_accessor_factory.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/application.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/factory/object_factory.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/factory/property_accessor_factory.cpp"})));
    
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/external/vcc/core"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/external/vcc/versioning/Git"})));

    // Update project name
    std::wstring projectName = L"NewProjectName";
    std::wstring projectNameDll = L"NewProjectNameDll";
    std::wstring projectNameExe = L"NewProjectNameExe";
    std::wstring projectNameGtest = L"unittest";

    auto jsonBuilder = std::make_unique<JsonBuilder>();
    jsonBuilder->SetIsBeautify(true);

    std::wstring vccJsonFileContent = ReadFile(ConcatPaths({this->GetWorkspaceTarget(), L".vcc/vcc.json"}));
    auto json = std::make_shared<Json>();
    jsonBuilder->Deserialize(vccJsonFileContent, json);
    json->SetString(L"ProjectName", projectName);
    json->SetString(L"ProjectNameDll", projectNameDll);
    json->SetString(L"ProjectNameExe", projectNameExe);
    json->SetBool(L"IsExcludeUnittest", false);
    WriteFile(ConcatPaths({this->GetWorkspaceTarget(), L".vcc/vcc.json"}), jsonBuilder->Serialize(json.get()));

    this->GetManager()->Update();
    std::wstring makefileFileContent = ReadFile(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"}));
    EXPECT_TRUE(CountSubstring(makefileFileContent, L"PROJ_NAME := NewProjectName") == 1);
    EXPECT_TRUE(CountSubstring(makefileFileContent, L"PROJ_NAME_DLL := NewProjectNameDll") == 1);
    EXPECT_TRUE(CountSubstring(makefileFileContent, L"PROJ_NAME_EXE := NewProjectNameExe") == 1);
    EXPECT_TRUE(CountSubstring(makefileFileContent, L"IS_EXCLUDE_UNITTEST := N") == 1);
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest"})));

    if (this->GetIsCopyDebugFolderToTestFolder()) {
        std::wstring path = ConcatPaths({this->GetTestFolder(), L"Update_Makefile"});
        RemoveDirectory(path);
        CopyDirectory(this->GetWorkspaceTarget(), path);
    }
}

TEST_F(VPGVccGenerationManagerTest, Generate)
{
    // Create file for generation
    this->_Option->SetProjectNameExe(L"VCCProject");
    this->_Option->SetProjectNameDll(L"VCCDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetIsExcludeUnittest(false);
    this->GetManager()->Add();

    // Empty class
    std::wstring codeEmpty =  L""
    L"#pragma once\r\n"
    L"\r\n"
    L"enum class VCCObjectEmptyProperty\r\n"
    L"{\r\n"
    L"};\r\n"
    L"\r\n";
    AppendFileOneLine(ConcatPaths({this->GetWorkspaceTarget(), L"include/type/ClassA", L"vcc_object_empty_property.hpp"}), codeEmpty, true);

    // Dependent class
    std::wstring codeDependent =  L""
    "#pragma once\r\n"
    "\r\n"
    "enum class VCCObjectDependentProperty\r\n"
    "{\r\n"
    "    EnumA // GETSET_SPTR(VCCObjectEmpty, EnumA) \r\n"
    "};\r\n";
    AppendFileOneLine(ConcatPaths({this->GetWorkspaceTarget(), L"include/type/ClassA/Dependent", L"vcc_object_dependent_property.hpp"}), codeDependent, true);
    
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
    AppendFileOneLine(ConcatPaths({this->GetWorkspaceTarget(), L"include/type/ClassA", L"vcc_object_single_property.hpp"}), codeSingle, true);

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
    AppendFileOneLine(ConcatPaths({this->GetWorkspaceTarget(), L"include/type/ClassA", L"vcc_object_no_access_property.hpp"}), codeNoAccess, true);
 
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
    AppendFileOneLine(ConcatPaths({this->GetWorkspaceTarget(), L"include/type/ClassA", L"vcc_object_access_mode_normal_property.hpp"}), codeNoAccessModeNormal, true);
 
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
    AppendFileOneLine(ConcatPaths({this->GetWorkspaceTarget(), L"include/type/ClassA", L"vcc_object_access_mode_vector_property.hpp"}), codeNoAccessVector, true);
 
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
    AppendFileOneLine(ConcatPaths({this->GetWorkspaceTarget(), L"include/type/ClassA", L"vcc_object_multi_property.hpp"}), codeMulti, true);
 
    // copied from TEST_F(VPGPropertyAccessorFileGenerationServiceTest, Object)
    std::wstring codeObject =  L""
    "#pragma once\r\n"
    "\r\n"
    "enum class VCCObjectObjectProperty\r\n"
    "{\r\n"
    "    Object // GETSET_SPTR_NULL(VCCObjectEmpty, Object)\r\n"
    "};\r\n";
    AppendFileOneLine(ConcatPaths({this->GetWorkspaceTarget(), L"include/type/ClassA", L"vcc_object_object_property.hpp"}), codeObject, true);
 
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
    AppendFileOneLine(ConcatPaths({this->GetWorkspaceTarget(), L"include/type/ClassA", L"vcc_object_container_property.hpp"}), codeContainer, true);
 
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
    AppendFileOneLine(ConcatPaths({this->GetWorkspaceTarget(), L"include/type/ClassA", L"vcc_object_mix_property.hpp"}), codeMIx, true);

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
    AppendFileOneLine(ConcatPaths({this->GetWorkspaceTarget(), L"include/type/ClassA", L"vcc_object_json_property.hpp"}), codeJson, true);

    this->_Option->SetTemplateWorkspace(this->GetWorkspaceSource());
    this->_Option->SetProjectPrefix(L"VCC");
    this->GetManager()->SetWorkspace(this->GetWorkspaceTarget());
    this->GetManager()->Generate();
    
    // Empty Class
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/model/ClassA", L"vcc_object_empty.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/propertyAccessor/ClassA", L"vcc_object_empty_property_accessor.hpp"})));
    EXPECT_FALSE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/propertyAccessor/ClassA", L"vcc_object_empty_property_accessor.cpp"})));

    // Dependent Class
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/model/ClassA/Dependent", L"vcc_object_dependent.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/propertyAccessor/ClassA/Dependent", L"vcc_object_dependent_property_accessor.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/propertyAccessor/ClassA/Dependent", L"vcc_object_dependent_property_accessor.cpp"})));

    // Single Class
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/model/ClassA", L"vcc_object_single.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/propertyAccessor/ClassA", L"vcc_object_single_property_accessor.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/propertyAccessor/ClassA", L"vcc_object_single_property_accessor.cpp"})));

    // NoAccess Class
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/model/ClassA", L"vcc_object_no_access.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/propertyAccessor/ClassA", L"vcc_object_no_access_property_accessor.hpp"})));
    EXPECT_FALSE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/propertyAccessor/ClassA", L"vcc_object_no_access_property_accessor.cpp"})));

    // Access Normal Class
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/model/ClassA", L"vcc_object_access_mode_normal.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/propertyAccessor/ClassA", L"vcc_object_access_mode_normal_property_accessor.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/propertyAccessor/ClassA", L"vcc_object_access_mode_normal_property_accessor.cpp"})));

    // Access Vector Class
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/model/ClassA", L"vcc_object_access_mode_vector.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/propertyAccessor/ClassA", L"vcc_object_access_mode_vector_property_accessor.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/propertyAccessor/ClassA", L"vcc_object_access_mode_vector_property_accessor.cpp"})));

    // Multi Class
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/model/ClassA", L"vcc_object_multi.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/propertyAccessor/ClassA", L"vcc_object_multi_property_accessor.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/propertyAccessor/ClassA", L"vcc_object_multi_property_accessor.cpp"})));

    // Object Class
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/model/ClassA", L"vcc_object_object.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/propertyAccessor/ClassA", L"vcc_object_object_property_accessor.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/propertyAccessor/ClassA", L"vcc_object_object_property_accessor.cpp"})));

    // Container Class
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/model/ClassA", L"vcc_object_container.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/propertyAccessor/ClassA", L"vcc_object_container_property_accessor.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/propertyAccessor/ClassA", L"vcc_object_container_property_accessor.cpp"})));

    // Mix Class
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/model/ClassA", L"vcc_object_mix.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/propertyAccessor/ClassA", L"vcc_object_mix_property_accessor.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/propertyAccessor/ClassA", L"vcc_object_mix_property_accessor.cpp"})));

    // Mix Class
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/model/ClassA", L"vcc_object_json.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/model/ClassA", L"vcc_object_json.cpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"include/propertyAccessor/ClassA", L"vcc_object_mix_property_accessor.hpp"})));
    EXPECT_TRUE(IsFilePresent(ConcatPaths({this->GetWorkspaceTarget(), L"src/propertyAccessor/ClassA", L"vcc_object_mix_property_accessor.cpp"})));

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
    AppendFileOneLine(ConcatPaths({this->GetWorkspaceTarget(), L"unittest/module/ClassA", L"class_test.cpp"}), unittest, true);

    if (this->GetIsCopyDebugFolderToTestFolder()) {
        std::wstring path = ConcatPaths({this->GetTestFolder(), L"Generate"});
        RemoveDirectory(path);
        CopyDirectory(this->GetWorkspaceTarget(), path);
    }
}