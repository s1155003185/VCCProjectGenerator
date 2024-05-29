#include <gtest/gtest.h>

#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "memory_macro.hpp"

#include "vpg_object_file_generation_service.hpp"

using namespace vcc;

class VPGObjectFileGenerationServiceTest : public testing::Test 
{
    GETSET_SPTR(LogProperty, LogProperty);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGObjectFileGenerationServiceTest/");
    GETSET(std::wstring, FilePathHpp, L"");
    public:
        void SetUp() override
        {
            this->_LogProperty->SetIsConsoleLog(false);
            std::filesystem::remove_all(PATH(this->GetWorkspace()));

            this->_FilePathHpp = ConcatPaths({this->_Workspace, L"vcc_object.hpp"});
        }

        void TearDown() override
        {
            std::filesystem::remove_all(PATH(this->GetWorkspace()));
        }
};

TEST_F(VPGObjectFileGenerationServiceTest, Empty)
{
    std::wstring classPrefix = L"";
    std::map<std::wstring, std::wstring> projectClassIncludeFiles;
    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
    VPGObjectFileGenerationService::Generate(this->GetLogProperty().get(), classPrefix, projectClassIncludeFiles,
        this->GetFilePathHpp(), enumClassList);    
    EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));
}

TEST_F(VPGObjectFileGenerationServiceTest, Single)
{
    std::wstring classPrefix = L"VPG";
    std::map<std::wstring, std::wstring> projectClassIncludeFiles;
    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;

    projectClassIncludeFiles.insert(std::make_pair(L"VPGClassA", L"vpg_class_a.hpp"));
    projectClassIncludeFiles.insert(std::make_pair(L"VPGClassB", L"vpg_class_b.hpp"));
    projectClassIncludeFiles.insert(std::make_pair(L"VPGClassC", L"vpg_class_c.hpp"));

    DECLARE_SPTR(VPGEnumClass, enumClass);
    enumClass->SetName(L"VPGObjectProperty");

    DECLARE_SPTR(VPGEnumClassProperty, enumClassPropertyA);
    enumClassPropertyA->SetEnum(L"EnumA");
    enumClassPropertyA->SetMacro(L"GETSET(std::wstring, EnumA, L\"\")");
    enumClassPropertyA->SetType1(L"std::wstring");
    enumClassPropertyA->SetType2(L"");
    enumClassPropertyA->SetPropertyName(L"EnumA");
    enumClassPropertyA->SetDefaultValue(L"");
    enumClass->InsertProperties(enumClassPropertyA);

    DECLARE_SPTR(VPGEnumClassProperty, enumClassPropertyB);
    enumClassPropertyB->SetEnum(L"EnumB");
    enumClassPropertyB->SetMacro(L"MAP(int, std::wstring, EnumB)");
    enumClassPropertyB->SetType1(L"int");
    enumClassPropertyB->SetType2(L"std::wstring");
    enumClassPropertyB->SetPropertyName(L"EnumB");
    enumClassPropertyB->SetDefaultValue(L"");
    enumClass->InsertProperties(enumClassPropertyB);

    enumClassList.push_back(enumClass);
    VPGObjectFileGenerationService::Generate(this->GetLogProperty().get(), classPrefix, projectClassIncludeFiles,
        this->GetFilePathHpp(), enumClassList);
    EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));

    std::wstring content = ReadFile(this->GetFilePathHpp());
    std::wstring exptectedResult = L""
        L"#pragma once\r\n"
        L"\r\n"
        L"#include <string>\r\n"
        L"\r\n"
        L"#include \"base_object.hpp\"\r\n"
        L"#include \"class_macro.hpp\"\r\n"
        L"#include \"object_type.hpp\"\r\n"
        L"\r\n"
        L"using namespace vcc;\r\n"
        L"\r\n"
        L"class VPGObject : public BaseObject<VPGObject>\r\n"
        L"{\r\n"
        L"    GETSET(std::wstring, EnumA, L\"\")\r\n"
        L"    MAP(int, std::wstring, EnumB)\r\n"
        L"\r\n"
        L"    public:\r\n"
        L"        VPGObject() : BaseObject(ObjectType::Object) {}\r\n"
        L"        virtual ~VPGObject() {}\r\n"
        L"};\r\n";
    EXPECT_EQ(content, exptectedResult);
}

