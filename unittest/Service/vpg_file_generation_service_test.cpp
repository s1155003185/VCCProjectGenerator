#include <gtest/gtest.h>

#include <filesystem>
#include <set>
#include <string>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "log_service.hpp"
#include "vpg_file_generation_service.hpp"
#include "string_helper.hpp"

using namespace std;

class VPGFileGenerationServiceTest : public testing::Test 
{
    GETOBJU(LogProperty, LogProperty, LogPropertyType::None);
    GET(wstring, Workspace, L"bin/Debug/FileGenerationServiceTest");
    GET(wstring, WorkspaceSource, L"bin/Debug/FileGenerationServiceTestSource");
    GET(wstring, WorkspaceTarget, L"bin/Debug/FileGenerationServiceTestTarget");
    
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

        void CreateFileInSourceWorkspace(std::wstring fileName, std::wstring content)
        {
            AppendFileOneLine(ConcatPath(this->GetWorkspaceSource(), fileName), content, true);
        }
        void CreateFileInTargetWorkspace(std::wstring fileName, std::wstring content)
        {
            AppendFileOneLine(ConcatPath(this->GetWorkspaceTarget(), fileName), content, true);
        }
    public:

        void SetUp() override
        {
            filesystem::remove_all(PATH(this->GetWorkspace()));
            filesystem::remove_all(PATH(this->GetWorkspaceSource()));
            filesystem::remove_all(PATH(this->GetWorkspaceTarget()));
            
            std::wstring code =  L"";
            code += L"#pragma once\r\n";
            code += L"\r\n";
            code += L"enum class VCCObjectProperty // Class Command\r\n";
            code += L"{\r\n";
            code += L"    EnumA, // GETSET(std::wstring, EnumA, L\"Default\") \r\n";
            code += L"    EnumB, // GET(int64_t, EnumB, 0) \r\n";
            code += L"    EnumC, // GETOBJ(ClassA, EnumC) \r\n";
            code += L"    EnumD, // GETUPTR(ClassB, EnumD, 1, 2, 3) \r\n";
            code += L"    EnumE,  // GETSET(EnumClass, EnumE, EnumClass::OptionA)\r\n";
            code += L"    EnumF // Nothing\r\n";
            code += L"};";
            this->CreateFileInSourceWorkspace(L"vcc_a_property.hpp", code);
        }

        void TearDown() override
        {

        }

        bool CheckFolderExists(std::wstring path)
        {
            return IsDirectoryExists(ConcatPath(this->GetWorkspace(), path));
        }
};

TEST_F(VPGFileGenerationServiceTest, GetClassMacroList)
{
    std::set<std::wstring> expectedResult = {
        L"GETSET",
        L"GET",
        L"STATICGET",
        L"GETOBJ",
        L"GETUPTR",
        L"GETOBJU",
        L"GETOBJS",
        L"GETSPTR",
        L"VECTOR",
        L"SET",
        L"MAP",
    };

    std::set<std::wstring> results = VPGFileGenerationService::GetClassMacroList(L"");
    EXPECT_EQ(results, expectedResult);
}

