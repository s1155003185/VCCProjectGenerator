#include <gtest/gtest.h>

#include "vpg_enum_class_reader.hpp"
#include "vpg_global.hpp"

TEST(VPGEnumClassReaderTest, GetCodeLine)
{
    size_t pos = 0;
    EXPECT_EQ(VPGGlobal::getEnumClassReader()->getCppCodeLine(L"a\nb\nc", pos, true), L"a\n");
    EXPECT_EQ(pos, 1UL);

    pos = 0;
    EXPECT_EQ(VPGGlobal::getEnumClassReader()->getCppCodeLine(L"a\r\nb\nc", pos, true), L"a\r\n");
    EXPECT_EQ(pos, 2UL);
    
    pos = 0;
    EXPECT_EQ(VPGGlobal::getEnumClassReader()->getCppCodeLine(L"a\\\nb\nc", pos, true), L"a\\\nb\n");
    EXPECT_EQ(pos, 4UL);

    pos = 0;
    EXPECT_EQ(VPGGlobal::getEnumClassReader()->getCppCodeLine(L"a\\\r\nb\nc", pos, true), L"a\\\r\nb\n");
    EXPECT_EQ(pos, 5UL);
}

TEST(VPGEnumClassReaderTest, getAttribute)
{
    std::vector<std::wstring> result = VPGGlobal::getEnumClassReader()->getAttribute(L"");
    std::vector<std::wstring> expectedResult = {};
    EXPECT_EQ(result, expectedResult);

    result = VPGGlobal::getEnumClassReader()->getAttribute(L"abc");
    expectedResult = {};
    EXPECT_EQ(result, expectedResult);
    
    result = VPGGlobal::getEnumClassReader()->getAttribute(L"@");
    expectedResult = { };
    EXPECT_EQ(result, expectedResult);
    
    result = VPGGlobal::getEnumClassReader()->getAttribute(L"@@");
    expectedResult = { };
    EXPECT_EQ(result, expectedResult);
    
    result = VPGGlobal::getEnumClassReader()->getAttribute(L"@@ReadWrite NoAccess");
    expectedResult = { L"@@ReadWrite NoAccess" };
    EXPECT_EQ(result, expectedResult);
    
    result = VPGGlobal::getEnumClassReader()->getAttribute(L"@@ReadWrite @NoAccess");
    expectedResult = { L"@@ReadWrite @NoAccess" };
    EXPECT_EQ(result, expectedResult);
    
    result = VPGGlobal::getEnumClassReader()->getAttribute(L"@@ReadWrite No@@Access");
    expectedResult = { L"@@ReadWrite No", L"@@Access" };
    EXPECT_EQ(result, expectedResult);
    
    result = VPGGlobal::getEnumClassReader()->getAttribute(L"@@ReadWrite @@NoAccess");
    expectedResult = { L"@@ReadWrite", L"@@NoAccess" };
    EXPECT_EQ(result, expectedResult);

    result = VPGGlobal::getEnumClassReader()->getAttribute(L"@@ReadWrite( @@NoAccess)");
    expectedResult = { L"@@ReadWrite(", L"@@NoAccess)" };
    EXPECT_EQ(result, expectedResult);
}

TEST(VPGEnumClassReaderTest, getJsonAttributes)
{
    EXPECT_EQ(VPGGlobal::getEnumClassReader()->getJsonAttributes(L"@@A@@Json@@B", L"@@Json"), nullptr);

    std::vector<std::wstring> keys = { L"Number", L"String" };
    EXPECT_EQ(VPGGlobal::getEnumClassReader()->getJsonAttributes(L"@@A@@Json { \"Number\" : 12, \"String\":\"abc\" } @@B { \"B\" : 123}", L"@@Json")->getKeys(), keys);
    EXPECT_EQ(VPGGlobal::getEnumClassReader()->getJsonAttributes(L"@@A@@Json{\"Number\":12,\"String\":\"abc\"}@@B{\"B\":123}", L"@@Json")->getKeys(), keys);
}

TEST(VPGEnumClassReaderTest, TableCommand)
{
    std::wstring code = L""
        L"#pragma once\r\n"
        L"\r\n"
        L"#include <string>\r\n"
        L"#include <vector>\r\n"
        L"\r\n"
        L"#include \"class_macro.hpp\"\r\n"
        L"\r\n"
        L"// table command 1\r\n"
        L"enum class VCCObjectProperty\r\n"
        L"{\r\n"
        L"};\r\n"
        L"\r\n"
        L"/* table command 1*/enum class VCCSingleLineProperty { };\r\n"
        L"\r\n"
        L"// empty command 1\r\n"
        L"\r\n"
        L"// table command 1\r\n"
        L"// table command 2\r\n"
        L"enum class VCCMultiProperty {\r\n"
        L"};\r\n";

    std::vector<std::shared_ptr<VPGEnumClass>> results;
    VPGGlobal::getEnumClassReader()->parse(code, results);
    EXPECT_EQ(results.size(), (size_t)3);
    // first
    auto element = results.at(0);
    EXPECT_EQ(element->getName(), L"VCCObjectProperty");
    EXPECT_EQ(element->getCommand(), L"table command 1");

    // second
    element = results.at(1);
    EXPECT_EQ(element->getName(), L"VCCSingleLineProperty");
    EXPECT_EQ(element->getCommand(), L"table command 1");

    // third
    element = results.at(2);
    EXPECT_EQ(element->getName(), L"VCCMultiProperty");
    EXPECT_EQ(element->getCommand(), L"table command 1\r\ntable command 2");
}

