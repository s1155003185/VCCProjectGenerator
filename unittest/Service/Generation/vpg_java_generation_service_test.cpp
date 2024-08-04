#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include "class_macro.hpp"
#include "file_helper.hpp"

#include "vpg_java_generation_service.hpp"

using namespace vcc;

class VPGJavaGenerationServiceTest : public testing::Test 
{
    GETSET_SPTR(LogProperty, LogProperty);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGJavaGenerationServiceTest/");
    GETSET_SPTR_NULL(VPGGenerationOptionExport, JavaOption);
    
    GETSET_SPTR_NULL(VPGGenerationOption, Option);
    public:
        void SetUp() override
        {
            this->_LogProperty->SetIsConsoleLog(false);
            std::filesystem::remove_all(PATH(this->GetWorkspace()));

            _Option = std::make_shared<VPGGenerationOption>();
            _Option->SetProjectPrefix(L"VPG");

            _JavaOption = std::make_shared<VPGGenerationOptionExport>();
            _Option->InsertExports(_JavaOption);
            _JavaOption->SetInterface(VPGGenerationOptionInterfaceType::Java);
            _JavaOption->SetWorkspace(_Workspace);
            _JavaOption->SetDllBridgeDirectory(L"src/main/java/com/vcc/test/");
            _JavaOption->SetTypeDirectory(L"src/main/java/com/vcc/type");
            _JavaOption->SetObjectDirectory(L"src/main/java/com/vcc/Module");
        }

        void TearDown() override
        {
            std::filesystem::remove_all(PATH(this->GetWorkspace()));
        }
};

TEST_F(VPGJavaGenerationServiceTest, GenerateJavaBridge)
{
    WriteFile(ConcatPaths({this->GetWorkspace(), L"DllFunctions.h"}),
        L"// <vcc:vccproj gen=\"DEMAND\"/>\r\n"
        "#ifndef DLL_FUNCTIONS_H\r\n"
        "#define DLL_FUNCTIONS_H\r\n"
        "\r\n"
        "#include <string>\r\n"
        "\r\n"
        "#ifdef _WIN32\r\n"
        "#define DLLEXPORT __declspec (dllexport)\r\n"
        "#else\r\n"
        "#define DLLEXPORT extern\r\n"
        "#endif\r\n"
        "\r\n"
        "// <vcc:dllInterfaceHeader gen=\"FORCE\">\r\n"
        "#include \"property_accessor_macro.hpp\"\r\n"
        "#include \"memory_macro.hpp\"\r\n"
        "#include \"exception_macro.hpp\"\r\n"
        "\r\n"
        "using namespace vcc;\r\n"
        "// </vcc:dllInterfaceHeader>\r\n"
        "\r\n"
        "extern \"C\"\r\n"
        "{\r\n"
        "\r\n"
        "DLLEXPORT int GetVersion(wchar_t **str);\r\n"
        "\r\n"
        "static std::shared_ptr<IObject> tmpObj;\r\n"
        "DLLEXPORT void *CreateObject(int64_t property)\r\n"
        "{\r\n"
        "    ObjectType objectType = static_cast<ObjectType>(property);\r\n"
        "    tmpObj = ObjectFactory::Create(objectType);\r\n"
        "    return tmpObj.get();\r\n"
        "}\r\n"
        "\r\n"
        "// <vcc:dllInterface gen=\"FORCE\">\r\n"
        "DLLEXPORT bool ReadBool(void *ref, int64_t property, int64_t index)\r\n"
        "{\r\n"
        "    TRY\r\n"
        "         IObject *object = static_cast<IObject *>(ref);\r\n"
        "         std::shared_ptr<IObject> ptr(object, [](IObject*){});\r\n"
        "         return PropertyAccessorFactory::Create(ptr)->ReadBool(LockType::ReadLock, property, index);\r\n"
        "     CATCH \r\n"
        "     return false;\r\n"
        "} \r\n"
        "\r\n"
        "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(char, Char)\r\n"
        "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_STRING\r\n"
        "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_OBJECT\r\n"
        "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_CONTAINER\r\n"
        "// </vcc:dllInterface>\r\n"
        "\r\n"
        "}\r\n"
        "\r\n"
        "#endif\r\n", true);

    VPGJavaGenerationService::GenerateJavaBridge(this->GetLogProperty().get(), ConcatPaths({this->GetWorkspace(), L"DllFunctions.h"}), this->GetOption().get());
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspace(), this->GetJavaOption()->GetDllBridgeDirectory(), L"VPGDllFunctions.java"})));
    EXPECT_EQ(ReadFile(ConcatPaths({this->GetWorkspace(), this->GetJavaOption()->GetDllBridgeDirectory(), L"VPGDllFunctions.java"})),
        L"package com.vcc.test;\r\n"
        "\r\n"
        "import com.sun.jna.Library;\r\n"
        "import com.sun.jna.Native;\r\n"
        "import com.sun.jna.ptr.PointerByReference;\r\n"
        "\r\n"
        "interface VPGDllFunctions extends Library {\r\n"
        "    VPGDllFunctions INSTANCE = (VPGDllFunctions)Native.load(\"vpg\", VPGDllFunctions.class);\r\n"
        "\r\n"
        "    Integer GetVersion(PointerByReference str);\r\n"
        "    PointerByReference CreateObject(Integer property);\r\n"
        "    Boolean ReadBool(PointerByReference ref, Integer property, Integer index);\r\n"
        "    Byte ReadChar(PointerByReference ref, Integer property, Integer index);\r\n"
        "    Byte ReadCharByKey(PointerByReference ref, Integer property, PointerByReference key);\r\n"
        "    void WriteChar(PointerByReference ref, Integer property, Byte value, Integer index);\r\n"
        "    void WriteCharByKey(PointerByReference ref, Integer property, Byte value, PointerByReference key);\r\n"
        "    void ReadString(PointerByReference ref, Integer property, PointerByReference value, Integer index);\r\n"
        "    void ReadStringByKey(PointerByReference ref, Integer property, PointerByReference value, PointerByReference key);\r\n"
        "    void WriteString(PointerByReference ref, Integer property, PointerByReference value, Integer index);\r\n"
        "    void WriteStringByKey(PointerByReference ref, Integer property, PointerByReference value, PointerByReference key);\r\n"
        "    PointerByReference ReadObject(PointerByReference ref, Integer property, Integer index);\r\n"
        "    PointerByReference ReadObjectByKey(PointerByReference ref, Integer property, PointerByReference key);\r\n"
        "    void WriteObject(PointerByReference ref, Integer property, PointerByReference value, Integer index);\r\n"
        "    void WriteObjectByKey(PointerByReference ref, Integer property, PointerByReference value, PointerByReference key);\r\n"
        "    Integer GetContainerCount(PointerByReference ref, Integer property);\r\n"
        "    Boolean IsContainKey(PointerByReference ref, Integer property, PointerByReference key);\r\n"
        "    void RemoveContainerElement(PointerByReference ref, Integer property, Integer index);\r\n"
        "    void RemoveContainerElementByKey(PointerByReference ref, Integer property, PointerByReference key);\r\n"
        "    void ClearContainer(PointerByReference ref, Integer property);\r\n"
        "}\r\n"
        );

}
