#include <gtest/gtest.h>

#include "memory_macro.hpp"
#include "vpg_enum_class_reader.hpp"
#include "vpg_file_generation_manager.hpp"

class VPGEnumClassReaderTest : public testing::Test 
{
    MANAGER(VPGEnumClassReader, Reader);
    public:
        void SetUp() override
        {
            DECLARE_UPTR(VPGFileGenerationManager, manager, nullptr);
            manager->GetClassMacroList(L"");
            _Reader->InsertClassMacroList(manager->GetClassMacros());
        }

        void TearDown() override
        {
        }
};

TEST_F(VPGEnumClassReaderTest, GetAttribute)
{
    std::vector<std::wstring> result = this->GetReader()->GetAttribute(L"");
    std::vector<std::wstring> expectedResult = {};
    EXPECT_EQ(result, expectedResult);

    result = this->GetReader()->GetAttribute(L"abc");
    expectedResult = {};
    EXPECT_EQ(result, expectedResult);
    
    result = this->GetReader()->GetAttribute(L"@");
    expectedResult = { };
    EXPECT_EQ(result, expectedResult);
    
    result = this->GetReader()->GetAttribute(L"@@");
    expectedResult = { };
    EXPECT_EQ(result, expectedResult);
    
    result = this->GetReader()->GetAttribute(L"@@ReadWrite NoAccess");
    expectedResult = { L"@@ReadWrite NoAccess" };
    EXPECT_EQ(result, expectedResult);
    
    result = this->GetReader()->GetAttribute(L"@@ReadWrite @NoAccess");
    expectedResult = { L"@@ReadWrite @NoAccess" };
    EXPECT_EQ(result, expectedResult);
    
    result = this->GetReader()->GetAttribute(L"@@ReadWrite No@@Access");
    expectedResult = { L"@@ReadWrite No", L"@@Access" };
    EXPECT_EQ(result, expectedResult);
    
    result = this->GetReader()->GetAttribute(L"@@ReadWrite @@NoAccess");
    expectedResult = { L"@@ReadWrite", L"@@NoAccess" };
    EXPECT_EQ(result, expectedResult);

    result = this->GetReader()->GetAttribute(L"@@ReadWrite( @@NoAccess)");
    expectedResult = { L"@@ReadWrite(", L"@@NoAccess)" };
    EXPECT_EQ(result, expectedResult);
}