TEST(VPGEnumClassReaderTest, TableAttribute)
{
    std::wstring code = L"#pragma once\r\n"
        "\r\n"
        "#include <string>\r\n"
        "#include <vector>\r\n"
        "\r\n"
        "// @@Json\r\n"
        "enum class VCCObjectProperty\r\n"
        "{\r\n"
        "    EnumA // GETSET(EnumTypeA, EnumA, L\"Default\") CommandA\r\n"
        "};\r\n";

    std::vector<std::shared_ptr<VPGEnumClass>> results;
    VPGGlobal::getEnumClassReader()->parse(code, results);
    EXPECT_EQ(results.size(), (size_t)1);
    // first
    auto element = results.at(0);
    EXPECT_EQ(element->getName(), L"VCCObjectProperty");
    EXPECT_EQ(element->getCommand(), L"");
    EXPECT_TRUE(element->getIsJson());
    EXPECT_EQ(element->getInheritClass(), L"");

    code = L"#pragma once\r\n"
        "\r\n"
        "#include <string>\r\n"
        "#include <vector>\r\n"
        "\r\n"
        "// @@Inherit{\"Class\":\"BaseObject\"}\r\n"
        "enum class VCCObjectProperty\r\n"
        "{\r\n"
        "    EnumA // GETSET(EnumTypeA, EnumA, L\"Default\") CommandA\r\n"
        "};\r\n";

    results.clear();
    VPGGlobal::getEnumClassReader()->parse(code, results);
    EXPECT_EQ(results.size(), (size_t)1);
    // first
    element = results.at(0);
    EXPECT_EQ(element->getName(), L"VCCObjectProperty");
    EXPECT_EQ(element->getCommand(), L"");
    EXPECT_FALSE(element->getIsJson());
    EXPECT_EQ(element->getInheritClass(), L"BaseObject");
}

void CheckVPGEnumClassReaderTestNormalEnumClassResult(const VPGEnumClassReader *reader, const std::wstring &code)
{
    std::vector<std::shared_ptr<VPGEnumClass>> results;
    reader->parse(code, results);
    EXPECT_EQ(results.size(), (size_t)1);

    auto element = results.at(0);
    EXPECT_EQ(element->getName(), L"VCCObjectProperty");
    EXPECT_EQ(element->getCommand(), L"");
    EXPECT_EQ(element->getProperties().size(), 4UL);
    EXPECT_EQ(element->getProperties().at(0)->getEnum(), L"EnumA");
    EXPECT_EQ(element->getProperties().at(0)->getEnumValue(), 0);
    EXPECT_EQ(element->getProperties().at(1)->getEnum(), L"EnumB");
    EXPECT_EQ(element->getProperties().at(1)->getEnumValue(), 1);
    EXPECT_EQ(element->getProperties().at(2)->getEnum(), L"EnumC");
    EXPECT_EQ(element->getProperties().at(2)->getEnumValue(), 999);
    EXPECT_EQ(element->getProperties().at(3)->getEnum(), L"EnumD");
    EXPECT_EQ(element->getProperties().at(3)->getEnumValue(), 1000);
}

TEST(VPGEnumClassReaderTest, NormalEnumClass1)
{
    CheckVPGEnumClassReaderTestNormalEnumClassResult(VPGGlobal::getEnumClassReader().get(),
        L"#pragma once\r\n"
        L"\r\n"
        L"enum class VCCObjectProperty\r\n"
        L"{\r\n"
        L"    EnumA = 0,\r\n"
        L"    EnumB,\r\n"
        L"    EnumC = 999,\r\n"
        L"    EnumD = 1000\r\n"
        L"};\r\n"
        L"\r\n");
}

TEST(VPGEnumClassReaderTest, NormalEnumClass2)
{      
    CheckVPGEnumClassReaderTestNormalEnumClassResult(VPGGlobal::getEnumClassReader().get(),
        L"#pragma once\r\n"
        L"\r\n"
        L"enum class VCCObjectProperty\r\n"
        L"{\r\n"
        L"    EnumA = 0\r\n"
        L"    ,EnumB\r\n"
        L"    ,EnumC = 999\r\n"
        L"    ,EnumD = 1000\r\n"
        L"};\r\n"
        L"\r\n");
}

