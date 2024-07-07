#include <gtest/gtest.h>

#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "memory_macro.hpp"

#include "vpg_file_generation_manager.hpp"
#include "vpg_enum_class_reader.hpp"
#include "vpg_include_path_service.hpp"
#include "vpg_object_file_generation_service.hpp"

using namespace vcc;

class VPGObjectFileGenerationServiceTest : public testing::Test 
{
    GETSET_SPTR(LogProperty, LogProperty);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGObjectFileGenerationServiceTest/");
    GETSET(std::wstring, FilePathHpp, L"");
    GETSET(std::wstring, FilePathCpp, L"");

    MANAGER(VPGEnumClassReader, Reader);
    MAP(std::wstring, std::wstring, IncludeFiles);
    MAP_SPTR_R(std::wstring, VPGEnumClass, EnumClasses);

    public:
        void SetUp() override
        {
            this->_LogProperty->SetIsConsoleLog(false);
            std::filesystem::remove_all(PATH(this->GetWorkspace()));

            this->_FilePathHpp = ConcatPaths({this->_Workspace, L"vcc_object.hpp"});
            this->_FilePathCpp = ConcatPaths({this->_Workspace, L"vcc_object.cpp"});

            DECLARE_UPTR(VPGFileGenerationManager, manager, nullptr);
            manager->GetClassMacroList(L"");
            _Reader->InsertClassMacroList(manager->GetClassMacros());

            VPGIncludePathService::GetWorkspaceIncludePath(L"", manager->GetClassMacros(), _IncludeFiles, _EnumClasses);
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
    VPGObjectFileGenerationService::GenerateHpp(this->GetLogProperty().get(), classPrefix, projectClassIncludeFiles,
        this->GetFilePathHpp(), enumClassList);    
    EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));
    EXPECT_FALSE(IsFileExists(this->GetFilePathCpp()));
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
    VPGObjectFileGenerationService::GenerateHpp(this->GetLogProperty().get(), classPrefix, projectClassIncludeFiles,
        this->GetFilePathHpp(), enumClassList);
    EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));
    EXPECT_FALSE(IsFileExists(this->GetFilePathCpp()));

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
    VPGObjectFileGenerationService::GenerateHpp(this->GetLogProperty().get(), classPrefix, projectClassIncludeFiles,
        this->GetFilePathHpp(), enumClassList);
    EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));
    EXPECT_FALSE(IsFileExists(this->GetFilePathCpp()));

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
    
    VPGObjectFileGenerationService::GenerateHpp(this->GetLogProperty().get(), classPrefix, projectClassIncludeFiles,
        this->GetFilePathHpp(), enumClassList);
    EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));
    EXPECT_FALSE(IsFileExists(this->GetFilePathCpp()));

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

