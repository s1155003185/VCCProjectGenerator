#include <gtest/gtest.h>

#include "json.hpp"
#include "json_builder.hpp"
#include "memory_macro.hpp"

using namespace std;
using namespace vcc;

TEST(JsonTest, Full) 
{
    std::wstring str = L"{\"name\":\"John\",\"age\":11,\"tel\":null,\"price\":1.1,\"FullName\":{\"firstName\":\"A\",\"lastName\":\"B\"},\"employees\":[[1,[2,{\"InArray\":true}]],{\"firstName\":\"C\",\"lastName\":\"D\"}]}";
    std::unique_ptr<JsonBuilder> builder = std::make_unique<JsonBuilder>();
    DECLARE_SPTR(JsonObject, json);
    builder->Deserialize(str, json);
    EXPECT_EQ(json->GetString(L"name"), L"John");
    EXPECT_EQ(json->GetInt32(L"age"), 11);
    EXPECT_EQ(json->IsNull(L"tel"), true);
    EXPECT_EQ(json->GetDouble(L"price"), 1.1);
    EXPECT_EQ(json->GetDecimalPlaces(L"price"), 1);
    EXPECT_TRUE(json->GetObject(L"FullName") != nullptr);
    EXPECT_EQ(json->GetArray(L"employees").size(), (size_t)2);
}