TEST(VPGEnumClassReaderTest, NormalEnumClass3)
{
    CheckVPGEnumClassReaderTestNormalEnumClassResult(VPGGlobal::getEnumClassReader().get(),
        L"#pragma once\r\n"
        L"\r\n"
        L"enum class VCCObjectProperty\r\n"
        L"{\r\n"
        L"    EnumA = 0\r\n"
        L"    , EnumB\r\n"
        L"    , EnumC = 999\r\n"
        L"    , EnumD = 1000\r\n"
        L"};\r\n"
        L"\r\n");
}

void CheckVPGEnumClassReaderTestNormalResult(const VPGEnumClassReader *reader, const std::wstring &code)
{
    std::vector<std::shared_ptr<VPGEnumClass>> results;
    reader->parse(code, results);
    EXPECT_EQ(results.size(), (size_t)3);
    // first
    auto element = results.at(0);
    EXPECT_EQ(element->getName(), L"VCCObjectProperty");
    EXPECT_EQ(element->getCommand(), L"Class Command");
    EXPECT_EQ(element->getProperties().size(), 1UL);
    EXPECT_EQ(element->getProperties().at(0)->getEnum(), L"EnumA");
    EXPECT_EQ(element->getProperties().at(0)->getCommand(), L"Command A");

    // second
    element = results.at(1);
    EXPECT_EQ(element->getName(), L"VCCSingleLineProperty");
    EXPECT_EQ(element->getCommand(), L"");
    EXPECT_EQ(element->getProperties().size(), 3UL);
    EXPECT_EQ(element->getProperties().at(0)->getEnum(), L"EnumA");
    EXPECT_EQ(element->getProperties().at(0)->getCommand(), L"");
    EXPECT_EQ(element->getProperties().at(1)->getEnum(), L"EnumB");
    EXPECT_EQ(element->getProperties().at(1)->getCommand(), L"Command B");
    EXPECT_EQ(element->getProperties().at(2)->getEnum(), L"EnumC");
    EXPECT_EQ(element->getProperties().at(2)->getCommand(), L"");

    // third
    element = results.at(2);
    EXPECT_EQ(element->getName(), L"VCCMultiProperty");
    EXPECT_EQ(element->getCommand(), L"");
    EXPECT_EQ(element->getProperties().size(), 5UL);
    EXPECT_EQ(element->getProperties().at(0)->getEnum(), L"EnumA");
    EXPECT_EQ(element->getProperties().at(0)->getCommand(), L"");
    EXPECT_EQ(element->getProperties().at(1)->getEnum(), L"EnumB");
    EXPECT_EQ(element->getProperties().at(1)->getCommand(), L"Command B");
    EXPECT_EQ(element->getProperties().at(2)->getEnum(), L"EnumC");
    EXPECT_EQ(element->getProperties().at(2)->getCommand(), L"Command C");
    EXPECT_EQ(element->getProperties().at(3)->getEnum(), L"EnumD");
    EXPECT_EQ(element->getProperties().at(3)->getCommand(), L"Command D\r\nCommand E");
    EXPECT_EQ(element->getProperties().at(4)->getEnum(), L"EnumE");
    EXPECT_EQ(element->getProperties().at(4)->getCommand(), L"Command F\r\nCommand G");
}

TEST(VPGEnumClassReaderTest, Normal1)
{
    CheckVPGEnumClassReaderTestNormalResult(VPGGlobal::getEnumClassReader().get(),
        L"#pragma once\r\n"
        L"\r\n"
        L"#include <string>\r\n"
        L"#include <vector>\r\n"
        L"\r\n"
        L"#include \"class_macro.hpp\"\r\n"
        L"/*\r\n"
        L"enum class VCCObjectProperty { // Class Command\r\n"
        L"    EnumA, \r\n"
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
        L"    EnumA, \r\n"
        L"    EnumB, // Command B\r\n"
        L"    EnumC, /* Command C*/\r\n"
        L"    EnumD, /* Command D *//* Command E */\r\n"
        L"    EnumE /* Command F */\r\n"
        L"      /* Command G */\r\n"
        L"};\r\n"
    );
}

