#include <gtest/gtest.h>

#include <filesystem>
#include <map>
#include <set>
#include <string>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "log_service.hpp"
#include "memory_macro.hpp"
#include "vpg_enum_class_reader.hpp"
#include "vpg_file_generation_manager.hpp"
#include "string_helper.hpp"

class VPGFileGenerationManagerTest : public testing::Test 
{
    GET_SPTR(LogProperty, LogProperty, LogPropertyType::None);
    MANAGER(VPGFileGenerationManager, Manager, _LogProperty);

    GET(std::wstring, Workspace, L"bin/Debug/FileGenerationServiceTest");
    GET(std::wstring, WorkspaceSource, L"");
    GET(std::wstring, WorkspaceTarget, L"");

    private:
        void CreateFolderInSourceWorkspace(std::wstring folder)
        {
            if (folder.empty())
                CreateDirectory(this->GetWorkspaceSource());
            else
                CreateDirectory(ConcatPaths({this->GetWorkspaceSource(), folder}));
        }

        void CreateFolderInTargetWorkspace(std::wstring folder)
        {
            if (folder.empty())
                CreateDirectory(this->GetWorkspaceTarget());
            else
                CreateDirectory(ConcatPaths({this->GetWorkspaceTarget(), folder}));
        }

        void CreateFileInSourceWorkspace(std::wstring fileName, std::wstring content)
        {
            AppendFileOneLine(ConcatPaths({this->GetWorkspaceSource(), fileName}), content, true);
        }
        void CreateFileInTargetWorkspace(std::wstring fileName, std::wstring content)
        {
            AppendFileOneLine(ConcatPaths({this->GetWorkspaceTarget(), fileName}), content, true);
        }
    public:
        std::unique_ptr<VPGEnumClassReader> _Reader = nullptr;

        VPGFileGenerationManagerTest() {}
        virtual ~VPGFileGenerationManagerTest() {}

        void SetUp() override
        {
            this->_WorkspaceSource = ConcatPaths({this->_Workspace, L"Source"});
            this->_WorkspaceTarget = ConcatPaths({this->_Workspace, L"Target"});

            std::filesystem::remove_all(PATH(this->GetWorkspace()));
            this->GetManager()->GetClassMacroList(L"");

            this->_Reader = std::make_unique<VPGEnumClassReader>(this->GetManager()->GetClassMacros());

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
            this->CreateFileInSourceWorkspace(L"vcc_a_property.hpp", code);
        }

        void TearDown() override
        {
            std::filesystem::remove_all(PATH(this->GetWorkspace()));
        }

        bool CheckFolderExists(std::wstring path)
        {
            return IsDirectoryExists(ConcatPaths({this->GetWorkspace(), path}));
        }
};

TEST_F(VPGFileGenerationManagerTest, GetClassNameFromEnumClassName)
{
    EXPECT_EQ(this->GetManager()->GetClassNameFromEnumClassName(L"VCCObjectProperty"), L"VCCObject");
}

TEST_F(VPGFileGenerationManagerTest, GetFileList)
{
    std::map<std::wstring, std::wstring> classList, enumList;
    this->GetManager()->GetFileList(this->_Reader.get(), this->GetWorkspaceSource(), L"");

    std::map<std::wstring, std::wstring> classResult;
    classResult.insert(std::make_pair(L"VCCObject", L"vcc_a.hpp"));
    classResult.insert(std::make_pair(L"VCCObjectPtr", L"vcc_a.hpp"));
    EXPECT_EQ(classResult, this->GetManager()->GetClassFiles());
    
    std::map<std::wstring, std::wstring> enumResult;
    enumResult.insert(std::make_pair(L"VCCObjectProperty", L"vcc_a_property.hpp"));
    enumResult.insert(std::make_pair(L"VCCObjectPtrProperty", L"vcc_a_property.hpp"));
    EXPECT_EQ(enumResult, this->GetManager()->GetEnumClassFiles());
}