TEST_F(VPGObjectFileGenerationServiceTest, Object)
{
    std::wstring classPrefix = L"VPG";
    std::map<std::wstring, std::wstring> projectClassIncludeFiles;
    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;

    projectClassIncludeFiles.insert(std::make_pair(L"VPGClassA", L"vpg_class_a.hpp"));
    projectClassIncludeFiles.insert(std::make_pair(L"VPGClassB", L"vpg_class_b.hpp"));
    projectClassIncludeFiles.insert(std::make_pair(L"VPGClassC", L"vpg_class_c.hpp"));

    DECLARE_SPTR(VPGEnumClass, enumClass);
    enumClass->SetName(L"VPGObjectProperty");

    DECLARE_SPTR(VPGEnumClassProperty, enumClassPropertyA);
    enumClassPropertyA->SetEnum(L"EnumA");
    enumClassPropertyA->SetMacro(L"GETSET_SPTR(VPGClassA, EnumA)");
    enumClassPropertyA->SetType1(L"VPGClassA");
    enumClassPropertyA->SetType2(L"");
    enumClassPropertyA->SetPropertyName(L"EnumA");
    enumClassPropertyA->SetDefaultValue(L"");
    enumClass->InsertProperties(enumClassPropertyA);

    DECLARE_SPTR(VPGEnumClassProperty, enumClassPropertyB);
    enumClassPropertyB->SetEnum(L"EnumB");
    enumClassPropertyB->SetMacro(L"MAP_SPTR_R(std::wstring, VPGClassB, EnumB)");
    enumClassPropertyB->SetType1(L"std::wstring");
    enumClassPropertyB->SetType2(L"VPGClassB");
    enumClassPropertyB->SetPropertyName(L"EnumB");
    enumClassPropertyB->SetDefaultValue(L"");
    enumClass->InsertProperties(enumClassPropertyB);

    enumClassList.push_back(enumClass);
    VPGObjectFileGenerationService::Generate(this->GetLogProperty().get(), classPrefix, projectClassIncludeFiles,
        this->GetFilePathHpp(), enumClassList);
    EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));

    std::wstring content = ReadFile(this->GetFilePathHpp());
    std::wstring exptectedResult = L""
        L"#pragma once\r\n"
        L"\r\n"
        L"#include <string>\r\n"
        L"\r\n"
        L"#include \"base_object.hpp\"\r\n"
        L"#include \"class_macro.hpp\"\r\n"
        L"#include \"object_type.hpp\"\r\n"
        L"#include \"vpg_class_a.hpp\"\r\n"
        L"#include \"vpg_class_b.hpp\"\r\n"
        L"\r\n"
        L"using namespace vcc;\r\n"
        L"\r\n"
        L"class VPGObject : public BaseObject<VPGObject>\r\n"
        L"{\r\n"
        L"    GETSET_SPTR(VPGClassA, EnumA)\r\n"
        L"    MAP_SPTR_R(std::wstring, VPGClassB, EnumB)\r\n"
        L"\r\n"
        L"    public:\r\n"
        L"        VPGObject() : BaseObject(ObjectType::Object) {}\r\n"
        L"        virtual ~VPGObject() {}\r\n"
        L"\r\n"
        L"        virtual std::shared_ptr<IObject> Clone() const override {\r\n"
        L"            std::shared_ptr<VPGObject> obj = std::make_shared<VPGObject>(*this);\r\n"
        L"            obj->CloneEnumA(this->_EnumA);\r\n"
        L"            obj->CloneEnumB(this->_EnumB);\r\n"
        L"            return obj;\r\n"
        L"        }\r\n"
        L"};\r\n";
    EXPECT_EQ(content, exptectedResult);
}

