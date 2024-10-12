#include <gtest/gtest.h>

#include <filesystem>
#include <map>
#include <string>
#include <vector>

#include "class_macro.hpp"
#include "file_helper.hpp"
#include "memory_macro.hpp"

#include "vpg_enum_class.hpp"
#include "vpg_property_accessor_generation_service.hpp"

using namespace vcc;

// same as TEST_F(VPGVccGenerationManagerTest, XXX)
// need to change TEST_F(VPGVccGenerationManagerTest, XXX)
class VPGPropertyAccessorFileGenerationServiceTest : public testing::Test 
{
    GETSET_SPTR(LogConfig, LogConfig);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGPropertyAccessorFileGenerationServiceTest/");
    
    GETSET(std::wstring, FilePathHpp, L"");
    GETSET(std::wstring, FilePathCpp, L"");
    GETSET(std::wstring, IncludeFileName, L"");

    MAP(std::wstring, std::wstring, ProjectIncludeList);
    public:
        void SetUp() override
        {
            this->_LogConfig->SetIsConsoleLog(false);
            std::filesystem::remove_all(PATH(this->GetWorkspace()));

            this->_FilePathHpp = ConcatPaths({this->GetWorkspace(), L"vpg_object_property_accessor.hpp"});
            this->_FilePathCpp = ConcatPaths({this->GetWorkspace(), L"vpg_object_property_accessor.cpp"});
            this->_IncludeFileName = L"vpg_object_property_accessor.hpp";

            this->InsertProjectIncludeList(L"VCCEnum", L"vcc_enum.hpp");
            this->InsertProjectIncludeList(L"VPGObject", L"vpg_object.hpp");
            this->InsertProjectIncludeList(L"VPGObjectProperty", L"vpg_object_property.hpp");
            this->InsertProjectIncludeList(L"VPGObjectA", L"vpg_object_a.hpp");
            this->InsertProjectIncludeList(L"VPGObjectAProperty", L"vpg_object_a_property.hpp");
            this->InsertProjectIncludeList(L"VPGObjectB", L"vpg_object_b.hpp");
            this->InsertProjectIncludeList(L"VPGObjectBProperty", L"vpg_object_b_property.hpp");
        }

        void TearDown() override
        {
            std::filesystem::remove_all(PATH(this->GetWorkspace()));
        }
};

TEST_F(VPGPropertyAccessorFileGenerationServiceTest, Single)
{
    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
    DECLARE_SPTR(VPGEnumClass, enumClass);
    enumClass->SetName(L"VPGObjectProperty");
    enumClassList.push_back(enumClass);

    DECLARE_SPTR(VPGEnumClassProperty, propBool);
    propBool->SetEnum(L"Bool");
    propBool->SetMacro(L"GETSET(bool, Bool, false)");
    propBool->SetType1(L"bool");
    propBool->SetType2(L"");
    propBool->SetPropertyName(L"Bool");
    propBool->SetDefaultValue(L"");
    enumClass->InsertProperties(propBool);

    DECLARE_SPTR(VPGEnumClassProperty, propString);
    propString->SetEnum(L"String");
    propString->SetMacro(L"GETSET(std::string, String, \"\")");
    propString->SetType1(L"std::string");
    propString->SetType2(L"");
    propString->SetPropertyName(L"String");
    propString->SetDefaultValue(L"");
    enumClass->InsertProperties(propString);

    DECLARE_SPTR(VPGEnumClassProperty, propWstring);
    propWstring->SetEnum(L"Wstring");
    propWstring->SetMacro(L"GETSET(std::wstring, Wstring, L\"\")");
    propWstring->SetType1(L"std::wstring");
    propWstring->SetType2(L"");
    propWstring->SetPropertyName(L"Wstring");
    propWstring->SetDefaultValue(L"");
    enumClass->InsertProperties(propWstring);

    DECLARE_SPTR(VPGEnumClassProperty, propEnum);
    propEnum->SetEnum(L"Enum");
    propEnum->SetMacro(L"GETSET(VCCEnum, Enum, L\"\")");
    propEnum->SetType1(L"VCCEnum");
    propEnum->SetType2(L"");
    propEnum->SetPropertyName(L"Enum");
    propEnum->SetDefaultValue(L"");
    enumClass->InsertProperties(propEnum);

    std::set<std::wstring> propertyTypes;
    VPGPropertyAccessorGenerationService::GenerateHpp(this->GetLogConfig().get(), this->GetFilePathHpp(), enumClassList);
    VPGPropertyAccessorGenerationService::GenerateCpp(this->GetLogConfig().get(), this->GetProjectIncludeList(), this->GetFilePathCpp(), enumClassList);
    EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));
    EXPECT_TRUE(IsFileExists(this->GetFilePathCpp()));

    std::wstring expectedHppContent = L""
    "#pragma once\r\n"
    "\r\n"
    "#include <string>\r\n"
    "\r\n"
    "#include \"base_property_accessor.hpp\"\r\n"
    "#include \"property_accessor_macro.hpp\"\r\n"
    "\r\n"
    "using namespace vcc;\r\n"
    "\r\n"
    "class VPGObjectPropertyAccessor : public BasePropertyAccessor\r\n"
    "{\r\n"
    "    PROPERTY_ACCESSOR_HEADER(bool, Bool)\r\n"
    "    PROPERTY_ACCESSOR_HEADER(long, Long)\r\n"
    "    PROPERTY_ACCESSOR_HEADER(std::wstring, String)\r\n"
    "\r\n"
    "    public:\r\n"
    "        VPGObjectPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}\r\n"
    "        virtual ~VPGObjectPropertyAccessor() {}\r\n"
    "};\r\n";
    EXPECT_EQ(ReadFile(this->GetFilePathHpp()), expectedHppContent);

    std::wstring expectedCppContent = L""
    "#include \"" + this->GetIncludeFileName() + L"\"\r\n"
    "\r\n"
    "#include <string>\r\n"
    "\r\n"
    "#include \"exception_macro.hpp\"\r\n"
    "#include \"i_object.hpp\"\r\n"
    "#include \"property_accessor_macro.hpp\"\r\n"
    "#include \"vcc_enum.hpp\"\r\n"
    "#include \"vpg_object.hpp\"\r\n"
    "#include \"vpg_object_property.hpp\"\r\n"
    "\r\n"
    "using namespace vcc;\r\n"
    "\r\n"
    "bool VPGObjectPropertyAccessor::_ReadBool(const int64_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::Bool:\r\n"
    "            return obj->GetBool();\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return false;\r\n"
    "}\r\n"
    "\r\n"
    "bool VPGObjectPropertyAccessor::_ReadBool(const int64_t &objectProperty, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "    return false;\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool &value, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::Bool:\r\n"
    "            obj->SetBool(value);\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool & /*value*/, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_InsertBool(const int64_t &objectProperty, const bool & /*value*/, const int64_t & /*index*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "long VPGObjectPropertyAccessor::_ReadLong(const int64_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::Enum:\r\n"
    "            return static_cast<long>(obj->GetEnum());\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return 0L;\r\n"
    "}\r\n"
    "\r\n"
    "long VPGObjectPropertyAccessor::_ReadLong(const int64_t &objectProperty, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "    return 0L;\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteLong(const int64_t &objectProperty, const long &value, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::Enum:\r\n"
    "            obj->SetEnum(static_cast<VCCEnum>(value));\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteLong(const int64_t &objectProperty, const long & /*value*/, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_InsertLong(const int64_t &objectProperty, const long & /*value*/, const int64_t & /*index*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "std::wstring VPGObjectPropertyAccessor::_ReadString(const int64_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::String:\r\n"
    "            return str2wstr(obj->GetString());\r\n"
    "        case VPGObjectProperty::Wstring:\r\n"
    "            return obj->GetWstring();\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return L\"\";\r\n"
    "}\r\n"
    "\r\n"
    "std::wstring VPGObjectPropertyAccessor::_ReadString(const int64_t &objectProperty, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "    return L\"\";\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring &value, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::String:\r\n"
    "            obj->SetString(wstr2str(value));\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::Wstring:\r\n"
    "            obj->SetWstring(value);\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring & /*value*/, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_InsertString(const int64_t &objectProperty, const std::wstring & /*value*/, const int64_t & /*index*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n";
    EXPECT_EQ(ReadFile(this->GetFilePathCpp()), expectedCppContent);
}

TEST_F(VPGPropertyAccessorFileGenerationServiceTest, NoAccess)
{
    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
    DECLARE_SPTR(VPGEnumClass, enumClass);
    enumClass->SetName(L"VPGObjectProperty");
    enumClassList.push_back(enumClass);

    DECLARE_SPTR(VPGEnumClassProperty, propBool);
    propBool->SetEnum(L"ReadWrite");
    propBool->SetMacro(L"GETSET(bool, ReadWrite, false)");
    propBool->SetType1(L"bool");
    propBool->SetType2(L"");
    propBool->SetPropertyName(L"ReadWrite");
    propBool->SetDefaultValue(L"");
    propBool->SetAccessMode(VPGEnumClassPropertyAccessMode::NoAccess);
    enumClass->InsertProperties(propBool);
    
    DECLARE_SPTR(VPGEnumClassProperty, propRead);
    propRead->SetEnum(L"Read");
    propRead->SetMacro(L"GETSET(bool, Read, false)");
    propRead->SetType1(L"bool");
    propRead->SetType2(L"");
    propRead->SetPropertyName(L"Read");
    propRead->SetDefaultValue(L"");
    propRead->SetAccessMode(VPGEnumClassPropertyAccessMode::NoAccess);
    enumClass->InsertProperties(propRead);
    
    DECLARE_SPTR(VPGEnumClassProperty, propWrite);
    propWrite->SetEnum(L"Write");
    propWrite->SetMacro(L"GETSET(bool, Write, false)");
    propWrite->SetType1(L"bool");
    propWrite->SetType2(L"");
    propWrite->SetPropertyName(L"Write");
    propWrite->SetDefaultValue(L"");
    propWrite->SetAccessMode(VPGEnumClassPropertyAccessMode::NoAccess);
    enumClass->InsertProperties(propWrite);
    
    DECLARE_SPTR(VPGEnumClassProperty, propNoAccess);
    propNoAccess->SetEnum(L"NoAccess");
    propNoAccess->SetMacro(L"GETSET(bool, NoAccess, false)");
    propNoAccess->SetType1(L"bool");
    propNoAccess->SetType2(L"");
    propNoAccess->SetPropertyName(L"NoAccess");
    propNoAccess->SetDefaultValue(L"");
    propNoAccess->SetAccessMode(VPGEnumClassPropertyAccessMode::NoAccess);
    enumClass->InsertProperties(propNoAccess);

    std::set<std::wstring> propertyTypes;
    VPGPropertyAccessorGenerationService::GenerateHpp(this->GetLogConfig().get(), this->GetFilePathHpp(), enumClassList);
    VPGPropertyAccessorGenerationService::GenerateCpp(this->GetLogConfig().get(), this->GetProjectIncludeList(), this->GetFilePathCpp(), enumClassList);
    EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));
    EXPECT_FALSE(IsFileExists(this->GetFilePathCpp()));

    std::wstring expectedHppContent = L""
    "#pragma once\r\n"
    "\r\n"
    "#include \"base_property_accessor.hpp\"\r\n"
    "#include \"property_accessor_macro.hpp\"\r\n"
    "\r\n"
    "using namespace vcc;\r\n"
    "\r\n"
    "class VPGObjectPropertyAccessor : public BasePropertyAccessor\r\n"
    "{\r\n"
    "\r\n"
    "    public:\r\n"
    "        VPGObjectPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}\r\n"
    "        virtual ~VPGObjectPropertyAccessor() {}\r\n"
    "};\r\n";
    EXPECT_EQ(ReadFile(this->GetFilePathHpp()), expectedHppContent);
}

