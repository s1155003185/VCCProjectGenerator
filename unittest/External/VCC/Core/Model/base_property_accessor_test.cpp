#include <gtest/gtest.h>

#include "base_object.hpp"
#include "base_property_accessor.hpp"
#include "class_macro.hpp"
#include "memory_macro.hpp"
#include "lock_type.hpp"

using namespace vcc;

class BasePropertyAccessorTestObject : public BaseObject<BasePropertyAccessorTestObject>
{
    GETSET(std::wstring, String, L"Test")
    GETSET(int, Integer, 1);
    GETSET_SPTR_NULL(IObject, Object);
    public:
        BasePropertyAccessorTestObject() : BaseObject() {}
        virtual ~BasePropertyAccessorTestObject() {}
};

class BasePropertyAccessorTestObjectPropertyAccessor : public BasePropertyAccessor
{
    protected:

        virtual std::wstring _ReadWstring(const size_t &objectProperty, const int64_t &index = -1) const override
        {
            assert(objectProperty == 0L);
            assert(index == -1);
            std::shared_ptr<BasePropertyAccessorTestObject> obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
            return obj->GetString();
        }

        virtual void _WriteWstring(const size_t &objectProperty, const std::wstring &value, const int64_t &index = -1) const override
        {
            assert(objectProperty == 0L);
            assert(index == -1);
            std::shared_ptr<BasePropertyAccessorTestObject> obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
            obj->SetString(value);
            obj->GetString();
        }
            
        virtual int _ReadInt(const size_t &objectProperty, const int64_t &index = -1) const override
        {
            assert(objectProperty == 0L);
            assert(index == -1);
            std::shared_ptr<BasePropertyAccessorTestObject> obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
            return obj->GetInteger();
        }

        virtual void _WriteInt(const size_t &objectProperty, const int &value, const int64_t &index = -1) const override
        {
            assert(objectProperty == 0L);
            assert(index == -1);
            std::shared_ptr<BasePropertyAccessorTestObject> obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
            obj->SetInteger(value);
        }

        virtual std::shared_ptr<IObject> _ReadObject(const size_t &objectProperty, const int64_t &index = -1) const override
        {
            assert(objectProperty == 0L);
            assert(index == -1);
            std::shared_ptr<BasePropertyAccessorTestObject> obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
            return obj->GetObject();
        }

        virtual void _WriteObject(const size_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index = -1) const override
        {
            assert(objectProperty == 0L);
            assert(index == -1);
            std::shared_ptr<BasePropertyAccessorTestObject> obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
            obj->SetObject(std::static_pointer_cast<BasePropertyAccessorTestObject>(value));
        }

    public:
        BasePropertyAccessorTestObjectPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}
        virtual ~BasePropertyAccessorTestObjectPropertyAccessor() {}
};

TEST(BasePropertyAccessorTest, Full) 
{
    DECLARE_SPTR(BasePropertyAccessorTestObject, obj);
    obj->SetObject(std::make_shared<BasePropertyAccessorTestObject>());
    std::unique_ptr<BasePropertyAccessorTestObjectPropertyAccessor> accessor = std::make_unique<BasePropertyAccessorTestObjectPropertyAccessor>(obj);
    EXPECT_EQ(accessor->ReadWstring(LockType::ReadLock, 0L), L"Test");
    EXPECT_EQ(accessor->ReadInt(LockType::ReadLock, 0L), 1);
    EXPECT_TRUE(accessor->ReadObject(LockType::ReadLock, 0L) != nullptr);

    accessor->WriteWstring(LockType::WriteLock, 0L, L"SetString");
    accessor->WriteInt(LockType::WriteLock, 0L, 2);
    EXPECT_EQ(accessor->ReadWstring(LockType::ReadLock, 0L), L"SetString");
    EXPECT_EQ(accessor->ReadInt(LockType::ReadLock, 0L), 2);
    EXPECT_TRUE(accessor->ReadObject(LockType::ReadLock, 0L) != nullptr);
}
