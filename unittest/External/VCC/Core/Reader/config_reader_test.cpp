#include <gtest/gtest.h>

#include <map>
#include <memory>
#include <string>

#include "config_reader.hpp"

using namespace vcc;

TEST(ConfigReaderTest, Full)
{
    std::unique_ptr<ConfigReader> reader = std::make_unique<ConfigReader>();
    std::wstring str = L"";
    str += L"# command\r\n";
    str += L"name1=value1\r\n";
    str += L"name2 = value2\r\n";
    ConfigElement element;
    reader->Parse(str, element);
    EXPECT_EQ(element.GetConfigs()->size(), (size_t)2);
    for (auto it = element.GetConfigs()->begin(); it != element.GetConfigs()->end(); it++) {
        EXPECT_TRUE((it->first == L"name1" && it->second == L"value1") || (it->first == L"name2" && it->second == L"value2"));
    }
}