TEST(VPGEnumClassReaderTest, Normal2)
{   
    CheckVPGEnumClassReaderTestNormalResult(VPGGlobal::getEnumClassReader().get(),
        L"#pragma once\r\n"
        L"\r\n"
        L"#include <string>\r\n"
        L"#include <vector>\r\n"
        L"\r\n"
        L"#include \"class_macro.hpp\"\r\n"
        L"/*\r\n"
        L"enum class VCCObjectProperty { // Class Command\r\n"
        L"    EnumA\r\n"
        L"    , EnumB // Command B\r\n"
        L"    , EnumC\r\n"
        L"    , EnumD\r\n"
        L"};*/\r\n"
        L"\r\n"
        L"enum class VCCObjectProperty // Class Command\r\n"
        L"{\r\n"
        L"    EnumA // Command A\r\n"
        L"};\r\n"
        L"\r\n"
        L"enum class VCCSingleLineProperty { EnumA, EnumB /* Command B */, EnumC }"
        L"\r\n"
        L"enum class VCCMultiProperty {\r\n"
        L"    EnumA\r\n"
        L"    , EnumB // Command B\r\n"
        L"    , EnumC /* Command C*/\r\n"
        L"    , EnumD /* Command D *//* Command E */\r\n"
        L"    , EnumE /* Command F */\r\n"
        L"      /* Command G */\r\n"
        L"};\r\n"
    );
}

TEST(VPGEnumClassReaderTest, Normal3)
{    
    CheckVPGEnumClassReaderTestNormalResult(VPGGlobal::getEnumClassReader().get(),
        L"#pragma once\r\n"
        L"\r\n"
        L"#include <string>\r\n"
        L"#include <vector>\r\n"
        L"\r\n"
        L"#include \"class_macro.hpp\"\r\n"
        L"/*\r\n"
        L"enum class VCCObjectProperty { // Class Command\r\n"
        L"    EnumA\r\n"
        L"    ,EnumB // Command B\r\n"
        L"    ,EnumC\r\n"
        L"    ,EnumD\r\n"
        L"};*/\r\n"
        L"\r\n"
        L"enum class VCCObjectProperty // Class Command\r\n"
        L"{\r\n"
        L"    EnumA // Command A\r\n"
        L"};\r\n"
        L"\r\n"
        L"enum class VCCSingleLineProperty { EnumA, EnumB /* Command B */, EnumC }"
        L"\r\n"
        L"enum class VCCMultiProperty {\r\n"
        L"    EnumA\r\n"
        L"    ,EnumB // Command B\r\n"
        L"    ,EnumC /* Command C*/\r\n"
        L"    ,EnumD /* Command D *//* Command E */\r\n"
        L"    ,EnumE /* Command F */\r\n"
        L"      /* Command G */\r\n"
        L"};\r\n"
    );
}

