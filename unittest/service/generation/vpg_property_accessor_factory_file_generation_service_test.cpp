#include <gtest/gtest.h>

#include <filesystem>
#include <string>
#include <vector>

#include "class_macro.hpp"
#include "file_helper.hpp"

#include "vpg_property_accessor_factory_file_generation_service.hpp"

using namespace vcc;

class VPGPropertyAccessorFactoryFileGenerationServiceTest : public testing::Test 
{
    GETSET_SPTR_NULL(LogConfig, LogConfig);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGPropertyAccessorFactoryFileGenerationServiceTest/");
    
    GETSET(std::wstring, FilePathHpp, L"");
    GETSET(std::wstring, FilePathCpp, L"");

    GETSET(std::wstring, ExpectedHpp, L"");

    public:
        void SetUp() override
        {
            this->_LogConfig = std::make_shared<LogConfig>();
            this->_LogConfig->SetIsConsoleLog(false);
            std::filesystem::remove_all(PATH(this->GetWorkspace()));

            this->_FilePathHpp = ConcatPaths({this->GetWorkspace(), L"property_accessor_factory.hpp"});
            this->_FilePathCpp = ConcatPaths({this->GetWorkspace(), L"property_accessor_factory.cpp"});

            this->_ExpectedHpp = L""
                "#pragma once\r\n"
                "\r\n"
                "#include <memory>\r\n"
                "\r\n"
                "#include \"base_factory.hpp\"\r\n"
                "#include \"i_object.hpp\"\r\n"
                "#include \"i_property_accessor.hpp\"\r\n"
                "\r\n"
                "using namespace vcc;\r\n"
                "\r\n"
                "class PropertyAccessorFactory : public BaseFactory\r\n"
                "{\r\n"
                "    private:\r\n"
                "        PropertyAccessorFactory() = default;\r\n"
                "        virtual ~PropertyAccessorFactory() {}\r\n"
                "\r\n"
                "    public:\r\n"
                "        static std::shared_ptr<IPropertyAccessor> Create(std::shared_ptr<IObject> object);\r\n"
                "};\r\n";
        }

        void TearDown() override
        {
            std::filesystem::remove_all(PATH(this->GetWorkspace()));
        }
};

TEST_F(VPGPropertyAccessorFactoryFileGenerationServiceTest, Empty)
{
    std::set<std::wstring> propertyTypes;
    VPGPropertyAccessorFactoryFileGenerationService::GenerateHpp(this->GetLogConfig().get(), this->GetFilePathHpp());
    VPGPropertyAccessorFactoryFileGenerationService::GenerateCpp(this->GetLogConfig().get(), L"VCC", {}, this->GetFilePathCpp(), propertyTypes);

    EXPECT_TRUE(IsFilePresent(this->GetFilePathHpp()));
    EXPECT_TRUE(IsFilePresent(this->GetFilePathCpp()));

    EXPECT_EQ(ReadFile(this->GetFilePathHpp()), this->GetExpectedHpp());

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
        "using namespace vcc;\r\n"
        "\r\n"
        "std::shared_ptr<IPropertyAccessor> PropertyAccessorFactory::Create(std::shared_ptr<IObject> object)\r\n"
        "{\r\n"
        "    assert(object != nullptr);\r\n"
        "\r\n"
        "    switch (object->GetObjectType())\r\n"
        "    {\r\n"
        "    default:\r\n"
        "        assert(false);\r\n"
        "        break;\r\n"
        "    }\r\n"
        "    return nullptr;\r\n"
        "}\r\n";
    EXPECT_EQ(ReadFile(this->GetFilePathCpp()), expectedResult);
}

TEST_F(VPGPropertyAccessorFactoryFileGenerationServiceTest, Normal)
{
    std::set<std::wstring> propertyTypes;
    propertyTypes.insert(L"Def");
    propertyTypes.insert(L"Abc");
    VPGPropertyAccessorFactoryFileGenerationService::GenerateHpp(this->GetLogConfig().get(), this->GetFilePathHpp());
    VPGPropertyAccessorFactoryFileGenerationService::GenerateCpp(this->GetLogConfig().get(), L"VCC", { L"abc.hpp" }, this->GetFilePathCpp(), propertyTypes);

    EXPECT_TRUE(IsFilePresent(this->GetFilePathHpp()));
    EXPECT_TRUE(IsFilePresent(this->GetFilePathCpp()));

    EXPECT_EQ(ReadFile(this->GetFilePathHpp()), this->GetExpectedHpp());

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
        "using namespace vcc;\r\n"
        "\r\n"
        "std::shared_ptr<IPropertyAccessor> PropertyAccessorFactory::Create(std::shared_ptr<IObject> object)\r\n"
        "{\r\n"
        "    assert(object != nullptr);\r\n"
        "\r\n"
        "    switch (object->GetObjectType())\r\n"
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
    EXPECT_EQ(ReadFile(this->GetFilePathCpp()), expectedResult);
}