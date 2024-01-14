#include <gtest/gtest.h>

#include "vpg_enum_class_reader.hpp"
#include "vpg_file_generation_service.hpp"

TEST(VPGEnumClassReaderTest, Normal)
{
    std::wstring code = L"";
    code += L"#pragma once\r\n";
    code += L"\r\n";
    code += L"#include <string>\r\n";
    code += L"#include <vector>\r\n";
    code += L"\r\n";
    code += L"#include \"class_macro.hpp\"\r\n";
    code += L"/*\r\n";
    code += L"enum class VCCHiddenProperty { // Class Command\r\n";
    code += L"    EnumA, // Command A\r\n";
    code += L"    EnumB, /* Command B*/\r\n";
    code += L"    EnumC,\r\n";
    code += L"    EnumD // Command D\r\n";
    code += L"};*/\r\n";
    code += L"\r\n";
    code += L"enum class VCCObjectProperty // Class Command\r\n";
    code += L"{\r\n";
    code += L"    EnumA, // Command A\r\n";
    code += L"    EnumB, /* Command B*/\r\n";
    code += L"    EnumC,\r\n";
    code += L"    EnumD // Command D\r\n";
    code += L"};\r\n";
    code += L"\r\n";    
    code += L"enum class VCCEnumProperty { EnumA, EnumB, /* Command B */ EnumC }";

    VPGEnumClassReader reader({});
    std::vector<std::shared_ptr<VPGEnumClass>> results;
    reader.Parse(code, results);
    EXPECT_EQ(results.size(), (size_t)2);
    // first
    std::shared_ptr<VPGEnumClass> element = results.at(0);
    EXPECT_EQ(element->GetName(), L"VCCObjectProperty");
    EXPECT_EQ(element->GetCommand(), L"Class Command");
    EXPECT_EQ(element->GetProperties()->size(), (size_t)4);
    EXPECT_EQ(element->GetProperties()->at(0)->GetEnum(), L"EnumA");
    EXPECT_EQ(element->GetProperties()->at(0)->GetCommand(), L"Command A");
    EXPECT_EQ(element->GetProperties()->at(1)->GetEnum(), L"EnumB");
    EXPECT_EQ(element->GetProperties()->at(1)->GetCommand(), L"Command B");
    EXPECT_EQ(element->GetProperties()->at(2)->GetEnum(), L"EnumC");
    EXPECT_EQ(element->GetProperties()->at(2)->GetCommand(), L"");
    EXPECT_EQ(element->GetProperties()->at(3)->GetEnum(), L"EnumD");
    EXPECT_EQ(element->GetProperties()->at(3)->GetCommand(), L"Command D");

    // second
    element = results.at(1);
    EXPECT_EQ(element->GetName(), L"VCCEnumProperty");
    EXPECT_EQ(element->GetCommand(), L"");
    EXPECT_EQ(element->GetProperties()->size(), (size_t)3);
    EXPECT_EQ(element->GetProperties()->at(0)->GetEnum(), L"EnumA");
    EXPECT_EQ(element->GetProperties()->at(0)->GetCommand(), L"");
    EXPECT_EQ(element->GetProperties()->at(1)->GetEnum(), L"EnumB");
    EXPECT_EQ(element->GetProperties()->at(1)->GetCommand(), L"Command B");
    EXPECT_EQ(element->GetProperties()->at(2)->GetEnum(), L"EnumC");
    EXPECT_EQ(element->GetProperties()->at(2)->GetCommand(), L"");
}

