#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "base_object.hpp"
#include "i_object.hpp"
#include "vector_helper.hpp"

using namespace vcc;

class VectorHelperTestObj : public BaseObject
{
    public:
        VectorHelperTestObj() = default;
        ~VectorHelperTestObj() {}
        
        virtual std::shared_ptr<IObject> Clone() const override { return nullptr; };
};

TEST(VectorHelperTest, FindIObject)
{
    std::vector<std::shared_ptr<VectorHelperTestObj>> v;
    v.push_back(std::make_shared<VectorHelperTestObj>());
    EXPECT_EQ(FindIObject(v, v.at(0)), 0);
}

TEST(VectorHelperTest, InsertIObject)
{
    std::vector<std::shared_ptr<VectorHelperTestObj>> v;
    InsertIObject(v, std::make_shared<VectorHelperTestObj>());
    EXPECT_EQ(v.size(), (size_t)1);
    RemoveIObject(v, v[0].get());
    EXPECT_TRUE(v.empty());
}