#include <gtest/gtest.h>

#include <map>
#include <memory>
#include <set>

#include "base_object.hpp"
#include "class_macro.hpp"
#include "object_type.hpp"

class ClassMacroTestClassElement : public vcc::BaseObject
{
    GETSET(int, Index, 0);
    public:
        ClassMacroTestClassElement(int i) : vcc::BaseObject(ObjectType::NA) { this->_Index = i; }
        virtual ~ClassMacroTestClassElement() {}
        
    virtual std::shared_ptr<vcc::IObject> clone() const override
    {
        return std::make_shared<ClassMacroTestClassElement>(*this);
    }
};

class ClassMacroTestClass : public vcc::BaseObject
{
    // virtual
    private:
        mutable bool _Bool = false;
        mutable std::shared_ptr<ClassMacroTestClassElement> _TestObject = nullptr;
    GETCUSTOM(bool, CustomBool, return true;)
    SETCUSTOM(CustomBool, bool, _Bool = value;)
    GETCUSTOM_SPTR(ClassMacroTestClassElement, CustomObject, return _TestObject;)
    SETCUSTOM_SPTR(CustomObject, ClassMacroTestClassElement, _TestObject = value;)

    // general
    GETSET(int, Number, 0)
    GETSET_VALIDATE(int64_t, ValidateNumber, 0, if (value < 0) throw std::runtime_error("ValidateNumber: Value cannot be negative."); )
    
    GETSET_SPTR(ClassMacroTestClassElement, Object)
    GETSET_SPTR_NULL(ClassMacroTestClassElement, ObjectNull)
    GETSET_VALIDATE_SPTR_NULL(ClassMacroTestClassElement, ObjectNullValidate, if (value == nullptr) throw std::runtime_error("ObjectNullValidate: Value cannot be nullptr.");)

    // vector
    VECTOR(int, Vector)
    VECTOR_VALIDATE(int64_t, VectorValidate, if (value < 0) throw std::runtime_error("VectorValidate: Value cannot be negative.");)
    VECTOR_SPTR(ClassMacroTestClassElement, VectorSPTR)
    VECTOR_VALIDATE_SPTR(ClassMacroTestClassElement, VectorSPTRValidate, if (value == nullptr) throw std::runtime_error("ObjectNullValidate: Value cannot be nullptr."););
    
    // Set
    SET(int, Set)
    SET_VALIDATE(int, SetValidate, if (value < 0) throw std::runtime_error("VectorValidate: Value cannot be negative."););
    SET_SPTR(ClassMacroTestClassElement, SetSPTR)
    SET_VALIDATE_SPTR(ClassMacroTestClassElement, SetSPTRValidate, if (value == nullptr) throw std::runtime_error("VectorValidate: Value cannot be nullptr."););
    
    // map
    MAP(int, std::wstring, Map)
    MAP(std::string, std::vector<std::shared_ptr<ClassMacroTestClassElement>>, MapVector)
    MAP_VALIDATE(int, std::wstring, MapValidate, if (value.empty()) throw std::runtime_error("VectorValidate: Value cannot be empty string.");)
    MAP_SPTR_R(std::wstring, ClassMacroTestClassElement, MapSPTR)
    MAP_VALIDATE_SPTR_R(std::wstring, ClassMacroTestClassElement, MapSPTRValidate, if (value == nullptr) throw std::runtime_error("VectorValidate: Value cannot be nullptr.");)
    
    // ordered map
    ORDERED_MAP(int, std::wstring, OrderedMap)
    ORDERED_MAP_VALIDATE(int, std::wstring, OrderedMapValidate, if (value.empty()) throw std::runtime_error("VectorValidate: Value cannot be empty string.");)
    ORDERED_MAP_SPTR_R(std::wstring, ClassMacroTestClassElement, OrderedMapSPTR)
    ORDERED_MAP_VALIDATE_SPTR_R(std::wstring, ClassMacroTestClassElement, OrderedMapSPTRValidate, if (value == nullptr) throw std::runtime_error("VectorValidate: Value cannot be nullptr.");)

    public:
        ClassMacroTestClass() : vcc::BaseObject(ObjectType::NA) {}
        virtual ~ClassMacroTestClass() {}
        