TEST(VPGEnumClassReaderTest, VCCEnumClassProperty)
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
        L"    EnumValidate, // GETSET_VALIDATE(std::wstring, EnumValidate, L\"Default\", validate)\r\n"
    // command
        L"    EnumD, // Command D\r\n"
        L"    EnumE,\r\n"
    // vector
        L"    EnumVector, // VECTOR(int, Vector)\r\n"
    // map
        L"    EnumMap // MAP(int, std::wstring, Map) \r\n"
        L"};\r\n";

    std::vector<std::shared_ptr<VPGEnumClass>> results;
    VPGGlobal::getEnumClassReader()->parse(code, results);
    EXPECT_EQ(results.size(), (size_t)1);
    // first
    auto element = results.at(0);
    EXPECT_EQ(element->getName(), L"VCCObjectProperty");
    EXPECT_EQ(element->getCommand(), L"");
    EXPECT_EQ(element->getProperties().size(), (size_t)8);
    EXPECT_EQ(element->getProperties().at(0)->getEnum(), L"EnumA");
    EXPECT_EQ(element->getProperties().at(0)->getMacro(), L"GETSET(EnumTypeA, EnumA, L\"Default\")");
    EXPECT_EQ(element->getProperties().at(0)->getType1(), L"EnumTypeA");
    EXPECT_EQ(element->getProperties().at(0)->getType2(), L"");
    EXPECT_EQ(element->getProperties().at(0)->getPropertyName(), L"EnumA");
    EXPECT_EQ(element->getProperties().at(0)->getDefaultValue(), L"L\"Default\"");
    EXPECT_EQ(element->getProperties().at(0)->getCommand(), L"");
    EXPECT_EQ(element->getProperties().at(0)->getValidate(), L"");
    EXPECT_EQ(element->getProperties().at(0)->getMacroType(), VPGEnumClassMacroType::Getset);
    EXPECT_EQ(element->getProperties().at(0)->getIsObject(), false);
    EXPECT_EQ(element->getProperties().at(1)->getEnum(), L"EnumB");
    EXPECT_EQ(element->getProperties().at(1)->getMacro(), L"GETSET_SPTR(EnumTypeB, EnumB)");
    EXPECT_EQ(element->getProperties().at(1)->getType1(), L"EnumTypeB");
    EXPECT_EQ(element->getProperties().at(1)->getType2(), L"");
    EXPECT_EQ(element->getProperties().at(1)->getPropertyName(), L"EnumB");
    EXPECT_EQ(element->getProperties().at(1)->getDefaultValue(), L"");
    EXPECT_EQ(element->getProperties().at(1)->getCommand(), L"");
    EXPECT_EQ(element->getProperties().at(1)->getValidate(), L"");
    EXPECT_EQ(element->getProperties().at(1)->getMacroType(), VPGEnumClassMacroType::GetsetSptr);
    EXPECT_EQ(element->getProperties().at(1)->getIsObject(), true);
    EXPECT_EQ(element->getProperties().at(2)->getEnum(), L"EnumC");
    EXPECT_EQ(element->getProperties().at(2)->getMacro(), L"GETSET_SPTR(EnumTypeC, EnumC, ArgumentA, ArgumentB, ArgumentC)");
    EXPECT_EQ(element->getProperties().at(2)->getType1(), L"EnumTypeC");
    EXPECT_EQ(element->getProperties().at(2)->getType2(), L"");
    EXPECT_EQ(element->getProperties().at(2)->getPropertyName(), L"EnumC");
    EXPECT_EQ(element->getProperties().at(2)->getDefaultValue(), L"ArgumentA, ArgumentB, ArgumentC");
    EXPECT_EQ(element->getProperties().at(2)->getCommand(), L"");
    EXPECT_EQ(element->getProperties().at(2)->getValidate(), L"");
    EXPECT_EQ(element->getProperties().at(2)->getMacroType(), VPGEnumClassMacroType::GetsetSptr);
    EXPECT_EQ(element->getProperties().at(2)->getIsObject(), true);
    EXPECT_EQ(element->getProperties().at(3)->getEnum(), L"EnumValidate");
    EXPECT_EQ(element->getProperties().at(3)->getMacro(), L"GETSET_VALIDATE(std::wstring, EnumValidate, L\"Default\", validate)");
    EXPECT_EQ(element->getProperties().at(3)->getType1(), L"std::wstring");
    EXPECT_EQ(element->getProperties().at(3)->getType2(), L"");
    EXPECT_EQ(element->getProperties().at(3)->getPropertyName(), L"EnumValidate");
    EXPECT_EQ(element->getProperties().at(3)->getDefaultValue(), L"L\"Default\"");
    EXPECT_EQ(element->getProperties().at(3)->getCommand(), L"");
    EXPECT_EQ(element->getProperties().at(3)->getValidate(), L"validate");
    EXPECT_EQ(element->getProperties().at(3)->getMacroType(), VPGEnumClassMacroType::GetsetValidate);
    EXPECT_EQ(element->getProperties().at(3)->getIsObject(), false);
    EXPECT_EQ(element->getProperties().at(4)->getEnum(), L"EnumD");
    EXPECT_EQ(element->getProperties().at(4)->getMacro(), L"");
    EXPECT_EQ(element->getProperties().at(4)->getType1(), L"");
    EXPECT_EQ(element->getProperties().at(4)->getType2(), L"");
    EXPECT_EQ(element->getProperties().at(4)->getPropertyName(), L"");
    EXPECT_EQ(element->getProperties().at(4)->getDefaultValue(), L"");
    EXPECT_EQ(element->getProperties().at(4)->getCommand(), L"Command D");
    EXPECT_EQ(element->getProperties().at(4)->getValidate(), L"");
    EXPECT_EQ(element->getProperties().at(4)->getMacroType(), VPGEnumClassMacroType::NA);
    EXPECT_EQ(element->getProperties().at(4)->getIsObject(), false);
    EXPECT_EQ(element->getProperties().at(5)->getEnum(), L"EnumE");
    EXPECT_EQ(element->getProperties().at(5)->getMacro(), L"");
    EXPECT_EQ(element->getProperties().at(5)->getType1(), L"");
    EXPECT_EQ(element->getProperties().at(5)->getType2(), L"");
    EXPECT_EQ(element->getProperties().at(5)->getPropertyName(), L"");
    EXPECT_EQ(element->getProperties().at(5)->getDefaultValue(), L"");
    EXPECT_EQ(element->getProperties().at(5)->getCommand(), L"");
    EXPECT_EQ(element->getProperties().at(5)->getValidate(), L"");
    EXPECT_EQ(element->getProperties().at(5)->getMacroType(), VPGEnumClassMacroType::NA);
    EXPECT_EQ(element->getProperties().at(5)->getIsObject(), false);
    EXPECT_EQ(element->getProperties().at(6)->getEnum(), L"EnumVector");
    EXPECT_EQ(element->getProperties().at(6)->getMacro(), L"VECTOR(int, Vector)");
    EXPECT_EQ(element->getProperties().at(6)->getType1(), L"int");
    EXPECT_EQ(element->getProperties().at(6)->getType2(), L"");
    EXPECT_EQ(element->getProperties().at(6)->getPropertyName(), L"Vector");
    EXPECT_EQ(element->getProperties().at(6)->getDefaultValue(), L"");
    EXPECT_EQ(element->getProperties().at(6)->getCommand(), L"");
    EXPECT_EQ(element->getProperties().at(6)->getValidate(), L"");
    EXPECT_EQ(element->getProperties().at(6)->getMacroType(), VPGEnumClassMacroType::Vector);
    EXPECT_EQ(element->getProperties().at(6)->getIsObject(), false);
    EXPECT_EQ(element->getProperties().at(7)->getEnum(), L"EnumMap");
    EXPECT_EQ(element->getProperties().at(7)->getMacro(), L"MAP(int, std::wstring, Map)");
    EXPECT_EQ(element->getProperties().at(7)->getType1(), L"int");
    EXPECT_EQ(element->getProperties().at(7)->getType2(), L"std::wstring");
    EXPECT_EQ(element->getProperties().at(7)->getPropertyName(), L"Map");
    EXPECT_EQ(element->getProperties().at(7)->getDefaultValue(), L"");
    EXPECT_EQ(element->getProperties().at(7)->getCommand(), L"");
    EXPECT_EQ(element->getProperties().at(7)->getValidate(), L"");
    EXPECT_EQ(element->getProperties().at(7)->getMacroType(), VPGEnumClassMacroType::Map);
    EXPECT_EQ(element->getProperties().at(7)->getIsObject(), false);
}