TEST_F(VPGObjectFileGenerationServiceTest, Multi)
{
    std::wstring classPrefix = L"VPG";
    std::map<std::wstring, std::wstring> projectClassIncludeFiles;
    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;

    projectClassIncludeFiles.insert(std::make_pair(L"VPGClassA", L"vpg_class_a.hpp"));
    projectClassIncludeFiles.insert(std::make_pair(L"VPGClassB", L"vpg_class_b.hpp"));
    projectClassIncludeFiles.insert(std::make_pair(L"VPGClassC", L"vpg_class_c.hpp"));

    DECLARE_SPTR(VPGEnumClass, enumClassA);
    enumClassA->SetName(L"VPGObjectAProperty");
    DECLARE_SPTR(VPGEnumClassProperty, enumClassPropertyA);
    enumClassPropertyA->SetEnum(L"EnumA");
    enumClassPropertyA->SetMacro(L"GETSET(std::wstring, EnumA, L\"\")");
    enumClassPropertyA->SetType1(L"std::wstring");
    enumClassPropertyA->SetType2(L"");
    enumClassPropertyA->SetPropertyName(L"EnumA");
    enumClassPropertyA->SetDefaultValue(L"");
    enumClassA->InsertProperties(enumClassPropertyA);
    enumClassList.push_back(enumClassA);

    DECLARE_SPTR(VPGEnumClass, enumClassB);
    enumClassB->SetName(L"VPGObjectBProperty");
    DECLARE_SPTR(VPGEnumClassProperty, enumClassPropertyB);
    enumClassPropertyB->SetEnum(L"EnumA");
    enumClassPropertyB->SetMacro(L"GETSET(std::wstring, EnumA, L\"\")");
    enumClassPropertyB->SetType1(L"std::wstring");
    enumClassPropertyB->SetType2(L"std::wstring");
    enumClassPropertyB->SetPropertyName(L"EnumA");
    enumClassPropertyB->SetDefaultValue(L"");
    enumClassB->InsertProperties(enumClassPropertyB);
    enumClassList.push_back(enumClassB);
    
    VPGObjectFileGenerationService::Generate(this->GetLogProperty().get(), classPrefix, projectClassIncludeFiles,
        this->GetFilePathHpp(), enumClassList);
    EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));

    std::wstring content = ReadFile(this->GetFilePathHpp());
    std::wstring exptectedResult = L""
        L"#pragma once\r\n"
        L"\r\n"
        L"#include <string>\r\n"
        L"\r\n"
        L"#include \"base_object.hpp\"\r\n"
        L"#include \"class_macro.hpp\"\r\n"
        L"#include \"object_type.hpp\"\r\n"
        L"\r\n"
        L"using namespace vcc;\r\n"
        L"\r\n"
        L"class VPGObjectA : public BaseObject<VPGObjectA>\r\n"
        L"{\r\n"
        L"    GETSET(std::wstring, EnumA, L\"\")\r\n"
        L"\r\n"
        L"    public:\r\n"
        L"        VPGObjectA() : BaseObject(ObjectType::ObjectA) {}\r\n"
        L"        virtual ~VPGObjectA() {}\r\n"
        L"};\r\n"
        L"\r\n"
        L"class VPGObjectB : public BaseObject<VPGObjectB>\r\n"
        L"{\r\n"
        L"    GETSET(std::wstring, EnumA, L\"\")\r\n"
        L"\r\n"
        L"    public:\r\n"
        L"        VPGObjectB() : BaseObject(ObjectType::ObjectB) {}\r\n"
        L"        virtual ~VPGObjectB() {}\r\n"
        L"};\r\n";
    EXPECT_EQ(content, exptectedResult);
}