TEST_F(VPGPropertyAccessorFileGenerationServiceTest, AccessMode_Normal)
{
    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
    DECLARE_SPTR(VPGEnumClass, enumClass);
    enumClass->SetName(L"VPGObjectProperty");
    enumClassList.push_back(enumClass);

    DECLARE_SPTR(VPGEnumClassProperty, propBool);
    propBool->SetEnum(L"ReadWrite");
    propBool->SetMacro(L"GETSET(bool, ReadWrite, false)");
    propBool->SetType1(L"bool");
    propBool->SetType2(L"");
    propBool->SetPropertyName(L"ReadWrite");
    propBool->SetDefaultValue(L"");
    propBool->SetAccessMode(VPGEnumClassPropertyAccessMode::ReadWrite);
    enumClass->InsertProperties(propBool);
    
    DECLARE_SPTR(VPGEnumClassProperty, propRead);
    propRead->SetEnum(L"Read");
    propRead->SetMacro(L"GETSET(bool, Read, false)");
    propRead->SetType1(L"bool");
    propRead->SetType2(L"");
    propRead->SetPropertyName(L"Read");
    propRead->SetDefaultValue(L"");
    propRead->SetAccessMode(VPGEnumClassPropertyAccessMode::ReadOnly);
    enumClass->InsertProperties(propRead);
    
    DECLARE_SPTR(VPGEnumClassProperty, propWrite);
    propWrite->SetEnum(L"Write");
    propWrite->SetMacro(L"GETSET(bool, Write, false)");
    propWrite->SetType1(L"bool");
    propWrite->SetType2(L"");
    propWrite->SetPropertyName(L"Write");
    propWrite->SetDefaultValue(L"");
    propWrite->SetAccessMode(VPGEnumClassPropertyAccessMode::WriteOnly);
    enumClass->InsertProperties(propWrite);
    
    DECLARE_SPTR(VPGEnumClassProperty, propNoAccess);
    propNoAccess->SetEnum(L"NoAccess");
    propNoAccess->SetMacro(L"GETSET(bool, NoAccess, false)");
    propNoAccess->SetType1(L"bool");
    propNoAccess->SetType2(L"");
    propNoAccess->SetPropertyName(L"NoAccess");
    propNoAccess->SetDefaultValue(L"");
    propNoAccess->SetAccessMode(VPGEnumClassPropertyAccessMode::NoAccess);
    enumClass->InsertProperties(propNoAccess);

    std::set<std::wstring> propertyTypes;
    VPGPropertyAccessorGenerationService::GenerateHpp(this->GetLogConfig().get(), this->GetFilePathHpp(), enumClassList);
    VPGPropertyAccessorGenerationService::GenerateCpp(this->GetLogConfig().get(), this->GetProjectIncludeList(), this->GetFilePathCpp(), enumClassList);
    EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));
    EXPECT_TRUE(IsFileExists(this->GetFilePathCpp()));

    std::wstring expectedHppContent = L""
    "#pragma once\r\n"
    "\r\n"
    "#include \"base_property_accessor.hpp\"\r\n"
    "#include \"property_accessor_macro.hpp\"\r\n"
    "\r\n"
    "using namespace vcc;\r\n"
    "\r\n"
    "class VPGObjectPropertyAccessor : public BasePropertyAccessor\r\n"
    "{\r\n"
    "    PROPERTY_ACCESSOR_HEADER(bool, Bool)\r\n"
    "\r\n"
    "    public:\r\n"
    "        VPGObjectPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}\r\n"
    "        virtual ~VPGObjectPropertyAccessor() {}\r\n"
    "};\r\n";
    EXPECT_EQ(ReadFile(this->GetFilePathHpp()), expectedHppContent);

    std::wstring expectedCppContent = L""
    "#include \"" + this->GetIncludeFileName() + L"\"\r\n"
    "\r\n"
    "#include \"exception_macro.hpp\"\r\n"
    "#include \"i_object.hpp\"\r\n"
    "#include \"property_accessor_macro.hpp\"\r\n"
    "#include \"vpg_object.hpp\"\r\n"
    "#include \"vpg_object_property.hpp\"\r\n"
    "\r\n"
    "using namespace vcc;\r\n"
    "\r\n"
    "bool VPGObjectPropertyAccessor::_ReadBool(const int64_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::ReadWrite:\r\n"
    "            return obj->GetReadWrite();\r\n"
    "        case VPGObjectProperty::Read:\r\n"
    "            return obj->GetRead();\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return false;\r\n"
    "}\r\n"
    "\r\n"
    "bool VPGObjectPropertyAccessor::_ReadBool(const int64_t &objectProperty, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "    return false;\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool &value, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::ReadWrite:\r\n"
    "            obj->SetReadWrite(value);\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::Write:\r\n"
    "            obj->SetWrite(value);\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool & /*value*/, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_InsertBool(const int64_t &objectProperty, const bool & /*value*/, const int64_t & /*index*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n";
    EXPECT_EQ(ReadFile(this->GetFilePathCpp()), expectedCppContent);
}

