#include <gtest/gtest.h>

#include "json.hpp"
#include "json_builder.hpp"

using namespace vcc;

TEST(JsonTest, Full) 
{
    std::unique_ptr<JsonBuilder> builder = std::make_unique<JsonBuilder>();

    auto json = std::make_shared<Json>();
    json->AddString(L"Version", L"0.0.1");
    json->AddBool(L"IsGit", true);
    json->AddDouble(L"Price", 2.33, 2);
    json->AddInt(L"State", 1);
    json->AddNull(L"Action");
    
    auto plugins = std::make_shared<Json>();
    json->AddArray(L"Plugins", plugins);
    plugins->AddArrayString(L"/path/of/Git");
    
    std::wstring jsonStr = builder->Serialize(json.get());

    auto resultJson = std::make_shared<Json>();
    builder->Deserialize(jsonStr, resultJson);

    EXPECT_EQ(json->GetString(L"Version"), resultJson->GetString(L"Version"));
    EXPECT_EQ(json->GetBool(L"IsGit"), resultJson->GetBool(L"IsGit"));
    EXPECT_EQ(json->GetDouble(L"Price"), resultJson->GetDouble(L"Price"));
    EXPECT_EQ(json->GetInt64(L"State"), resultJson->GetInt64(L"State"));
    EXPECT_EQ(json->IsNull(L"Action"), resultJson->IsNull(L"Action"));
    EXPECT_EQ(json->GetArray(L"Plugins").size(), resultJson->GetArray(L"Plugins").size());
    EXPECT_EQ(json->GetArray(L"Plugins").at(0)->GetJsonInternalValue(), resultJson->GetArray(L"Plugins").at(0)->GetJsonInternalValue());

    // Set
    json->SetString(L"Version", L"v0.0.1");
    json->SetBool(L"IsGit", false);
    json->SetDouble(L"Price", 1.23456, 2);
    json->SetInt(L"State", 12);
    json->SetNull(L"Action");
    EXPECT_EQ(json->GetString(L"Version"), L"v0.0.1");
    EXPECT_EQ(json->GetBool(L"IsGit"), false);
    EXPECT_EQ(json->GetDouble(L"Price"), 1.23);
    EXPECT_EQ(json->GetInt64(L"State"), 12);
    EXPECT_EQ(json->IsNull(L"Action"), true);
    // EXPECT_EQ(json->GetArray(L"Plugins").size(), resultJson->GetArray(L"Plugins").size());
    // EXPECT_EQ(json->GetArray(L"Plugins").at(0)->GetJsonInternalValue(), resultJson->GetArray(L"Plugins").at(0)->GetJsonInternalValue());

}