TEST(VPGEnumClassReaderTest, VCCEnumClassMultiMacro)
{
    std::wstring code = L"#pragma once\r\n"
        "\r\n"
        "enum class VCCObjectProperty\r\n"
        "{\r\n"
        "    EnumA, // GETCUSTOM(int64_t, EnumA, return 100;)\r\n"
        "    EnumB, // SETCUSTOM(EnumB , int64_t, return 100;) @@NoProperty\r\n"
        "    EnumC // GETCUSTOM(int64_t, EnumC, return 100;) SETCUSTOM(EnumC, int64_t, _EnumC = value;) @@NoProperty\r\n"
        "};\r\n";

    std::vector<std::shared_ptr<VPGEnumClass>> results;
    VPGGlobal::getEnumClassReader()->parse(code, results);
    EXPECT_EQ(results.size(), (size_t)1);
    // first
    auto element = results.at(0);
    EXPECT_EQ(element->getName(), L"VCCObjectProperty");
    EXPECT_EQ(element->getCommand(), L"");
    EXPECT_EQ(element->getProperties().size(), (size_t)4);
    EXPECT_EQ(element->getProperties().at(0)->getEnum(), L"EnumA");
    EXPECT_EQ(element->getProperties().at(0)->getMacro(), L"GETCUSTOM(int64_t, EnumA, return 100;)");
    EXPECT_EQ(element->getProperties().at(0)->getType1(), L"int64_t");
    EXPECT_EQ(element->getProperties().at(0)->getType2(), L"");
    EXPECT_EQ(element->getProperties().at(0)->getPropertyName(), L"EnumA");
    EXPECT_EQ(element->getProperties().at(0)->getDefaultValue(), L"return 100;");
    EXPECT_EQ(element->getProperties().at(0)->getCommand(), L"");
    EXPECT_EQ(element->getProperties().at(0)->getMacroType(), VPGEnumClassMacroType::Getcustom);
    EXPECT_EQ(element->getProperties().at(1)->getEnum(), L"EnumB");
    EXPECT_EQ(element->getProperties().at(1)->getMacro(), L"SETCUSTOM(EnumB , int64_t, return 100;)");
    EXPECT_EQ(element->getProperties().at(1)->getType1(), L"int64_t");
    EXPECT_EQ(element->getProperties().at(1)->getType2(), L"");
    EXPECT_EQ(element->getProperties().at(1)->getPropertyName(), L"EnumB");
    EXPECT_EQ(element->getProperties().at(1)->getDefaultValue(), L"return 100;");
    EXPECT_EQ(element->getProperties().at(1)->getCommand(), L"");
    EXPECT_EQ(element->getProperties().at(1)->getMacroType(), VPGEnumClassMacroType::Setcustom);
    EXPECT_EQ(element->getProperties().at(2)->getEnum(), L"EnumC");
    EXPECT_EQ(element->getProperties().at(2)->getMacro(), L"GETCUSTOM(int64_t, EnumC, return 100;)");
    EXPECT_EQ(element->getProperties().at(2)->getType1(), L"int64_t");
    EXPECT_EQ(element->getProperties().at(2)->getType2(), L"");
    EXPECT_EQ(element->getProperties().at(2)->getPropertyName(), L"EnumC");
    EXPECT_EQ(element->getProperties().at(2)->getDefaultValue(), L"return 100;");
    EXPECT_EQ(element->getProperties().at(2)->getCommand(), L"");
    EXPECT_EQ(element->getProperties().at(2)->getMacroType(), VPGEnumClassMacroType::Getcustom);
    EXPECT_EQ(element->getProperties().at(3)->getEnum(), L"EnumC");
    EXPECT_EQ(element->getProperties().at(3)->getMacro(), L"SETCUSTOM(EnumC, int64_t, _EnumC = value;)");
    EXPECT_EQ(element->getProperties().at(3)->getType1(), L"int64_t");
    EXPECT_EQ(element->getProperties().at(3)->getType2(), L"");
    EXPECT_EQ(element->getProperties().at(3)->getPropertyName(), L"EnumC");
    EXPECT_EQ(element->getProperties().at(3)->getDefaultValue(), L"_EnumC = value;");
    EXPECT_EQ(element->getProperties().at(3)->getCommand(), L"");
    EXPECT_EQ(element->getProperties().at(3)->getMacroType(), VPGEnumClassMacroType::Setcustom);
}

