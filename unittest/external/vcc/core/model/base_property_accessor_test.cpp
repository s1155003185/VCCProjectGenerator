#include <gtest/gtest.h>

#include "base_object.hpp"
#include "base_property_accessor.hpp"
#include "class_macro.hpp"
#include "exception_macro.hpp"
#include "lock_type.hpp"

#include <memory>

class BasePropertyAccessorTestObject : public vcc::BaseObject
{
    GETSET(std::wstring, String, L"Test")
    GETSET(int, Integer, 1);
    GETSET_SPTR_NULL(vcc::IObject, Object);

    MAP(std::wstring, std::wstring, MapString)
    VECTOR_SPTR(BasePropertyAccessorTestObject, VectorObject)

    public:
        BasePropertyAccessorTestObject() : vcc::BaseObject() {}
        virtual ~BasePropertyAccessorTestObject() {}

        virtual std::shared_ptr<vcc::IObject> clone() const override
        {
            return std::make_shared<BasePropertyAccessorTestObject>(*this);
        }
};

class BasePropertyAccessorTestObjectPropertyAccessor : public vcc::BasePropertyAccessor
{
    protected:

        virtual std::wstring _readStringAtIndex(const int64_t &objectProperty, const int64_t &index = -1) const override
        {
            assert(objectProperty == 0L);
            assert(index == -1);
            auto obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
            return obj->getString();
        }

        virtual void _writeStringAtIndex(const int64_t &objectProperty, const std::wstring &value, const int64_t &index = -1) override
        {
            assert(objectProperty == 0L);
            assert(index == -1);
            auto obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
            obj->setString(value);
            obj->getString();
        }
            
        virtual int _readIntAtIndex(const int64_t &objectProperty, const int64_t &index = -1) const override
        {
            assert(objectProperty == 0L);
            assert(index == -1);
            auto obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
            return obj->getInteger();
        }

        virtual void _writeIntAtIndex(const int64_t &objectProperty, const int &value, const int64_t &index = -1) override
        {
            assert(objectProperty == 0L);
            assert(index == -1);
            auto obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
            obj->setInteger(value);
        }

        virtual std::shared_ptr<vcc::IObject> _readObject(const int64_t &objectProperty) const override
        {
            assert(objectProperty == 0L);
            auto obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
            return obj->getObject();
        }

        virtual void _writeObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> value, const int64_t &index = -1) override
        {
            assert(objectProperty == 0L);
            assert(index == -1);
            auto obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
            obj->setObject(std::static_pointer_cast<BasePropertyAccessorTestObject>(value));
        }

        void _insertObjectAtIndex(const int64_t &objectProperty, std::shared_ptr<vcc::IObject> value, const int64_t &index) override
        {
            TRY
                assert(objectProperty == 0L);
                assert(index >= -1);
                auto obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
                assert(obj != nullptr);
                obj->insertVectorObjectAtIndex(index, std::static_pointer_cast<BasePropertyAccessorTestObject>(value));
            CATCH
        }
        
        virtual std::wstring _readStringAtKey(const int64_t &objectProperty, const void *key) const override
        {
            assert(objectProperty == 0L);
            assert(key != nullptr);
            auto obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
            const std::wstring *keyPtr = static_cast<const std::wstring *>(key);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            return obj->getMapStringAtKey(*keyPtr);
        }

        virtual void _writeStringAtKey(const int64_t &objectProperty, const std::wstring &value, const void *key) override
        {
            assert(objectProperty == 0L);
            assert(key != nullptr);
            auto obj = std::static_pointer_cast<BasePropertyAccessorTestObject>(_Object);
            const std::wstring *keyPtr = static_cast<const std::wstring *>(key);
            if (keyPtr == nullptr)
                THROW_EXCEPTION_MSG(ExceptionType::KeyInvalid, L"Invalid Property Accessor Map Key");
            obj->insertMapStringAtKey(*keyPtr, value);
        }
    public:
        BasePropertyAccessorTestObjectPropertyAccessor(std::shared_ptr<vcc::IObject> object) : vcc::BasePropertyAccessor(object) {}
        virtual ~BasePropertyAccessorTestObjectPropertyAccessor() {}
};

std::shared_ptr<vcc::IObject> create()
{
    return std::make_shared<BasePropertyAccessorTestObject>();
}
TEST(BasePropertyAccessorTest, Full) 
{
    auto obj = std::make_shared<BasePropertyAccessorTestObject>();
    obj->setObject(std::make_shared<BasePropertyAccessorTestObject>());
    std::unique_ptr<BasePropertyAccessorTestObjectPropertyAccessor> accessor = std::make_unique<BasePropertyAccessorTestObjectPropertyAccessor>(obj);
    EXPECT_EQ(accessor->readStringAtIndex(vcc::LockType::ReadLock, 0L), L"Test");
    EXPECT_EQ(accessor->readIntAtIndex(vcc::LockType::ReadLock, 0L), 1);
    EXPECT_TRUE(accessor->readObject(vcc::LockType::ReadLock, 0L) != nullptr);

    accessor->writeStringAtIndex(vcc::LockType::WriteLock, 0L, L"SetString");
    accessor->writeIntAtIndex(vcc::LockType::WriteLock, 0L, 2);
    EXPECT_EQ(accessor->readStringAtIndex(vcc::LockType::ReadLock, 0L), L"SetString");
    EXPECT_EQ(accessor->readIntAtIndex(vcc::LockType::ReadLock, 0L), 2);
    EXPECT_TRUE(accessor->readObject(vcc::LockType::ReadLock, 0L) != nullptr);

    std::shared_ptr<vcc::IObject> newObj = create();
    accessor->insertObjectAtIndex(vcc::LockType::NoLock, 0L, newObj);

    std::wstring mapKey1 = L"key";
    std::wstring mapKey2 = L"key";
    accessor->writeStringAtKey(vcc::LockType::WriteLock, 0L, L"value", &mapKey1);
    EXPECT_EQ(accessor->readStringAtKey(vcc::LockType::ReadLock, 0L, &mapKey2), L"value");
}
