#include <gtest/gtest.h>

#include <iostream>
#include <memory>
#include <string>

#include "json.hpp"
#include "json_builder.hpp"

TEST(JsonBuilderTest, String)
{
    std::wstring str = L"{\"name\":\"John\"}";
    auto builder = std::make_unique<vcc::JsonBuilder>();
    auto json = std::make_shared<vcc::Json>();
    builder->deserialize(str, json);
    EXPECT_EQ(json->getJsonInternalType(), vcc::JsonInternalType::Json);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"name")->getJsonInternalType(), vcc::JsonInternalType::String);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"name")->getJsonInternalValue(), L"John");
    EXPECT_EQ(builder->serialize(json.get()), str);
}

TEST(JsonBuilderTest, Boolean)
{
    std::wstring str = L"{\"answer\":true}";
    auto builder = std::make_unique<vcc::JsonBuilder>();
    auto json = std::make_shared<vcc::Json>();
    builder->deserialize(str, json);
    EXPECT_EQ(json->getJsonInternalType(), vcc::JsonInternalType::Json);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"answer")->getJsonInternalType(), vcc::JsonInternalType::Boolean);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"answer")->getJsonInternalValue(), L"true");
    EXPECT_EQ(builder->serialize(json.get()), str);
}

TEST(JsonBuilderTest, Number)
{
    std::wstring str = L"{\"age\":11}";
    auto builder = std::make_unique<vcc::JsonBuilder>();
    auto json = std::make_shared<vcc::Json>();
    builder->deserialize(str, json);
    EXPECT_EQ(json->getJsonInternalType(), vcc::JsonInternalType::Json);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"age")->getJsonInternalType(), vcc::JsonInternalType::Number);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"age")->getJsonInternalValue(), L"11");
    EXPECT_EQ(builder->serialize(json.get()), str);
}

TEST(JsonBuilderTest, Object)
{
    std::wstring str = L"{\"Name\":{\"firstName\":\"A\",\"lastName\":\"B\"}}";
    auto builder = std::make_unique<vcc::JsonBuilder>();
    auto json = std::make_shared<vcc::Json>();
    builder->deserialize(str, json);
    EXPECT_EQ(json->getJsonInternalType(), vcc::JsonInternalType::Json);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"Name")->getJsonInternalType(), vcc::JsonInternalType::Object);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"Name")->getJsonInternalArray()[0]->getJsonInternalType(), vcc::JsonInternalType::Json);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"Name")->getJsonInternalArray()[0]->getJsonInternalNameValuePairsAtKey(L"firstName")->getJsonInternalValue(), L"A");
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"Name")->getJsonInternalArray()[0]->getJsonInternalNameValuePairsAtKey(L"lastName")->getJsonInternalValue(), L"B");
    EXPECT_EQ(builder->serialize(json.get()), str);
}

TEST(JsonBuilderTest, Array)
{
    std::wstring str = L"{\"employees\":[1,true,null,\"Str\\\"ing\",[1,2],{\"firstName\":\"A\",\"lastName\":\"B\"}]}";
    auto builder = std::make_unique<vcc::JsonBuilder>();
    auto json = std::make_shared<vcc::Json>();
    builder->deserialize(str, json);
    EXPECT_EQ(json->getJsonInternalType(), vcc::JsonInternalType::Json);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"employees")->getJsonInternalType(), vcc::JsonInternalType::Array);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"employees")->getJsonInternalArray().size(), (size_t)6);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"employees")->getJsonInternalArray().at(0)->getJsonInternalType(), vcc::JsonInternalType:: Number);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"employees")->getJsonInternalArray().at(0)->getJsonInternalValue(), L"1");
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"employees")->getJsonInternalArray().at(1)->getJsonInternalType(), vcc::JsonInternalType:: Boolean);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"employees")->getJsonInternalArray().at(1)->getJsonInternalValue(), L"true");
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"employees")->getJsonInternalArray().at(2)->getJsonInternalType(), vcc::JsonInternalType:: Null);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"employees")->getJsonInternalArray().at(3)->getJsonInternalType(), vcc::JsonInternalType:: String);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"employees")->getJsonInternalArray().at(3)->getJsonInternalValue(), L"Str\"ing");
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"employees")->getJsonInternalArray().at(4)->getJsonInternalType(), vcc::JsonInternalType:: Array);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"employees")->getJsonInternalArray().at(4)->getJsonInternalArray().size(), (size_t)2);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"employees")->getJsonInternalArray().at(5)->getJsonInternalArray()[0]->getJsonInternalNameValuePairsAtKey(L"firstName")->getJsonInternalValue(), L"A");
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"employees")->getJsonInternalArray().at(5)->getJsonInternalArray()[0]->getJsonInternalNameValuePairsAtKey(L"lastName")->getJsonInternalValue(), L"B");
    EXPECT_EQ(builder->serialize(json.get()), str);
}

