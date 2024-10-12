#include <gtest/gtest.h>

#include <filesystem>
#include <string>
#include <vector>

#include "class_macro.hpp"
#include "file_helper.hpp"

#include "vpg_object_factory_file_generation_service.hpp"

using namespace vcc;

class VPGObjectFactoryFileGenerationServiceTest : public testing::Test 
{
    GETSET_SPTR(LogConfig, LogConfig);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGObjectFactoryFileGenerationServiceTest/");
    
    GETSET(std::wstring, FilePathHpp, L"");
    GETSET(std::wstring, FilePathCpp, L"");

    GETSET(std::wstring, ExpectedHpp, L"");

    public:
        void SetUp() override
        {
            this->_LogConfig->SetIsConsoleLog(false);
            std::filesystem::remove_all(PATH(this->GetWorkspace()));

            this->_FilePathHpp = ConcatPaths({this->GetWorkspace(), L"object_factory.hpp"});
            this->_FilePathCpp = ConcatPaths({this->GetWorkspace(), L"object_factory.cpp"});

            this->_ExpectedHpp = L""
                "#pragma once\r\n"
                "\r\n"
                "#include <memory>\r\n"
                "\r\n"
                "#include \"base_factory.hpp\"\r\n"
                "#include \"i_object.hpp\"\r\n"
                "#include \"object_type.hpp\"\r\n"
                "\r\n"
                "using namespace vcc;\r\n"
                "\r\n"
                "class ObjectFactory : public BaseFactory\r\n"
                "{\r\n"
                "    private:\r\n"
                "        ObjectFactory() = default;\r\n"
                "        virtual ~ObjectFactory() {}\r\n"
                "\r\n"
                "    public:\r\n"
                "        static std::shared_ptr<IObject> Create(const ObjectType &objectType);\r\n"
                "};\r\n";
        }

        void TearDown() override
        {
            std::filesystem::remove_all(PATH(this->GetWorkspace()));
        }
};

TEST_F(VPGObjectFactoryFileGenerationServiceTest, Empty)
{
    std::set<std::wstring> propertyTypes;
    VPGObjectFactoryFileGenerationService::GenerateHpp(this->GetLogConfig().get(), this->GetFilePathHpp());
    VPGObjectFactoryFileGenerationService::GenerateCpp(this->GetLogConfig().get(), L"VCC", {}, this->GetFilePathCpp(), propertyTypes);

    EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));
    EXPECT_TRUE(IsFileExists(this->GetFilePathCpp()));

    EXPECT_EQ(ReadFile(this->GetFilePathHpp()), this->GetExpectedHpp());

    std::wstring expectedResult = L""
        "#include \"object_factory.hpp\"\r\n"
        "\r\n"
        "#include <assert.h>\r\n"
        "#include <memory>\r\n"
        "\r\n"
        "#include \"i_object.hpp\"\r\n"
        "#include \"object_type.hpp\"\r\n"
        "\r\n"
        "using namespace vcc;\r\n"
        "\r\n"
        "std::shared_ptr<IObject> ObjectFactory::Create(const ObjectType &objectType)\r\n"
        "{\r\n"
        "    switch (objectType)\r\n"
        "    {\r\n"
        "    default:\r\n"
        "        assert(false);\r\n"
        "        break;\r\n"
        "    }\r\n"
        "    return nullptr;\r\n"
        "}\r\n";
    EXPECT_EQ(ReadFile(this->GetFilePathCpp()), expectedResult);
}

TEST_F(VPGObjectFactoryFileGenerationServiceTest, Normal)
{
    std::set<std::wstring> propertyTypes;
    propertyTypes.insert(L"Def");
    propertyTypes.insert(L"Abc");
    VPGObjectFactoryFileGenerationService::GenerateHpp(this->GetLogConfig().get(), this->GetFilePathHpp());
    VPGObjectFactoryFileGenerationService::GenerateCpp(this->GetLogConfig().get(), L"VCC", { L"abc.hpp" }, this->GetFilePathCpp(), propertyTypes);

    EXPECT_TRUE(IsFileExists(this->GetFilePathHpp()));
    EXPECT_TRUE(IsFileExists(this->GetFilePathCpp()));

    EXPECT_EQ(ReadFile(this->GetFilePathHpp()), this->GetExpectedHpp());

    std::wstring expectedResult = L""
        "#include \"object_factory.hpp\"\r\n"
        "\r\n"
        "#include <assert.h>\r\n"
        "#include <memory>\r\n"
        "\r\n"
        "#include \"abc.hpp\"\r\n"
        "#include \"i_object.hpp\"\r\n"
        "#include \"object_type.hpp\"\r\n"
        "\r\n"
        "using namespace vcc;\r\n"
        "\r\n"
        "std::shared_ptr<IObject> ObjectFactory::Create(const ObjectType &objectType)\r\n"
        "{\r\n"
        "    switch (objectType)\r\n"
        "    {\r\n"
        "    case ObjectType::Abc:\r\n"
        "        return std::make_shared<VCCAbc>();\r\n"
        "    case ObjectType::Def:\r\n"
        "        return std::make_shared<VCCDef>();\r\n"
        "    default:\r\n"
        "        assert(false);\r\n"
        "        break;\r\n"
        "    }\r\n"
        "    return nullptr;\r\n"
        "}\r\n";
    EXPECT_EQ(ReadFile(this->GetFilePathCpp()), expectedResult);
}