TEST_F(VPGEnumClassReaderTest, Normal)
{
    std::wstring code = L""
        L"#pragma once\r\n"
        L"\r\n"
        L"#include <string>\r\n"
        L"#include <vector>\r\n"
        L"\r\n"
        L"#include \"class_macro.hpp\"\r\n"
        L"/*\r\n"
        L"enum class VCCObjectProperty { // Class Command\r\n"
        L"    EnumA,\r\n"
        L"    EnumB, // Command B\r\n"
        L"    EnumC,\r\n"
        L"    EnumD\r\n"
        L"};*/\r\n"
        L"\r\n"
        L"enum class VCCObjectProperty // Class Command\r\n"
        L"{\r\n"
        L"    EnumA // Command A\r\n"
        L"};\r\n"
        L"\r\n"
        L"enum class VCCSingleLineProperty { EnumA, EnumB, /* Command B */ EnumC }"
        L"\r\n"
        L"enum class VCCMultiProperty {\r\n"
        L"    EnumA,\r\n"
        L"    EnumB, // Command B\r\n"
        L"    EnumC, /* Command C*/\r\n"
        L"    EnumD, /* Command D *//* Command E */\r\n"
        L"    EnumE /* Command F */\r\n"
        L"      /* Command G */\r\n"
        L"};\r\n";

    std::vector<std::shared_ptr<VPGEnumClass>> results;
    this->GetReader()->Parse(code, results);
    EXPECT_EQ(results.size(), (size_t)3);
    // first
    std::shared_ptr<VPGEnumClass> element = results.at(0);
    EXPECT_EQ(element->GetName(), L"VCCObjectProperty");
    EXPECT_EQ(element->GetCommand(), L"Class Command");
    EXPECT_EQ(element->GetProperties().size(), 1UL);
    EXPECT_EQ(element->GetProperties().at(0)->GetEnum(), L"EnumA");
    EXPECT_EQ(element->GetProperties().at(0)->GetCommand(), L"Command A");

    // second
    element = results.at(1);
    EXPECT_EQ(element->GetName(), L"VCCSingleLineProperty");
    EXPECT_EQ(element->GetCommand(), L"");
    EXPECT_EQ(element->GetProperties().size(), 3UL);
    EXPECT_EQ(element->GetProperties().at(0)->GetEnum(), L"EnumA");
    EXPECT_EQ(element->GetProperties().at(0)->GetCommand(), L"");
    EXPECT_EQ(element->GetProperties().at(1)->GetEnum(), L"EnumB");
    EXPECT_EQ(element->GetProperties().at(1)->GetCommand(), L"Command B");
    EXPECT_EQ(element->GetProperties().at(2)->GetEnum(), L"EnumC");
    EXPECT_EQ(element->GetProperties().at(2)->GetCommand(), L"");

    // third
    element = results.at(2);
    EXPECT_EQ(element->GetName(), L"VCCMultiProperty");
    EXPECT_EQ(element->GetCommand(), L"");
    EXPECT_EQ(element->GetProperties().size(), 5UL);
    EXPECT_EQ(element->GetProperties().at(0)->GetEnum(), L"EnumA");
    EXPECT_EQ(element->GetProperties().at(0)->GetCommand(), L"");
    EXPECT_EQ(element->GetProperties().at(1)->GetEnum(), L"EnumB");
    EXPECT_EQ(element->GetProperties().at(1)->GetCommand(), L"Command B");
    EXPECT_EQ(element->GetProperties().at(2)->GetEnum(), L"EnumC");
    EXPECT_EQ(element->GetProperties().at(2)->GetCommand(), L"Command C");
    EXPECT_EQ(element->GetProperties().at(3)->GetEnum(), L"EnumD");
    EXPECT_EQ(element->GetProperties().at(3)->GetCommand(), L"Command D\r\nCommand E");
    EXPECT_EQ(element->GetProperties().at(4)->GetEnum(), L"EnumE");
    EXPECT_EQ(element->GetProperties().at(4)->GetCommand(), L"Command F\r\nCommand G");
}

