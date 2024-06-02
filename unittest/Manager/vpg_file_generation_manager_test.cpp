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
    GETSET_SPTR(LogProperty, LogProperty, LogPropertyType::None);
    MANAGER(VPGFileGenerationManager, Manager, _LogProperty);

    GETSET(std::wstring, Workspace, L"bin/Debug/FileGenerationServiceTest");
    GETSET(std::wstring, WorkspaceSource, L"");
    GETSET(std::wstring, WorkspaceTarget, L"");

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
            code += L"    EnumC, // GETSET(int64_t, EnumC, 0) \r\n";
            code += L"    EnumD, // GETSET(ExceptionType, EnumD, ExceptionType::NA)\r\n";
            code += L"    EnumE  // VECTOR(ExceptionType, EnumE) \r\n";
            code += L"};\r\n";
            code += L"\r\n";
            code += L"enum class VCCObjectPtrProperty // Class Command\r\n";
            code += L"{\r\n";
            code += L"    EnumA, // GETSET_SPTR(Json, EnumA) \r\n";
            code += L"    EnumB, // GETSET_SPTR(Json, EnumB, 1, 2, 3) \r\n";
            code += L"    EnumC, // VECTOR_SPTR(Json, EnumC) \r\n";
            code += L"    EnumD, // SET_SPTR(Json, EnumD) \r\n";
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
    EXPECT_TRUE(this->GetManager()->GetIncludeFiles().at(L"VCCObject") == L"vcc_a.hpp");
    EXPECT_TRUE(this->GetManager()->GetIncludeFiles().at(L"VCCObjectPtr") == L"vcc_a.hpp");
    EXPECT_TRUE(this->GetManager()->GetIncludeFiles().at(L"VCCObjectProperty") == L"vcc_a_property.hpp");
    EXPECT_TRUE(this->GetManager()->GetIncludeFiles().at(L"VCCObjectPtrProperty") == L"vcc_a_property.hpp");
}