TEST_F(VPGFileGenerationServiceTest, GenerateProperty)
{
    VPGFileGenerationService::GernerateProperty(*this->GetLogProperty(), L"VCC", L"", this->GetWorkspaceSource(), 
        this->GetWorkspaceTarget(), this->GetWorkspaceTarget(), this->GetWorkspaceTarget(), this->GetWorkspaceTarget());

    // ------------------------------------------------------------------------------------------ //
    //                                      Object Type File                                      //
    // ------------------------------------------------------------------------------------------ //
    std::wstring objectTypeFileContent = ReadFile(ConcatPath(this->GetWorkspaceTarget(), L"vcc_object_type.hpp"));
    std::wstring expectedObjectTypeFileContent = L"";
    expectedObjectTypeFileContent += L"#pragma once\r\n";
    expectedObjectTypeFileContent += L"\r\n";
    expectedObjectTypeFileContent += L"enum class VCCObjectType {\r\n";
    expectedObjectTypeFileContent += L"    Object\r\n";
    expectedObjectTypeFileContent += L"};\r\n";
    EXPECT_EQ(objectTypeFileContent, expectedObjectTypeFileContent);

    // ------------------------------------------------------------------------------------------ //
    //                                      Class File                                            //
    // ------------------------------------------------------------------------------------------ //

    std::wstring objectClassFileContent = ReadFile(ConcatPath(this->GetWorkspaceTarget(), L"vcc_a.hpp"));
    std::wstring expectedObjectClassFileContent = L"";
    expectedObjectClassFileContent += L"#pragma once\r\n";
    expectedObjectClassFileContent += L"\r\n";
    expectedObjectClassFileContent += L"#include \"base_object.hpp\"\r\n";
    expectedObjectClassFileContent += L"#include \"class_marco.hpp\"\r\n";
    expectedObjectClassFileContent += L"\r\n";
    expectedObjectClassFileContent += L"using namespace vcc;\r\n";
    expectedObjectClassFileContent += L"\r\n";
    expectedObjectClassFileContent += L"class ClassA;\r\n";
    expectedObjectClassFileContent += L"class ClassB;\r\n";
    expectedObjectClassFileContent += L"class EnumClass;\r\n";
    expectedObjectClassFileContent += L"class Object : public BaseObject\r\n";
    expectedObjectClassFileContent += L"{\r\n";
    expectedObjectClassFileContent += INDENT + L"GETSET(std::wstring, EnumA, L\"Default\")\r\n";
    expectedObjectClassFileContent += INDENT + L"GET(int64_t, EnumB, 0)\r\n";
    expectedObjectClassFileContent += INDENT + L"GETOBJ(ClassA, EnumC)\r\n";
    expectedObjectClassFileContent += INDENT + L"GETUPTR(ClassB, EnumD, 1, 2, 3)\r\n";
    expectedObjectClassFileContent += INDENT + L"GETSET(EnumClass, EnumE, EnumClass::OptionA)\r\n";
    expectedObjectClassFileContent += L"\r\n";
    expectedObjectClassFileContent += INDENT + L"public:\r\n";
    expectedObjectClassFileContent += INDENT + INDENT + L"Object() : BaseObject(ObjectType::Object) {}\r\n";
    expectedObjectClassFileContent += INDENT + INDENT + L"~Object() {}\r\n";
    expectedObjectClassFileContent += L"\r\n";
    expectedObjectClassFileContent += INDENT + INDENT +  L"virtual std::wstring GetKey() override { return L\"\"; }\r\n";
    expectedObjectClassFileContent += INDENT + INDENT +  L"virtual std::wstring ToString() override { return L\"\"; }\r\n";
    expectedObjectClassFileContent += INDENT + INDENT +  L"virtual std::shared_ptr<IObject> Clone() override { return make_shared<Object>(*this); }\r\n";
    expectedObjectClassFileContent += L"};\r\n";
    expectedObjectClassFileContent += L"\r\n";
    EXPECT_EQ(objectClassFileContent, expectedObjectClassFileContent);

    // ------------------------------------------------------------------------------------------ //
    //                                      Property Accessor File                                //
    // ------------------------------------------------------------------------------------------ //
    // std::wstring objectPropertyAccessorFileContentHpp = ReadFile(ConcatPath(this->GetWorkspaceTarget(), L"vcc_a_property_accessor.hpp"));
    // std::wstring objectPropertyAccessorFileContentCpp = ReadFile(ConcatPath(this->GetWorkspaceTarget(), L"vcc_a_property_accessor.cpp"));
    // std::wstring expectedOBjectPropertyAccessorFileContentHpp = L"";
    // std::wstring expectedOBjectPropertyAccessorFileContentCpp = L"";
    // EXPECT_EQ(objectPropertyAccessorFileContentHpp, expectedOBjectPropertyAccessorFileContentHpp);
    // EXPECT_EQ(objectPropertyAccessorFileContentCpp, expectedOBjectPropertyAccessorFileContentCpp);

}