TEST(VPGEnumClassReaderTest, AccessMode)
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
    VPGGlobal::getEnumClassReader()->parse(code, results);
    EXPECT_EQ(results.size(), (size_t)1);
    // first
    auto element = results.at(0);
    EXPECT_EQ(element->getName(), L"VCCObjectProperty");
    EXPECT_EQ(element->getCommand(), L"");
    EXPECT_EQ(element->getProperties().size(), 5UL);
    EXPECT_EQ(element->getProperties().at(0)->getEnum(), L"EnumA");
    EXPECT_EQ(element->getProperties().at(0)->getCommand(), L"");
    EXPECT_EQ(element->getProperties().at(0)->getAccessMode(), VPGEnumClassAttributeAccessMode::ReadWrite);
    EXPECT_EQ(element->getProperties().at(1)->getEnum(), L"EnumB");
    EXPECT_EQ(element->getProperties().at(1)->getCommand(), L"");
    EXPECT_EQ(element->getProperties().at(1)->getAccessMode(), VPGEnumClassAttributeAccessMode::ReadOnly);
    EXPECT_EQ(element->getProperties().at(2)->getEnum(), L"EnumC");
    EXPECT_EQ(element->getProperties().at(2)->getCommand(), L"");
    EXPECT_EQ(element->getProperties().at(2)->getAccessMode(), VPGEnumClassAttributeAccessMode::WriteOnly);
    EXPECT_EQ(element->getProperties().at(3)->getEnum(), L"EnumD");
    EXPECT_EQ(element->getProperties().at(3)->getCommand(), L"");
    EXPECT_EQ(element->getProperties().at(3)->getAccessMode(), VPGEnumClassAttributeAccessMode::ReadWrite);
    EXPECT_EQ(element->getProperties().at(4)->getEnum(), L"EnumE");
    EXPECT_EQ(element->getProperties().at(4)->getCommand(), L"");
    EXPECT_EQ(element->getProperties().at(4)->getAccessMode(), VPGEnumClassAttributeAccessMode::NoAccess);
}

TEST(VPGEnumClassReaderTest, EnumClassMixedWithOthers)
{
    std::wstring code = L""
        "#pragma once\r\n"
        "\r\n"
        "#include <string>\r\n"
        "#include <vector>\r\n"
        "\r\n"
        "#include \"class_macro.hpp\"\r\n"
        "using namespace vcc;\r\n"
        "\r\n"
        "typedef abc\r\n"
        "enum EnumA {\r\n"
        "};\r\n"
        "int a = a;\r\n"
        "const string = L\";\";\r\n"
        "enum class EnumB {};\r\n"
        "enum class EnumC;\r\n"
        "class ClassA {};\r\n"
        "namespace Name {\r\n"
        "   enum EnumAInNamespace {};"
        "   enum class EnumBInNamespace {};\r\n"
        "   enum class EnumCInNamespace;\r\n"
        "   namespace nestedNamespace {\r\n"
        "       enum class EnumNestedNamespace {};\r\n"
        "   }\r\n"
        "}\r\n"
        "enum EnumAOneLine {}; enum class EnumBOneLine {}; enum class EnumCOneLine;\r\n";
    std::vector<std::shared_ptr<VPGEnumClass>> results;
    VPGGlobal::getEnumClassReader()->parse(code, results);
    EXPECT_EQ(results.size(), (size_t)7);
    auto element = results.at(0);
    EXPECT_EQ(element->getName(), L"EnumA");
    element = results.at(1);
    EXPECT_EQ(element->getName(), L"EnumB");
    element = results.at(2);
    EXPECT_EQ(element->getName(), L"Name::EnumAInNamespace");
    element = results.at(3);
    EXPECT_EQ(element->getName(), L"Name::EnumBInNamespace");
    element = results.at(4);
    EXPECT_EQ(element->getName(), L"Name::nestedNamespace::EnumNestedNamespace");
    element = results.at(5);
    EXPECT_EQ(element->getName(), L"EnumAOneLine");
    element = results.at(6);
    EXPECT_EQ(element->getName(), L"EnumBOneLine");
}

