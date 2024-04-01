#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "log_property.hpp"
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
            this->_Option->SetProjectNameEXE(L"VCCProject");
            this->_Option->SetProjectNameDLL(L"VCCDllProject");
            this->_Option->SetProjectName(L"ProjectName");
            this->_Option->SetProjectNameGtest(L"VCCUnitTest");
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
    this->_Option->SetProjectNameEXE(L"VCCProject");
    this->_Option->SetProjectNameDLL(L"VCCDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetProjectNameGtest(L"VCCUnitTest");
    this->_Option->SetIsExcludeVCCUnitTest(false);
    this->GetManager()->Add();
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"VCCUnitTest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Core"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Version/Git"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"VCCUnitTest/External/VCC/Core"})));
    if (this->GetIsCopyDebugFolderToTestFolder())
        CopyDirectory(this->GetWorkspaceTarget(), ConcatPaths({this->GetTestFolder(), L"VCCComplex"}));

    // EXE only
    std::filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    this->_Option->SetProjectNameEXE(L"VCCProject");
    this->_Option->SetProjectNameDLL(L"");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetProjectNameGtest(L"VCCUnitTest");
    this->_Option->SetIsExcludeVCCUnitTest(false);
    this->GetManager()->Add();
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_FALSE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_FALSE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_FALSE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"VCCUnitTest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Core"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Version/Git"})));
    if (this->GetIsCopyDebugFolderToTestFolder())
        CopyDirectory(this->GetWorkspaceTarget(), ConcatPaths({this->GetTestFolder(), L"VCCEXE"}));

    // DLL only
    std::filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    this->_Option->SetProjectNameEXE(L"");
    this->_Option->SetProjectNameDLL(L"VCCDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetProjectNameGtest(L"VCCUnitTest");
    this->_Option->SetIsExcludeVCCUnitTest(false);
    this->GetManager()->Add();
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_FALSE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"VCCUnitTest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Core"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Version/Git"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"VCCUnitTest/External/VCC/Core"})));
    if (this->GetIsCopyDebugFolderToTestFolder())
        CopyDirectory(this->GetWorkspaceTarget(), ConcatPaths({this->GetTestFolder(), L"VCCDLL"}));

    // No unittest
    std::filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    this->_Option->SetProjectNameEXE(L"VCCProject");
    this->_Option->SetProjectNameDLL(L"VCCDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetProjectNameGtest(L"");
    this->_Option->SetIsExcludeVCCUnitTest(false);
    this->GetManager()->Add();
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"VCCUnitTest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Core"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Version/Git"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"VCCUnitTest/External/VCC/Core"})));
    if (this->GetIsCopyDebugFolderToTestFolder())
        CopyDirectory(this->GetWorkspaceTarget(), ConcatPaths({this->GetTestFolder(), L"NoUnittest"}));

    // Exclude VCC Unittest
    std::filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    this->_Option->SetProjectNameEXE(L"VCCProject");
    this->_Option->SetProjectNameDLL(L"VCCDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetProjectNameGtest(L"VCCUnitTest");
    this->_Option->SetIsExcludeVCCUnitTest(true);
    this->GetManager()->Add();
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"VCCUnitTest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Core"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Version/Git"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"VCCUnitTest/External/VCC/Core"})));
    if (this->GetIsCopyDebugFolderToTestFolder())
        CopyDirectory(this->GetWorkspaceTarget(), ConcatPaths({this->GetTestFolder(), L"NoVCCUnittest"}));

    // Plugins
    std::filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
    this->_Option->SetProjectNameEXE(L"VCCProject");
    this->_Option->SetProjectNameDLL(L"VCCDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetProjectNameGtest(L"VCCUnitTest");
    this->_Option->InsertPlugins(L"VCC/Versioning/Git");
    this->_Option->SetIsExcludeVCCUnitTest(false);
    this->GetManager()->Add();
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"VCCUnitTest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Core"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Versioning/Git"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"VCCUnitTest/External/VCC/Core"})));
    if (this->GetIsCopyDebugFolderToTestFolder())
        CopyDirectory(this->GetWorkspaceTarget(), ConcatPaths({this->GetTestFolder(), L"WithPlugins"}));
}

TEST_F(VPGVccGenerationManagerTest, Update)
{
    // cannot test really update case as cannot modify original project git resposne

    // no plugin to have plugin
    this->_Option->SetProjectNameEXE(L"VCCProject");
    this->_Option->SetProjectNameDLL(L"VCCDllProject");
    this->_Option->SetProjectName(L"ProjectName");
    this->_Option->SetProjectNameGtest(L"VCCUnitTest");
    this->GetManager()->Add();
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"VCCUnitTest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Core"})));
    EXPECT_FALSE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Version/Git"})));

    this->_Option->InsertPlugins(L"VCC/Versioning/Git");
    this->GetManager()->Update();
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"Makefile"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllEntryPoint.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.cpp"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"DllFunctions.h"})));
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspaceTarget(), L"main.cpp"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"VCCUnitTest"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Core"})));
    EXPECT_TRUE(IsDirectoryExists(ConcatPaths({this->GetWorkspaceTarget(), L"include/External/VCC/Versioning/Git"})));
}

TEST_F(VPGVccGenerationManagerTest, Generate)
{            
    // Create file for generation
    std::wstring code =  L"";
    code += L"#pragma once\r\n";
    code += L"\r\n";
    code += L"enum class VCCObjectProperty // Class Command\r\n";
    code += L"{\r\n";
    code += L"    EnumA, // Nothing\r\n";
    code += L"    EnumB, // GETSET(std::wstring, EnumB, L\"Default\") \r\n";
    code += L"    EnumC, // GET(int64_t, EnumC, 0) \r\n";
    code += L"    EnumD, // GETSET(EnumClass, EnumD, EnumClass::OptionA)\r\n";
    code += L"    EnumE  // VECTOR(EnumClass, EnumE) \r\n";
    code += L"};\r\n";
    code += L"\r\n";
    code += L"enum class VCCObjectPtrProperty // Class Command\r\n";
    code += L"{\r\n";
    code += L"    EnumA, // GET_SPTR(ClassA, EnumA) \r\n";
    code += L"    EnumB, // GET_SPTR(ClassB, EnumB, 1, 2, 3) \r\n";
    code += L"    EnumC, // VECTOR_SPTR(ClassA, EnumC) \r\n";
    code += L"    EnumD, // SET_SPTR(ClassA, EnumD) \r\n";
    code += L"};";
    AppendFileOneLine(ConcatPaths({this->GetWorkspaceSource(), L"include/Type/ClassA", L"vcc_a_property.hpp"}), code, true);
    
    // Only use Source Directory
    this->_Option->SetWorkspaceSource(this->GetWorkspaceSource());
    this->_Option->SetWorkspaceDestination(this->GetWorkspaceSource());
    this->_Option->SetProjectPrefix(L"VCC");
    this->GetManager()->Generate();
    if (this->GetIsCopyDebugFolderToTestFolder())
       CopyDirectory(this->GetWorkspaceSource(), ConcatPaths({this->GetTestFolder(), L"Generate"}));
}