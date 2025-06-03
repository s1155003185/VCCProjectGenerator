#include <gtest/gtest.h>

#include <algorithm>
#include <map>
#include <memory>
#include <string>

#include "base_object.hpp"
#include "i_object.hpp"
#include "map_helper.hpp"

class MapHelperTestObj : public vcc::BaseObject
{
    public:
        MapHelperTestObj() = default;
        ~MapHelperTestObj() {}
        
        virtual std::shared_ptr<vcc::IObject> Clone() const override { return nullptr; };
};

TEST(MapHelperTest, FlipMap)
{
    std::map<int, int> m, result;
    m.insert(std::make_pair(1,2));
    result.insert(std::make_pair(2,1));
    EXPECT_EQ(vcc::Flip(m), result);
}

TEST(MapHelperTest, FlipPair)
{
    EXPECT_EQ(vcc::Flip(std::make_pair(1,2)), std::make_pair(2,1));
}

TEST(MapHelperTest, Find)
{
    std::map<int64_t, std::shared_ptr<MapHelperTestObj>> tmp;
    vcc::SetIObject(tmp, (int64_t)1, std::make_shared<MapHelperTestObj>());
    std::set<int64_t> result;
    result.insert(1);
    EXPECT_EQ(vcc::Find(tmp, tmp[1]), result);
}

