#include <gtest/gtest.h>

#include "json.hpp"
#include "json_builder.hpp"

TEST(JsonTest, Full) 
{
    auto builder = std::make_unique<vcc::JsonBuilder>();

    auto json = std::make_shared<vcc::Json>();
    json->addString(L"Version", L"0.0.1");
    json->addBool(L"IsGit", true);
    json->addDouble(L"Price", 2.33, 2);
    json->addInt(L"State", 1);
    json->addNull(L"Action");
    
    auto plugins = std::make_shared<vcc::Json>();
    json->addArray(L"Plugins", plugins);
    plugins->addArrayString(L"/path/of/Git");
    
    std::wstring jsonStr = builder->serialize(json.get());

    auto resultJson = std::make_shared<vcc::Json>();
    builder->deserialize(jsonStr, resultJson);

    EXPECT_EQ(json->getString(L"Version"), resultJson->getString(L"Version"));
    EXPECT_EQ(json->getBool(L"IsGit"), resultJson->getBool(L"IsGit"));
    EXPECT_EQ(json->getDouble(L"Price"), resultJson->getDouble(L"Price"));
    EXPECT_EQ(json->getInt64(L"State"), resultJson->getInt64(L"State"));
    EXPECT_EQ(json->isNull(L"Action"), resultJson->isNull(L"Action"));
    EXPECT_EQ(json->getArray(L"Plugins").size(), resultJson->getArray(L"Plugins").size());
    EXPECT_EQ(json->getArray(L"Plugins").at(0)->getJsonInternalValue(), resultJson->getArray(L"Plugins").at(0)->getJsonInternalValue());

    // Set
    json->setString(L"Version", L"v0.0.1");
    json->setBool(L"IsGit", false);
    json->setDouble(L"Price", 1.23456, 2);
    json->setInt(L"State", 12);
    json->setNull(L"Action");
    EXPECT_EQ(json->getString(L"Version"), L"v0.0.1");
    EXPECT_EQ(json->getBool(L"IsGit"), false);
    EXPECT_EQ(json->getDouble(L"Price"), 1.23);
    EXPECT_EQ(json->getInt64(L"State"), 12);
    EXPECT_EQ(json->isNull(L"Action"), true);
    // EXPECT_EQ(json->getArray(L"Plugins").size(), resultJson->getArray(L"Plugins").size());
    // EXPECT_EQ(json->getArray(L"Plugins").at(0)->getJsonInternalValue(), resultJson->getArray(L"Plugins").at(0)->getJsonInternalValue());

}
