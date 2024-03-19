#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "json_builder.hpp"

using namespace vcc;

TEST(JsonBuilderTest, Normal)
{
    std::unique_ptr<JsonBuilder> builder = std::make_unique<JsonBuilder>();
}
