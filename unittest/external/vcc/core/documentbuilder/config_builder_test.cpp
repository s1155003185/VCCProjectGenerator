#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "config.hpp"
#include "config_builder.hpp"

TEST(ConfigBuilderTest, Full)
{
    auto reader = std::make_unique<vcc::ConfigBuilder>();
    
    auto element = std::make_shared<vcc::Config>();
    std::wstring str = L"";
    str += L"# command\r\n";
    str += L"name1=value1\r\n";
    str += L"name2 = value2\r\n";

    std::wstring resultStr = L"";
    resultStr += L"# command\r\n";
    resultStr += L"name1=value1\r\n";
    resultStr += L"name2=value2\r\n";

    reader->Deserialize(str, element);
    EXPECT_EQ(reader->Serialize(element.get()), resultStr);
}
