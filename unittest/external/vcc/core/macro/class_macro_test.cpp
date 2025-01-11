#include <gtest/gtest.h>

#include <map>
#include <memory>
#include <set>

#include "base_object.hpp"
#include "class_macro.hpp"
#include "object_type.hpp"

using namespace vcc;

class ClassMacroTestClassElement : public BaseObject
{
    GETSET(int, Index, 0);
    public:
        ClassMacroTestClassElement(int i) : BaseObject(ObjectType::NA) { this->_Index = i; }
        virtual ~ClassMacroTestClassElement() {}
        
    virtual std::shared_ptr<IObject> Clone() const override
    {
        return std::make_shared<ClassMacroTestClassElement>(*this);
    }
};

class ClassMacroTestClass : public BaseObject
{
    GETSET(int, Number, 0)
    VECTOR(int, Vector)
    VECTOR_SPTR(ClassMacroTestClassElement, VectorSPTR)
    SET(int, Set)
    SET_SPTR(ClassMacroTestClassElement, SetSPTR)
    MAP(int, std::wstring, Map)
    MAP(std::string, std::vector<std::shared_ptr<ClassMacroTestClassElement>>, MapVector)
    MAP_SPTR_R(std::wstring, ClassMacroTestClassElement, MapSPTR)
    ORDERED_MAP(int, std::wstring, OrderedMap)
    ORDERED_MAP_SPTR_R(std::wstring, ClassMacroTestClassElement, OrderedMapSPTR)


    public:
        ClassMacroTestClass() : BaseObject(ObjectType::NA) {}
        virtual ~ClassMacroTestClass() {}
        
        virtual std::shared_ptr<IObject> Clone() const override
        {
            auto obj = std::make_shared<ClassMacroTestClass>(*this);
            obj->CloneVector(this->GetVector());
            obj->CloneVectorSPTR(this->GetVectorSPTR());
            obj->CloneSet(this->GetSet());
            obj->CloneSetSPTR(this->GetSetSPTR());
            obj->CloneMap(this->GetMap());
            obj->CloneMapVector(this->GetMapVector());
            obj->CloneMapSPTR(this->GetMapSPTR());
            obj->CloneOrderedMap(this->GetOrderedMap());
            obj->CloneOrderedMapSPTR(this->GetOrderedMapSPTR());
            return obj;
        };
};

TEST(ClassMacroTest, CloneSingle)
{
    int i = 10;
    auto testClass = std::make_unique<ClassMacroTestClassElement>(i);
    auto cloneClass = std::dynamic_pointer_cast<ClassMacroTestClassElement>(testClass->Clone());
    EXPECT_EQ(cloneClass->GetIndex(), i);
}

TEST(ClassMacroTest, Clone) 
{
    std::unique_ptr<ClassMacroTestClass> testClass = std::make_unique<ClassMacroTestClass>();
    testClass->SetNumber(1);

    auto cloneClass = std::dynamic_pointer_cast<ClassMacroTestClass>(testClass->Clone());
    EXPECT_EQ(testClass->GetNumber(), cloneClass->GetNumber());
}

TEST(ClassMacroTest, GETSET) 
{
    std::unique_ptr<ClassMacroTestClass> testClass = std::make_unique<ClassMacroTestClass>();
    testClass->SetNumber(1);
    EXPECT_EQ(testClass->GetNumber(), 1);
}

