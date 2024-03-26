#include <gtest/gtest.h>

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
    //DECLARE_SPTR(JsonObject, json);
    std::shared_ptr<JsonObject> json = std::make_shared<JsonObject>();
    builder->Deserialize(str, json);
    EXPECT_EQ(json->GetType(), JsonType::Json);
    EXPECT_EQ(json->GetJsonNameValuePairs()[L"name"]->GetType(), JsonType::String);
    EXPECT_EQ(json->GetJsonNameValuePairs()[L"name"]->GetValue(), L"John");
}

TEST(JsonBuilderTest, Number)
{
    std::wstring str = L"{\"age\":11}";
    std::unique_ptr<JsonBuilder> builder = std::make_unique<JsonBuilder>();
}

TEST(JsonBuilderTest, Object)
{
    std::wstring str = L"{\"Name\":{ \"firstName\":\"A\", \"lastName\":\"B\" }}";
    std::unique_ptr<JsonBuilder> builder = std::make_unique<JsonBuilder>();
}

TEST(JsonBuilderTest, Array)
{
    std::wstring str = L"{\"employees\":[{ \"firstName\":\"A\", \"lastName\":\"B\" }, { \"firstName\":\"C\", \"lastName\":\"D\" }]}";
    std::unique_ptr<JsonBuilder> builder = std::make_unique<JsonBuilder>();
}

TEST(JsonBuilderTest, Null)
{
    std::wstring str = L"{\"Name\":null}";
    std::unique_ptr<JsonBuilder> builder = std::make_unique<JsonBuilder>();
}

TEST(JsonBuilderTest, Full)
{
    std::wstring str = L"{\"name\":\"John\", \"age\":11, \"tel\":null, \"FullName\":{ \"firstName\":\"A\", \"lastName\":\"B\", {\"employees\":[{ \"firstName\":\"A\", \"lastName\":\"B\" }, { \"firstName\":\"C\", \"lastName\":\"D\" }] }}";
    std::unique_ptr<JsonBuilder> builder = std::make_unique<JsonBuilder>();
}