TEST(JsonBuilderTest, Null)
{
    std::wstring str = L"{\"Name\":null}";
    auto builder = std::make_unique<vcc::JsonBuilder>();
    auto json = std::make_shared<vcc::Json>();
    builder->deserialize(str, json);
    EXPECT_EQ(json->getJsonInternalType(), vcc::JsonInternalType::Json);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"Name")->getJsonInternalType(), vcc::JsonInternalType::Null);
    EXPECT_EQ(builder->serialize(json.get()), str);
}

TEST(JsonBuilderTest, Full)
{
    std::wstring str = L"{\"name\":\"John\",\"age\":11,\"tel\":null,\"FullName\":{\"firstName\":\"A\",\"lastName\":\"B\"},\"employees\":[{\"firstName\":\"A\",\"lastName\":\"B\"},{\"firstName\":\"C\",\"lastName\":\"D\"}]}";
    auto builder = std::make_unique<vcc::JsonBuilder>();
    auto json = std::make_shared<vcc::Json>();
    builder->deserialize(str, json);
    EXPECT_EQ(json->getJsonInternalType(), vcc::JsonInternalType::Json);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"name")->getJsonInternalType(), vcc::JsonInternalType::String);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"name")->getJsonInternalValue(), L"John");
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"age")->getJsonInternalType(), vcc::JsonInternalType::Number);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"age")->getJsonInternalValue(), L"11");
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"tel")->getJsonInternalType(), vcc::JsonInternalType::Null);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"FullName")->getJsonInternalType(), vcc::JsonInternalType::Object);
    EXPECT_EQ(json->getJsonInternalNameValuePairsAtKey(L"employees")->getJsonInternalType(), vcc::JsonInternalType::Array);
    EXPECT_EQ(builder->serialize(json.get()), str);
}

TEST(JsonBuilderTest, Beautify)
{
    std::wstring str = L"{\"name\":\"John\",\"age\":11,\"tel\":null,\"FullName\":{\"firstName\":\"A\",\"lastName\":\"B\"},\"employees\":[{\"firstName\":\"A\",\"lastName\":\"B\"},{\"firstName\":\"C\",\"lastName\":\"D\"}]}";
    auto builder = std::make_unique<vcc::JsonBuilder>();
    auto json = std::make_shared<vcc::Json>();
    builder->deserialize(str, json);

    std::wstring indent = L"\t";
    builder->setIsBeautify(true);
    builder->setIndent(indent);
    builder->setNameColonSpace(L" ");
    builder->setColonValueSpace(L" ");
    std::wstring result = L"";
    result += L"{\r\n"; 
    result += indent + L"\"name\" : \"John\",\r\n";
    result += indent + L"\"age\" : 11,\r\n";
    result += indent + L"\"tel\" : null,\r\n";
    result += indent + L"\"FullName\" : {\r\n";
    result += indent + indent + L"\"firstName\" : \"A\",\r\n";
    result += indent + indent + L"\"lastName\" : \"B\"\r\n";
    result += indent + L"},\r\n";
    result += indent + L"\"employees\" : [\r\n";
    result += indent + indent + L"{\r\n";
    result += indent + indent + indent + L"\"firstName\" : \"A\",\r\n";
    result += indent + indent + indent + L"\"lastName\" : \"B\"\r\n";
    result += indent + indent + L"},\r\n";
    result += indent + indent + L"{\r\n";
    result += indent + indent + indent + L"\"firstName\" : \"C\",\r\n";
    result += indent + indent + indent + L"\"lastName\" : \"D\"\r\n";
    result += indent + indent + L"}\r\n";
    result += indent + L"]\r\n";
    result += L"}";

    EXPECT_EQ(builder->serialize(json.get()), result);
}