TEST_F(VPGObjectFileGenerationServiceTest, Json)
{
    // Camel Case = camelCase
    // Snake Case = snake_case
    // Pascal Case = PascalCase
    // Kebab Case = kebab-case
    // Uppercase = UPPERCASE
    // Lowercase = lowercase
    // dot-separated lowercase = my.variable.name
    std::wstring enumClass = L""
        "#pragma once\r\n"
        "\r\n"
        "// @@Json\r\n"
        "enum class VPGObjectProperty\r\n"
        "{\r\n"
        "    Boolean, // GETSET(bool, Boolean, false)\r\n"
        "    Integer, // GETSET(int, Integer, 0)\r\n"
        "    Enum, // GETSET(JsonInternalType, Enum, JsonInternalType::String)\r\n"
        "    Double, // GETSET(double, Double, 0)\r\n"
        "    String, // GETSET(std::string, String, \"\")\r\n"
        "    Wstring, // GETSET(std::wstring, Wstring, L\"\")\r\n"
        "    Vector, // VECTOR(int, Vector)\r\n"
        "    Map, // MAP(int, int, Map)\r\n"
        "    Set, // SET(int, Set)\r\n"
        "    OrderedMap, // ORDERED_MAP(int, int, OrderedMap)\r\n"
        "    Object, // GETSET_SPTR(VPGObject, Object)\r\n"
        "    VectorObject, // VECTOR_SPTR(VPGObject, VectorObject)\r\n"
        "    MapObject, // MAP_SPTR_R(int, VPGObject, MapObject)\r\n"
        "    SetObject, // SET_SPTR(VPGObject, SetObject)\r\n"
        "    OrdredMapObject // ORDERED_MAP_SPTR_R(int, VPGObject, OrdredMapObject)\r\n"
        "};\r\n";
    WriteFile(ConcatPaths({this->_Workspace, L"vcc_object_property.hpp"}), enumClass, true);

    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
    this->GetReader()->Parse(enumClass, enumClassList);

    std::wstring classPrefix = L"VPG";
    std::map<std::wstring, std::wstring> projectClassIncludeFiles;

    projectClassIncludeFiles.insert(std::make_pair(L"VPGObject", L"vcc_object.hpp"));
    projectClassIncludeFiles.insert(std::make_pair(L"JsonInternalType", L"json.hpp"));

    VPGObjectFileGenerationService::GenerateHpp(this->GetLogProperty().get(), classPrefix, projectClassIncludeFiles,
        this->GetFilePathHpp(), enumClassList);
    VPGObjectFileGenerationService::GenerateCpp(this->GetLogProperty().get(), this->GetIncludeFiles(), this->GetEnumClasses(),
        this->GetFilePathCpp(), enumClassList);
    EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));
    EXPECT_TRUE(IsFileExists(this->GetFilePathCpp()));

    EXPECT_EQ(ReadFile(this->GetFilePathHpp()),
        L"#pragma once\r\n"
        L"\r\n"
        L"#include <string>\r\n"
        L"\r\n"
        L"#include \"base_json_object.hpp\"\r\n"
        L"#include \"base_object.hpp\"\r\n"
        L"#include \"class_macro.hpp\"\r\n"
        L"#include \"i_document.hpp\"\r\n"
        L"#include \"json.hpp\"\r\n"
        L"#include \"object_type.hpp\"\r\n"
        L"\r\n"
        L"using namespace vcc;\r\n"
        L"\r\n"
        L"class VPGObject : public BaseObject<VPGObject>, public BaseJsonObject\r\n"
        L"{\r\n"
        L"    GETSET(bool, Boolean, false)\r\n"
        L"    GETSET(int, Integer, 0)\r\n"
        L"    GETSET(JsonInternalType, Enum, JsonInternalType::String)\r\n"
        L"    GETSET(double, Double, 0)\r\n"
        L"    GETSET(std::string, String, \"\")\r\n"
        L"    GETSET(std::wstring, Wstring, L\"\")\r\n"
        L"    VECTOR(int, Vector)\r\n"
        L"    MAP(int, int, Map)\r\n"
        L"    SET(int, Set)\r\n"
        L"    ORDERED_MAP(int, int, OrderedMap)\r\n"
        L"    GETSET_SPTR(VPGObject, Object)\r\n"
        L"    VECTOR_SPTR(VPGObject, VectorObject)\r\n"
        L"    MAP_SPTR_R(int, VPGObject, MapObject)\r\n"
        L"    SET_SPTR(VPGObject, SetObject)\r\n"
        L"    ORDERED_MAP_SPTR_R(int, VPGObject, OrdredMapObject)\r\n"
        L"\r\n"
        L"    public:\r\n"
        L"        VPGObject() : BaseObject(ObjectType::Object) {}\r\n"
        L"        virtual ~VPGObject() {}\r\n"
        L"\r\n"
        L"        virtual std::shared_ptr<IObject> Clone() const override {\r\n"
        L"            std::shared_ptr<VPGObject> obj = std::make_shared<VPGObject>(*this);\r\n"
        L"            obj->CloneObject(this->_Object);\r\n"
        L"            obj->CloneVectorObject(this->_VectorObject);\r\n"
        L"            obj->CloneMapObject(this->_MapObject);\r\n"
        L"            obj->CloneSetObject(this->_SetObject);\r\n"
        L"            obj->CloneOrdredMapObject(this->_OrdredMapObject);\r\n"
        L"            return obj;\r\n"
        L"        }\r\n"
        L"\r\n"
        L"        virtual std::shared_ptr<Json> ToJson() const override;\r\n"
        L"        virtual void DeserializeJson(std::shared_ptr<IDocument> document) const override;\r\n"
        L"};\r\n");

    EXPECT_EQ(ReadFile(this->GetFilePathCpp()),
        L"#include \"vcc_object.hpp\"\r\n"
        "\r\n"
        "#include <assert.h>\r\n"
        "#include <memory>\r\n"
        "#include <string>\r\n"
        "\r\n"
        "#include \"exception_macro.hpp\"\r\n"
        "#include \"i_document.hpp\"\r\n"
        "#include \"i_document_builder.hpp\"\r\n"
        "#include \"json.hpp\"\r\n"
        "#include \"memory_macro.hpp\"\r\n"
        "#include \"number_helper.hpp\"\r\n"
        "#include \"string_helper.hpp\"\r\n"
        "\r\n"
        "using namespace vcc;\r\n"
        "\r\n"
        "std::shared_ptr<Json> VPGObject::ToJson() const\r\n"
        "{\r\n"
        "    TRY\r\n"
        "        DECLARE_UPTR(Json, json);\r\n"
        "        // Boolean\r\n"
        "        json->AddBool(L\"Boolean\", _Boolean);\r\n"
        "        // Integer\r\n"
        "        json->AddInt(L\"Integer\", _Integer);\r\n"
        "        // Enum\r\n"
        "        switch (_Enum)\r\n"
        "        {\r\n"
        "        case JsonInternalType::Array:\r\n"
        "            json->AddString(L\"Enum\", L\"Array\");\r\n"
        "            break;\r\n"
        "        case JsonInternalType::Boolean:\r\n"
        "            json->AddString(L\"Enum\", L\"Boolean\");\r\n"
        "            break;\r\n"
        "        case JsonInternalType::Number:\r\n"
        "            json->AddString(L\"Enum\", L\"Number\");\r\n"
        "            break;\r\n"
        "        case JsonInternalType::Null:\r\n"
        "            json->AddString(L\"Enum\", L\"Null\");\r\n"
        "            break;\r\n"
        "        case JsonInternalType::Object:\r\n"
        "            json->AddString(L\"Enum\", L\"Object\");\r\n"
        "            break;\r\n"
        "        case JsonInternalType::Json:\r\n"
        "            json->AddString(L\"Enum\", L\"Json\");\r\n"
        "            break;\r\n"
        "        case JsonInternalType::String:\r\n"
        "            json->AddString(L\"Enum\", L\"String\");\r\n"
        "            break;\r\n"
        "        default:\r\n"
        "            assert(false);\r\n"
        "            break;\r\n"
        "        }\r\n"
        "        // Double\r\n"
        "        json->AddDouble(L\"Double\", _Double, GetDecimalPlaces(_Double));\r\n"
        "        // String\r\n"
        "        json->AddString(L\"String\", str2wstr(_String));\r\n"
        "        // Wstring\r\n"
        "        json->AddString(L\"Wstring\", _Wstring);\r\n"
        "        // Vector\r\n"
        "        DECLARE_SPTR(Json, tmpVector);\r\n"
        "        json->AddArray(L\"Vector\", tmpVector);\r\n"
        "        for (auto const &element : _Vector) {\r\n"
        "            tmpVector->AddArrayInt(element);\r\n"
        "        }\r\n"
        "        // Map\r\n"
        "        DECLARE_SPTR(Json, tmpMap);\r\n"
        "        json->AddObject(L\"Map\", tmpMap);\r\n"
        "        for (auto const &element : _Map) {\r\n"
        "            tmpMap->AddInt(std::to_wstring(element.first), element.second);\r\n"
        "        }\r\n"
        "        // Set\r\n"
        "        DECLARE_SPTR(Json, tmpSet);\r\n"
        "        json->AddArray(L\"Set\", tmpSet);\r\n"
        "        for (auto const &element : _Set) {\r\n"
        "            tmpSet->AddArrayInt(element);\r\n"
        "        }\r\n"
        "        // OrderedMap\r\n"
        "        DECLARE_SPTR(Json, tmpOrderedMap);\r\n"
        "        json->AddObject(L\"OrderedMap\", tmpOrderedMap);\r\n"
        "        for (auto const &element : _OrderedMap) {\r\n"
        "            tmpOrderedMap->AddInt(std::to_wstring(element.first), element.second);\r\n"
        "        }\r\n"
        "        // Object\r\n"
        "        json->AddObject(L\"Object\", _Object->ToJson());\r\n"
        "        // VectorObject\r\n"
        "        DECLARE_SPTR(Json, tmpVectorObject);\r\n"
        "        json->AddArray(L\"VectorObject\", tmpVectorObject);\r\n"
        "        for (auto const &element : _VectorObject) {\r\n"
        "            tmpVectorObject->AddArrayObject(element->ToJson());\r\n"
        "        }\r\n"
        "        // MapObject\r\n"
        "        DECLARE_SPTR(Json, tmpMapObject);\r\n"
        "        json->AddObject(L\"MapObject\", tmpMapObject);\r\n"
        "        for (auto const &element : _MapObject) {\r\n"
        "            tmpMapObject->AddObject(std::to_wstring(element.first), element->ToJson());\r\n"
        "        }\r\n"
        "        // SetObject\r\n"
        "        DECLARE_SPTR(Json, tmpSetObject);\r\n"
        "        json->AddArray(L\"SetObject\", tmpSetObject);\r\n"
        "        for (auto const &element : _SetObject) {\r\n"
        "            tmpSetObject->AddArrayObject(element->ToJson());\r\n"
        "        }\r\n"
        "        // OrdredMapObject\r\n"
        "        DECLARE_SPTR(Json, tmpOrdredMapObject);\r\n"
        "        json->AddObject(L\"OrdredMapObject\", tmpOrdredMapObject);\r\n"
        "        for (auto const &element : _OrdredMapObject) {\r\n"
        "            tmpOrdredMapObject->AddObject(std::to_wstring(element.first), element->ToJson());\r\n"
        "        }\r\n"
        "        return json;\r\n"
        "    CATCH\r\n"
        "    return nullptr;\r\n"
        "}\r\n"
        "\r\n"
        "void VPGObject::DeserializeJson(std::shared_ptr<IDocument> document) const\r\n"
        "{\r\n"
        "    TRY\r\n"
        "        std::shared_ptr<Json> json = std::dynamic_pointer_cast<Json>(document);\r\n"
        "        assert(json != nullptr);\r\n"
        "        // Boolean\r\n"
        "        if (json->IsContainKey(L\"Boolean\"))\r\n"
        "            _Boolean = json->GetBool(L\"Boolean\");\r\n"
        "        // Integer\r\n"
        "        if (json->IsContainKey(L\"Integer\"))\r\n"
        "            _Integer = static_cast<int>(json->GetInt64(L\"Integer\"));\r\n"
        "        // Enum\r\n"
        "        if (json->IsContainKey(L\"Enum\")) {\r\n"
        "            std::wstring tmpEnum = json->GetString(L\"Enum\");\r\n"
        "            ToUpper(tmpEnum);\r\n"
        "            if (tmpEnum == L\"ARRAY\")\r\n"
        "                _Enum = JsonInternalType::Array;\r\n"
        "            else if (tmpEnum == L\"BOOLEAN\")\r\n"
        "                _Enum = JsonInternalType::Boolean;\r\n"
        "            else if (tmpEnum == L\"NUMBER\")\r\n"
        "                _Enum = JsonInternalType::Number;\r\n"
        "            else if (tmpEnum == L\"NULL\")\r\n"
        "                _Enum = JsonInternalType::Null;\r\n"
        "            else if (tmpEnum == L\"OBJECT\")\r\n"
        "                _Enum = JsonInternalType::Object;\r\n"
        "            else if (tmpEnum == L\"JSON\")\r\n"
        "                _Enum = JsonInternalType::Json;\r\n"
        "            else if (tmpEnum == L\"STRING\")\r\n"
        "                _Enum = JsonInternalType::String;\r\n"
        "            else\r\n"
        "                THROW_EXCEPTION_MSG(ExceptionType::ParserError, L\"Unknow Interface: \" + tmpEnum);\r\n"
        "        }\r\n"
        "        // Double\r\n"
        "        if (json->IsContainKey(L\"Double\"))\r\n"
        "            _Double = static_cast<double>(json->GetDouble(L\"Double\"));\r\n"
        "        // String\r\n"
        "        if (json->IsContainKey(L\"String\"))\r\n"
        "            _String = wstr2str(json->GetString(L\"String\"));\r\n"
        "        // Wstring\r\n"
        "        if (json->IsContainKey(L\"Wstring\"))\r\n"
        "            _Wstring = json->GetString(L\"Wstring\");\r\n"
        "        // Vector\r\n"
        "        ClearVector();\r\n"
        "        if (json->IsContainKey(L\"Vector\")) {\r\n"
        "            for (auto const &element : json->GetArray(L\"Vector\")) {\r\n"
        "                InsertVector(static_cast<int>(element->GetArrayElementInt64()));\r\n"
        "            }\r\n"
        "        }\r\n"
        "        // Map\r\n"
        "        ClearMap();\r\n"
        "        if (json->IsContainKey(L\"Map\") && json->GetObject(L\"Map\") != nullptr) {\r\n"
        "            std::shared_ptr<Json> tmpObject = json->GetObject(L\"Map\");\r\n"
        "            std::vector<std::wstring> tmpKeys = tmpObject->GetKeys();\r\n"
        "            for (auto const &key : tmpKeys) {\r\n"
        "                InsertMap(std::stoi(key), static_cast<int>(tmpObject->GetInt64(key)));\r\n"
        "            }\r\n"
        "        }\r\n"
        "        // Set\r\n"
        "        ClearSet();\r\n"
        "        if (json->IsContainKey(L\"Set\")) {\r\n"
        "            for (auto const &element : json->GetArray(L\"Set\")) {\r\n"
        "                InsertSet(static_cast<int>(element->GetArrayElementInt64()));\r\n"
        "            }\r\n"
        "        }\r\n"
        "        // OrderedMap\r\n"
        "        ClearOrderedMap();\r\n"
        "        if (json->IsContainKey(L\"OrderedMap\") && json->GetObject(L\"OrderedMap\") != nullptr) {\r\n"
        "            std::shared_ptr<Json> tmpObject = json->GetObject(L\"OrderedMap\");\r\n"
        "            std::vector<std::wstring> tmpKeys = tmpObject->GetKeys();\r\n"
        "            for (auto const &key : tmpKeys) {\r\n"
        "                InsertOrderedMap(std::stoi(key), static_cast<int>(tmpObject->GetInt64(key)));\r\n"
        "            }\r\n"
        "        }\r\n"
        "        // Object\r\n"
        "        _Object = nullptr;\r\n"
        "        if (json->IsContainKey(L\"Object\") && json->GetObject(L\"Object\") != nullptr)\r\n"
        "            _Object->DeserializeJson(json->GetObject(L\"Object\"));\r\n"
        "        // VectorObject\r\n"
        "        ClearVectorObject();\r\n"
        "        if (json->IsContainKey(L\"VectorObject\")) {\r\n"
        "            for (auto const &element : json->GetArray(L\"VectorObject\")) {\r\n"
        "                DECLARE_SPTR(VPGObject, tmpVectorObject);\r\n"
        "                tmpVectorObject->DeserializeJson(element);\r\n"
        "                InsertVectorObject(tmpVectorObject);\r\n"
        "            }\r\n"
        "        }\r\n"
        "        // MapObject\r\n"
        "        ClearMapObject();\r\n"
        "        if (json->IsContainKey(L\"MapObject\") && json->GetObject(L\"MapObject\") != nullptr) {\r\n"
        "            std::shared_ptr<Json> tmpObject = json->GetObject(L\"MapObject\");\r\n"
        "            std::vector<std::wstring> tmpKeys = tmpObject->GetKeys();\r\n"
        "            for (auto const &key : tmpKeys) {\r\n"
        "                if (tmpObject->GetObject(key) != nullptr) {\r\n"
        "                    DECLARE_SPTR(VPGObject, tmpElementObject);\r\n"
        "                    tmpElementObject->DeserializeJson(tmpObject->GetObject(key));\r\n"
        "                    InsertMapObject(std::stoi(key), tmpElementObject);\r\n"
        "                } else\r\n"
        "                    InsertMapObject(std::stoi(key), nullptr);\r\n"
        "            }\r\n"
        "        }\r\n"
        "        // SetObject\r\n"
        "        ClearSetObject();\r\n"
        "        if (json->IsContainKey(L\"SetObject\")) {\r\n"
        "            for (auto const &element : json->GetArray(L\"SetObject\")) {\r\n"
        "                DECLARE_SPTR(VPGObject, tmpSetObject);\r\n"
        "                tmpSetObject->DeserializeJson(element);\r\n"
        "                InsertSetObject(tmpSetObject);\r\n"
        "            }\r\n"
        "        }\r\n"
        "        // OrdredMapObject\r\n"
        "        ClearOrdredMapObject();\r\n"
        "        if (json->IsContainKey(L\"OrdredMapObject\") && json->GetObject(L\"OrdredMapObject\") != nullptr) {\r\n"
        "            std::shared_ptr<Json> tmpObject = json->GetObject(L\"OrdredMapObject\");\r\n"
        "            std::vector<std::wstring> tmpKeys = tmpObject->GetKeys();\r\n"
        "            for (auto const &key : tmpKeys) {\r\n"
        "                if (tmpObject->GetObject(key) != nullptr) {\r\n"
        "                    DECLARE_SPTR(VPGObject, tmpElementObject);\r\n"
        "                    tmpElementObject->DeserializeJson(tmpObject->GetObject(key));\r\n"
        "                    InsertOrdredMapObject(std::stoi(key), tmpElementObject);\r\n"
        "                } else\r\n"
        "                    InsertOrdredMapObject(std::stoi(key), nullptr);\r\n"
        "            }\r\n"
        "        }\r\n"
        "    CATCH\r\n"
        "}\r\n");
}