TEST_F(VPGFileGenerationManagerTest, GenerateProperty)
{
    VPGGenerationOption option;
    option.SetProjectPrefix(L"VCC");
    option.SetWorkspaceSource(L"");
    option.SetWorkspaceDestination(L"");
    option.SetTypeWorkspace(this->GetWorkspaceSource());
    option.SetObjectTypeDirectory(this->GetWorkspaceTarget());
    option.SetModelDirectory(this->GetWorkspaceTarget());
    option.SetPropertyAccessorDirectoryHpp(this->GetWorkspaceTarget());
    option.SetPropertyAccessorDirectoryCpp(this->GetWorkspaceTarget());
    option.SetPropertyAccessorFactoryDirectoryHpp(this->GetWorkspaceTarget());
    option.SetPropertyAccessorFactoryDirectoryCpp(this->GetWorkspaceTarget());

    this->GetManager()->GernerateProperty(this->GetLogProperty().get(), &option);

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
    expectedObjectClassFileContent += L"#include \"exception_type.hpp\"\r\n";
    expectedObjectClassFileContent += L"#include \"json.hpp\"\r\n";
    expectedObjectClassFileContent += L"#include \"object_type.hpp\"\r\n";
    expectedObjectClassFileContent += L"\r\n";
    expectedObjectClassFileContent += L"using namespace vcc;\r\n";
    expectedObjectClassFileContent += L"\r\n";
    expectedObjectClassFileContent += L"class VCCObject : public BaseObject<VCCObject>\r\n";
    expectedObjectClassFileContent += L"{\r\n";
    expectedObjectClassFileContent += INDENT + L"GETSET(std::wstring, EnumB, L\"Default\")\r\n";
    expectedObjectClassFileContent += INDENT + L"GETSET(int64_t, EnumC, 0)\r\n";
    expectedObjectClassFileContent += INDENT + L"GETSET(ExceptionType, EnumD, ExceptionType::NA)\r\n";
    expectedObjectClassFileContent += INDENT + L"VECTOR(ExceptionType, EnumE)\r\n";
    expectedObjectClassFileContent += L"\r\n";
    expectedObjectClassFileContent += INDENT + L"public:\r\n";
    expectedObjectClassFileContent += INDENT + INDENT + L"VCCObject() : BaseObject(ObjectType::Object) {}\r\n";
    expectedObjectClassFileContent += INDENT + INDENT + L"virtual ~VCCObject() {}\r\n";
    expectedObjectClassFileContent += L"};\r\n";
    expectedObjectClassFileContent += L"\r\n";
    expectedObjectClassFileContent += L"class VCCObjectPtr : public BaseObject<VCCObjectPtr>\r\n";
    expectedObjectClassFileContent += L"{\r\n";
    expectedObjectClassFileContent += INDENT + L"GETSET_SPTR(Json, EnumA)\r\n";
    expectedObjectClassFileContent += INDENT + L"GETSET_SPTR(Json, EnumB, 1, 2, 3)\r\n";
    expectedObjectClassFileContent += INDENT + L"VECTOR_SPTR(Json, EnumC)\r\n";
    expectedObjectClassFileContent += INDENT + L"SET_SPTR(Json, EnumD)\r\n";
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
    std::wstring objectPropertyAccessorFileContentHpp = ReadFile(ConcatPaths({this->GetWorkspaceTarget(), L"vcc_a_property_accessor.hpp"}));
    std::wstring objectPropertyAccessorFileContentCpp = ReadFile(ConcatPaths({this->GetWorkspaceTarget(), L"vcc_a_property_accessor.cpp"}));
    std::wstring expectedOBjectPropertyAccessorFileContentHpp = L""
        "#pragma once\r\n"
        "\r\n"
        "#include <string>\r\n"
        "\r\n"
        "#include \"base_property_accessor.hpp\"\r\n"
        "#include \"property_accessor_macro.hpp\"\r\n"
        "\r\n"
        "using namespace vcc;\r\n"
        "\r\n"
        "class VCCObjectPropertyAccessor : public BasePropertyAccessor\r\n"
        "{\r\n"
        "    PROPERTY_ACCESSOR_HEADER(long, Long)\r\n"
        "    PROPERTY_ACCESSOR_HEADER(std::wstring, Wstring)\r\n"
        "\r\n"
        "    PROPERTY_ACCESSOR_CONTAINER_HEADER\r\n"
        "\r\n"
        "    public:\r\n"
        "        VCCObjectPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}\r\n"
        "        virtual ~VCCObjectPropertyAccessor() {}\r\n"
        "};\r\n"
        "\r\n"
        "class VCCObjectPtrPropertyAccessor : public BasePropertyAccessor\r\n"
        "{\r\n"
        "    PROPERTY_ACCESSOR_OBJECT_HEADER(std::shared_ptr<IObject>, Object)\r\n"
        "\r\n"
        "    PROPERTY_ACCESSOR_CONTAINER_HEADER\r\n"
        "\r\n"
        "    public:\r\n"
        "        VCCObjectPtrPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}\r\n"
        "        virtual ~VCCObjectPtrPropertyAccessor() {}\r\n"
        "};\r\n";
    std::wstring expectedOBjectPropertyAccessorFileContentCpp = L""
    "#include \"vcc_a_property_accessor.hpp\"\r\n"
    "\r\n"
    "#include <memory>\r\n"
    "#include <string>\r\n"
    "#include <vector>\r\n"
    "\r\n"
    "#include \"exception_macro.hpp\"\r\n"
    "#include \"exception_type.hpp\"\r\n"
    "#include \"i_object.hpp\"\r\n"
    "#include \"i_type_union.hpp\"\r\n"
    "#include \"json.hpp\"\r\n"
    "#include \"property_accessor_key.hpp\"\r\n"
    "#include \"property_accessor_macro.hpp\"\r\n"
    "#include \"vcc_a.hpp\"\r\n"
    "#include \"vcc_a_property.hpp\"\r\n"
    "\r\n"
    "using namespace vcc;\r\n"
    "\r\n"
    "long VCCObjectPropertyAccessor::_ReadLong(const size_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VCCObject> obj = std::static_pointer_cast<VCCObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VCCObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VCCObjectProperty::EnumC:\r\n"
    "            return obj->GetEnumC();\r\n"
    "        case VCCObjectProperty::EnumD:\r\n"
    "            return static_cast<long>(obj->GetEnumD());\r\n"
    "        case VCCObjectProperty::EnumE:\r\n"
    "            return static_cast<long>(obj->GetEnumE(index));\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return 0L;\r\n"
    "}\r\n"
    "\r\n"
    "long VCCObjectPropertyAccessor::_ReadLong(const size_t &objectProperty, const ITypeUnion * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "    return 0L;\r\n"
    "}\r\n"
    "\r\n"
    "void VCCObjectPropertyAccessor::_WriteLong(const size_t &objectProperty, const long &value, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VCCObject> obj = std::static_pointer_cast<VCCObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VCCObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VCCObjectProperty::EnumC:\r\n"
    "            obj->SetEnumC(value);\r\n"
    "            break;\r\n"
    "        case VCCObjectProperty::EnumD:\r\n"
    "            obj->SetEnumD(static_cast<ExceptionType>(value));\r\n"
    "            break;\r\n"
    "        case VCCObjectProperty::EnumE:\r\n"
    "            if (index > -1)\r\n"
    "                obj->SetEnumE(index, static_cast<ExceptionType>(value));\r\n"
    "            else\r\n"
    "                obj->InsertEnumE(static_cast<ExceptionType>(value));\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VCCObjectPropertyAccessor::_WriteLong(const size_t &objectProperty, const long & /*value*/, const ITypeUnion * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "std::wstring VCCObjectPropertyAccessor::_ReadWstring(const size_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VCCObject> obj = std::static_pointer_cast<VCCObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VCCObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VCCObjectProperty::EnumB:\r\n"
    "            return obj->GetEnumB();\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return L\"\";\r\n"
    "}\r\n"
    "\r\n"
    "std::wstring VCCObjectPropertyAccessor::_ReadWstring(const size_t &objectProperty, const ITypeUnion * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "    return L\"\";\r\n"
    "}\r\n"
    "\r\n"
    "void VCCObjectPropertyAccessor::_WriteWstring(const size_t &objectProperty, const std::wstring &value, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VCCObject> obj = std::static_pointer_cast<VCCObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VCCObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VCCObjectProperty::EnumB:\r\n"
    "            obj->SetEnumB(value);\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VCCObjectPropertyAccessor::_WriteWstring(const size_t &objectProperty, const std::wstring & /*value*/, const ITypeUnion * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "size_t VCCObjectPropertyAccessor::_GetContainerCount(const size_t &objectProperty) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VCCObject> obj = std::static_pointer_cast<VCCObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VCCObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VCCObjectProperty::EnumE:\r\n"
    "            return obj->GetEnumE().size();\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return 0;\r\n"
    "}\r\n"
    "\r\n"
    "bool VCCObjectPropertyAccessor::_IsContainKey(const size_t &objectProperty, const ITypeUnion * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "    return false;\r\n"
    "}\r\n"
    "\r\n"
    "void VCCObjectPropertyAccessor::_RemoveContainerElement(const size_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VCCObject> obj = std::static_pointer_cast<VCCObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VCCObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VCCObjectProperty::EnumE:\r\n"
    "            obj->RemoveEnumE(index);\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VCCObjectPropertyAccessor::_RemoveContainerElement(const size_t &objectProperty, const ITypeUnion * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VCCObjectPropertyAccessor::_ClearContainer(const size_t &objectProperty) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VCCObject> obj = std::static_pointer_cast<VCCObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VCCObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VCCObjectProperty::EnumE:\r\n"
    "            obj->ClearEnumE();\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "std::shared_ptr<IObject> VCCObjectPtrPropertyAccessor::_ReadObject(const size_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VCCObjectPtr> obj = std::static_pointer_cast<VCCObjectPtr>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VCCObjectPtrProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VCCObjectPtrProperty::EnumA:\r\n"
    "            return std::static_pointer_cast<IObject>(obj->GetEnumA());\r\n"
    "        case VCCObjectPtrProperty::EnumB:\r\n"
    "            return std::static_pointer_cast<IObject>(obj->GetEnumB());\r\n"
    "        case VCCObjectPtrProperty::EnumC:\r\n"
    "            return std::static_pointer_cast<IObject>(obj->GetEnumC(index));\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return nullptr;\r\n"
    "}\r\n"
    "\r\n"
    "std::shared_ptr<IObject> VCCObjectPtrPropertyAccessor::_ReadObject(const size_t &objectProperty, const ITypeUnion * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "    return nullptr;\r\n"
    "}\r\n"
    "\r\n"
    "void VCCObjectPtrPropertyAccessor::_WriteObject(const size_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VCCObjectPtr> obj = std::static_pointer_cast<VCCObjectPtr>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VCCObjectPtrProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VCCObjectPtrProperty::EnumA:\r\n"
    "            obj->SetEnumA(std::static_pointer_cast<Json>(value));\r\n"
    "            break;\r\n"
    "        case VCCObjectPtrProperty::EnumB:\r\n"
    "            obj->SetEnumB(std::static_pointer_cast<Json>(value));\r\n"
    "            break;\r\n"
    "        case VCCObjectPtrProperty::EnumC:\r\n"
    "            if (index > -1)\r\n"
    "                obj->SetEnumC(index, std::static_pointer_cast<Json>(value));\r\n"
    "            else\r\n"
    "                obj->InsertEnumC(std::static_pointer_cast<Json>(value));\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VCCObjectPtrPropertyAccessor::_WriteObject(const size_t &objectProperty, std::shared_ptr<IObject> /*value*/, const ITypeUnion * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "std::shared_ptr<IObject> VCCObjectPtrPropertyAccessor::_CloneObject(const size_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VCCObjectPtr> obj = std::static_pointer_cast<VCCObjectPtr>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VCCObjectPtrProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VCCObjectPtrProperty::EnumA:\r\n"
    "            return std::static_pointer_cast<IObject>(obj->GetEnumA()->Clone());\r\n"
    "        case VCCObjectPtrProperty::EnumB:\r\n"
    "            return std::static_pointer_cast<IObject>(obj->GetEnumB()->Clone());\r\n"
    "        case VCCObjectPtrProperty::EnumC:\r\n"
    "            return std::static_pointer_cast<IObject>(obj->CloneEnumC(index));\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return nullptr;\r\n"
    "}\r\n"
    "\r\n"
    "std::shared_ptr<IObject> VCCObjectPtrPropertyAccessor::_CloneObject(const size_t &objectProperty, const ITypeUnion * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "    return nullptr;\r\n"
    "}\r\n"
    "\r\n"
    "size_t VCCObjectPtrPropertyAccessor::_GetContainerCount(const size_t &objectProperty) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VCCObjectPtr> obj = std::static_pointer_cast<VCCObjectPtr>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VCCObjectPtrProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VCCObjectPtrProperty::EnumC:\r\n"
    "            return obj->GetEnumC().size();\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return 0;\r\n"
    "}\r\n"
    "\r\n"
    "bool VCCObjectPtrPropertyAccessor::_IsContainKey(const size_t &objectProperty, const ITypeUnion * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "    return false;\r\n"
    "}\r\n"
    "\r\n"
    "void VCCObjectPtrPropertyAccessor::_RemoveContainerElement(const size_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VCCObjectPtr> obj = std::static_pointer_cast<VCCObjectPtr>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VCCObjectPtrProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VCCObjectPtrProperty::EnumC:\r\n"
    "            obj->RemoveEnumC(index);\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VCCObjectPtrPropertyAccessor::_RemoveContainerElement(const size_t &objectProperty, const ITypeUnion * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VCCObjectPtrPropertyAccessor::_ClearContainer(const size_t &objectProperty) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VCCObjectPtr> obj = std::static_pointer_cast<VCCObjectPtr>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VCCObjectPtrProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VCCObjectPtrProperty::EnumC:\r\n"
    "            obj->ClearEnumC();\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "";
    EXPECT_EQ(objectPropertyAccessorFileContentHpp, expectedOBjectPropertyAccessorFileContentHpp);
    EXPECT_EQ(objectPropertyAccessorFileContentCpp, expectedOBjectPropertyAccessorFileContentCpp);

    // ------------------------------------------------------------------------------------------ //
    //                                      Property Accessor Factory                             //
    // ------------------------------------------------------------------------------------------ //
    std::wstring objectPropertyAccessorFactoryFileContentHpp = ReadFile(ConcatPaths({this->GetWorkspaceTarget(), L"property_accessor_factory.hpp"}));
    std::wstring objectPropertyAccessorFactoryFileContentCpp = ReadFile(ConcatPaths({this->GetWorkspaceTarget(), L"property_accessor_factory.cpp"}));
}