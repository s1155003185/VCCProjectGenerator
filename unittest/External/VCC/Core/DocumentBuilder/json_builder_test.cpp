#include <gtest/gtest.h>

#include <iostream>
#include <memory>
#include <string>

#include "memory_macro.hpp"
#include "json.hpp"
#include "json_builder.hpp"

using namespace vcc;

TEST(JsonBuilderTest, String)
{
    std::wstring str = L"{\"name\":\"John\"}";
    std::unique_ptr<JsonBuilder> builder = std::make_unique<JsonBuilder>();
    DECLARE_SPTR(Json, json);
    builder->Deserialize(str, json);
    EXPECT_EQ(json->GetType(), JsonType::Json);
    EXPECT_EQ(json->GetNameValuePairs(L"name")->GetType(), JsonType::String);
    EXPECT_EQ(json->GetNameValuePairs(L"name")->GetValue(), L"John");
    EXPECT_EQ(builder->Serialize(json.get()), str);
}

TEST(JsonBuilderTest, Boolean)
{
    std::wstring str = L"{\"answer\":true}";
    std::unique_ptr<JsonBuilder> builder = std::make_unique<JsonBuilder>();
    DECLARE_SPTR(Json, json);
    builder->Deserialize(str, json);
    EXPECT_EQ(json->GetType(), JsonType::Json);
    EXPECT_EQ(json->GetNameValuePairs(L"answer")->GetType(), JsonType::Boolean);
    EXPECT_EQ(json->GetNameValuePairs(L"answer")->GetValue(), L"true");
    EXPECT_EQ(builder->Serialize(json.get()), str);
}

TEST(JsonBuilderTest, Number)
{
    std::wstring str = L"{\"age\":11}";
    std::unique_ptr<JsonBuilder> builder = std::make_unique<JsonBuilder>();
    DECLARE_SPTR(Json, json);
    builder->Deserialize(str, json);
    EXPECT_EQ(json->GetType(), JsonType::Json);
    EXPECT_EQ(json->GetNameValuePairs(L"age")->GetType(), JsonType::Number);
    EXPECT_EQ(json->GetNameValuePairs(L"age")->GetValue(), L"11");
    EXPECT_EQ(builder->Serialize(json.get()), str);
}

TEST(JsonBuilderTest, Object)
{
    std::wstring str = L"{\"Name\":{\"firstName\":\"A\",\"lastName\":\"B\"}}";
    std::unique_ptr<JsonBuilder> builder = std::make_unique<JsonBuilder>();
    DECLARE_SPTR(Json, json);
    builder->Deserialize(str, json);
    EXPECT_EQ(json->GetType(), JsonType::Json);
    EXPECT_EQ(json->GetNameValuePairs(L"Name")->GetType(), JsonType::Object);
    EXPECT_EQ(json->GetNameValuePairs(L"Name")->GetArray()[0]->GetType(), JsonType::Json);
    EXPECT_EQ(json->GetNameValuePairs(L"Name")->GetArray()[0]->GetNameValuePairs(L"firstName")->GetValue(), L"A");
    EXPECT_EQ(json->GetNameValuePairs(L"Name")->GetArray()[0]->GetNameValuePairs(L"lastName")->GetValue(), L"B");
    EXPECT_EQ(builder->Serialize(json.get()), str);
}

TEST(JsonBuilderTest, Array)
{
    std::wstring str = L"{\"employees\":[1,true,null,\"Str\\\"ing\",[1,2],{\"firstName\":\"A\",\"lastName\":\"B\"}]}";
    std::unique_ptr<JsonBuilder> builder = std::make_unique<JsonBuilder>();
    DECLARE_SPTR(Json, json);
    builder->Deserialize(str, json);
    EXPECT_EQ(json->GetType(), JsonType::Json);
    EXPECT_EQ(json->GetNameValuePairs(L"employees")->GetType(), JsonType::Array);
    EXPECT_EQ(json->GetNameValuePairs(L"employees")->GetArray().size(), (size_t)6);
    EXPECT_EQ(json->GetNameValuePairs(L"employees")->GetArray().at(0)->GetType(), JsonType:: Number);
    EXPECT_EQ(json->GetNameValuePairs(L"employees")->GetArray().at(0)->GetValue(), L"1");
    EXPECT_EQ(json->GetNameValuePairs(L"employees")->GetArray().at(1)->GetType(), JsonType:: Boolean);
    EXPECT_EQ(json->GetNameValuePairs(L"employees")->GetArray().at(1)->GetValue(), L"true");
    EXPECT_EQ(json->GetNameValuePairs(L"employees")->GetArray().at(2)->GetType(), JsonType:: Null);
    EXPECT_EQ(json->GetNameValuePairs(L"employees")->GetArray().at(3)->GetType(), JsonType:: String);
    EXPECT_EQ(json->GetNameValuePairs(L"employees")->GetArray().at(3)->GetValue(), L"Str\"ing");
    EXPECT_EQ(json->GetNameValuePairs(L"employees")->GetArray().at(4)->GetType(), JsonType:: Array);
    EXPECT_EQ(json->GetNameValuePairs(L"employees")->GetArray().at(4)->GetArray().size(), (size_t)2);
    EXPECT_EQ(json->GetNameValuePairs(L"employees")->GetArray().at(5)->GetArray()[0]->GetNameValuePairs(L"firstName")->GetValue(), L"A");
    EXPECT_EQ(json->GetNameValuePairs(L"employees")->GetArray().at(5)->GetArray()[0]->GetNameValuePairs(L"lastName")->GetValue(), L"B");
    EXPECT_EQ(builder->Serialize(json.get()), str);
}

TEST(JsonBuilderTest, Null)
{
    std::wstring str = L"{\"Name\":null}";
    std::unique_ptr<JsonBuilder> builder = std::make_unique<JsonBuilder>();
    DECLARE_SPTR(Json, json);
    builder->Deserialize(str, json);
    EXPECT_EQ(json->GetType(), JsonType::Json);
    EXPECT_EQ(json->GetNameValuePairs(L"Name")->GetType(), JsonType::Null);
    EXPECT_EQ(builder->Serialize(json.get()), str);
}

TEST(JsonBuilderTest, Full)
{
    std::wstring str = L"{\"name\":\"John\",\"age\":11,\"tel\":null,\"FullName\":{\"firstName\":\"A\",\"lastName\":\"B\"},\"employees\":[{\"firstName\":\"A\",\"lastName\":\"B\"},{\"firstName\":\"C\",\"lastName\":\"D\"}]}";
    std::unique_ptr<JsonBuilder> builder = std::make_unique<JsonBuilder>();
    DECLARE_SPTR(Json, json);
    builder->Deserialize(str, json);
    EXPECT_EQ(json->GetType(), JsonType::Json);
    EXPECT_EQ(json->GetNameValuePairs(L"name")->GetType(), JsonType::String);
    EXPECT_EQ(json->GetNameValuePairs(L"name")->GetValue(), L"John");
    EXPECT_EQ(json->GetNameValuePairs(L"age")->GetType(), JsonType::Number);
    EXPECT_EQ(json->GetNameValuePairs(L"age")->GetValue(), L"11");
    EXPECT_EQ(json->GetNameValuePairs(L"tel")->GetType(), JsonType::Null);
    EXPECT_EQ(json->GetNameValuePairs(L"FullName")->GetType(), JsonType::Object);
    EXPECT_EQ(json->GetNameValuePairs(L"employees")->GetType(), JsonType::Array);
    EXPECT_EQ(builder->Serialize(json.get()), str);
}