        virtual std::shared_ptr<vcc::IObject> clone() const override
        {
            auto obj = std::make_shared<ClassMacroTestClass>(*this);
            obj->cloneVector(this->getVector());
            obj->cloneVectorSPTR(this->getVectorSPTR());
            obj->cloneSet(this->getSet());
            obj->cloneSetSPTR(this->getSetSPTR());
            obj->cloneMap(this->getMap());
            obj->cloneMapVector(this->getMapVector());
            obj->cloneMapSPTR(this->getMapSPTR());
            obj->cloneOrderedMap(this->getOrderedMap());
            obj->cloneOrderedMapSPTR(this->getOrderedMapSPTR());
            return obj;
        };
};

TEST(ClassMacroTest, cloneSingle)
{
    int i = 10;
    auto testClass = std::make_unique<ClassMacroTestClassElement>(i);
    auto cloneClass = std::dynamic_pointer_cast<ClassMacroTestClassElement>(testClass->clone());
    EXPECT_EQ(cloneClass->getIndex(), i);
}

TEST(ClassMacroTest, clone) 
{
    std::unique_ptr<ClassMacroTestClass> testClass = std::make_unique<ClassMacroTestClass>();
    testClass->setNumber(1);

    auto cloneClass = std::dynamic_pointer_cast<ClassMacroTestClass>(testClass->clone());
    EXPECT_EQ(testClass->getNumber(), cloneClass->getNumber());
}

TEST(ClassMacroTest, GETSET) 
{
    std::unique_ptr<ClassMacroTestClass> testClass = std::make_unique<ClassMacroTestClass>();
    testClass->setNumber(1);
    EXPECT_EQ(testClass->getNumber(), 1);
}

TEST(ClassMacroTest, Vector) 
{
    std::unique_ptr<ClassMacroTestClass> testClass = std::make_unique<ClassMacroTestClass>();
    testClass->insertVector(1);
    testClass->insertVector(2);
    testClass->insertVector(3);
    EXPECT_TRUE(testClass->getVector().size() == 3);
    
    testClass->insertVectorAtIndex(2,4);
    EXPECT_TRUE(testClass->getVector().size() == 4);
    EXPECT_EQ(testClass->getVector().at(0), 1);
    EXPECT_EQ(testClass->getVector().at(1), 2);
    EXPECT_EQ(testClass->getVector().at(2), 4);
    EXPECT_EQ(testClass->getVector().at(3), 3);

    std::vector<int> insertVector { 10, 11, 12 };
    testClass->insertVectorAtIndex(0, insertVector);
    EXPECT_TRUE(testClass->getVector().size() == 7);
    EXPECT_EQ(testClass->getVector().at(0), 10);
    EXPECT_EQ(testClass->getVector().at(1), 11);
    EXPECT_EQ(testClass->getVector().at(2), 12);
    EXPECT_EQ(testClass->getVector().at(3), 1);
    EXPECT_EQ(testClass->getVector().at(4), 2);
    EXPECT_EQ(testClass->getVector().at(5), 4);
    EXPECT_EQ(testClass->getVector().at(6), 3);

    testClass->removeVectorAtIndex(1);
    EXPECT_TRUE(testClass->getVector().size() == 6);
    EXPECT_EQ(testClass->getVector().at(0), 10);
    EXPECT_EQ(testClass->getVector().at(1), 12);
    EXPECT_EQ(testClass->getVector().at(2), 1);
    EXPECT_EQ(testClass->getVector().at(3), 2);
    EXPECT_EQ(testClass->getVector().at(4), 4);
    EXPECT_EQ(testClass->getVector().at(5), 3);

    testClass->clearVector();
    EXPECT_TRUE(testClass->getVector().size() == 0);

    // Object
    testClass->insertVectorSPTR(std::make_shared<ClassMacroTestClassElement>(1));
    testClass->insertVectorSPTR(std::make_shared<ClassMacroTestClassElement>(2));
    testClass->insertVectorSPTR(std::make_shared<ClassMacroTestClassElement>(3));
    EXPECT_TRUE(testClass->getVectorSPTR().size() == 3);

    testClass->insertVectorSPTRAtIndex(2, std::make_shared<ClassMacroTestClassElement>(4));
    EXPECT_TRUE(testClass->getVectorSPTR().size() == 4);
    EXPECT_EQ(testClass->getVectorSPTR().at(0)->getIndex(), 1);
    EXPECT_EQ(testClass->getVectorSPTR().at(1)->getIndex(), 2);
    EXPECT_EQ(testClass->getVectorSPTR().at(2)->getIndex(), 4);
    EXPECT_EQ(testClass->getVectorSPTR().at(3)->getIndex(), 3);

    std::vector<std::shared_ptr<ClassMacroTestClassElement>> insertVectorO { std::make_shared<ClassMacroTestClassElement>(10), std::make_shared<ClassMacroTestClassElement>(11), std::make_shared<ClassMacroTestClassElement>(12) };
    testClass->insertVectorSPTRAtIndex(0, insertVectorO);
    EXPECT_TRUE(testClass->getVectorSPTR().size() == 7);
    EXPECT_EQ(testClass->getVectorSPTR().at(0)->getIndex(), 10);
    EXPECT_EQ(testClass->getVectorSPTR().at(1)->getIndex(), 11);
    EXPECT_EQ(testClass->getVectorSPTR().at(2)->getIndex(), 12);
    EXPECT_EQ(testClass->getVectorSPTR().at(3)->getIndex(), 1);
    EXPECT_EQ(testClass->getVectorSPTR().at(4)->getIndex(), 2);
    EXPECT_EQ(testClass->getVectorSPTR().at(5)->getIndex(), 4);
    EXPECT_EQ(testClass->getVectorSPTR().at(6)->getIndex(), 3);

    testClass->removeVectorSPTRAtIndex(1);
    EXPECT_TRUE(testClass->getVectorSPTR().size() == 6);
    EXPECT_EQ(testClass->getVectorSPTR().at(0)->getIndex(), 10);
    EXPECT_EQ(testClass->getVectorSPTR().at(1)->getIndex(), 12);
    EXPECT_EQ(testClass->getVectorSPTR().at(2)->getIndex(), 1);
    EXPECT_EQ(testClass->getVectorSPTR().at(3)->getIndex(), 2);
    EXPECT_EQ(testClass->getVectorSPTR().at(4)->getIndex(), 4);
    EXPECT_EQ(testClass->getVectorSPTR().at(5)->getIndex(), 3);

    testClass->clearVectorSPTR();
    EXPECT_TRUE(testClass->getVectorSPTR().size() == 0);
}

