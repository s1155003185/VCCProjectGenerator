#include <gtest/gtest.h>

#include <map>
#include <memory>
#include <set>

#include "base_object.hpp"
#include "class_macro.hpp"
#include "object_type.hpp"

using namespace std;
using namespace vcc;

class ClassMacroTestClassElement : public BaseObject<ClassMacroTestClassElement>
{
    GETSET(int, Index, 0);
    public:
        ClassMacroTestClassElement(int i) : BaseObject(ObjectType::NA) { this->_Index = i; }
        virtual ~ClassMacroTestClassElement() {}
};

class ClassMacroTestClass : public BaseObject<ClassMacroTestClass>
{
    GETSET(int, Number, 0)
    VECTOR(int, Vector)
    VECTOR_SPTR(ClassMacroTestClassElement, VectorSPTR)
    SET(int, Set)
    SET_SPTR(ClassMacroTestClassElement, SetSPTR)
    MAP(int, std::wstring, Map)
    MAP(string, vector<std::shared_ptr<ClassMacroTestClassElement>>, MapVector)
    MAP_SPTR_R(std::wstring, ClassMacroTestClassElement, MapSPTR)

    public:
        ClassMacroTestClass() : BaseObject(ObjectType::NA) {}
        virtual ~ClassMacroTestClass() {}
        
        virtual std::shared_ptr<IObject> Clone() const override {
            std::shared_ptr<ClassMacroTestClass> obj = make_shared<ClassMacroTestClass>(*this);
            obj->CloneVector(this->GetVector());
            obj->CloneVectorSPTR(this->GetVectorSPTR());
            obj->CloneSet(this->GetSet());
            obj->CloneSetSPTR(this->GetSetSPTR());
            obj->CloneMap(this->GetMap());
            obj->CloneMapVector(this->GetMapVector());
            obj->CloneMapSPTR(this->GetMapSPTR());
            return obj;
        };
};

TEST(ClassMacroTest, CloneSingle)
{
    int i = 10;
    unique_ptr<ClassMacroTestClassElement> testClass = make_unique<ClassMacroTestClassElement>(i);
    shared_ptr<ClassMacroTestClassElement> cloneClass = dynamic_pointer_cast<ClassMacroTestClassElement>(testClass->Clone());
    EXPECT_EQ(cloneClass->GetIndex(), i);
}

TEST(ClassMacroTest, Clone) 
{
    unique_ptr<ClassMacroTestClass> testClass = make_unique<ClassMacroTestClass>();
    testClass->SetNumber(1);

    shared_ptr<ClassMacroTestClass> cloneClass = dynamic_pointer_cast<ClassMacroTestClass>(testClass->Clone());
    EXPECT_EQ(testClass->GetNumber(), cloneClass->GetNumber());
}

TEST(ClassMacroTest, GETSET) 
{
    unique_ptr<ClassMacroTestClass> testClass = make_unique<ClassMacroTestClass>();
    testClass->SetNumber(1);
    EXPECT_EQ(testClass->GetNumber(), 1);
}

