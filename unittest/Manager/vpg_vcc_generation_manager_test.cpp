#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "json.hpp"
#include "json_builder.hpp"
#include "log_property.hpp"
#include "process_service.hpp"
#include "vpg_vcc_generation_manager.hpp"

using namespace vcc;

class VPGVccGenerationManagerTest : public testing::Test 
{
    GET_SPTR(LogProperty, LogProperty);
    GET_SPTR(VPGGenerationOption, Option);
    GET(std::wstring, Workspace, L"bin/Debug/VPGVccGenerationManagerTest/");
    GET(std::wstring, WorkspaceSource, L"");
    GET(std::wstring, WorkspaceTarget, L"");
    
    GET(std::wstring, TestFolder, L"../VPGVccGenerationManagerTest_VCCTestProject");
    GET(bool, IsCopyDebugFolderToTestFolder, false);

    MANAGER(VPGVccGenerationManager, Manager, _LogProperty, _Option);

    public:
        void SetUp() override
        {
            this->_WorkspaceSource = this->_Workspace + L"Source";
            this->_WorkspaceTarget = this->_Workspace + L"Target";

            this->_LogProperty->SetIsConsoleLog(false);
            std::filesystem::remove_all(PATH(this->GetWorkspace()));

            CreateDirectory(this->GetWorkspace());
            CreateDirectory(this->GetWorkspaceSource());
            CreateDirectory(this->GetWorkspaceTarget());

            // option for initialize source
            this->_Option->SetProjectType(VPGProjectType::VccComplex);
            this->_Option->SetWorkspaceSource(L".");
            this->_Option->SetWorkspaceDestination(this->GetWorkspaceSource());
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
            dirOption.InsertIncludeFileFilters(L"include/External/VCC/*");
            dirOption.InsertIncludeFileFilters(L"include/Type/*_type.hpp");
            dirOption.InsertIncludeFileFilters(L"src/External/VCC/*");
            dirOption.InsertIncludeFileFilters(L"unittest/External/VCC/*");
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
            this->_Option->SetWorkspaceSource(this->GetWorkspaceSource());
            this->_Option->SetWorkspaceDestination(this->GetWorkspaceTarget());
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
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Core"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Version/Git"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest/External/VCC/Core"})));
        
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
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_FALSE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_FALSE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_FALSE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Core"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Version/Git"})));

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
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_FALSE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Core"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Version/Git"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest/External/VCC/Core"})));

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
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Core"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Version/Git"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest/External/VCC/Core"})));

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
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Core"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Version/Git"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest/External/VCC/Core"})));

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
    this->_Option->InsertPlugins(L"VCC/Versioning/Git");
    this->_Option->SetIsExcludeVCCUnitTest(false);
    this->GetManager()->Add();
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Core"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Versioning/Git"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest/External/VCC/Core"})));

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
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Core"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Version/Git"})));

    this->_Option->InsertPlugins(L"VCC/Versioning/Git");
    this->GetManager()->Update();
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Core"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Versioning/Git"})));

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
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"unittest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Core"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Version/Git"})));

    // Update project name
    std::wstring projectName = L"NewProjectName";
    std::wstring projectNameDll = L"NewProjectNameDll";
    std::wstring projectNameExe = L"NewProjectNameExe";
    std::wstring projectNameGtest = L"unittest";

    DECLARE_UPTR(JsonBuilder, jsonBuilder);
    jsonBuilder->SetIsBeautify(true);

    std::wstring vccJsonFileContent = ReadFile(ConcatPaths({this->GetWorkspaceTarget(), L".vcc/vcc.json"}));
    DECLARE_SPTR(Json, json);
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

    std::wstring codeA =  L"";
    codeA += L"#pragma once\r\n";
    codeA += L"\r\n";
    codeA += L"enum class VCCObjectProperty // Class Command\r\n";
    codeA += L"{\r\n";
    codeA += L"    EnumA, // Nothing\r\n";
    codeA += L"    EnumB, // GETSET(std::wstring, EnumB, L\"Default\") \r\n";
    codeA += L"    EnumC, // GET(int64_t, EnumC, 0) \r\n";
    codeA += L"    EnumD  // VECTOR(ExceptionType, EnumD) \r\n";
    codeA += L"};\r\n";
    codeA += L"\r\n";
    AppendFileOneLine(ConcatPaths({this->GetWorkspaceTarget(), L"include/Type/ClassA", L"vcc_a_property.hpp"}), codeA, true);

    std::wstring codeB =  L"";
    codeB += L"#pragma once\r\n";
    codeB += L"\r\n";
    codeB += L"enum class VCCObjectBPtrProperty // Class Command\r\n";
    codeB += L"{\r\n";
    codeB += L"    EnumA // GET_SPTR(VCCObject, EnumA) \r\n";
    codeB += L"};\r\n";
    AppendFileOneLine(ConcatPaths({this->GetWorkspaceTarget(), L"include/Type/ClassA", L"vcc_b_property.hpp"}), codeB, true);
    
    std::wstring unittest = L"";
    unittest += L"#include <gtest/gtest.h>\r\n";
    unittest += L"\r\n";
    unittest += L"#include \"memory_macro.hpp\"\r\n";
    unittest += L"\r\n";
    unittest += L"#include \"vcc_a.hpp\"\r\n";
    unittest += L"#include \"vcc_b.hpp\"\r\n";
    unittest += L"\r\n";
    unittest += L"TEST(ClassTest, Normal)\r\n";
    unittest += L"{\r\n";
    unittest += L"    DECLARE_SPTR(VCCObjectAPtr, objA);\r\n";
    unittest += L"    DECLARE_SPTR(VCCObjectBPtr, objB);\r\n";
    unittest += L"}\r\n";
    AppendFileOneLine(ConcatPaths({this->GetWorkspaceTarget(), L"unittest/Module/ClassA", L"class_test.cpp"}), unittest, true);

    this->_Option->SetWorkspaceSource(this->GetWorkspaceSource());
    this->_Option->SetWorkspaceDestination(this->GetWorkspaceTarget());
    this->_Option->SetProjectPrefix(L"VCC");
    this->GetManager()->Generate();
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/Model/ClassA", L"vcc_a.hpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/Model/ClassA", L"vcc_b.hpp"})));

    if (this->GetIsCopyDebugFolderToTestFolder()) {
        std::wstring path = ConcatPaths({this->GetTestFolder(), L"Generate"});
        RemoveDirectory(path);
        CopyDirectory(this->GetWorkspaceTarget(), path);
    }
}