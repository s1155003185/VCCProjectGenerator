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
    GETSET(int, N, 0)
    VECTOR(int, V)
    VECTOR_SPTR(ClassMacroTestClassElement, VO)
    SET(int, S)
    SET_SPTR(ClassMacroTestClassElement, SO)
    MAP(int, std::wstring, M)
    MAP(string, vector<std::shared_ptr<ClassMacroTestClassElement>>, MO)

    public:
        ClassMacroTestClass() : BaseObject(ObjectType::NA) {}
        virtual ~ClassMacroTestClass() {}
        
        virtual std::shared_ptr<ClassMacroTestClass> Clone() override {
            std::shared_ptr<ClassMacroTestClass> obj = make_shared<ClassMacroTestClass>(*this);
            obj->CloneV(this->GetV());
            obj->CloneVO(this->GetVO());
            obj->CloneS(this->GetS());
            obj->CloneSO(this->GetSO());
            obj->CloneM(this->GetM());

            // TODO: Handle Map
            obj->CloneMO(this->GetMO());
            return obj;
        };
};

TEST(ClassMacroTest, CloneSingle)
{
    int i = 10;
    unique_ptr<ClassMacroTestClassElement> testClass = make_unique<ClassMacroTestClassElement>(i);
    shared_ptr<ClassMacroTestClassElement> cloneClass = testClass->Clone();
    EXPECT_EQ(cloneClass->GetIndex(), i);
}

TEST(ClassMacroTest, Clone) 
{
    unique_ptr<ClassMacroTestClass> testClass = make_unique<ClassMacroTestClass>();
    testClass->SetN(1);

    shared_ptr<ClassMacroTestClass> cloneClass = testClass->Clone();
    EXPECT_EQ(testClass->GetN(), cloneClass->GetN());
}

TEST(ClassMacroTest, GETSET) 
{
    unique_ptr<ClassMacroTestClass> testClass = make_unique<ClassMacroTestClass>();
    testClass->SetN(1);
    EXPECT_EQ(testClass->GetN(), 1);
}

TEST(ClassMacroTest, Vector) 
{
    unique_ptr<ClassMacroTestClass> testClass = make_unique<ClassMacroTestClass>();
    testClass->InsertV(1);
    testClass->InsertV(2);
    testClass->InsertV(3);
    EXPECT_TRUE(testClass->GetV().size() == 3);
    
    testClass->InsertV(2,4);
    EXPECT_TRUE(testClass->GetV().size() == 4);
    EXPECT_EQ(testClass->GetV().at(0), 1);
    EXPECT_EQ(testClass->GetV().at(1), 2);
    EXPECT_EQ(testClass->GetV().at(2), 4);
    EXPECT_EQ(testClass->GetV().at(3), 3);

    vector<int> insertVector { 10, 11, 12 };
    testClass->InsertV(0, insertVector);
    EXPECT_TRUE(testClass->GetV().size() == 7);
    EXPECT_EQ(testClass->GetV().at(0), 10);
    EXPECT_EQ(testClass->GetV().at(1), 11);
    EXPECT_EQ(testClass->GetV().at(2), 12);
    EXPECT_EQ(testClass->GetV().at(3), 1);
    EXPECT_EQ(testClass->GetV().at(4), 2);
    EXPECT_EQ(testClass->GetV().at(5), 4);
    EXPECT_EQ(testClass->GetV().at(6), 3);

    testClass->RemoveV(1);
    EXPECT_TRUE(testClass->GetV().size() == 6);
    EXPECT_EQ(testClass->GetV().at(0), 10);
    EXPECT_EQ(testClass->GetV().at(1), 12);
    EXPECT_EQ(testClass->GetV().at(2), 1);
    EXPECT_EQ(testClass->GetV().at(3), 2);
    EXPECT_EQ(testClass->GetV().at(4), 4);
    EXPECT_EQ(testClass->GetV().at(5), 3);

    testClass->ClearV();
    EXPECT_TRUE(testClass->GetV().size() == 0);

    // Object
    testClass->InsertVO(make_shared<ClassMacroTestClassElement>(1));
    testClass->InsertVO(make_shared<ClassMacroTestClassElement>(2));
    testClass->InsertVO(make_shared<ClassMacroTestClassElement>(3));
    EXPECT_TRUE(testClass->GetVO().size() == 3);

    testClass->InsertVO(2, make_shared<ClassMacroTestClassElement>(4));
    EXPECT_TRUE(testClass->GetVO().size() == 4);
    EXPECT_EQ(testClass->GetVO().at(0)->GetIndex(), 1);
    EXPECT_EQ(testClass->GetVO().at(1)->GetIndex(), 2);
    EXPECT_EQ(testClass->GetVO().at(2)->GetIndex(), 4);
    EXPECT_EQ(testClass->GetVO().at(3)->GetIndex(), 3);

    vector<std::shared_ptr<ClassMacroTestClassElement>> insertVectorO { make_shared<ClassMacroTestClassElement>(10), make_shared<ClassMacroTestClassElement>(11), make_shared<ClassMacroTestClassElement>(12) };
    testClass->InsertVO(0, insertVectorO);
    EXPECT_TRUE(testClass->GetVO().size() == 7);
    EXPECT_EQ(testClass->GetVO().at(0)->GetIndex(), 10);
    EXPECT_EQ(testClass->GetVO().at(1)->GetIndex(), 11);
    EXPECT_EQ(testClass->GetVO().at(2)->GetIndex(), 12);
    EXPECT_EQ(testClass->GetVO().at(3)->GetIndex(), 1);
    EXPECT_EQ(testClass->GetVO().at(4)->GetIndex(), 2);
    EXPECT_EQ(testClass->GetVO().at(5)->GetIndex(), 4);
    EXPECT_EQ(testClass->GetVO().at(6)->GetIndex(), 3);

    testClass->RemoveVO(1);
    EXPECT_TRUE(testClass->GetVO().size() == 6);
    EXPECT_EQ(testClass->GetVO().at(0)->GetIndex(), 10);
    EXPECT_EQ(testClass->GetVO().at(1)->GetIndex(), 12);
    EXPECT_EQ(testClass->GetVO().at(2)->GetIndex(), 1);
    EXPECT_EQ(testClass->GetVO().at(3)->GetIndex(), 2);
    EXPECT_EQ(testClass->GetVO().at(4)->GetIndex(), 4);
    EXPECT_EQ(testClass->GetVO().at(5)->GetIndex(), 3);

    testClass->ClearVO();
    EXPECT_TRUE(testClass->GetVO().size() == 0);
}

TEST(ClassMacroTest, Set) 
{
    unique_ptr<ClassMacroTestClass> testClass = make_unique<ClassMacroTestClass>();
    testClass->InsertS(1);
    testClass->InsertS(2);
    testClass->InsertS(3);
    EXPECT_TRUE(testClass->GetS().size() == 3);

    set<int> insertSet { 1, 2, 4, 5 };
    testClass->InsertS(insertSet);
    EXPECT_TRUE(testClass->GetS().size() == 5);

    testClass->RemoveS(1);
    EXPECT_TRUE(testClass->GetS().size() == 4);

    testClass->ClearS();
    EXPECT_TRUE(testClass->GetS().size() == 0);
}

// TEST(ClassMacroTest, Map) 
// {
//     unique_ptr<ClassMacroTestClass> testClass = make_unique<ClassMacroTestClass>();
// }