TEST_F(VPGPropertyAccessorFileGenerationServiceTest, AccessMode_Vector)
{
    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
    DECLARE_SPTR(VPGEnumClass, enumClass);
    enumClass->SetName(L"VPGObjectProperty");
    enumClassList.push_back(enumClass);

    DECLARE_SPTR(VPGEnumClassProperty, propBool);
    propBool->SetEnum(L"ReadWrite");
    propBool->SetMacro(L"VECTOR(bool, ReadWrite, false)");
    propBool->SetType1(L"bool");
    propBool->SetType2(L"");
    propBool->SetPropertyName(L"ReadWrite");
    propBool->SetDefaultValue(L"");
    propBool->SetAccessMode(VPGEnumClassPropertyAccessMode::ReadWrite);
    enumClass->InsertProperties(propBool);
    
    DECLARE_SPTR(VPGEnumClassProperty, propRead);
    propRead->SetEnum(L"Read");
    propRead->SetMacro(L"VECTOR(bool, Read, false)");
    propRead->SetType1(L"bool");
    propRead->SetType2(L"");
    propRead->SetPropertyName(L"Read");
    propRead->SetDefaultValue(L"");
    propRead->SetAccessMode(VPGEnumClassPropertyAccessMode::ReadOnly);
    enumClass->InsertProperties(propRead);
    
    DECLARE_SPTR(VPGEnumClassProperty, propWrite);
    propWrite->SetEnum(L"Write");
    propWrite->SetMacro(L"VECTOR(bool, Write, false)");
    propWrite->SetType1(L"bool");
    propWrite->SetType2(L"");
    propWrite->SetPropertyName(L"Write");
    propWrite->SetDefaultValue(L"");
    propWrite->SetAccessMode(VPGEnumClassPropertyAccessMode::WriteOnly);
    enumClass->InsertProperties(propWrite);
    
    DECLARE_SPTR(VPGEnumClassProperty, propNoAccess);
    propNoAccess->SetEnum(L"NoAccess");
    propNoAccess->SetMacro(L"VECTOR(bool, NoAccess, false)");
    propNoAccess->SetType1(L"bool");
    propNoAccess->SetType2(L"");
    propNoAccess->SetPropertyName(L"NoAccess");
    propNoAccess->SetDefaultValue(L"");
    propNoAccess->SetAccessMode(VPGEnumClassPropertyAccessMode::NoAccess);
    enumClass->InsertProperties(propNoAccess);

    std::set<std::wstring> propertyTypes;
    VPGPropertyAccessorGenerationService::GenerateHpp(this->GetLogConfig().get(), this->GetFilePathHpp(), enumClassList);
    VPGPropertyAccessorGenerationService::GenerateCpp(this->GetLogConfig().get(), this->GetProjectIncludeList(), this->GetFilePathCpp(), enumClassList);
    EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));
    EXPECT_TRUE(IsFileExists(this->GetFilePathCpp()));

    std::wstring expectedHppContent = L""
    "#pragma once\r\n"
    "\r\n"
    "#include \"base_property_accessor.hpp\"\r\n"
    "#include \"property_accessor_macro.hpp\"\r\n"
    "\r\n"
    "using namespace vcc;\r\n"
    "\r\n"
    "class VPGObjectPropertyAccessor : public BasePropertyAccessor\r\n"
    "{\r\n"
    "    PROPERTY_ACCESSOR_HEADER(bool, Bool)\r\n"
    "\r\n"
    "    PROPERTY_ACCESSOR_CONTAINER_HEADER\r\n"
    "\r\n"
    "    public:\r\n"
    "        VPGObjectPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}\r\n"
    "        virtual ~VPGObjectPropertyAccessor() {}\r\n"
    "};\r\n";
    EXPECT_EQ(ReadFile(this->GetFilePathHpp()), expectedHppContent);

    std::wstring expectedCppContent = L""
    "#include \"" + this->GetIncludeFileName() + L"\"\r\n"
    "\r\n"
    "#include <vector>\r\n"
    "\r\n"
    "#include \"exception_macro.hpp\"\r\n"
    "#include \"i_object.hpp\"\r\n"
    "#include \"property_accessor_macro.hpp\"\r\n"
    "#include \"vpg_object.hpp\"\r\n"
    "#include \"vpg_object_property.hpp\"\r\n"
    "\r\n"
    "using namespace vcc;\r\n"
    "\r\n"
    "bool VPGObjectPropertyAccessor::_ReadBool(const int64_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::ReadWrite:\r\n"
    "            return obj->GetReadWrite(index);\r\n"
    "        case VPGObjectProperty::Read:\r\n"
    "            return obj->GetRead(index);\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return false;\r\n"
    "}\r\n"
    "\r\n"
    "bool VPGObjectPropertyAccessor::_ReadBool(const int64_t &objectProperty, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "    return false;\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool &value, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::ReadWrite:\r\n"
    "            if (index > -1)\r\n"
    "                obj->SetReadWrite(index, value);\r\n"
    "            else\r\n"
    "                obj->InsertReadWrite(value);\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::Write:\r\n"
    "            if (index > -1)\r\n"
    "                obj->SetWrite(index, value);\r\n"
    "            else\r\n"
    "                obj->InsertWrite(value);\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool & /*value*/, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_InsertBool(const int64_t &objectProperty, const bool &value, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::ReadWrite:\r\n"
    "            if (index > -1)\r\n"
    "                obj->InsertReadWrite(index, value);\r\n"
    "            else\r\n"
    "                obj->InsertReadWrite(value);\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::Write:\r\n"
    "            if (index > -1)\r\n"
    "                obj->InsertWrite(index, value);\r\n"
    "            else\r\n"
    "                obj->InsertWrite(value);\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "size_t VPGObjectPropertyAccessor::_GetContainerCount(const int64_t &objectProperty) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::ReadWrite:\r\n"
    "            return obj->GetReadWrite().size();\r\n"
    "        case VPGObjectProperty::Read:\r\n"
    "            return obj->GetRead().size();\r\n"
    "        case VPGObjectProperty::Write:\r\n"
    "            return obj->GetWrite().size();\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return 0;\r\n"
    "}\r\n"
    "\r\n"
    "std::set<void *> VPGObjectPropertyAccessor::_GetMapKeys(const int64_t &objectProperty) const\r\n"
    "{\r\n"
    "    std::set<void *> result;\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "    return result;\r\n"
    "}\r\n"
    "\r\n"
    "bool VPGObjectPropertyAccessor::_IsContainKey(const int64_t &objectProperty, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "    return false;\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_RemoveContainerElement(const int64_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::ReadWrite:\r\n"
    "            obj->RemoveReadWrite(index);\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::Write:\r\n"
    "            obj->RemoveWrite(index);\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_RemoveContainerElement(const int64_t &objectProperty, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_ClearContainer(const int64_t &objectProperty) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::ReadWrite:\r\n"
    "            obj->ClearReadWrite();\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::Write:\r\n"
    "            obj->ClearWrite();\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n";
    EXPECT_EQ(ReadFile(this->GetFilePathCpp()), expectedCppContent);
}