TEST(VPGEnumClassReaderTest, VCCEnumClassProperty)
{
    std::wstring code = L"";
    code += L"#pragma once\r\n";
    code += L"\r\n";
    code += L"#include <string>\r\n";
    code += L"#include <vector>\r\n";
    code += L"\r\n";
    code += L"enum class VCCObjectProperty\r\n";
    code += L"{\r\n";
    code += L"    EnumA, // GET(EnumTypeA, EnumA, L\"Default\") CommandA\r\n";
    code += L"    EnumB, // GETSPTR(EnumTypeB, EnumB)\r\n";
    code += L"    EnumC, // GETSPTR(EnumTypeC, EnumC, ArgumentA, ArgumentB, ArgumentC)\r\n";
    code += L"    EnumD, // Command D\r\n";
    code += L"    EnumE\r\n";
    code += L"};\r\n";

    VPGEnumClassReader reader(VPGFileGenerationService::GetClassMacroList(L""));
    std::vector<std::shared_ptr<VPGEnumClass>> results;
    reader.Parse(code, results);
    EXPECT_EQ(results.size(), (size_t)1);
    // first
    std::shared_ptr<VPGEnumClass> element = results.at(0);
    EXPECT_EQ(element->GetName(), L"VCCObjectProperty");
    EXPECT_EQ(element->GetCommand(), L"");
    EXPECT_EQ(element->GetProperties()->size(), (size_t)5);
    EXPECT_EQ(element->GetProperties()->at(0)->GetEnum(), L"EnumA");
    EXPECT_EQ(element->GetProperties()->at(0)->GetMacro(), L"GET(EnumTypeA, EnumA, L\"Default\")");
    EXPECT_EQ(element->GetProperties()->at(0)->GetType(), L"EnumTypeA");
    EXPECT_EQ(element->GetProperties()->at(0)->GetPropertyName(), L"EnumA");
    EXPECT_EQ(element->GetProperties()->at(0)->GetDefaultValue(), L"L\"Default\"");
    EXPECT_EQ(element->GetProperties()->at(0)->GetCommand(), L"CommandA");
    EXPECT_EQ(element->GetProperties()->at(1)->GetEnum(), L"EnumB");
    EXPECT_EQ(element->GetProperties()->at(1)->GetMacro(), L"GETSPTR(EnumTypeB, EnumB)");
    EXPECT_EQ(element->GetProperties()->at(1)->GetType(), L"EnumTypeB");
    EXPECT_EQ(element->GetProperties()->at(1)->GetPropertyName(), L"EnumB");
    EXPECT_EQ(element->GetProperties()->at(1)->GetDefaultValue(), L"");
    EXPECT_EQ(element->GetProperties()->at(1)->GetCommand(), L"");
    EXPECT_EQ(element->GetProperties()->at(2)->GetEnum(), L"EnumC");
    EXPECT_EQ(element->GetProperties()->at(2)->GetMacro(), L"GETSPTR(EnumTypeC, EnumC, ArgumentA, ArgumentB, ArgumentC)");
    EXPECT_EQ(element->GetProperties()->at(2)->GetType(), L"EnumTypeC");
    EXPECT_EQ(element->GetProperties()->at(2)->GetPropertyName(), L"EnumC");
    EXPECT_EQ(element->GetProperties()->at(2)->GetDefaultValue(), L"ArgumentA, ArgumentB, ArgumentC");
    EXPECT_EQ(element->GetProperties()->at(2)->GetCommand(), L"");
    EXPECT_EQ(element->GetProperties()->at(3)->GetEnum(), L"EnumD");
    EXPECT_EQ(element->GetProperties()->at(3)->GetMacro(), L"");
    EXPECT_EQ(element->GetProperties()->at(3)->GetType(), L"");
    EXPECT_EQ(element->GetProperties()->at(3)->GetPropertyName(), L"");
    EXPECT_EQ(element->GetProperties()->at(3)->GetDefaultValue(), L"");
    EXPECT_EQ(element->GetProperties()->at(3)->GetCommand(), L"Command D");
    EXPECT_EQ(element->GetProperties()->at(4)->GetEnum(), L"EnumE");
    EXPECT_EQ(element->GetProperties()->at(4)->GetMacro(), L"");
    EXPECT_EQ(element->GetProperties()->at(4)->GetType(), L"");
    EXPECT_EQ(element->GetProperties()->at(4)->GetPropertyName(), L"");
    EXPECT_EQ(element->GetProperties()->at(4)->GetDefaultValue(), L"");
    EXPECT_EQ(element->GetProperties()->at(4)->GetCommand(), L"");
}