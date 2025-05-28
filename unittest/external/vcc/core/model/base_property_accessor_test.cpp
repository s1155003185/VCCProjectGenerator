#include <gtest/gtest.h>

#include "base_object.hpp"
#include "base_property_accessor.hpp"
#include "class_macro.hpp"
#include "exception_macro.hpp"
#include "lock_type.hpp"

#include <memory>

using namespace vcc;

class BasePropertyAccessorTestObject : public BaseObject
{
    GETSET(std::wstring, String, L"Test")
    GETSET(int, Integer, 1);
    GETSET_SPTR_NULL(IObject, Object);

    MAP(std::wstring, std::wstring, MapString)
    VECTOR_SPTR(BasePropertyAccessorTestObject, VectorObject)

    public:
        BasePropertyAccessorTestObject() : BaseObject() {}
        virtual ~BasePropertyAccessorTestObject() {}

        virtual std::shared_ptr<IObject> Clone() const override
        {
            return std::make_shared<BasePropertyAccessorTestObject>(*this);
        }
};

class BasePropertyAccessorTestObjectPropertyAccessor : public BasePropertyAccessor
{
    protected:

        virtual std::wstring _ReadStringAtIndex(const int64_t &objectProperty, const int64_t &index = -1) const override
        {
            assert(objectProperty == 0L);
            assert(index == -1);
            auto obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
            return obj->GetString();
        }

        virtual void _WriteStringAtIndex(const int64_t &objectProperty, const std::wstring &value, const int64_t &index = -1) override
        {
            assert(objectProperty == 0L);
            assert(index == -1);
            auto obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
            obj->SetString(value);
            obj->GetString();
        }
            
        virtual int _ReadIntAtIndex(const int64_t &objectProperty, const int64_t &index = -1) const override
        {
            assert(objectProperty == 0L);
            assert(index == -1);
            auto obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
            return obj->GetInteger();
        }

        virtual void _WriteIntAtIndex(const int64_t &objectProperty, const int &value, const int64_t &index = -1) override
        {
            assert(objectProperty == 0L);
            assert(index == -1);
            auto obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
            obj->SetInteger(value);
        }

        virtual std::shared_ptr<IObject> _ReadObject(const int64_t &objectProperty) const override
        {
            assert(objectProperty == 0L);
            auto obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
            return obj->GetObject();
        }

        virtual void _WriteObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index = -1) override
        {
            assert(objectProperty == 0L);
            assert(index == -1);
            auto obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
            obj->SetObject(std::static_pointer_cast<BasePropertyAccessorTestObject>(value));
        }

        void _InsertObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<IObject> value, const int64_t &index) override
        {
            TRY
                assert(objectProperty == 0L);
                assert(index >= -1);
                auto obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
                assert(obj != nullptr);
                obj->InsertVectorObjectAtIndex(index, std::static_pointer_cast<BasePropertyAccessorTestObject>(value));
            CATCH
        }
        
        virtual std::wstring _ReadStringAtKey(const int64_t &objectProperty, const void *key) const override
        {
            assert(objectProperty == 0L);
            assert(key != nullptr);
            auto obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
            const std::wstring *keyPtr = static_cast<const std::wstring *>(key);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            return obj->GetMapStringAtKey(*keyPtr);
        }

        virtual void _WriteStringAtKey(const int64_t &objectProperty, const std::wstring &value, const void *key) override
        {
            assert(objectProperty == 0L);
            assert(key != nullptr);
            auto obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
            const std::wstring *keyPtr = static_cast<const std::wstring *>(key);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            obj->InsertMapStringAtKey(*keyPtr, value);
        }
    public:
        BasePropertyAccessorTestObjectPropertyAccessor(std::shared_ptr<IObject> object) : BasePropertyAccessor(object) {}
        virtual ~BasePropertyAccessorTestObjectPropertyAccessor() {}
};

std::shared_ptr<IObject> Create()
{
    return std::make_shared<BasePropertyAccessorTestObject>();
}
TEST(BasePropertyAccessorTest, Full) 
{
    auto obj = std::make_shared<BasePropertyAccessorTestObject>();
    obj->SetObject(std::make_shared<BasePropertyAccessorTestObject>());
    std::unique_ptr<BasePropertyAccessorTestObjectPropertyAccessor> accessor = std::make_unique<BasePropertyAccessorTestObjectPropertyAccessor>(obj);
    EXPECT_EQ(accessor->ReadStringAtIndex(LockType::ReadLock, 0L), L"Test");
    EXPECT_EQ(accessor->ReadIntAtIndex(LockType::ReadLock, 0L), 1);
    EXPECT_TRUE(accessor->ReadObject(LockType::ReadLock, 0L) != nullptr);

    accessor->WriteStringAtIndex(LockType::WriteLock, 0L, L"SetString");
    accessor->WriteIntAtIndex(LockType::WriteLock, 0L, 2);
    EXPECT_EQ(accessor->ReadStringAtIndex(LockType::ReadLock, 0L), L"SetString");
    EXPECT_EQ(accessor->ReadIntAtIndex(LockType::ReadLock, 0L), 2);
    EXPECT_TRUE(accessor->ReadObject(LockType::ReadLock, 0L) != nullptr);

    std::shared_ptr<IObject> newObj = Create();
    accessor->InsertObjectAtIndex(LockType::NoLock, 0L, newObj);

    std::wstring mapKey1 = L"key";
    std::wstring mapKey2 = L"key";
    accessor->WriteStringAtKey(LockType::WriteLock, 0L, L"value", &mapKey1);
    EXPECT_EQ(accessor->ReadStringAtKey(LockType::ReadLock, 0L, &mapKey2), L"value");
}
