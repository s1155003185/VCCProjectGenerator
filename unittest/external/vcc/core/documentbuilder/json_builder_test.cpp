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
    builder->Deserialize(str, json);
    EXPECT_EQ(json->GetJsonInternalType(), vcc::JsonInternalType::Json);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"name")->GetJsonInternalType(), vcc::JsonInternalType::String);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"name")->GetJsonInternalValue(), L"John");
    EXPECT_EQ(builder->Serialize(json.get()), str);
}

TEST(JsonBuilderTest, Boolean)
{
    std::wstring str = L"{\"answer\":true}";
    auto builder = std::make_unique<vcc::JsonBuilder>();
    auto json = std::make_shared<vcc::Json>();
    builder->Deserialize(str, json);
    EXPECT_EQ(json->GetJsonInternalType(), vcc::JsonInternalType::Json);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"answer")->GetJsonInternalType(), vcc::JsonInternalType::Boolean);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"answer")->GetJsonInternalValue(), L"true");
    EXPECT_EQ(builder->Serialize(json.get()), str);
}

TEST(JsonBuilderTest, Number)
{
    std::wstring str = L"{\"age\":11}";
    auto builder = std::make_unique<vcc::JsonBuilder>();
    auto json = std::make_shared<vcc::Json>();
    builder->Deserialize(str, json);
    EXPECT_EQ(json->GetJsonInternalType(), vcc::JsonInternalType::Json);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"age")->GetJsonInternalType(), vcc::JsonInternalType::Number);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"age")->GetJsonInternalValue(), L"11");
    EXPECT_EQ(builder->Serialize(json.get()), str);
}

TEST(JsonBuilderTest, Object)
{
    std::wstring str = L"{\"Name\":{\"firstName\":\"A\",\"lastName\":\"B\"}}";
    auto builder = std::make_unique<vcc::JsonBuilder>();
    auto json = std::make_shared<vcc::Json>();
    builder->Deserialize(str, json);
    EXPECT_EQ(json->GetJsonInternalType(), vcc::JsonInternalType::Json);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"Name")->GetJsonInternalType(), vcc::JsonInternalType::Object);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"Name")->GetJsonInternalArray()[0]->GetJsonInternalType(), vcc::JsonInternalType::Json);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"Name")->GetJsonInternalArray()[0]->GetJsonInternalNameValuePairsAtKey(L"firstName")->GetJsonInternalValue(), L"A");
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"Name")->GetJsonInternalArray()[0]->GetJsonInternalNameValuePairsAtKey(L"lastName")->GetJsonInternalValue(), L"B");
    EXPECT_EQ(builder->Serialize(json.get()), str);
}

TEST(JsonBuilderTest, Array)
{
    std::wstring str = L"{\"employees\":[1,true,null,\"Str\\\"ing\",[1,2],{\"firstName\":\"A\",\"lastName\":\"B\"}]}";
    auto builder = std::make_unique<vcc::JsonBuilder>();
    auto json = std::make_shared<vcc::Json>();
    builder->Deserialize(str, json);
    EXPECT_EQ(json->GetJsonInternalType(), vcc::JsonInternalType::Json);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"employees")->GetJsonInternalType(), vcc::JsonInternalType::Array);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"employees")->GetJsonInternalArray().size(), (size_t)6);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"employees")->GetJsonInternalArray().at(0)->GetJsonInternalType(), vcc::JsonInternalType:: Number);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"employees")->GetJsonInternalArray().at(0)->GetJsonInternalValue(), L"1");
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"employees")->GetJsonInternalArray().at(1)->GetJsonInternalType(), vcc::JsonInternalType:: Boolean);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"employees")->GetJsonInternalArray().at(1)->GetJsonInternalValue(), L"true");
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"employees")->GetJsonInternalArray().at(2)->GetJsonInternalType(), vcc::JsonInternalType:: Null);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"employees")->GetJsonInternalArray().at(3)->GetJsonInternalType(), vcc::JsonInternalType:: String);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"employees")->GetJsonInternalArray().at(3)->GetJsonInternalValue(), L"Str\"ing");
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"employees")->GetJsonInternalArray().at(4)->GetJsonInternalType(), vcc::JsonInternalType:: Array);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"employees")->GetJsonInternalArray().at(4)->GetJsonInternalArray().size(), (size_t)2);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"employees")->GetJsonInternalArray().at(5)->GetJsonInternalArray()[0]->GetJsonInternalNameValuePairsAtKey(L"firstName")->GetJsonInternalValue(), L"A");
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"employees")->GetJsonInternalArray().at(5)->GetJsonInternalArray()[0]->GetJsonInternalNameValuePairsAtKey(L"lastName")->GetJsonInternalValue(), L"B");
    EXPECT_EQ(builder->Serialize(json.get()), str);
}

TEST(JsonBuilderTest, Null)
{
    std::wstring str = L"{\"Name\":null}";
    auto builder = std::make_unique<vcc::JsonBuilder>();
    auto json = std::make_shared<vcc::Json>();
    builder->Deserialize(str, json);
    EXPECT_EQ(json->GetJsonInternalType(), vcc::JsonInternalType::Json);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"Name")->GetJsonInternalType(), vcc::JsonInternalType::Null);
    EXPECT_EQ(builder->Serialize(json.get()), str);
}

TEST(JsonBuilderTest, Full)
{
    std::wstring str = L"{\"name\":\"John\",\"age\":11,\"tel\":null,\"FullName\":{\"firstName\":\"A\",\"lastName\":\"B\"},\"employees\":[{\"firstName\":\"A\",\"lastName\":\"B\"},{\"firstName\":\"C\",\"lastName\":\"D\"}]}";
    auto builder = std::make_unique<vcc::JsonBuilder>();
    auto json = std::make_shared<vcc::Json>();
    builder->Deserialize(str, json);
    EXPECT_EQ(json->GetJsonInternalType(), vcc::JsonInternalType::Json);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"name")->GetJsonInternalType(), vcc::JsonInternalType::String);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"name")->GetJsonInternalValue(), L"John");
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"age")->GetJsonInternalType(), vcc::JsonInternalType::Number);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"age")->GetJsonInternalValue(), L"11");
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"tel")->GetJsonInternalType(), vcc::JsonInternalType::Null);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"FullName")->GetJsonInternalType(), vcc::JsonInternalType::Object);
    EXPECT_EQ(json->GetJsonInternalNameValuePairsAtKey(L"employees")->GetJsonInternalType(), vcc::JsonInternalType::Array);
    EXPECT_EQ(builder->Serialize(json.get()), str);
}

TEST(JsonBuilderTest, Beautify)
{
    std::wstring str = L"{\"name\":\"John\",\"age\":11,\"tel\":null,\"FullName\":{\"firstName\":\"A\",\"lastName\":\"B\"},\"employees\":[{\"firstName\":\"A\",\"lastName\":\"B\"},{\"firstName\":\"C\",\"lastName\":\"D\"}]}";
    auto builder = std::make_unique<vcc::JsonBuilder>();
    auto json = std::make_shared<vcc::Json>();
    builder->Deserialize(str, json);

    std::wstring indent = L"\t";
    builder->SetIsBeautify(true);
    builder->SetIndent(indent);
    builder->SetNameColonSpace(L" ");
    builder->SetColonValueSpace(L" ");
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

    EXPECT_EQ(builder->Serialize(json.get()), result);
}