// TEST_F(VPGObjectFileGenerationServiceTest, Json_Multi)
// {
//     std::wstring classPrefix = L"VPG";
//     std::map<std::wstring, std::wstring> projectClassIncludeFiles;
//     std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;

//     projectClassIncludeFiles.insert(std::make_pair(L"VPGClassA", L"vpg_class_a.hpp"));
//     projectClassIncludeFiles.insert(std::make_pair(L"VPGClassB", L"vpg_class_b.hpp"));
//     projectClassIncludeFiles.insert(std::make_pair(L"VPGClassC", L"vpg_class_c.hpp"));

//     // Camel Case = camelCase
//     // Snake Case = snake_case
//     // Pascal Case = PascalCase
//     // Kebab Case = kebab-case
//     // Uppercase = UPPERCASE
//     // Lowercase = lowercase
//     // dot-separated lowercase = my.variable.name
//     DECLARE_SPTR(VPGEnumClass, enumClassA);
//     enumClassA->SetName(L"VPGObjectProperty");
//     enumClassA->SetCommand(L"@@Json{ \"Key.NamingStyle\": \"Camel\" }");
//     DECLARE_SPTR(VPGEnumClassProperty, enumClassPropertyA);
//     enumClassPropertyA->SetEnum(L"EnumA");
//     enumClassPropertyA->SetMacro(L"GETSET(std::wstring, EnumA, L\"\")");
//     enumClassPropertyA->SetType1(L"std::wstring");
//     enumClassPropertyA->SetType2(L"");
//     enumClassPropertyA->SetPropertyName(L"EnumA");
//     enumClassPropertyA->SetDefaultValue(L"");
//     enumClassA->InsertProperties(enumClassPropertyA);
//     enumClassList.push_back(enumClassA);
    
