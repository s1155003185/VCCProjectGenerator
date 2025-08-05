#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "base_object.hpp"
#include "i_object.hpp"
#include "vector_helper.hpp"

class VectorHelperTestObj : public vcc::BaseObject
{
    public:
        VectorHelperTestObj() = default;
        ~VectorHelperTestObj() {}
        
        virtual std::shared_ptr<vcc::IObject> clone() const override { return nullptr; };
};

TEST(VectorHelperTest, FindIObject)
{
    std::vector<std::shared_ptr<VectorHelperTestObj>> v;
    v.push_back(std::make_shared<VectorHelperTestObj>());
    EXPECT_EQ(vcc::findIObject(v, v.at(0).get()), 0);
}

TEST(VectorHelperTest, InsertAndRemoveIObject)
{
    std::vector<std::shared_ptr<VectorHelperTestObj>> v;
    vcc::insertIObject(v, std::make_shared<VectorHelperTestObj>());
    vcc::insertIObject(v, std::make_shared<VectorHelperTestObj>());
    EXPECT_EQ(v.size(), (size_t)2);
    auto objectId = v[1]->getObjectId();
    vcc::removeIObject(v, v[0].get());
    EXPECT_EQ(v.size(), (size_t)1);
    EXPECT_EQ(v[0]->getObjectId(), objectId);
}
