#include <gtest/gtest.h>

#include <filesystem>
#include <string>
#include <vector>

#include "class_macro.hpp"
#include "file_helper.hpp"

#include "vpg_object_factory_file_generation_service.hpp"

class VPGObjectFactoryFileGenerationServiceTest : public testing::Test 
{
    GETSET_SPTR_NULL(vcc::LogConfig, LogConfig);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGObjectFactoryFileGenerationServiceTest/");
    
    GETSET(std::wstring, FilePathHpp, L"");
    GETSET(std::wstring, FilePathCpp, L"");

    GETSET(std::wstring, ExpectedHpp, L"");

    public:
        void SetUp() override
        {
            this->_LogConfig = std::make_shared<vcc::LogConfig>();
            this->_LogConfig->setIsConsoleLog(false);
            std::filesystem::remove_all(PATH(this->getWorkspace()));

            this->_FilePathHpp = vcc::concatPaths({this->getWorkspace(), L"object_factory.hpp"});
            this->_FilePathCpp = vcc::concatPaths({this->getWorkspace(), L"object_factory.cpp"});

            this->_ExpectedHpp = L""
                "#pragma once\r\n"
                "\r\n"
                "#include <memory>\r\n"
                "\r\n"
                "#include \"base_factory.hpp\"\r\n"
                "#include \"i_object.hpp\"\r\n"
                "#include \"object_type.hpp\"\r\n"
                "\r\n"
                "class ObjectFactory : public vcc::BaseFactory\r\n"
                "{\r\n"
                "    private:\r\n"
                "        ObjectFactory() = default;\r\n"
                "        virtual ~ObjectFactory() {}\r\n"
                "\r\n"
                "    public:\r\n"
                "        static std::shared_ptr<vcc::IObject> create(const ObjectType &objectType, std::shared_ptr<vcc::IObject> parentObject = nullptr);\r\n"
                "};\r\n";
        }

        void TearDown() override
        {
            std::filesystem::remove_all(PATH(this->getWorkspace()));
        }
};

TEST_F(VPGObjectFactoryFileGenerationServiceTest, Empty)
{
    std::set<std::wstring> propertyTypes;
    VPGObjectFactoryFileGenerationService::GenerateHpp(this->getLogConfig().get(), this->getFilePathHpp());
    VPGObjectFactoryFileGenerationService::GenerateCpp(this->getLogConfig().get(), L"VCC", {}, this->getFilePathCpp(), propertyTypes);

    EXPECT_TRUE(vcc::isFilePresent(this->getFilePathHpp()));
    EXPECT_TRUE(vcc::isFilePresent(this->getFilePathCpp()));

    EXPECT_EQ(vcc::readFile(this->getFilePathHpp()), this->getExpectedHpp());

    std::wstring expectedResult = L""
        "#include \"object_factory.hpp\"\r\n"
        "\r\n"
        "#include <assert.h>\r\n"
        "#include <memory>\r\n"
        "\r\n"
        "#include \"exception_macro.hpp\"\r\n"
        "#include \"i_object.hpp\"\r\n"
        "#include \"object_type.hpp\"\r\n"
        "\r\n"
        "std::shared_ptr<vcc::IObject> ObjectFactory::create(const ObjectType &objectType, std::shared_ptr<vcc::IObject> parentObject)\r\n"
        "{\r\n"
        "    std::shared_ptr<vcc::IObject> result = nullptr;\r\n"
        "    TRY\r\n"
        "        switch (objectType)\r\n"
        "        {\r\n"
        "        default:\r\n"
        "            assert(false);\r\n"
        "            break;\r\n"
        "        }\r\n"
        "        if (result != nullptr)\r\n"
        "            result->setParentObject(parentObject);\r\n"
        "    CATCH\r\n"
        "    return result;\r\n"
        "}\r\n";
    EXPECT_EQ(vcc::readFile(this->getFilePathCpp()), expectedResult);
}

TEST_F(VPGObjectFactoryFileGenerationServiceTest, Normal)
{
    std::set<std::wstring> propertyTypes;
    propertyTypes.insert(L"Def");
    propertyTypes.insert(L"Abc");
    VPGObjectFactoryFileGenerationService::GenerateHpp(this->getLogConfig().get(), this->getFilePathHpp());
    VPGObjectFactoryFileGenerationService::GenerateCpp(this->getLogConfig().get(), L"VCC", { L"abc.hpp" }, this->getFilePathCpp(), propertyTypes);

    EXPECT_TRUE(vcc::isFilePresent(this->getFilePathHpp()));
    EXPECT_TRUE(vcc::isFilePresent(this->getFilePathCpp()));

    EXPECT_EQ(vcc::readFile(this->getFilePathHpp()), this->getExpectedHpp());

    std::wstring expectedResult = L""
        "#include \"object_factory.hpp\"\r\n"
        "\r\n"
        "#include <assert.h>\r\n"
        "#include <memory>\r\n"
        "\r\n"
        "#include \"abc.hpp\"\r\n"
        "#include \"exception_macro.hpp\"\r\n"
        "#include \"i_object.hpp\"\r\n"
        "#include \"object_type.hpp\"\r\n"
        "\r\n"
        "std::shared_ptr<vcc::IObject> ObjectFactory::create(const ObjectType &objectType, std::shared_ptr<vcc::IObject> parentObject)\r\n"
        "{\r\n"
        "    std::shared_ptr<vcc::IObject> result = nullptr;\r\n"
        "    TRY\r\n"
        "        switch (objectType)\r\n"
        "        {\r\n"
        "        case ObjectType::Abc:\r\n"
        "            result = std::make_shared<VCCAbc>();\r\n"
        "            break;\r\n"
        "        case ObjectType::Def:\r\n"
        "            result = std::make_shared<VCCDef>();\r\n"
        "            break;\r\n"
        "        default:\r\n"
        "            assert(false);\r\n"
        "            break;\r\n"
        "        }\r\n"
        "        if (result != nullptr)\r\n"
        "            result->setParentObject(parentObject);\r\n"
        "    CATCH\r\n"
        "    return result;\r\n"
        "}\r\n";
    EXPECT_EQ(vcc::readFile(this->getFilePathCpp()), expectedResult);
}