TEST(ClassMacroTest, Vector) 
{
    std::unique_ptr<ClassMacroTestClass> testClass = std::make_unique<ClassMacroTestClass>();
    testClass->InsertVector(1);
    testClass->InsertVector(2);
    testClass->InsertVector(3);
    EXPECT_TRUE(testClass->GetVector().size() == 3);
    
    testClass->InsertVectorAtIndex(2,4);
    EXPECT_TRUE(testClass->GetVector().size() == 4);
    EXPECT_EQ(testClass->GetVector().at(0), 1);
    EXPECT_EQ(testClass->GetVector().at(1), 2);
    EXPECT_EQ(testClass->GetVector().at(2), 4);
    EXPECT_EQ(testClass->GetVector().at(3), 3);

    std::vector<int> insertVector { 10, 11, 12 };
    testClass->InsertVectorAtIndex(0, insertVector);
    EXPECT_TRUE(testClass->GetVector().size() == 7);
    EXPECT_EQ(testClass->GetVector().at(0), 10);
    EXPECT_EQ(testClass->GetVector().at(1), 11);
    EXPECT_EQ(testClass->GetVector().at(2), 12);
    EXPECT_EQ(testClass->GetVector().at(3), 1);
    EXPECT_EQ(testClass->GetVector().at(4), 2);
    EXPECT_EQ(testClass->GetVector().at(5), 4);
    EXPECT_EQ(testClass->GetVector().at(6), 3);

    testClass->RemoveVectorAtIndex(1);
    EXPECT_TRUE(testClass->GetVector().size() == 6);
    EXPECT_EQ(testClass->GetVector().at(0), 10);
    EXPECT_EQ(testClass->GetVector().at(1), 12);
    EXPECT_EQ(testClass->GetVector().at(2), 1);
    EXPECT_EQ(testClass->GetVector().at(3), 2);
    EXPECT_EQ(testClass->GetVector().at(4), 4);
    EXPECT_EQ(testClass->GetVector().at(5), 3);

    testClass->ClearVector();
    EXPECT_TRUE(testClass->GetVector().size() == 0);

    // Object
    testClass->InsertVectorSPTR(std::make_shared<ClassMacroTestClassElement>(1));
    testClass->InsertVectorSPTR(std::make_shared<ClassMacroTestClassElement>(2));
    testClass->InsertVectorSPTR(std::make_shared<ClassMacroTestClassElement>(3));
    EXPECT_TRUE(testClass->GetVectorSPTR().size() == 3);

    testClass->InsertVectorSPTRAtIndex(2, std::make_shared<ClassMacroTestClassElement>(4));
    EXPECT_TRUE(testClass->GetVectorSPTR().size() == 4);
    EXPECT_EQ(testClass->GetVectorSPTR().at(0)->GetIndex(), 1);
    EXPECT_EQ(testClass->GetVectorSPTR().at(1)->GetIndex(), 2);
    EXPECT_EQ(testClass->GetVectorSPTR().at(2)->GetIndex(), 4);
    EXPECT_EQ(testClass->GetVectorSPTR().at(3)->GetIndex(), 3);

    std::vector<std::shared_ptr<ClassMacroTestClassElement>> insertVectorO { std::make_shared<ClassMacroTestClassElement>(10), std::make_shared<ClassMacroTestClassElement>(11), std::make_shared<ClassMacroTestClassElement>(12) };
    testClass->InsertVectorSPTRAtIndex(0, insertVectorO);
    EXPECT_TRUE(testClass->GetVectorSPTR().size() == 7);
    EXPECT_EQ(testClass->GetVectorSPTR().at(0)->GetIndex(), 10);
    EXPECT_EQ(testClass->GetVectorSPTR().at(1)->GetIndex(), 11);
    EXPECT_EQ(testClass->GetVectorSPTR().at(2)->GetIndex(), 12);
    EXPECT_EQ(testClass->GetVectorSPTR().at(3)->GetIndex(), 1);
    EXPECT_EQ(testClass->GetVectorSPTR().at(4)->GetIndex(), 2);
    EXPECT_EQ(testClass->GetVectorSPTR().at(5)->GetIndex(), 4);
    EXPECT_EQ(testClass->GetVectorSPTR().at(6)->GetIndex(), 3);

    testClass->RemoveVectorSPTRAtIndex(1);
    EXPECT_TRUE(testClass->GetVectorSPTR().size() == 6);
    EXPECT_EQ(testClass->GetVectorSPTR().at(0)->GetIndex(), 10);
    EXPECT_EQ(testClass->GetVectorSPTR().at(1)->GetIndex(), 12);
    EXPECT_EQ(testClass->GetVectorSPTR().at(2)->GetIndex(), 1);
    EXPECT_EQ(testClass->GetVectorSPTR().at(3)->GetIndex(), 2);
    EXPECT_EQ(testClass->GetVectorSPTR().at(4)->GetIndex(), 4);
    EXPECT_EQ(testClass->GetVectorSPTR().at(5)->GetIndex(), 3);

    testClass->ClearVectorSPTR();
    EXPECT_TRUE(testClass->GetVectorSPTR().size() == 0);
}

TEST(ClassMacroTest, Set) 
{
    std::unique_ptr<ClassMacroTestClass> testClass = std::make_unique<ClassMacroTestClass>();
    testClass->InsertSet(1);
    testClass->InsertSet(2);
    testClass->InsertSet(3);
    EXPECT_TRUE(testClass->GetSet().size() == 3);

    std::set<int> insertSet { 1, 2, 4, 5 };
    testClass->InsertSet(insertSet);
    EXPECT_TRUE(testClass->GetSet().size() == 5);

    testClass->RemoveSet(1);
    EXPECT_TRUE(testClass->GetSet().size() == 4);

    testClass->ClearSet();
    EXPECT_TRUE(testClass->GetSet().size() == 0);
}

TEST(ClassMacroTest, Map) 
{
    std::unique_ptr<ClassMacroTestClass> testClass = std::make_unique<ClassMacroTestClass>();
    testClass->InsertMapAtKey(1, L"abc");
    testClass->InsertMapAtKey(2, L"abcd");
    EXPECT_EQ(testClass->GetMap()[1], L"abc");
    EXPECT_EQ(testClass->GetMap()[2], L"abcd");
}

TEST(ClassMacroTest, MapSPTR_R)
{   
    std::unique_ptr<ClassMacroTestClass> testClass = std::make_unique<ClassMacroTestClass>();
    testClass->InsertMapSPTRAtKey(L"1", std::make_shared<ClassMacroTestClassElement>(1));
    EXPECT_EQ(testClass->GetMapSPTR()[L"1"]->GetIndex(), 1);
}

TEST(ClassMacroTest, OrderedMap) 
{
    std::unique_ptr<ClassMacroTestClass> testClass = std::make_unique<ClassMacroTestClass>();
    testClass->InsertOrderedMapAtKey(1, L"abc");
    testClass->InsertOrderedMapAtKey(2, L"abcd");
    EXPECT_EQ(testClass->GetOrderedMap()[0].first, 1);
    EXPECT_EQ(testClass->GetOrderedMap()[0].second, L"abc");
    EXPECT_EQ(testClass->GetOrderedMap()[1].first, 2);
    EXPECT_EQ(testClass->GetOrderedMap()[1].second, L"abcd");
}

TEST(ClassMacroTest, OrderedMapSPTR_R)
{   
    std::unique_ptr<ClassMacroTestClass> testClass = std::make_unique<ClassMacroTestClass>();
    testClass->InsertOrderedMapSPTRAtKey(L"1", std::make_shared<ClassMacroTestClassElement>(1));
    EXPECT_EQ(testClass->GetOrderedMapSPTR().at(0).second->GetIndex(), 1);
}