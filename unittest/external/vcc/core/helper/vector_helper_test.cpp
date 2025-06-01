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
        
        virtual std::shared_ptr<vcc::IObject> Clone() const override { return nullptr; };
};

TEST(VectorHelperTest, FindIObject)
{
    std::vector<std::shared_ptr<VectorHelperTestObj>> v;
    v.push_back(std::make_shared<VectorHelperTestObj>());
    EXPECT_EQ(vcc::FindIObject(v, v.at(0).get()), 0);
}

TEST(VectorHelperTest, InsertAndRemoveIObject)
{
    std::vector<std::shared_ptr<VectorHelperTestObj>> v;
    vcc::InsertIObject(v, std::make_shared<VectorHelperTestObj>());
    vcc::InsertIObject(v, std::make_shared<VectorHelperTestObj>());
    EXPECT_EQ(v.size(), (size_t)2);
    auto objectId = v[1]->GetObjectId();
    vcc::RemoveIObject(v, v[0].get());
    EXPECT_EQ(v.size(), (size_t)1);
    EXPECT_EQ(v[0]->GetObjectId(), objectId);
}