TEST(ClassMacroTest, Vector) 
{
    unique_ptr<ClassMacroTestClass> testClass = make_unique<ClassMacroTestClass>();
    testClass->InsertVector(1);
    testClass->InsertVector(2);
    testClass->InsertVector(3);
    EXPECT_TRUE(testClass->GetVector().size() == 3);
    
    testClass->InsertVector(2,4);
    EXPECT_TRUE(testClass->GetVector().size() == 4);
    EXPECT_EQ(testClass->GetVector().at(0), 1);
    EXPECT_EQ(testClass->GetVector().at(1), 2);
    EXPECT_EQ(testClass->GetVector().at(2), 4);
    EXPECT_EQ(testClass->GetVector().at(3), 3);

    vector<int> insertVector { 10, 11, 12 };
    testClass->InsertVector(0, insertVector);
    EXPECT_TRUE(testClass->GetVector().size() == 7);
    EXPECT_EQ(testClass->GetVector().at(0), 10);
    EXPECT_EQ(testClass->GetVector().at(1), 11);
    EXPECT_EQ(testClass->GetVector().at(2), 12);
    EXPECT_EQ(testClass->GetVector().at(3), 1);
    EXPECT_EQ(testClass->GetVector().at(4), 2);
    EXPECT_EQ(testClass->GetVector().at(5), 4);
    EXPECT_EQ(testClass->GetVector().at(6), 3);

    testClass->RemoveVector(1);
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
    testClass->InsertVectorSPTR(make_shared<ClassMacroTestClassElement>(1));
    testClass->InsertVectorSPTR(make_shared<ClassMacroTestClassElement>(2));
    testClass->InsertVectorSPTR(make_shared<ClassMacroTestClassElement>(3));
    EXPECT_TRUE(testClass->GetVectorSPTR().size() == 3);

    testClass->InsertVectorSPTR(2, make_shared<ClassMacroTestClassElement>(4));
    EXPECT_TRUE(testClass->GetVectorSPTR().size() == 4);
    EXPECT_EQ(testClass->GetVectorSPTR().at(0)->GetIndex(), 1);
    EXPECT_EQ(testClass->GetVectorSPTR().at(1)->GetIndex(), 2);
    EXPECT_EQ(testClass->GetVectorSPTR().at(2)->GetIndex(), 4);
    EXPECT_EQ(testClass->GetVectorSPTR().at(3)->GetIndex(), 3);

    vector<std::shared_ptr<ClassMacroTestClassElement>> insertVectorO { make_shared<ClassMacroTestClassElement>(10), make_shared<ClassMacroTestClassElement>(11), make_shared<ClassMacroTestClassElement>(12) };
    testClass->InsertVectorSPTR(0, insertVectorO);
    EXPECT_TRUE(testClass->GetVectorSPTR().size() == 7);
    EXPECT_EQ(testClass->GetVectorSPTR().at(0)->GetIndex(), 10);
    EXPECT_EQ(testClass->GetVectorSPTR().at(1)->GetIndex(), 11);
    EXPECT_EQ(testClass->GetVectorSPTR().at(2)->GetIndex(), 12);
    EXPECT_EQ(testClass->GetVectorSPTR().at(3)->GetIndex(), 1);
    EXPECT_EQ(testClass->GetVectorSPTR().at(4)->GetIndex(), 2);
    EXPECT_EQ(testClass->GetVectorSPTR().at(5)->GetIndex(), 4);
    EXPECT_EQ(testClass->GetVectorSPTR().at(6)->GetIndex(), 3);

    testClass->RemoveVectorSPTR(1);
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
    unique_ptr<ClassMacroTestClass> testClass = make_unique<ClassMacroTestClass>();
    testClass->InsertSet(1);
    testClass->InsertSet(2);
    testClass->InsertSet(3);
    EXPECT_TRUE(testClass->GetSet().size() == 3);

    set<int> insertSet { 1, 2, 4, 5 };
    testClass->InsertSet(insertSet);
    EXPECT_TRUE(testClass->GetSet().size() == 5);

    testClass->RemoveSet(1);
    EXPECT_TRUE(testClass->GetSet().size() == 4);

    testClass->ClearSet();
    EXPECT_TRUE(testClass->GetSet().size() == 0);
}

TEST(ClassMacroTest, Map) 
{
    unique_ptr<ClassMacroTestClass> testClass = make_unique<ClassMacroTestClass>();
    testClass->InsertMap(1, L"abc");
    testClass->InsertMap(2, L"abcd");
    EXPECT_EQ(testClass->GetMap()[1], L"abc");
    EXPECT_EQ(testClass->GetMap()[2], L"abcd");
}

TEST(ClassMacroTest, MapSPTR_R)
{   
    unique_ptr<ClassMacroTestClass> testClass = make_unique<ClassMacroTestClass>();
    testClass->InsertMapSPTR(L"1", make_shared<ClassMacroTestClassElement>(1));
    EXPECT_EQ(testClass->GetMapSPTR()[L"1"]->GetIndex(), 1);
}