TEST_F(VPGEnumClassReaderTest, VCCEnumClassProperty)
{
    std::wstring code = L""
        L"#pragma once\r\n"
        L"\r\n"
        L"#include <string>\r\n"
        L"#include <vector>\r\n"
        L"\r\n"
        L"enum class VCCObjectProperty\r\n"
        L"{\r\n"
    // normal get set
        L"    EnumA, // GETSET(EnumTypeA, EnumA, L\"Default\") CommandA\r\n"
        L"    EnumB, // GETSET_SPTR(EnumTypeB, EnumB)\r\n"
        L"    EnumC, // GETSET_SPTR(EnumTypeC, EnumC, ArgumentA, ArgumentB, ArgumentC)\r\n"
    // command
        L"    EnumD, // Command D\r\n"
        L"    EnumE,\r\n"
    // vector
        L"    EnumVector, // VECTOR(int, Vector)\r\n"
    // map
        L"    EnumMap // MAP(int, std::wstring, Map) \r\n"
        L"};\r\n";

    std::vector<std::shared_ptr<VPGEnumClass>> results;
    this->GetReader()->Parse(code, results);
    EXPECT_EQ(results.size(), (size_t)1);
    // first
    std::shared_ptr<VPGEnumClass> element = results.at(0);
    EXPECT_EQ(element->GetName(), L"VCCObjectProperty");
    EXPECT_EQ(element->GetCommand(), L"");
    EXPECT_EQ(element->GetProperties().size(), (size_t)7);
    EXPECT_EQ(element->GetProperties().at(0)->GetEnum(), L"EnumA");
    EXPECT_EQ(element->GetProperties().at(0)->GetMacro(), L"GETSET(EnumTypeA, EnumA, L\"Default\")");
    EXPECT_EQ(element->GetProperties().at(0)->GetType1(), L"EnumTypeA");
    EXPECT_EQ(element->GetProperties().at(0)->GetType2(), L"");
    EXPECT_EQ(element->GetProperties().at(0)->GetPropertyName(), L"EnumA");
    EXPECT_EQ(element->GetProperties().at(0)->GetDefaultValue(), L"L\"Default\"");
    EXPECT_EQ(element->GetProperties().at(0)->GetCommand(), L"");
    EXPECT_EQ(element->GetProperties().at(1)->GetEnum(), L"EnumB");
    EXPECT_EQ(element->GetProperties().at(1)->GetMacro(), L"GETSET_SPTR(EnumTypeB, EnumB)");
    EXPECT_EQ(element->GetProperties().at(1)->GetType1(), L"EnumTypeB");
    EXPECT_EQ(element->GetProperties().at(1)->GetType2(), L"");
    EXPECT_EQ(element->GetProperties().at(1)->GetPropertyName(), L"EnumB");
    EXPECT_EQ(element->GetProperties().at(1)->GetDefaultValue(), L"");
    EXPECT_EQ(element->GetProperties().at(1)->GetCommand(), L"");
    EXPECT_EQ(element->GetProperties().at(2)->GetEnum(), L"EnumC");
    EXPECT_EQ(element->GetProperties().at(2)->GetMacro(), L"GETSET_SPTR(EnumTypeC, EnumC, ArgumentA, ArgumentB, ArgumentC)");
    EXPECT_EQ(element->GetProperties().at(2)->GetType1(), L"EnumTypeC");
    EXPECT_EQ(element->GetProperties().at(2)->GetType2(), L"");
    EXPECT_EQ(element->GetProperties().at(2)->GetPropertyName(), L"EnumC");
    EXPECT_EQ(element->GetProperties().at(2)->GetDefaultValue(), L"ArgumentA, ArgumentB, ArgumentC");
    EXPECT_EQ(element->GetProperties().at(2)->GetCommand(), L"");
    EXPECT_EQ(element->GetProperties().at(3)->GetEnum(), L"EnumD");
    EXPECT_EQ(element->GetProperties().at(3)->GetMacro(), L"");
    EXPECT_EQ(element->GetProperties().at(3)->GetType1(), L"");
    EXPECT_EQ(element->GetProperties().at(3)->GetType2(), L"");
    EXPECT_EQ(element->GetProperties().at(3)->GetPropertyName(), L"");
    EXPECT_EQ(element->GetProperties().at(3)->GetDefaultValue(), L"");
    EXPECT_EQ(element->GetProperties().at(3)->GetCommand(), L"Command D");
    EXPECT_EQ(element->GetProperties().at(4)->GetEnum(), L"EnumE");
    EXPECT_EQ(element->GetProperties().at(4)->GetMacro(), L"");
    EXPECT_EQ(element->GetProperties().at(4)->GetType1(), L"");
    EXPECT_EQ(element->GetProperties().at(4)->GetType2(), L"");
    EXPECT_EQ(element->GetProperties().at(4)->GetPropertyName(), L"");
    EXPECT_EQ(element->GetProperties().at(4)->GetDefaultValue(), L"");
    EXPECT_EQ(element->GetProperties().at(4)->GetCommand(), L"");
    EXPECT_EQ(element->GetProperties().at(5)->GetEnum(), L"EnumVector");
    EXPECT_EQ(element->GetProperties().at(5)->GetMacro(), L"VECTOR(int, Vector)");
    EXPECT_EQ(element->GetProperties().at(5)->GetType1(), L"int");
    EXPECT_EQ(element->GetProperties().at(5)->GetType2(), L"");
    EXPECT_EQ(element->GetProperties().at(5)->GetPropertyName(), L"Vector");
    EXPECT_EQ(element->GetProperties().at(5)->GetDefaultValue(), L"");
    EXPECT_EQ(element->GetProperties().at(5)->GetCommand(), L"");
    EXPECT_EQ(element->GetProperties().at(6)->GetEnum(), L"EnumMap");
    EXPECT_EQ(element->GetProperties().at(6)->GetMacro(), L"MAP(int, std::wstring, Map)");
    EXPECT_EQ(element->GetProperties().at(6)->GetType1(), L"int");
    EXPECT_EQ(element->GetProperties().at(6)->GetType2(), L"std::wstring");
    EXPECT_EQ(element->GetProperties().at(6)->GetPropertyName(), L"Map");
    EXPECT_EQ(element->GetProperties().at(6)->GetDefaultValue(), L"");
    EXPECT_EQ(element->GetProperties().at(6)->GetCommand(), L"");
}