TEST(VPGEnumClassReaderTest, EnumClassWithManager)
{
    std::wstring code = L""
        "#pragma once\r\n"
        "\r\n"
        "enum class Property {\r\n"
        "    Property, // GETSET(std::wstring, Property, L\"\")\r\n"
        "    Manager1, // MANAGER_SPTR(GitManager, GitManager1, _LogProperty)\r\n"
        "    Manager2, // MANAGER_SPTR_NULL(vcc::GitManager, GitManager2)\r\n"
        "    Manager3, // MANAGER_SPTR_PARENT(GitManager, GitManager3, GitBaseManager)\r\n"
        "    Action1, // ACTION(AddGitLog) \r\n"
        "    Action2, // ACTION_WITH_ARG_SPTR(DeleteGitLog, GitLog) \r\n"
        "    Action3 // ACTION_WITH_ARG_SPTR(ModifyGitLog, GitLog) @@ActionResult { \"Redo.Class\" : \"redoActionResult\", \"Undo.Class\" : \"undoActionResult\" }\r\n"
        "};\r\n";
    std::vector<std::shared_ptr<VPGEnumClass>> results;
    VPGGlobal::getEnumClassReader()->parse(code, results);
    EXPECT_EQ(results.size(), (size_t)1);
    auto element = results.at(0);
    EXPECT_EQ(element->getName(), L"Property");

    // Property
    EXPECT_EQ(element->getProperties().size(), (size_t)7);
    EXPECT_EQ((int64_t)element->getProperties().at(0)->getPropertyType(), (int64_t)VPGEnumClassAttributeType::Property);
    EXPECT_EQ(element->getProperties().at(0)->getType1(), L"std::wstring");
    EXPECT_EQ(element->getProperties().at(0)->getPropertyName(), L"Property");
    EXPECT_EQ((int64_t)element->getProperties().at(1)->getPropertyType(), (int64_t)VPGEnumClassAttributeType::Manager);
    EXPECT_EQ(element->getProperties().at(1)->getType1(), L"GitManager");
    EXPECT_EQ(element->getProperties().at(1)->getPropertyName(), L"GitManager1");
    EXPECT_EQ(element->getProperties().at(1)->getDefaultValue(), L"_LogProperty");
    EXPECT_EQ((int64_t)element->getProperties().at(2)->getPropertyType(), (int64_t)VPGEnumClassAttributeType::Manager);
    EXPECT_EQ(element->getProperties().at(2)->getType1(), L"vcc::GitManager");
    EXPECT_EQ(element->getProperties().at(2)->getPropertyName(), L"GitManager2");
    EXPECT_EQ((int64_t)element->getProperties().at(3)->getPropertyType(), (int64_t)VPGEnumClassAttributeType::Manager);
    EXPECT_EQ(element->getProperties().at(3)->getType1(), L"GitManager");
    EXPECT_EQ(element->getProperties().at(3)->getPropertyName(), L"GitManager3");
    EXPECT_EQ(element->getProperties().at(3)->getDefaultValue(), L"GitBaseManager");
    EXPECT_EQ((int64_t)element->getProperties().at(4)->getPropertyType(), (int64_t)VPGEnumClassAttributeType::Action);
    EXPECT_EQ(element->getProperties().at(4)->getType1(), L"");
    EXPECT_EQ(element->getProperties().at(4)->getPropertyName(), L"AddGitLog");
    EXPECT_EQ(element->getProperties().at(4)->getActionResultRedoClass(), L"vcc::OperationResult");
    EXPECT_EQ(element->getProperties().at(4)->getActionResultUndoClass(), L"vcc::OperationResult");
    EXPECT_EQ((int64_t)element->getProperties().at(5)->getPropertyType(), (int64_t)VPGEnumClassAttributeType::Action);
    EXPECT_EQ(element->getProperties().at(5)->getType1(), L"GitLog");
    EXPECT_EQ(element->getProperties().at(5)->getPropertyName(), L"DeleteGitLog");
    EXPECT_EQ(element->getProperties().at(5)->getActionResultRedoClass(), L"vcc::OperationResult");
    EXPECT_EQ(element->getProperties().at(5)->getActionResultUndoClass(), L"vcc::OperationResult");
    EXPECT_EQ((int64_t)element->getProperties().at(6)->getPropertyType(), (int64_t)VPGEnumClassAttributeType::Action);
    EXPECT_EQ(element->getProperties().at(6)->getType1(), L"GitLog");
    EXPECT_EQ(element->getProperties().at(6)->getPropertyName(), L"ModifyGitLog");
    EXPECT_EQ(element->getProperties().at(6)->getActionResultRedoClass(), L"redoActionResult");
    EXPECT_EQ(element->getProperties().at(6)->getActionResultUndoClass(), L"undoActionResult");
}