TEST_F(VPGPropertyAccessorFileGenerationServiceTest, Multi)
{
    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
    DECLARE_SPTR(VPGEnumClass, enumClassA);
    enumClassA->SetName(L"VPGObjectAProperty");
    enumClassList.push_back(enumClassA);

    DECLARE_SPTR(VPGEnumClassProperty, propBool);
    propBool->SetEnum(L"Bool");
    propBool->SetMacro(L"GETSET(bool, Bool, false)");
    propBool->SetType1(L"bool");
    propBool->SetType2(L"");
    propBool->SetPropertyName(L"Bool");
    propBool->SetDefaultValue(L"");
    enumClassA->InsertProperties(propBool);

    DECLARE_SPTR(VPGEnumClass, enumClassB);
    enumClassB->SetName(L"VPGObjectBProperty");
    enumClassList.push_back(enumClassB);
    enumClassB->InsertProperties(propBool);

    std::set<std::wstring> propertyTypes;
    VPGPropertyAccessorGenerationService::GenerateHpp(this->GetLogConfig().get(), this->GetFilePathHpp(), enumClassList);
    VPGPropertyAccessorGenerationService::GenerateCpp(this->GetLogConfig().get(), this->GetProjectIncludeList(), this->GetFilePathCpp(), enumClassList);
    EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));
    EXPECT_TRUE(IsFileExists(this->GetFilePathCpp()));

    std::wstring expectedHppContent = L""
    "#pragma once\r\n"
    "\r\n"
    "#include \"base_property_accessor.hpp\"\r\n"
    "#include \"property_accessor_macro.hpp\"\r\n"
    "\r\n"
    "using namespace vcc;\r\n"
    "\r\n"
    "class VPGObjectAPropertyAccessor : public BasePropertyAccessor\r\n"
    "{\r\n"
    "    PROPERTY_ACCESSOR_HEADER(bool, Bool)\r\n"
    "\r\n"
    "    public:\r\n"
    "        VPGObjectAPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}\r\n"
    "        virtual ~VPGObjectAPropertyAccessor() {}\r\n"
    "};\r\n"
    "\r\n"
    "class VPGObjectBPropertyAccessor : public BasePropertyAccessor\r\n"
    "{\r\n"
    "    PROPERTY_ACCESSOR_HEADER(bool, Bool)\r\n"
    "\r\n"
    "    public:\r\n"
    "        VPGObjectBPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}\r\n"
    "        virtual ~VPGObjectBPropertyAccessor() {}\r\n"
    "};\r\n";
    EXPECT_EQ(ReadFile(this->GetFilePathHpp()), expectedHppContent);

    std::wstring expectedCppContent = L""
    "#include \"" + this->GetIncludeFileName() + L"\"\r\n"
    "\r\n"
    "#include \"exception_macro.hpp\"\r\n"
    "#include \"i_object.hpp\"\r\n"
    "#include \"property_accessor_macro.hpp\"\r\n"
    "#include \"vpg_object_a.hpp\"\r\n"
    "#include \"vpg_object_a_property.hpp\"\r\n"
    "#include \"vpg_object_b.hpp\"\r\n"
    "#include \"vpg_object_b_property.hpp\"\r\n"
    "\r\n"
    "using namespace vcc;\r\n"
    "\r\n"
    "bool VPGObjectAPropertyAccessor::_ReadBool(const int64_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObjectA> obj = std::static_pointer_cast<VPGObjectA>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectAProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectAProperty::Bool:\r\n"
    "            return obj->GetBool();\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return false;\r\n"
    "}\r\n"
    "\r\n"
    "bool VPGObjectAPropertyAccessor::_ReadBool(const int64_t &objectProperty, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "    return false;\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectAPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool &value, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObjectA> obj = std::static_pointer_cast<VPGObjectA>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectAProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectAProperty::Bool:\r\n"
    "            obj->SetBool(value);\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectAPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool & /*value*/, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectAPropertyAccessor::_InsertBool(const int64_t &objectProperty, const bool & /*value*/, const int64_t & /*index*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "bool VPGObjectBPropertyAccessor::_ReadBool(const int64_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObjectB> obj = std::static_pointer_cast<VPGObjectB>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectBProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectBProperty::Bool:\r\n"
    "            return obj->GetBool();\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return false;\r\n"
    "}\r\n"
    "\r\n"
    "bool VPGObjectBPropertyAccessor::_ReadBool(const int64_t &objectProperty, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "    return false;\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectBPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool &value, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObjectB> obj = std::static_pointer_cast<VPGObjectB>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectBProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectBProperty::Bool:\r\n"
    "            obj->SetBool(value);\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectBPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool & /*value*/, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectBPropertyAccessor::_InsertBool(const int64_t &objectProperty, const bool & /*value*/, const int64_t & /*index*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n";
    EXPECT_EQ(ReadFile(this->GetFilePathCpp()), expectedCppContent);
}