TEST_F(VPGFileGenerationManagerTest, GenerateProperty)
{
    this->GetManager()->GernerateProperty(this->GetLogProperty().get(), L"VCC", L"", this->GetWorkspaceSource(), 
        this->GetWorkspaceTarget(), this->GetWorkspaceTarget(), this->GetWorkspaceTarget(), this->GetWorkspaceTarget());

    // ------------------------------------------------------------------------------------------ //
    //                                      Object Type File                                      //
    // ------------------------------------------------------------------------------------------ //
    std::wstring objectTypeFileContent = ReadFile(ConcatPaths({this->GetWorkspaceTarget(), L"object_type.hpp"}));
    std::wstring expectedObjectTypeFileContent = L"";
    expectedObjectTypeFileContent += L"#pragma once\r\n";
    expectedObjectTypeFileContent += L"\r\n";
    expectedObjectTypeFileContent += L"enum class ObjectType {\r\n";
    expectedObjectTypeFileContent += L"    NA,\r\n";
    expectedObjectTypeFileContent += L"    Object,\r\n";
    expectedObjectTypeFileContent += L"    ObjectPtr\r\n";
    expectedObjectTypeFileContent += L"};\r\n";
    EXPECT_EQ(objectTypeFileContent, expectedObjectTypeFileContent);

    // ------------------------------------------------------------------------------------------ //
    //                                      Class File                                            //
    // ------------------------------------------------------------------------------------------ //

    std::wstring objectClassFileContent = ReadFile(ConcatPaths({this->GetWorkspaceTarget(), L"vcc_a.hpp"}));
    std::wstring expectedObjectClassFileContent = L"";
    expectedObjectClassFileContent += L"#pragma once\r\n";
    expectedObjectClassFileContent += L"\r\n";
    expectedObjectClassFileContent += L"#include <string>\r\n";
    expectedObjectClassFileContent += L"\r\n";
    expectedObjectClassFileContent += L"#include \"base_object.hpp\"\r\n";
    expectedObjectClassFileContent += L"#include \"class_macro.hpp\"\r\n";
    expectedObjectClassFileContent += L"#include \"object_type.hpp\"\r\n";
    expectedObjectClassFileContent += L"\r\n";
    expectedObjectClassFileContent += L"using namespace vcc;\r\n";
    expectedObjectClassFileContent += L"\r\n";
    expectedObjectClassFileContent += L"class ClassA;\r\n";
    expectedObjectClassFileContent += L"class ClassB;\r\n";
    expectedObjectClassFileContent += L"enum class EnumClass;\r\n";
    expectedObjectClassFileContent += L"class VCCObject : public BaseObject<VCCObject>\r\n";
    expectedObjectClassFileContent += L"{\r\n";
    expectedObjectClassFileContent += INDENT + L"GETSET(std::wstring, EnumB, L\"Default\")\r\n";
    expectedObjectClassFileContent += INDENT + L"GET(int64_t, EnumC, 0)\r\n";
    expectedObjectClassFileContent += INDENT + L"GETSET(EnumClass, EnumD, EnumClass::OptionA)\r\n";
    expectedObjectClassFileContent += INDENT + L"VECTOR(EnumClass, EnumE)\r\n";
    expectedObjectClassFileContent += L"\r\n";
    expectedObjectClassFileContent += INDENT + L"public:\r\n";
    expectedObjectClassFileContent += INDENT + INDENT + L"VCCObject() : BaseObject(ObjectType::Object) {}\r\n";
    expectedObjectClassFileContent += INDENT + INDENT + L"virtual ~VCCObject() {}\r\n";
    expectedObjectClassFileContent += L"};\r\n";
    expectedObjectClassFileContent += L"\r\n";
    expectedObjectClassFileContent += L"class VCCObjectPtr : public BaseObject<VCCObjectPtr>\r\n";
    expectedObjectClassFileContent += L"{\r\n";
    expectedObjectClassFileContent += INDENT + L"GET_SPTR(ClassA, EnumA)\r\n";
    expectedObjectClassFileContent += INDENT + L"GET_SPTR(ClassB, EnumB, 1, 2, 3)\r\n";
    expectedObjectClassFileContent += INDENT + L"VECTOR_SPTR(ClassA, EnumC)\r\n";
    expectedObjectClassFileContent += INDENT + L"SET_SPTR(ClassA, EnumD)\r\n";
    expectedObjectClassFileContent += L"\r\n";
    expectedObjectClassFileContent += INDENT + L"public:\r\n";
    expectedObjectClassFileContent += INDENT + INDENT + L"VCCObjectPtr() : BaseObject(ObjectType::ObjectPtr) {}\r\n";
    expectedObjectClassFileContent += INDENT + INDENT + L"virtual ~VCCObjectPtr() {}\r\n";
    expectedObjectClassFileContent += L"\r\n";
    expectedObjectClassFileContent += INDENT + INDENT + L"virtual std::shared_ptr<IObject> Clone() const override {\r\n";
    expectedObjectClassFileContent += INDENT + INDENT + INDENT + L"std::shared_ptr<VCCObjectPtr> obj = std::make_shared<VCCObjectPtr>(*this);\r\n";
    expectedObjectClassFileContent += INDENT + INDENT + INDENT + L"obj->CloneEnumA(this->_EnumA);\r\n";
    expectedObjectClassFileContent += INDENT + INDENT + INDENT + L"obj->CloneEnumB(this->_EnumB);\r\n";
    expectedObjectClassFileContent += INDENT + INDENT + INDENT + L"obj->CloneEnumC(this->_EnumC);\r\n";
    expectedObjectClassFileContent += INDENT + INDENT + INDENT + L"obj->CloneEnumD(this->_EnumD);\r\n";
    expectedObjectClassFileContent += INDENT + INDENT + INDENT + L"return obj;\r\n";
    expectedObjectClassFileContent += INDENT + INDENT + L"}\r\n";
    expectedObjectClassFileContent += L"};\r\n";
    EXPECT_EQ(objectClassFileContent, expectedObjectClassFileContent);

    // ------------------------------------------------------------------------------------------ //
    //                                      Property Accessor File                                //
    // ------------------------------------------------------------------------------------------ //
    // std::wstring objectPropertyAccessorFileContentHpp = ReadFile(ConcatPaths({this->GetWorkspaceTarget(), L"vcc_a_property_accessor.hpp"));
    // std::wstring objectPropertyAccessorFileContentCpp = ReadFile(ConcatPaths({this->GetWorkspaceTarget(), L"vcc_a_property_accessor.cpp"));
    // std::wstring expectedOBjectPropertyAccessorFileContentHpp = L"";
    // std::wstring expectedOBjectPropertyAccessorFileContentCpp = L"";
    // EXPECT_EQ(objectPropertyAccessorFileContentHpp, expectedOBjectPropertyAccessorFileContentHpp);
    // EXPECT_EQ(objectPropertyAccessorFileContentCpp, expectedOBjectPropertyAccessorFileContentCpp);

}