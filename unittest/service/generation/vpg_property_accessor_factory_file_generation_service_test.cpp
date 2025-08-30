#include <gtest/gtest.h>

#include <filesystem>
#include <string>
#include <vector>

#include "class_macro.hpp"
#include "file_helper.hpp"

#include "vpg_property_accessor_factory_file_generation_service.hpp"

class VPGPropertyAccessorFactoryFileGenerationServiceTest : public testing::Test 
{
    GETSET_SPTR_NULL(vcc::LogConfig, LogConfig);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGPropertyAccessorFactoryFileGenerationServiceTest/");
    
    GETSET(std::wstring, FilePathHpp, L"");
    GETSET(std::wstring, FilePathCpp, L"");

    GETSET(std::wstring, ExpectedHpp, L"");

    public:
        void SetUp() override
        {
            this->_LogConfig = std::make_shared<vcc::LogConfig>();
            this->_LogConfig->setIsConsoleLog(false);
            std::filesystem::remove_all(PATH(this->getWorkspace()));

            this->_FilePathHpp = vcc::concatPaths({this->getWorkspace(), L"property_accessor_factory.hpp"});
            this->_FilePathCpp = vcc::concatPaths({this->getWorkspace(), L"property_accessor_factory.cpp"});

            this->_ExpectedHpp = L""
                "#pragma once\r\n"
                "\r\n"
                "#include <memory>\r\n"
                "\r\n"
                "#include \"base_factory.hpp\"\r\n"
                "#include \"i_object.hpp\"\r\n"
                "#include \"i_property_accessor.hpp\"\r\n"
                "\r\n"
                "class PropertyAccessorFactory : public vcc::BaseFactory\r\n"
                "{\r\n"
                "    private:\r\n"
                "        PropertyAccessorFactory() = delete;\r\n"
                "        virtual ~PropertyAccessorFactory() {}\r\n"
                "\r\n"
                "    public:\r\n"
                "        static std::shared_ptr<vcc::IPropertyAccessor> create(std::shared_ptr<vcc::IObject> object);\r\n"
                "};\r\n";
        }

        void TearDown() override
        {
            std::filesystem::remove_all(PATH(this->getWorkspace()));
        }
};

TEST_F(VPGPropertyAccessorFactoryFileGenerationServiceTest, Empty)
{
    std::set<std::wstring> propertyTypes;
    VPGPropertyAccessorFactoryFileGenerationService::generateHpp(this->getLogConfig().get(), this->getFilePathHpp());
    VPGPropertyAccessorFactoryFileGenerationService::generateCpp(this->getLogConfig().get(), L"VCC", {}, this->getFilePathCpp(), propertyTypes);

    EXPECT_TRUE(vcc::isFilePresent(this->getFilePathHpp()));
    EXPECT_TRUE(vcc::isFilePresent(this->getFilePathCpp()));

    EXPECT_EQ(vcc::readFile(this->getFilePathHpp()), this->getExpectedHpp());

    std::wstring expectedResult = L""
        "#include \"property_accessor_factory.hpp\"\r\n"
        "\r\n"
        "#include <assert.h>\r\n"
        "#include <memory>\r\n"
        "\r\n"
        "#include \"base_property_accessor.hpp\"\r\n"
        "#include \"i_object.hpp\"\r\n"
        "#include \"i_property_accessor.hpp\"\r\n"
        "\r\n"
        "std::shared_ptr<vcc::IPropertyAccessor> PropertyAccessorFactory::create(std::shared_ptr<vcc::IObject> object)\r\n"
        "{\r\n"
        "    assert(object != nullptr);\r\n"
        "\r\n"
        "    switch (object->getObjectType())\r\n"
        "    {\r\n"
        "    default:\r\n"
        "        assert(false);\r\n"
        "        break;\r\n"
        "    }\r\n"
        "    return nullptr;\r\n"
        "}\r\n";
    EXPECT_EQ(vcc::readFile(this->getFilePathCpp()), expectedResult);
}

TEST_F(VPGPropertyAccessorFactoryFileGenerationServiceTest, Normal)
{
    std::set<std::wstring> propertyTypes;
    propertyTypes.insert(L"Def");
    propertyTypes.insert(L"Abc");
    VPGPropertyAccessorFactoryFileGenerationService::generateHpp(this->getLogConfig().get(), this->getFilePathHpp());
    VPGPropertyAccessorFactoryFileGenerationService::generateCpp(this->getLogConfig().get(), L"VCC", { L"abc.hpp" }, this->getFilePathCpp(), propertyTypes);

    EXPECT_TRUE(vcc::isFilePresent(this->getFilePathHpp()));
    EXPECT_TRUE(vcc::isFilePresent(this->getFilePathCpp()));

    EXPECT_EQ(vcc::readFile(this->getFilePathHpp()), this->getExpectedHpp());

    std::wstring expectedResult = L""
        "#include \"property_accessor_factory.hpp\"\r\n"
        "\r\n"
        "#include <assert.h>\r\n"
        "#include <memory>\r\n"
        "\r\n"
        "#include \"abc.hpp\"\r\n"
        "#include \"base_property_accessor.hpp\"\r\n"
        "#include \"i_object.hpp\"\r\n"
        "#include \"i_property_accessor.hpp\"\r\n"
        "\r\n"
        "std::shared_ptr<vcc::IPropertyAccessor> PropertyAccessorFactory::create(std::shared_ptr<vcc::IObject> object)\r\n"
        "{\r\n"
        "    assert(object != nullptr);\r\n"
        "\r\n"
        "    switch (object->getObjectType())\r\n"
        "    {\r\n"
        "    case ObjectType::Abc:\r\n"
        "        return std::make_shared<VCCAbcPropertyAccessor>(object);\r\n"
        "    case ObjectType::Def:\r\n"
        "        return std::make_shared<VCCDefPropertyAccessor>(object);\r\n"
        "    default:\r\n"
        "        assert(false);\r\n"
        "        break;\r\n"
        "    }\r\n"
        "    return nullptr;\r\n"
        "}\r\n";
    EXPECT_EQ(vcc::readFile(this->getFilePathCpp()), expectedResult);
}