#include <gtest/gtest.h>

#include "vpg_enum_class_reader.hpp"

TEST(VPGEnumClassReaderTest, Full)
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

    VPGEnumClassReader reader;
    std::vector<VPGEnumClass> results;
    reader.Parse(code, results);
    EXPECT_EQ(results.size(), (size_t)2);
    // first
    VPGEnumClass element = results.at(0);
    EXPECT_EQ(element.Name, L"VCCObjectProperty");
    EXPECT_EQ(element.Command, L"Class Command");
    EXPECT_EQ(element.Properties.size(), (size_t)4);
    EXPECT_EQ(element.Properties.at(0).Name, L"EnumA");
    EXPECT_EQ(element.Properties.at(0).Command, L"Command A");
    EXPECT_EQ(element.Properties.at(1).Name, L"EnumB");
    EXPECT_EQ(element.Properties.at(1).Command, L"Command B");
    EXPECT_EQ(element.Properties.at(2).Name, L"EnumC");
    EXPECT_EQ(element.Properties.at(2).Command, L"");
    EXPECT_EQ(element.Properties.at(3).Name, L"EnumD");
    EXPECT_EQ(element.Properties.at(3).Command, L"Command D");

    // second
    element = results.at(1);
    EXPECT_EQ(element.Name, L"VCCEnumProperty");
    EXPECT_EQ(element.Command, L"");
    EXPECT_EQ(element.Properties.size(), (size_t)3);
    EXPECT_EQ(element.Properties.at(0).Name, L"EnumA");
    EXPECT_EQ(element.Properties.at(0).Command, L"");
    EXPECT_EQ(element.Properties.at(1).Name, L"EnumB");
    EXPECT_EQ(element.Properties.at(1).Command, L"Command B");
    EXPECT_EQ(element.Properties.at(2).Name, L"EnumC");
    EXPECT_EQ(element.Properties.at(2).Command, L"");
}