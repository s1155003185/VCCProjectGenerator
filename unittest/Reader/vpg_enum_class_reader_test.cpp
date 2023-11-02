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
    std::vector<VPGEnumClass> results;
    reader.Parse(code, results);
    EXPECT_EQ(results.size(), (size_t)2);
    // first
    VPGEnumClass element = results.at(0);
    EXPECT_EQ(element.Name, L"VCCObjectProperty");
    EXPECT_EQ(element.Command, L"Class Command");
    EXPECT_EQ(element.Properties.size(), (size_t)4);
    EXPECT_EQ(element.Properties.at(0).Enum, L"EnumA");
    EXPECT_EQ(element.Properties.at(0).Command, L"Command A");
    EXPECT_EQ(element.Properties.at(1).Enum, L"EnumB");
    EXPECT_EQ(element.Properties.at(1).Command, L"Command B");
    EXPECT_EQ(element.Properties.at(2).Enum, L"EnumC");
    EXPECT_EQ(element.Properties.at(2).Command, L"");
    EXPECT_EQ(element.Properties.at(3).Enum, L"EnumD");
    EXPECT_EQ(element.Properties.at(3).Command, L"Command D");

    // second
    element = results.at(1);
    EXPECT_EQ(element.Name, L"VCCEnumProperty");
    EXPECT_EQ(element.Command, L"");
    EXPECT_EQ(element.Properties.size(), (size_t)3);
    EXPECT_EQ(element.Properties.at(0).Enum, L"EnumA");
    EXPECT_EQ(element.Properties.at(0).Command, L"");
    EXPECT_EQ(element.Properties.at(1).Enum, L"EnumB");
    EXPECT_EQ(element.Properties.at(1).Command, L"Command B");
    EXPECT_EQ(element.Properties.at(2).Enum, L"EnumC");
    EXPECT_EQ(element.Properties.at(2).Command, L"");
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
    code += L"    EnumB, // GETOBJ(EnumTypeB, EnumB)\r\n";
    code += L"    EnumC, // GETUPTR(EnumTypeC, EnumC, ArgumentA, ArgumentB, ArgumentC)\r\n";
    code += L"    EnumD, // Command D\r\n";
    code += L"    EnumE\r\n";
    code += L"};\r\n";

    VPGEnumClassReader reader(VPGFileGenerationService::GetClassMacroList(L""));
    std::vector<VPGEnumClass> results;
    reader.Parse(code, results);
    EXPECT_EQ(results.size(), (size_t)1);
    // first
    VPGEnumClass element = results.at(0);
    EXPECT_EQ(element.Name, L"VCCObjectProperty");
    EXPECT_EQ(element.Command, L"");
    EXPECT_EQ(element.Properties.size(), (size_t)5);
    EXPECT_EQ(element.Properties.at(0).Enum, L"EnumA");
    EXPECT_EQ(element.Properties.at(0).Macro, L"GET(EnumTypeA, EnumA, L\"Default\")");
    EXPECT_EQ(element.Properties.at(0).Type, L"EnumTypeA");
    EXPECT_EQ(element.Properties.at(0).PropertyName, L"EnumA");
    EXPECT_EQ(element.Properties.at(0).DefaultValue, L"L\"Default\"");
    EXPECT_EQ(element.Properties.at(0).Command, L"CommandA");
    EXPECT_EQ(element.Properties.at(1).Enum, L"EnumB");
    EXPECT_EQ(element.Properties.at(1).Macro, L"GETOBJ(EnumTypeB, EnumB)");
    EXPECT_EQ(element.Properties.at(1).Type, L"EnumTypeB");
    EXPECT_EQ(element.Properties.at(1).PropertyName, L"EnumB");
    EXPECT_EQ(element.Properties.at(1).DefaultValue, L"");
    EXPECT_EQ(element.Properties.at(1).Command, L"");
    EXPECT_EQ(element.Properties.at(2).Enum, L"EnumC");
    EXPECT_EQ(element.Properties.at(2).Macro, L"GETUPTR(EnumTypeC, EnumC, ArgumentA, ArgumentB, ArgumentC)");
    EXPECT_EQ(element.Properties.at(2).Type, L"EnumTypeC");
    EXPECT_EQ(element.Properties.at(2).PropertyName, L"EnumC");
    EXPECT_EQ(element.Properties.at(2).DefaultValue, L"ArgumentA, ArgumentB, ArgumentC");
    EXPECT_EQ(element.Properties.at(2).Command, L"");
    EXPECT_EQ(element.Properties.at(3).Enum, L"EnumD");
    EXPECT_EQ(element.Properties.at(3).Macro, L"");
    EXPECT_EQ(element.Properties.at(3).Type, L"");
    EXPECT_EQ(element.Properties.at(3).PropertyName, L"");
    EXPECT_EQ(element.Properties.at(3).DefaultValue, L"");
    EXPECT_EQ(element.Properties.at(3).Command, L"Command D");
    EXPECT_EQ(element.Properties.at(4).Enum, L"EnumE");
    EXPECT_EQ(element.Properties.at(4).Macro, L"");
    EXPECT_EQ(element.Properties.at(4).Type, L"");
    EXPECT_EQ(element.Properties.at(4).PropertyName, L"");
    EXPECT_EQ(element.Properties.at(4).DefaultValue, L"");
    EXPECT_EQ(element.Properties.at(4).Command, L"");
}