TEST(ClassMacroTest, set) 
{
    std::unique_ptr<ClassMacroTestClass> testClass = std::make_unique<ClassMacroTestClass>();
    testClass->insertSet(1);
    testClass->insertSet(2);
    testClass->insertSet(3);
    EXPECT_TRUE(testClass->getSet().size() == 3);

    std::set<int> insertSet { 1, 2, 4, 5 };
    testClass->insertSet(insertSet);
    EXPECT_TRUE(testClass->getSet().size() == 5);

    testClass->removeSet(1);
    EXPECT_TRUE(testClass->getSet().size() == 4);

    testClass->clearSet();
    EXPECT_TRUE(testClass->getSet().size() == 0);
}

TEST(ClassMacroTest, Map) 
{
    std::unique_ptr<ClassMacroTestClass> testClass = std::make_unique<ClassMacroTestClass>();
    testClass->insertMapAtKey(1, L"abc");
    testClass->insertMapAtKey(2, L"abcd");
    EXPECT_EQ(testClass->getMap()[1], L"abc");
    EXPECT_EQ(testClass->getMap()[2], L"abcd");
}

TEST(ClassMacroTest, MapSPTR_R)
{   
    std::unique_ptr<ClassMacroTestClass> testClass = std::make_unique<ClassMacroTestClass>();
    testClass->insertMapSPTRAtKey(L"1", std::make_shared<ClassMacroTestClassElement>(1));
    EXPECT_EQ(testClass->getMapSPTR()[L"1"]->getIndex(), 1);
}

TEST(ClassMacroTest, OrderedMap) 
{
    std::unique_ptr<ClassMacroTestClass> testClass = std::make_unique<ClassMacroTestClass>();
    testClass->insertOrderedMapAtKey(1, L"abc");
    testClass->insertOrderedMapAtKey(2, L"abcd");
    EXPECT_EQ(testClass->getOrderedMap()[0].first, 1);
    EXPECT_EQ(testClass->getOrderedMap()[0].second, L"abc");
    EXPECT_EQ(testClass->getOrderedMap()[1].first, 2);
    EXPECT_EQ(testClass->getOrderedMap()[1].second, L"abcd");
}

TEST(ClassMacroTest, OrderedMapSPTR_R)
{   
    std::unique_ptr<ClassMacroTestClass> testClass = std::make_unique<ClassMacroTestClass>();
    testClass->insertOrderedMapSPTRAtKey(L"1", std::make_shared<ClassMacroTestClassElement>(1));
    EXPECT_EQ(testClass->getOrderedMapSPTR().at(0).second->getIndex(), 1);
}