TEST_F(VPGEnumClassReaderTest, AccessMode)
{
    std::wstring code = L""
        "#pragma once\r\n"
        ""
        "enum class VCCObjectProperty\r\n"
        "{\r\n"
        "    EnumA, // GETSET(int, EnumA, 0)\r\n"
        "    EnumB, // GETSET(int, EnumB, 0) @@ReadOnly\r\n"
        "    EnumC, // GETSET(int, EnumC, 0) @@WriteOnly\r\n"
        "    EnumD, // GETSET(int, EnumD, 0) @@ReadWrite\r\n"
        "    EnumE // GETSET(int, EnumE, 0) @@NoAccess\r\n"
        "};\r\n";

    std::vector<std::shared_ptr<VPGEnumClass>> results;
    this->GetReader()->Parse(code, results);
    EXPECT_EQ(results.size(), (size_t)1);
    // first
    std::shared_ptr<VPGEnumClass> element = results.at(0);
    EXPECT_EQ(element->GetName(), L"VCCObjectProperty");
    EXPECT_EQ(element->GetCommand(), L"");
    EXPECT_EQ(element->GetProperties().size(), 5UL);
    EXPECT_EQ(element->GetProperties().at(0)->GetEnum(), L"EnumA");
    EXPECT_EQ(element->GetProperties().at(0)->GetCommand(), L"");
    EXPECT_EQ(element->GetProperties().at(0)->GetAccessMode(), VPGEnumClassPropertyAccessMode::ReadWrite);
    EXPECT_EQ(element->GetProperties().at(1)->GetEnum(), L"EnumB");
    EXPECT_EQ(element->GetProperties().at(1)->GetCommand(), L"");
    EXPECT_EQ(element->GetProperties().at(1)->GetAccessMode(), VPGEnumClassPropertyAccessMode::ReadOnly);
    EXPECT_EQ(element->GetProperties().at(2)->GetEnum(), L"EnumC");
    EXPECT_EQ(element->GetProperties().at(2)->GetCommand(), L"");
    EXPECT_EQ(element->GetProperties().at(2)->GetAccessMode(), VPGEnumClassPropertyAccessMode::WriteOnly);
    EXPECT_EQ(element->GetProperties().at(3)->GetEnum(), L"EnumD");
    EXPECT_EQ(element->GetProperties().at(3)->GetCommand(), L"");
    EXPECT_EQ(element->GetProperties().at(3)->GetAccessMode(), VPGEnumClassPropertyAccessMode::ReadWrite);
    EXPECT_EQ(element->GetProperties().at(4)->GetEnum(), L"EnumE");
    EXPECT_EQ(element->GetProperties().at(4)->GetCommand(), L"");
    EXPECT_EQ(element->GetProperties().at(4)->GetAccessMode(), VPGEnumClassPropertyAccessMode::NoAccess);
}