TEST_F(VPGPropertyAccessorFileGenerationServiceTest, Object)
{
    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
    DECLARE_SPTR(VPGEnumClass, enumClass);
    enumClass->SetName(L"VPGObjectProperty");
    enumClassList.push_back(enumClass);

    DECLARE_SPTR(VPGEnumClassProperty, propObject);
    propObject->SetEnum(L"Object");
    propObject->SetMacro(L"GETSET_SPTR_NULL(VPGObjectA, Object)");
    propObject->SetType1(L"VPGObjectA");
    propObject->SetType2(L"");
    propObject->SetPropertyName(L"Object");
    propObject->SetDefaultValue(L"");
    enumClass->InsertProperties(propObject);

    std::set<std::wstring> propertyTypes;
    VPGPropertyAccessorGenerationService::GenerateHpp(this->GetLogConfig().get(), this->GetFilePathHpp(), enumClassList);
    VPGPropertyAccessorGenerationService::GenerateCpp(this->GetLogConfig().get(), this->GetProjectIncludeList(), this->GetFilePathCpp(), enumClassList);
    EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));
    EXPECT_TRUE(IsFileExists(this->GetFilePathCpp()));

    std::wstring expectedHppContent = L""
    "#pragma once\r\n"
    "\r\n"
    "#include \"base_property_accessor.hpp\"\r\n"
    "#include \"property_accessor_macro.hpp\"\r\n"
    "\r\n"
    "using namespace vcc;\r\n"
    "\r\n"
    "class VPGObjectPropertyAccessor : public BasePropertyAccessor\r\n"
    "{\r\n"
    "    PROPERTY_ACCESSOR_OBJECT_HEADER(std::shared_ptr<IObject>, Object)\r\n"
    "\r\n"
    "    public:\r\n"
    "        VPGObjectPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}\r\n"
    "        virtual ~VPGObjectPropertyAccessor() {}\r\n"
    "};\r\n";
    EXPECT_EQ(ReadFile(this->GetFilePathHpp()), expectedHppContent);

    std::wstring expectedCppContent = L""
    "#include \"" + this->GetIncludeFileName() + L"\"\r\n"
    "\r\n"
    "#include <memory>\r\n"
    "\r\n"
    "#include \"exception_macro.hpp\"\r\n"
    "#include \"i_object.hpp\"\r\n"
    "#include \"property_accessor_macro.hpp\"\r\n"
    "#include \"vpg_object.hpp\"\r\n"
    "#include \"vpg_object_a.hpp\"\r\n"
    "#include \"vpg_object_property.hpp\"\r\n"
    "\r\n"
    "using namespace vcc;\r\n"
    "\r\n"
    "std::shared_ptr<IObject> VPGObjectPropertyAccessor::_ReadObject(const int64_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::Object:\r\n"
    "            return std::static_pointer_cast<IObject>(obj->GetObject());\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return nullptr;\r\n"
    "}\r\n"
    "\r\n"
    "std::shared_ptr<IObject> VPGObjectPropertyAccessor::_ReadObject(const int64_t &objectProperty, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "    return nullptr;\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteObject(const int64_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::Object:\r\n"
    "            obj->SetObject(std::static_pointer_cast<VPGObjectA>(value));\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteObject(const int64_t &objectProperty, std::shared_ptr<IObject> /*value*/, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_InsertObject(const int64_t &objectProperty, std::shared_ptr<IObject> /*value*/, const int64_t & /*index*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "std::shared_ptr<IObject> VPGObjectPropertyAccessor::_CloneObject(const int64_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::Object:\r\n"
    "            return std::static_pointer_cast<IObject>(obj->GetObject()->Clone());\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return nullptr;\r\n"
    "}\r\n"
    "\r\n"
    "std::shared_ptr<IObject> VPGObjectPropertyAccessor::_CloneObject(const int64_t &objectProperty, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "    return nullptr;\r\n"
    "}\r\n";
    EXPECT_EQ(ReadFile(this->GetFilePathCpp()), expectedCppContent);
}

TEST_F(VPGPropertyAccessorFileGenerationServiceTest, Container)
{
    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
    DECLARE_SPTR(VPGEnumClass, enumClass);
    enumClass->SetName(L"VPGObjectProperty");
    enumClassList.push_back(enumClass);

    // need to check normal, enum and object
    DECLARE_SPTR(VPGEnumClassProperty, propVectorInt);
    propVectorInt->SetEnum(L"VectorInt");
    propVectorInt->SetMacro(L"VECTOR(int, VectorInt)");
    propVectorInt->SetType1(L"int");
    propVectorInt->SetType2(L"");
    propVectorInt->SetPropertyName(L"VectorInt");
    propVectorInt->SetDefaultValue(L"");
    enumClass->InsertProperties(propVectorInt);
    
    DECLARE_SPTR(VPGEnumClassProperty, propVectorEnum);
    propVectorEnum->SetEnum(L"VectorEnum");
    propVectorEnum->SetMacro(L"VECTOR(VCCEnum, VectorEnum)");
    propVectorEnum->SetType1(L"VCCEnum");
    propVectorEnum->SetType2(L"");
    propVectorEnum->SetPropertyName(L"VectorEnum");
    propVectorEnum->SetDefaultValue(L"");
    enumClass->InsertProperties(propVectorEnum);
    
    DECLARE_SPTR(VPGEnumClassProperty, propVectorObj);
    propVectorObj->SetEnum(L"VectorObj");
    propVectorObj->SetMacro(L"VECTOR_SPTR(VPGObjectA, VectorObj)");
    propVectorObj->SetType1(L"VPGObjectA");
    propVectorObj->SetType2(L"");
    propVectorObj->SetPropertyName(L"VectorObj");
    propVectorObj->SetDefaultValue(L"");
    enumClass->InsertProperties(propVectorObj);

    DECLARE_SPTR(VPGEnumClassProperty, propMapInt);
    propMapInt->SetEnum(L"MapInt");
    propMapInt->SetMacro(L"MAP(int, int, MapInt)");
    propMapInt->SetType1(L"int");
    propMapInt->SetType2(L"int");
    propMapInt->SetPropertyName(L"MapInt");
    propMapInt->SetDefaultValue(L"");
    enumClass->InsertProperties(propMapInt);
    
    DECLARE_SPTR(VPGEnumClassProperty, propMapEnum);
    propMapEnum->SetEnum(L"MapEnum");
    propMapEnum->SetMacro(L"MAP(char, VCCEnum, MapEnum)");
    propMapEnum->SetType1(L"char");
    propMapEnum->SetType2(L"VCCEnum");
    propMapEnum->SetPropertyName(L"MapEnum");
    propMapEnum->SetDefaultValue(L"");
    enumClass->InsertProperties(propMapEnum);
    
    DECLARE_SPTR(VPGEnumClassProperty, propMapObj);
    propMapObj->SetEnum(L"MapObj");
    propMapObj->SetMacro(L"MAP_SPTR(double, VPGObjectA, MapObj)");
    propMapObj->SetType1(L"double");
    propMapObj->SetType2(L"VPGObjectA");
    propMapObj->SetPropertyName(L"MapObj");
    propMapObj->SetDefaultValue(L"");
    enumClass->InsertProperties(propMapObj);
    
    DECLARE_SPTR(VPGEnumClassProperty, propOrderedMapInt);
    propOrderedMapInt->SetEnum(L"OrderedMapInt");
    propOrderedMapInt->SetMacro(L"ORDERED_MAP(int, int, OrderedMapInt)");
    propOrderedMapInt->SetType1(L"int");
    propOrderedMapInt->SetType2(L"int");
    propOrderedMapInt->SetPropertyName(L"OrderedMapInt");
    propOrderedMapInt->SetDefaultValue(L"");
    enumClass->InsertProperties(propOrderedMapInt);
    
    DECLARE_SPTR(VPGEnumClassProperty, propOrderedMapEnum);
    propOrderedMapEnum->SetEnum(L"OrderedMapEnum");
    propOrderedMapEnum->SetMacro(L"ORDERED_MAP(char, VCCEnum, OrderedMapEnum)");
    propOrderedMapEnum->SetType1(L"char");
    propOrderedMapEnum->SetType2(L"VCCEnum");
    propOrderedMapEnum->SetPropertyName(L"OrderedMapEnum");
    propOrderedMapEnum->SetDefaultValue(L"");
    enumClass->InsertProperties(propOrderedMapEnum);
    
    DECLARE_SPTR(VPGEnumClassProperty, propOrderedMapObj);
    propOrderedMapObj->SetEnum(L"OrderedMapObj");
    propOrderedMapObj->SetMacro(L"ORDERED_MAP_SPTR(double, VPGObjectA, OrderedMapObj)");
    propOrderedMapObj->SetType1(L"double");
    propOrderedMapObj->SetType2(L"VPGObjectA");
    propOrderedMapObj->SetPropertyName(L"OrderedMapObj");
    propOrderedMapObj->SetDefaultValue(L"");
    enumClass->InsertProperties(propOrderedMapObj);
    
    std::set<std::wstring> propertyTypes;
    VPGPropertyAccessorGenerationService::GenerateHpp(this->GetLogConfig().get(), this->GetFilePathHpp(), enumClassList);
    VPGPropertyAccessorGenerationService::GenerateCpp(this->GetLogConfig().get(), this->GetProjectIncludeList(), this->GetFilePathCpp(), enumClassList);
    EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));
    EXPECT_TRUE(IsFileExists(this->GetFilePathCpp()));

    std::wstring expectedHppContent = L""
    "#pragma once\r\n"
    "\r\n"
    "#include \"base_property_accessor.hpp\"\r\n"
    "#include \"property_accessor_macro.hpp\"\r\n"
    "\r\n"
    "using namespace vcc;\r\n"
    "\r\n"
    "class VPGObjectPropertyAccessor : public BasePropertyAccessor\r\n"
    "{\r\n"
    "    PROPERTY_ACCESSOR_HEADER(int, Int)\r\n"
    "    PROPERTY_ACCESSOR_HEADER(long, Long)\r\n"
    "    PROPERTY_ACCESSOR_OBJECT_HEADER(std::shared_ptr<IObject>, Object)\r\n"
    "\r\n"
    "    PROPERTY_ACCESSOR_CONTAINER_HEADER\r\n"
    "\r\n"
    "    public:\r\n"
    "        VPGObjectPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}\r\n"
    "        virtual ~VPGObjectPropertyAccessor() {}\r\n"
    "};\r\n";
    EXPECT_EQ(ReadFile(this->GetFilePathHpp()), expectedHppContent);

    std::wstring expectedCppContent = L""
    "#include \"" + this->GetIncludeFileName() + L"\"\r\n"
    "\r\n"
    "#include <map>\r\n"
    "#include <memory>\r\n"
    "#include <vector>\r\n"
    "\r\n"
    "#include \"exception_macro.hpp\"\r\n"
    "#include \"i_object.hpp\"\r\n"
    "#include \"property_accessor_macro.hpp\"\r\n"
    "#include \"vcc_enum.hpp\"\r\n"
    "#include \"vpg_object.hpp\"\r\n"
    "#include \"vpg_object_a.hpp\"\r\n"
    "#include \"vpg_object_property.hpp\"\r\n"
    "\r\n"
    "using namespace vcc;\r\n"
    "\r\n"
    "int VPGObjectPropertyAccessor::_ReadInt(const int64_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::VectorInt:\r\n"
    "            return obj->GetVectorInt(index);\r\n"
    "        case VPGObjectProperty::OrderedMapInt:\r\n"
    "            return obj->GetOrderedMapIntByIndex(index).second;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return 0;\r\n"
    "}\r\n"
    "\r\n"
    "int VPGObjectPropertyAccessor::_ReadInt(const int64_t &objectProperty, const void *key) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        assert(key != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::MapInt: {\r\n"
    "            const int *keyPtr = static_cast<const int *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            return obj->GetMapInt(*keyPtr);\r\n"
    "        }\r\n"
    "        case VPGObjectProperty::OrderedMapInt: {\r\n"
    "            const int *keyPtr = static_cast<const int *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            return obj->GetOrderedMapIntByKey(*keyPtr).second;\r\n"
    "        }\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return 0;\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteInt(const int64_t &objectProperty, const int &value, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::VectorInt:\r\n"
    "            if (index > -1)\r\n"
    "                obj->SetVectorInt(index, value);\r\n"
    "            else\r\n"
    "                obj->InsertVectorInt(value);\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::OrderedMapInt:\r\n"
    "            if (index > -1)\r\n"
    "                obj->SetOrderedMapIntByIndex(index, value);\r\n"
    "            else\r\n"
    "                obj->InsertOrderedMapInt(value);\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteInt(const int64_t &objectProperty, const int &value, const void *key) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        assert(key != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::MapInt: {\r\n"
    "            const int *keyPtr = static_cast<const int *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            if (obj->IsMapIntContainKey(*keyPtr))\r\n"
    "                obj->SetMapInt(*keyPtr, value);\r\n"
    "            else\r\n"
    "                obj->InsertMapInt(*keyPtr, value);\r\n"
    "            break;\r\n"
    "        }\r\n"
    "        case VPGObjectProperty::OrderedMapInt: {\r\n"
    "            const int *keyPtr = static_cast<const int *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            if (obj->IsOrderedMapIntContainKey(*keyPtr))\r\n"
    "                obj->SetOrderedMapIntByKey(*keyPtr, value);\r\n"
    "            else\r\n"
    "                obj->InsertOrderedMapInt(*keyPtr, value);\r\n"
    "            break;\r\n"
    "        }\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_InsertInt(const int64_t &objectProperty, const int &value, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::VectorInt:\r\n"
    "            if (index > -1)\r\n"
    "                obj->InsertVectorInt(index, value);\r\n"
    "            else\r\n"
    "                obj->InsertVectorInt(value);\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::OrderedMapInt:\r\n"
    "            if (index > -1)\r\n"
    "                obj->InsertOrderedMapInt(index, value);\r\n"
    "            else\r\n"
    "                obj->InsertOrderedMapInt(value);\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "long VPGObjectPropertyAccessor::_ReadLong(const int64_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::VectorEnum:\r\n"
    "            return static_cast<long>(obj->GetVectorEnum(index));\r\n"
    "        case VPGObjectProperty::OrderedMapEnum:\r\n"
    "            return static_cast<long>(obj->GetOrderedMapEnumByIndex(index).second);\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return 0L;\r\n"
    "}\r\n"
    "\r\n"
    "long VPGObjectPropertyAccessor::_ReadLong(const int64_t &objectProperty, const void *key) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        assert(key != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::MapEnum: {\r\n"
    "            const char *keyPtr = static_cast<const char *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            return static_cast<long>(obj->GetMapEnum(*keyPtr));\r\n"
    "        }\r\n"
    "        case VPGObjectProperty::OrderedMapEnum: {\r\n"
    "            const char *keyPtr = static_cast<const char *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            return static_cast<long>(obj->GetOrderedMapEnumByKey(*keyPtr).second);\r\n"
    "        }\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return 0L;\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteLong(const int64_t &objectProperty, const long &value, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::VectorEnum:\r\n"
    "            if (index > -1)\r\n"
    "                obj->SetVectorEnum(index, static_cast<VCCEnum>(value));\r\n"
    "            else\r\n"
    "                obj->InsertVectorEnum(static_cast<VCCEnum>(value));\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::OrderedMapEnum:\r\n"
    "            if (index > -1)\r\n"
    "                obj->SetOrderedMapEnumByIndex(index, static_cast<VCCEnum>(value));\r\n"
    "            else\r\n"
    "                obj->InsertOrderedMapEnum(static_cast<VCCEnum>(value));\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteLong(const int64_t &objectProperty, const long &value, const void *key) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        assert(key != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::MapEnum: {\r\n"
    "            const char *keyPtr = static_cast<const char *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            if (obj->IsMapEnumContainKey(*keyPtr))\r\n"
    "                obj->SetMapEnum(*keyPtr, static_cast<VCCEnum>(value));\r\n"
    "            else\r\n"
    "                obj->InsertMapEnum(*keyPtr, static_cast<VCCEnum>(value));\r\n"
    "            break;\r\n"
    "        }\r\n"
    "        case VPGObjectProperty::OrderedMapEnum: {\r\n"
    "            const char *keyPtr = static_cast<const char *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            if (obj->IsOrderedMapEnumContainKey(*keyPtr))\r\n"
    "                obj->SetOrderedMapEnumByKey(*keyPtr, static_cast<VCCEnum>(value));\r\n"
    "            else\r\n"
    "                obj->InsertOrderedMapEnum(*keyPtr, static_cast<VCCEnum>(value));\r\n"
    "            break;\r\n"
    "        }\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_InsertLong(const int64_t &objectProperty, const long &value, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::VectorEnum:\r\n"
    "            if (index > -1)\r\n"
    "                obj->InsertVectorEnum(index, static_cast<VCCEnum>(value));\r\n"
    "            else\r\n"
    "                obj->InsertVectorEnum(static_cast<VCCEnum>(value));\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::OrderedMapEnum:\r\n"
    "            if (index > -1)\r\n"
    "                obj->InsertOrderedMapEnum(index, static_cast<VCCEnum>(value));\r\n"
    "            else\r\n"
    "                obj->InsertOrderedMapEnum(static_cast<VCCEnum>(value));\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "std::shared_ptr<IObject> VPGObjectPropertyAccessor::_ReadObject(const int64_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::VectorObj:\r\n"
    "            return std::static_pointer_cast<IObject>(obj->GetVectorObj(index));\r\n"
    "        case VPGObjectProperty::OrderedMapObj:\r\n"
    "            return std::static_pointer_cast<IObject>(obj->GetOrderedMapObjByIndex(index).second);\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return nullptr;\r\n"
    "}\r\n"
    "\r\n"
    "std::shared_ptr<IObject> VPGObjectPropertyAccessor::_ReadObject(const int64_t &objectProperty, const void *key) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        assert(key != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::MapObj: {\r\n"
    "            const double *keyPtr = static_cast<const double *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            return std::static_pointer_cast<IObject>(obj->GetMapObj(*keyPtr));\r\n"
    "        }\r\n"
    "        case VPGObjectProperty::OrderedMapObj: {\r\n"
    "            const double *keyPtr = static_cast<const double *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            return std::static_pointer_cast<IObject>(obj->GetOrderedMapObjByKey(*keyPtr).second);\r\n"
    "        }\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return nullptr;\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteObject(const int64_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::VectorObj:\r\n"
    "            if (index > -1)\r\n"
    "                obj->SetVectorObj(index, std::static_pointer_cast<VPGObjectA>(value));\r\n"
    "            else\r\n"
    "                obj->InsertVectorObj(std::static_pointer_cast<VPGObjectA>(value));\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::OrderedMapObj:\r\n"
    "            if (index > -1)\r\n"
    "                obj->SetOrderedMapObjByIndex(index, std::static_pointer_cast<VPGObjectA>(value));\r\n"
    "            else\r\n"
    "                obj->InsertOrderedMapObj(std::static_pointer_cast<VPGObjectA>(value));\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteObject(const int64_t &objectProperty, std::shared_ptr<IObject> value, const void *key) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        assert(key != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::MapObj: {\r\n"
    "            const double *keyPtr = static_cast<const double *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            if (obj->IsMapObjContainKey(*keyPtr))\r\n"
    "                obj->SetMapObj(*keyPtr, std::static_pointer_cast<VPGObjectA>(value));\r\n"
    "            else\r\n"
    "                obj->InsertMapObj(*keyPtr, std::static_pointer_cast<VPGObjectA>(value));\r\n"
    "            break;\r\n"
    "        }\r\n"
    "        case VPGObjectProperty::OrderedMapObj: {\r\n"
    "            const double *keyPtr = static_cast<const double *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            if (obj->IsOrderedMapObjContainKey(*keyPtr))\r\n"
    "                obj->SetOrderedMapObjByKey(*keyPtr, std::static_pointer_cast<VPGObjectA>(value));\r\n"
    "            else\r\n"
    "                obj->InsertOrderedMapObj(*keyPtr, std::static_pointer_cast<VPGObjectA>(value));\r\n"
    "            break;\r\n"
    "        }\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_InsertObject(const int64_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::VectorObj:\r\n"
    "            if (index > -1)\r\n"
    "                obj->InsertVectorObj(index, std::static_pointer_cast<VPGObjectA>(value));\r\n"
    "            else\r\n"
    "                obj->InsertVectorObj(std::static_pointer_cast<VPGObjectA>(value));\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::OrderedMapObj:\r\n"
    "            if (index > -1)\r\n"
    "                obj->InsertOrderedMapObj(index, std::static_pointer_cast<VPGObjectA>(value));\r\n"
    "            else\r\n"
    "                obj->InsertOrderedMapObj(std::static_pointer_cast<VPGObjectA>(value));\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "std::shared_ptr<IObject> VPGObjectPropertyAccessor::_CloneObject(const int64_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::VectorObj:\r\n"
    "            return std::static_pointer_cast<IObject>(obj->CloneVectorObj(index));\r\n"
    "        case VPGObjectProperty::OrderedMapObj:\r\n"
    "            return std::static_pointer_cast<IObject>(obj->CloneOrderedMapObjByIndex(index));\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return nullptr;\r\n"
    "}\r\n"
    "\r\n"
    "std::shared_ptr<IObject> VPGObjectPropertyAccessor::_CloneObject(const int64_t &objectProperty, const void *key) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        assert(key != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::MapObj: {\r\n"
    "            const double *keyPtr = static_cast<const double *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            return std::static_pointer_cast<IObject>(obj->CloneMapObj(*keyPtr));\r\n"
    "        }\r\n"
    "        case VPGObjectProperty::OrderedMapObj: {\r\n"
    "            const double *keyPtr = static_cast<const double *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            return std::static_pointer_cast<IObject>(obj->CloneOrderedMapObjByKey(*keyPtr));\r\n"
    "        }\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return nullptr;\r\n"
    "}\r\n"
    "\r\n"
    "size_t VPGObjectPropertyAccessor::_GetContainerCount(const int64_t &objectProperty) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::VectorInt:\r\n"
    "            return obj->GetVectorInt().size();\r\n"
    "        case VPGObjectProperty::VectorEnum:\r\n"
    "            return obj->GetVectorEnum().size();\r\n"
    "        case VPGObjectProperty::VectorObj:\r\n"
    "            return obj->GetVectorObj().size();\r\n"
    "        case VPGObjectProperty::MapInt:\r\n"
    "            return obj->GetMapInt().size();\r\n"
    "        case VPGObjectProperty::MapEnum:\r\n"
    "            return obj->GetMapEnum().size();\r\n"
    "        case VPGObjectProperty::MapObj:\r\n"
    "            return obj->GetMapObj().size();\r\n"
    "        case VPGObjectProperty::OrderedMapInt:\r\n"
    "            return obj->GetOrderedMapInt().size();\r\n"
    "        case VPGObjectProperty::OrderedMapEnum:\r\n"
    "            return obj->GetOrderedMapEnum().size();\r\n"
    "        case VPGObjectProperty::OrderedMapObj:\r\n"
    "            return obj->GetOrderedMapObj().size();\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return 0;\r\n"
    "}\r\n"
    "\r\n"
    "std::set<void *> VPGObjectPropertyAccessor::_GetMapKeys(const int64_t &objectProperty) const\r\n"
    "{\r\n"
    "    std::set<void *> result;\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::MapInt:\r\n"
    "            return obj->GetMapIntVoidKeys();\r\n"
    "        case VPGObjectProperty::MapEnum:\r\n"
    "            return obj->GetMapEnumVoidKeys();\r\n"
    "        case VPGObjectProperty::MapObj:\r\n"
    "            return obj->GetMapObjVoidKeys();\r\n"
    "        case VPGObjectProperty::OrderedMapInt:\r\n"
    "            return obj->GetOrderedMapIntVoidKeys();\r\n"
    "        case VPGObjectProperty::OrderedMapEnum:\r\n"
    "            return obj->GetOrderedMapEnumVoidKeys();\r\n"
    "        case VPGObjectProperty::OrderedMapObj:\r\n"
    "            return obj->GetOrderedMapObjVoidKeys();\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return result;\r\n"
    "}\r\n"
    "\r\n"
    "bool VPGObjectPropertyAccessor::_IsContainKey(const int64_t &objectProperty, const void *key) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        assert(key != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::MapInt: {\r\n"
    "            const int *keyPtr = static_cast<const int *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            return obj->IsMapIntContainKey(*keyPtr);\r\n"
    "        }\r\n"
    "        case VPGObjectProperty::MapEnum: {\r\n"
    "            const char *keyPtr = static_cast<const char *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            return obj->IsMapEnumContainKey(*keyPtr);\r\n"
    "        }\r\n"
    "        case VPGObjectProperty::MapObj: {\r\n"
    "            const double *keyPtr = static_cast<const double *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            return obj->IsMapObjContainKey(*keyPtr);\r\n"
    "        }\r\n"
    "        case VPGObjectProperty::OrderedMapInt: {\r\n"
    "            const int *keyPtr = static_cast<const int *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            return obj->IsOrderedMapIntContainKey(*keyPtr);\r\n"
    "        }\r\n"
    "        case VPGObjectProperty::OrderedMapEnum: {\r\n"
    "            const char *keyPtr = static_cast<const char *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            return obj->IsOrderedMapEnumContainKey(*keyPtr);\r\n"
    "        }\r\n"
    "        case VPGObjectProperty::OrderedMapObj: {\r\n"
    "            const double *keyPtr = static_cast<const double *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            return obj->IsOrderedMapObjContainKey(*keyPtr);\r\n"
    "        }\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return false;\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_RemoveContainerElement(const int64_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::VectorInt:\r\n"
    "            obj->RemoveVectorInt(index);\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::VectorEnum:\r\n"
    "            obj->RemoveVectorEnum(index);\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::VectorObj:\r\n"
    "            obj->RemoveVectorObj(index);\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::OrderedMapInt:\r\n"
    "            obj->RemoveOrderedMapIntByIndex(index);\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::OrderedMapEnum:\r\n"
    "            obj->RemoveOrderedMapEnumByIndex(index);\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::OrderedMapObj:\r\n"
    "            obj->RemoveOrderedMapObjByIndex(index);\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_RemoveContainerElement(const int64_t &objectProperty, const void *key) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        assert(key != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::MapInt: {\r\n"
    "            const int *keyPtr = static_cast<const int *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            obj->RemoveMapInt(*keyPtr);\r\n"
    "            break;\r\n"
    "        }\r\n"
    "        case VPGObjectProperty::MapEnum: {\r\n"
    "            const char *keyPtr = static_cast<const char *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            obj->RemoveMapEnum(*keyPtr);\r\n"
    "            break;\r\n"
    "        }\r\n"
    "        case VPGObjectProperty::MapObj: {\r\n"
    "            const double *keyPtr = static_cast<const double *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            obj->RemoveMapObj(*keyPtr);\r\n"
    "            break;\r\n"
    "        }\r\n"
    "        case VPGObjectProperty::OrderedMapInt: {\r\n"
    "            const int *keyPtr = static_cast<const int *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            obj->RemoveOrderedMapIntByKey(*keyPtr);\r\n"
    "            break;\r\n"
    "        }\r\n"
    "        case VPGObjectProperty::OrderedMapEnum: {\r\n"
    "            const char *keyPtr = static_cast<const char *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            obj->RemoveOrderedMapEnumByKey(*keyPtr);\r\n"
    "            break;\r\n"
    "        }\r\n"
    "        case VPGObjectProperty::OrderedMapObj: {\r\n"
    "            const double *keyPtr = static_cast<const double *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            obj->RemoveOrderedMapObjByKey(*keyPtr);\r\n"
    "            break;\r\n"
    "        }\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_ClearContainer(const int64_t &objectProperty) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::VectorInt:\r\n"
    "            obj->ClearVectorInt();\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::VectorEnum:\r\n"
    "            obj->ClearVectorEnum();\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::VectorObj:\r\n"
    "            obj->ClearVectorObj();\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::MapInt:\r\n"
    "            obj->ClearMapInt();\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::MapEnum:\r\n"
    "            obj->ClearMapEnum();\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::MapObj:\r\n"
    "            obj->ClearMapObj();\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::OrderedMapInt:\r\n"
    "            obj->ClearOrderedMapInt();\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::OrderedMapEnum:\r\n"
    "            obj->ClearOrderedMapEnum();\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::OrderedMapObj:\r\n"
    "            obj->ClearOrderedMapObj();\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n";
    EXPECT_EQ(ReadFile(this->GetFilePathCpp()), expectedCppContent);
}

TEST_F(VPGPropertyAccessorFileGenerationServiceTest, Mix)
{
    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
    DECLARE_SPTR(VPGEnumClass, enumClass);
    enumClass->SetName(L"VPGObjectProperty");
    enumClassList.push_back(enumClass);

    DECLARE_SPTR(VPGEnumClassProperty, propBool);
    propBool->SetEnum(L"Bool");
    propBool->SetMacro(L"GETSET(bool, Bool, false)");
    propBool->SetType1(L"bool");
    propBool->SetType2(L"");
    propBool->SetPropertyName(L"Bool");
    propBool->SetDefaultValue(L"");
    enumClass->InsertProperties(propBool);

    DECLARE_SPTR(VPGEnumClassProperty, propString);
    propString->SetEnum(L"String");
    propString->SetMacro(L"GETSET(std::wstring, String, L\"\")");
    propString->SetType1(L"std::wstring");
    propString->SetType2(L"");
    propString->SetPropertyName(L"String");
    propString->SetDefaultValue(L"");
    enumClass->InsertProperties(propString);

    DECLARE_SPTR(VPGEnumClassProperty, propObject);
    propObject->SetEnum(L"Object");
    propObject->SetMacro(L"GETSET_SPTR_NULL(VPGObjectA, Object)");
    propObject->SetType1(L"VPGObjectA");
    propObject->SetType2(L"");
    propObject->SetPropertyName(L"Object");
    propObject->SetDefaultValue(L"");
    enumClass->InsertProperties(propObject);

    DECLARE_SPTR(VPGEnumClassProperty, propVector);
    propVector->SetEnum(L"Vector");
    propVector->SetMacro(L"VECTOR(std::wstring, Vector)");
    propVector->SetType1(L"std::wstring");
    propVector->SetType2(L"");
    propVector->SetPropertyName(L"Vector");
    propVector->SetDefaultValue(L"");
    enumClass->InsertProperties(propVector);

    DECLARE_SPTR(VPGEnumClassProperty, propMap);
    propMap->SetEnum(L"Map");
    propMap->SetMacro(L"MAP(int, double, Map)");
    propMap->SetType1(L"int");
    propMap->SetType2(L"double");
    propMap->SetPropertyName(L"Map");
    propMap->SetDefaultValue(L"");
    enumClass->InsertProperties(propMap);


    std::set<std::wstring> propertyTypes;
    VPGPropertyAccessorGenerationService::GenerateHpp(this->GetLogConfig().get(), this->GetFilePathHpp(), enumClassList);
    VPGPropertyAccessorGenerationService::GenerateCpp(this->GetLogConfig().get(), this->GetProjectIncludeList(), this->GetFilePathCpp(), enumClassList);
    EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));
    EXPECT_TRUE(IsFileExists(this->GetFilePathCpp()));

    std::wstring expectedHppContent = L""
    "#pragma once\r\n"
    "\r\n"
    "#include <string>\r\n"
    "\r\n"
    "#include \"base_property_accessor.hpp\"\r\n"
    "#include \"property_accessor_macro.hpp\"\r\n"
    "\r\n"
    "using namespace vcc;\r\n"
    "\r\n"
    "class VPGObjectPropertyAccessor : public BasePropertyAccessor\r\n"
    "{\r\n"
    "    PROPERTY_ACCESSOR_HEADER(bool, Bool)\r\n"
    "    PROPERTY_ACCESSOR_HEADER(double, Double)\r\n"
    "    PROPERTY_ACCESSOR_HEADER(std::wstring, String)\r\n"
    "    PROPERTY_ACCESSOR_OBJECT_HEADER(std::shared_ptr<IObject>, Object)\r\n"
    "\r\n"
    "    PROPERTY_ACCESSOR_CONTAINER_HEADER\r\n"
    "\r\n"
    "    public:\r\n"
    "        VPGObjectPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}\r\n"
    "        virtual ~VPGObjectPropertyAccessor() {}\r\n"
    "};\r\n";
    EXPECT_EQ(ReadFile(this->GetFilePathHpp()), expectedHppContent);

    std::wstring expectedCppContent = L""
    "#include \"" + this->GetIncludeFileName() + L"\"\r\n"
    "\r\n"
    "#include <map>\r\n"
    "#include <memory>\r\n"
    "#include <string>\r\n"
    "#include <vector>\r\n"
    "\r\n"
    "#include \"exception_macro.hpp\"\r\n"
    "#include \"i_object.hpp\"\r\n"
    "#include \"property_accessor_macro.hpp\"\r\n"
    "#include \"vpg_object.hpp\"\r\n"
    "#include \"vpg_object_a.hpp\"\r\n"
    "#include \"vpg_object_property.hpp\"\r\n"
    "\r\n"
    "using namespace vcc;\r\n"
    "\r\n"
    "bool VPGObjectPropertyAccessor::_ReadBool(const int64_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::Bool:\r\n"
    "            return obj->GetBool();\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return false;\r\n"
    "}\r\n"
    "\r\n"
    "bool VPGObjectPropertyAccessor::_ReadBool(const int64_t &objectProperty, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "    return false;\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool &value, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::Bool:\r\n"
    "            obj->SetBool(value);\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteBool(const int64_t &objectProperty, const bool & /*value*/, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_InsertBool(const int64_t &objectProperty, const bool & /*value*/, const int64_t & /*index*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "double VPGObjectPropertyAccessor::_ReadDouble(const int64_t &objectProperty, const int64_t & /*index*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "    return 0;\r\n"
    "}\r\n"
    "\r\n"
    "double VPGObjectPropertyAccessor::_ReadDouble(const int64_t &objectProperty, const void *key) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        assert(key != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::Map: {\r\n"
    "            const int *keyPtr = static_cast<const int *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            return obj->GetMap(*keyPtr);\r\n"
    "        }\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return 0;\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteDouble(const int64_t &objectProperty, const double & /*value*/, const int64_t & /*index*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteDouble(const int64_t &objectProperty, const double &value, const void *key) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        assert(key != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::Map: {\r\n"
    "            const int *keyPtr = static_cast<const int *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            if (obj->IsMapContainKey(*keyPtr))\r\n"
    "                obj->SetMap(*keyPtr, value);\r\n"
    "            else\r\n"
    "                obj->InsertMap(*keyPtr, value);\r\n"
    "            break;\r\n"
    "        }\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_InsertDouble(const int64_t &objectProperty, const double & /*value*/, const int64_t & /*index*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "std::wstring VPGObjectPropertyAccessor::_ReadString(const int64_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::String:\r\n"
    "            return obj->GetString();\r\n"
    "        case VPGObjectProperty::Vector:\r\n"
    "            return obj->GetVector(index);\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return L\"\";\r\n"
    "}\r\n"
    "\r\n"
    "std::wstring VPGObjectPropertyAccessor::_ReadString(const int64_t &objectProperty, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "    return L\"\";\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring &value, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::String:\r\n"
    "            obj->SetString(value);\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::Vector:\r\n"
    "            if (index > -1)\r\n"
    "                obj->SetVector(index, value);\r\n"
    "            else\r\n"
    "                obj->InsertVector(value);\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteString(const int64_t &objectProperty, const std::wstring & /*value*/, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_InsertString(const int64_t &objectProperty, const std::wstring &value, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::Vector:\r\n"
    "            if (index > -1)\r\n"
    "                obj->InsertVector(index, value);\r\n"
    "            else\r\n"
    "                obj->InsertVector(value);\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "std::shared_ptr<IObject> VPGObjectPropertyAccessor::_ReadObject(const int64_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::Object:\r\n"
    "            return std::static_pointer_cast<IObject>(obj->GetObject());\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return nullptr;\r\n"
    "}\r\n"
    "\r\n"
    "std::shared_ptr<IObject> VPGObjectPropertyAccessor::_ReadObject(const int64_t &objectProperty, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "    return nullptr;\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteObject(const int64_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::Object:\r\n"
    "            obj->SetObject(std::static_pointer_cast<VPGObjectA>(value));\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_WriteObject(const int64_t &objectProperty, std::shared_ptr<IObject> /*value*/, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_InsertObject(const int64_t &objectProperty, std::shared_ptr<IObject> /*value*/, const int64_t & /*index*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "std::shared_ptr<IObject> VPGObjectPropertyAccessor::_CloneObject(const int64_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::Object:\r\n"
    "            return std::static_pointer_cast<IObject>(obj->GetObject()->Clone());\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return nullptr;\r\n"
    "}\r\n"
    "\r\n"
    "std::shared_ptr<IObject> VPGObjectPropertyAccessor::_CloneObject(const int64_t &objectProperty, const void * /*key*/) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        THROW_EXCEPTION_MSG_FOR_BASE_PROPERTY_ACCESSOR_DETAIL_PROPERTY_NOT_FOUND\r\n"
    "    CATCH\r\n"
    "    return nullptr;\r\n"
    "}\r\n"
    "\r\n"
    "size_t VPGObjectPropertyAccessor::_GetContainerCount(const int64_t &objectProperty) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::Vector:\r\n"
    "            return obj->GetVector().size();\r\n"
    "        case VPGObjectProperty::Map:\r\n"
    "            return obj->GetMap().size();\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return 0;\r\n"
    "}\r\n"
    "\r\n"
    "std::set<void *> VPGObjectPropertyAccessor::_GetMapKeys(const int64_t &objectProperty) const\r\n"
    "{\r\n"
    "    std::set<void *> result;\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::Map:\r\n"
    "            return obj->GetMapVoidKeys();\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return result;\r\n"
    "}\r\n"
    "\r\n"
    "bool VPGObjectPropertyAccessor::_IsContainKey(const int64_t &objectProperty, const void *key) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        assert(key != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::Map: {\r\n"
    "            const int *keyPtr = static_cast<const int *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            return obj->IsMapContainKey(*keyPtr);\r\n"
    "        }\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "    return false;\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_RemoveContainerElement(const int64_t &objectProperty, const int64_t &index) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        assert(index >= -1);\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::Vector:\r\n"
    "            obj->RemoveVector(index);\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_RemoveContainerElement(const int64_t &objectProperty, const void *key) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        assert(key != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::Map: {\r\n"
    "            const int *keyPtr = static_cast<const int *>(key);\r\n"
    "            assert(keyPtr != nullptr);\r\n"
    "            if (keyPtr == nullptr)\r\n"
    "                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L\"Invalid Property Accessor Map Key\");\r\n"
    "            obj->RemoveMap(*keyPtr);\r\n"
    "            break;\r\n"
    "        }\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n"
    "\r\n"
    "void VPGObjectPropertyAccessor::_ClearContainer(const int64_t &objectProperty) const\r\n"
    "{\r\n"
    "    TRY\r\n"
    "        std::shared_ptr<VPGObject> obj = std::static_pointer_cast<VPGObject>(_Object);\r\n"
    "        assert(obj != nullptr);\r\n"
    "        switch(static_cast<VPGObjectProperty>(objectProperty))\r\n"
    "        {\r\n"
    "        case VPGObjectProperty::Vector:\r\n"
    "            obj->ClearVector();\r\n"
    "            break;\r\n"
    "        case VPGObjectProperty::Map:\r\n"
    "            obj->ClearMap();\r\n"
    "            break;\r\n"
    "        default:\r\n"
    "            assert(false);\r\n"
    "        }\r\n"
    "    CATCH\r\n"
    "}\r\n";
    EXPECT_EQ(ReadFile(this->GetFilePathCpp()), expectedCppContent);
}