//     VPGObjectFileGenerationService::GenerateHpp(this->GetLogProperty().get(), classPrefix, projectClassIncludeFiles,
//         this->GetFilePathHpp(), enumClassList);
//     VPGObjectFileGenerationService::GenerateCpp(this->GetLogProperty().get(), classPrefix, projectClassIncludeFiles,
//         this->GetFilePathHpp(), enumClassList);
//     EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));
//     EXPECT_TRUE(IsFileExists(this->GetFilePathCpp()));

//     EXPECT_EQ(ReadFile(this->GetFilePathHpp()),
//         L"#pragma once\r\n"
//         L"\r\n"
//         L"#include <string>\r\n"
//         L"\r\n"
//         L"#include \"base_object.hpp\"\r\n"
//         L"#include \"base_json_object.hpp\"\r\n"
//         L"#include \"class_macro.hpp\"\r\n"
//         L"#include \"object_type.hpp\"\r\n"
//         L"\r\n"
//         L"using namespace vcc;\r\n"
//         L"\r\n"
//         L"class VPGObject : public BaseObject<VPGObjectA>, public BaseJsonObject\r\n"
//         L"{\r\n"
//         L"    GETSET(std::wstring, EnumA, L\"\")\r\n"
//         L"\r\n"
//         L"    public:\r\n"
//         L"        VPGObjectA() : BaseObject(ObjectType::ObjectA) {}\r\n"
//         L"        virtual ~VPGObjectA() {}\r\n"
//         L"\r\n"
//         L"        virtual std::shared_ptr<Json> ToJson() const override;\r\n"
//         L"        virtual void DeserializeJson(std::shared_ptr<IDocument> document) const override;\r\n"
//         L"};\r\n");

//     EXPECT_EQ(ReadFile(this->GetFilePathCpp()),
//         L"#pragma once\r\n"
//         L"\r\n"
//         L"#include <string>\r\n"
//         L"\r\n"
//         L"#include \"base_object.hpp\"\r\n"
//         L"#include \"class_macro.hpp\"\r\n"
//         L"#include \"object_type.hpp\"\r\n"
//         L"\r\n"
//         L"using namespace vcc;\r\n"
//         L"\r\n"
//         L"class VPGObject : public BaseObject<VPGObjectA>\r\n"
//         L"{\r\n"
//         L"    GETSET(std::wstring, EnumA, L\"\")\r\n"
//         L"\r\n"
//         L"    public:\r\n"
//         L"        VPGObjectA() : BaseObject(ObjectType::ObjectA) {}\r\n"
//         L"        virtual ~VPGObjectA() {}\r\n"
//         L"};\r\n");
// }
