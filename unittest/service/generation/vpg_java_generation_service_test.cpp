#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include "class_macro.hpp"
#include "file_helper.hpp"

#include "vpg_file_generation_manager.hpp"
#include "vpg_java_generation_service.hpp"

using namespace vcc;

class VPGJavaGenerationServiceTest : public testing::Test 
{
    GETSET_SPTR(LogConfig, LogConfig);
    GETSET(std::wstring, Workspace, L"bin/Debug/VPGJavaGenerationServiceTest/");
    GETSET_SPTR_NULL(VPGGenerationOptionExport, JavaOption);
    
    GETSET_SPTR_NULL(VPGGenerationOption, Option);
    
    MANAGER_SPTR(VPGEnumClassReader, Reader);

    public:
        void SetUp() override
        {
            this->_LogConfig->SetIsConsoleLog(false);

            DECLARE_UPTR(VPGFileGenerationManager, manager, nullptr, L"");
            manager->GetClassMacroList(L"");
            _Reader->InsertClassMacroList(manager->GetClassMacros());

            std::filesystem::remove_all(PATH(this->GetWorkspace()));

            _Option = std::make_shared<VPGGenerationOption>();
            _Option->SetProjectPrefix(L"VPG");
            _Option->SetTypeWorkspace(_Workspace);

            _JavaOption = std::make_shared<VPGGenerationOptionExport>();
            _Option->InsertExports(_JavaOption);
            _JavaOption->SetInterface(VPGGenerationOptionInterfaceType::Java);
            _JavaOption->SetWorkspace(_Workspace);
            _JavaOption->SetDllBridgeDirectory(L"src/main/java/com/vcc/test/");
            _JavaOption->SetTypeDirectory(L"src/main/java/com/vcc/type");
            _JavaOption->SetObjectDirectory(L"src/main/java/com/vcc/module");
            _JavaOption->SetFormDirectory(L"src/main/java/com/vcc/form");
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
        "// <vcc:dllInterfaceHeader gen=\"REPLACE\">\r\n"
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
        "// <vcc:dllInterface gen=\"REPLACE\">\r\n"
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

    VPGJavaGenerationService::GenerateJavaBridge(this->GetLogConfig().get(), L"", ConcatPaths({this->GetWorkspace(), L"DllFunctions.h"}), this->GetOption().get());
    EXPECT_TRUE(IsFileExists(ConcatPaths({this->GetWorkspace(), this->GetJavaOption()->GetDllBridgeDirectory(), L"VPGDllFunctions.java"})));
    EXPECT_EQ(ReadFile(ConcatPaths({this->GetWorkspace(), this->GetJavaOption()->GetDllBridgeDirectory(), L"VPGDllFunctions.java"})),
        L"package com.vcc.test;\r\n"
        "\r\n"
        "import com.sun.jna.Library;\r\n"
        "import com.sun.jna.Native;\r\n"
        "import com.sun.jna.Pointer;\r\n"
        "import com.sun.jna.ptr.PointerByReference;\r\n"
        "\r\n"
        "public interface VPGDllFunctions extends Library {\r\n"
        "    VPGDllFunctions Instance = loadLibrary();\r\n"
        "\r\n"
        "    static private VPGDllFunctions loadLibrary() {\r\n"
        "        String prefix = System.getProperty(\"os.name\").startsWith(\"Windows\") ? \"lib\" : \"\";\r\n"
        "        var lib = (VPGDllFunctions)Native.load(prefix + \"vpg\", VPGDllFunctions.class);\r\n"
        "        lib.ApplicationStart();\r\n"
        "        return lib;\r\n"
        "    }\r\n"
        "\r\n"
        "    int GetVersion(PointerByReference str);\r\n"
        "    Pointer CreateObject(long property);\r\n"
        "    boolean ReadBool(Pointer ref, long property, long index);\r\n"
        "    byte ReadChar(Pointer ref, long property, long index);\r\n"
        "    byte ReadCharByKey(Pointer ref, long property, Pointer key);\r\n"
        "    void WriteChar(Pointer ref, long property, byte value, long index);\r\n"
        "    void WriteCharByKey(Pointer ref, long property, byte value, Pointer key);\r\n"
        "    void InsertChar(Pointer ref, long property, byte value, long index);\r\n"
        "    void ReadString(Pointer ref, long property, PointerByReference value, long index);\r\n"
        "    void ReadStringByKey(Pointer ref, long property, PointerByReference value, Pointer key);\r\n"
        "    void WriteString(Pointer ref, long property, PointerByReference value, long index);\r\n"
        "    void WriteStringByKey(Pointer ref, long property, PointerByReference value, Pointer key);\r\n"
        "    void InsertString(Pointer ref, long property, PointerByReference value, long index);\r\n"
        "    Pointer ReadObject(Pointer ref, long property, long index);\r\n"
        "    Pointer ReadObjectByKey(Pointer ref, long property, Pointer key);\r\n"
        "    void WriteObject(Pointer ref, long property, Pointer value, long index);\r\n"
        "    void WriteObjectByKey(Pointer ref, long property, Pointer value, Pointer key);\r\n"
        "    Pointer AddObject(Pointer ref, long property, long objectType, long index);\r\n"
        "    void InsertObject(Pointer ref, long property, Pointer value, long index);\r\n"
        "    long GetContainerCount(Pointer ref, long property);\r\n"
        "    Pointer GetMapKeys(Pointer ref, long property);\r\n"
        "    boolean IsContainKey(Pointer ref, long property, Pointer key);\r\n"
        "    void RemoveContainerElement(Pointer ref, long property, long index);\r\n"
        "    void RemoveContainerElementByKey(Pointer ref, long property, Pointer key);\r\n"
        "    void ClearContainer(Pointer ref, long property);\r\n"
        "}\r\n"
        );
}

TEST_F(VPGJavaGenerationServiceTest, GenerateEnum)
{
    std::wstring enumClass1 =
        L"#param once\r\n"
        "enum class VPGTypeA {\r\n"
        "   EnumA,\r\n"
        "   EnumB,\r\n"
        "   EnumC\r\n"
        "};\r\n";

    std::wstring enumClass2 =
        L"#param once\r\n"
        "enum class VPGTypeBProperty {\r\n"
        "   EnumA = 0, // GETSET(std::wstring, EnumA, L"")\r\n"
        "   EnumB, // GETSET(int64_t, EnumB, 1)\r\n"
        "   EnumC = 999 // VECTOR(double, EnumC)\r\n"
        "};\r\n";

    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList1;
    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList2;
    this->GetReader()->Parse(enumClass1, enumClassList1);
    this->GetReader()->Parse(enumClass2, enumClassList2);

    std::wstring filePath1 = ConcatPaths({this->GetWorkspace(), this->GetJavaOption()->GetTypeDirectory(), L"VPGTypeA.java"});
    std::wstring filePath2 = ConcatPaths({this->GetWorkspace(), this->GetJavaOption()->GetTypeDirectory(), L"VPGTypeBProperty.java"});
    
    VPGJavaGenerationService::GenerateEnum(this->GetLogConfig().get(), filePath1, L"", enumClassList1.at(0).get(), this->GetOption().get(), this->GetJavaOption().get());
    VPGJavaGenerationService::GenerateEnum(this->GetLogConfig().get(), filePath2, L"", enumClassList2.at(0).get(), this->GetOption().get(), this->GetJavaOption().get());
    
    EXPECT_TRUE(IsFileExists(filePath1));
    EXPECT_EQ(ReadFile(filePath1),
        L"package com.vcc.type;\r\n"
        "\r\n"
        "import java.util.Objects;\r\n"
        "\r\n"
        "public enum VPGTypeA {\r\n"
        "    EnumA(0),\r\n"
        "    EnumB(1),\r\n"
        "    EnumC(2);\r\n"
        "\r\n"
        "    public final Integer value;\r\n"
        "\r\n"
        "    VPGTypeA(Integer value) {\r\n"
        "        this.value = value;\r\n"
        "    }\r\n"
        "\r\n"
        "    public Integer getValue() {\r\n"
        "        return value;\r\n"
        "    }\r\n"
        "\r\n"
        "    public static VPGTypeA parse(Integer value) {\r\n"
        "        for (VPGTypeA type : values()) {\r\n"
        "            if (Objects.equals(type.value, value)) {\r\n"
        "                return type;\r\n"
        "            }\r\n"
        "        }\r\n"
        "        throw new IllegalArgumentException(\"Invalid VPGTypeA value: \" + value);\r\n"
        "    }\r\n"
        "\r\n"
        "}\r\n");

    EXPECT_TRUE(IsFileExists(filePath2));
    EXPECT_EQ(ReadFile(filePath2),
        L"package com.vcc.type;\r\n"
        "\r\n"
        "import java.util.Objects;\r\n"
        "\r\n"
        "public enum VPGTypeBProperty {\r\n"
        "    EnumA(0),\r\n"
        "    EnumB(1),\r\n"
        "    EnumC(999);\r\n"
        "\r\n"
        "    public final Integer value;\r\n"
        "\r\n"
        "    VPGTypeBProperty(Integer value) {\r\n"
        "        this.value = value;\r\n"
        "    }\r\n"
        "\r\n"
        "    public Integer getValue() {\r\n"
        "        return value;\r\n"
        "    }\r\n"
        "\r\n"
        "    public static VPGTypeBProperty parse(Integer value) {\r\n"
        "        for (VPGTypeBProperty type : values()) {\r\n"
        "            if (Objects.equals(type.value, value)) {\r\n"
        "                return type;\r\n"
        "            }\r\n"
        "        }\r\n"
        "        throw new IllegalArgumentException(\"Invalid VPGTypeBProperty value: \" + value);\r\n"
        "    }\r\n"
        "\r\n"
        "}\r\n");
}

TEST_F(VPGJavaGenerationServiceTest, GenerateObject)
{   
    std::wstring enumClass =
        L"#param once\r\n"
        "enum class VPGTypeBProperty {\r\n"
        "    Bool = 0, // GETSET(bool, Bool, false)\r\n"
        "    Long , // GETSET(long, Long, 0)\r\n"
        "    Float, // GETSET(float, Float, 0)\r\n"
        "    Char, // GETSET(char, Char, L'\\0')\r\n"
        "    WChar, // GETSET(wchar_t, WChar, L'\\0')\r\n"
        "    String, // GETSET(std::wstring, String, L\"\")\r\n"
        "    Enum, // GETSET(VPGTypeBProperty, Enum, VPGTypeBProperty::Bool)\r\n"
        "    Object = 50, // GETSET_SPTR_NULL(VPGTypeB, Object)\r\n"
        "    VectorDouble = 80, // VECTOR(double, VectorDouble)\r\n"
        "    VectorString, // VECTOR(std::wstring, VectorString)\r\n"
        "    Map, // MAP(int, double, Map)\r\n"
        "    OrderedMap, // ORDERED_MAP(int, double, OrderedMap\r\n"
        "    VectorObject, // VECTOR_SPTR(VPGTypeB, VectorObject)\r\n"
        "    MapObject, // MAP_SPTR_R(std::wstring, VPGTypeB, MapObject)\r\n"
        "    OrderedMapObject // ORDERED_MAP_SPTR_R(double, VPGTypeB, OrderedMapObject)\r\n" 
        "};\r\n";

    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
    this->GetReader()->Parse(enumClass, enumClassList);

    std::map<std::wstring, std::wstring> typeWorkspaceClassRelativePathMap;
    std::wstring filePath = ConcatPaths({this->GetWorkspace(), this->GetJavaOption()->GetObjectDirectory(), L"VPGTypeB.java"});
    VPGJavaGenerationService::GenerateObject(this->GetLogConfig().get(), filePath, L"", enumClassList.at(0).get(), typeWorkspaceClassRelativePathMap, typeWorkspaceClassRelativePathMap, this->GetOption().get(), this->GetJavaOption().get());
    
    EXPECT_TRUE(IsFileExists(filePath));
    EXPECT_EQ(ReadFile(filePath),
        L"package com.vcc.module;\r\n"
        "\r\n"
        "import com.sun.jna.Memory;\r\n"
        "import com.sun.jna.Native;\r\n"
        "import com.sun.jna.Pointer;\r\n"
        "import com.sun.jna.ptr.PointerByReference;\r\n"
        "import com.vcc.test.VPGDllFunctions;\r\n"
        "import com.vcc.type.VPGTypeBProperty;\r\n"
        "import java.util.HashSet;\r\n"
        "import java.util.Set;\r\n"
        "\r\n"
        "public class VPGTypeB {\r\n"
        "    public Pointer Handle = null;\r\n"
        "\r\n"
        "    public VPGTypeB(Pointer handle) {\r\n"
        "        this.Handle = handle;\r\n"
        "    }\r\n"
        "\r\n"
        "    // <editor-fold defaultstate=\"collapsed\" desc=\"Generated Properties\">\r\n"
        "    public boolean getBool() {\r\n"
        "        return VPGDllFunctions.Instance.ReadBool(Handle, VPGTypeBProperty.Bool.getValue(), -1);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void setBool(boolean value) {\r\n"
        "        VPGDllFunctions.Instance.WriteBool(Handle, VPGTypeBProperty.Bool.getValue(), value, -1);\r\n"
        "    }\r\n"
        "\r\n"
        "    public long getLong() {\r\n"
        "        return VPGDllFunctions.Instance.ReadLong(Handle, VPGTypeBProperty.Long.getValue(), -1);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void setLong(long value) {\r\n"
        "        VPGDllFunctions.Instance.WriteLong(Handle, VPGTypeBProperty.Long.getValue(), value, -1);\r\n"
        "    }\r\n"
        "\r\n"
        "    public float getFloat() {\r\n"
        "        return VPGDllFunctions.Instance.ReadFloat(Handle, VPGTypeBProperty.Float.getValue(), -1);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void setFloat(float value) {\r\n"
        "        VPGDllFunctions.Instance.WriteFloat(Handle, VPGTypeBProperty.Float.getValue(), value, -1);\r\n"
        "    }\r\n"
        "\r\n"
        "    public byte getChar() {\r\n"
        "        return VPGDllFunctions.Instance.ReadChar(Handle, VPGTypeBProperty.Char.getValue(), -1);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void setChar(byte value) {\r\n"
        "        VPGDllFunctions.Instance.WriteChar(Handle, VPGTypeBProperty.Char.getValue(), value, -1);\r\n"
        "    }\r\n"
        "\r\n"
        "    public char getWChar() {\r\n"
        "        return VPGDllFunctions.Instance.ReadWchar(Handle, VPGTypeBProperty.WChar.getValue(), -1);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void setWChar(char value) {\r\n"
        "        VPGDllFunctions.Instance.WriteWchar(Handle, VPGTypeBProperty.WChar.getValue(), value, -1);\r\n"
        "    }\r\n"
        "\r\n"
        "    public String getString() {\r\n"
        "        PointerByReference result = new PointerByReference();\r\n"
        "        VPGDllFunctions.Instance.ReadString(Handle, VPGTypeBProperty.String.getValue(), result, -1);\r\n"
        "        return result.getValue().getWideString(0);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void setString(String value) {\r\n"
        "        Pointer valuePtr = new Memory(Native.WCHAR_SIZE * (value.length() + 1));\r\n"
        "        valuePtr.setWideString(0, value);\r\n"
        "        PointerByReference valueReference = new PointerByReference();\r\n"
        "        valueReference.setValue(valuePtr);\r\n"
        "        VPGDllFunctions.Instance.WriteString(Handle, VPGTypeBProperty.String.getValue(), valueReference, -1);\r\n"
        "    }\r\n"
        "\r\n"
        "    public VPGTypeBProperty getEnum() {\r\n"
        "        return VPGTypeBProperty.parse((int)VPGDllFunctions.Instance.ReadLong(Handle, VPGTypeBProperty.Enum.getValue(), -1));\r\n"
        "    }\r\n"
        "\r\n"
        "    public void setEnum(VPGTypeBProperty value) {\r\n"
        "        VPGDllFunctions.Instance.WriteLong(Handle, VPGTypeBProperty.Enum.getValue(), value.getValue(), -1);\r\n"
        "    }\r\n"
        "\r\n"
        "    public VPGTypeB getObject() {\r\n"
        "        return new VPGTypeB(VPGDllFunctions.Instance.ReadObject(Handle, VPGTypeBProperty.Object.getValue(), -1));\r\n"
        "    }\r\n"
        "\r\n"
        "    public void setObject(VPGTypeB value) {\r\n"
        "        VPGDllFunctions.Instance.WriteObject(Handle, VPGTypeBProperty.Object.getValue(), value.Handle, -1);\r\n"
        "    }\r\n"
        "\r\n"
        "    public long getVectorDoubleCount() {\r\n"
        "        return VPGDllFunctions.Instance.GetContainerCount(Handle, VPGTypeBProperty.VectorDouble.getValue());\r\n"
        "    }\r\n"
        "\r\n"
        "    public double getVectorDoubleAt(long index) {\r\n"
        "        return VPGDllFunctions.Instance.ReadDouble(Handle, VPGTypeBProperty.VectorDouble.getValue(), index);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void setVectorDoubleAt(long index, double value) {\r\n"
        "        VPGDllFunctions.Instance.WriteDouble(Handle, VPGTypeBProperty.VectorDouble.getValue(), value, index);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void insertVectorDouble(double value) {\r\n"
        "        insertVectorDoubleAt(-1, value);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void insertVectorDoubleAt(long index, double value) {\r\n"
        "        VPGDllFunctions.Instance.InsertDouble(Handle, VPGTypeBProperty.VectorDouble.getValue(), value, index);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void removeVectorDoubleAt(long index) {\r\n"
        "        VPGDllFunctions.Instance.RemoveContainerElement(Handle, VPGTypeBProperty.VectorDouble.getValue(), index);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void clearVectorDouble() {\r\n"
        "        VPGDllFunctions.Instance.ClearContainer(Handle, VPGTypeBProperty.VectorDouble.getValue());\r\n"
        "    }\r\n"
        "\r\n"
        "    public long getVectorStringCount() {\r\n"
        "        return VPGDllFunctions.Instance.GetContainerCount(Handle, VPGTypeBProperty.VectorString.getValue());\r\n"
        "    }\r\n"
        "\r\n"
        "    public String getVectorStringAt(long index) {\r\n"
        "        PointerByReference result = new PointerByReference();\r\n"
        "        VPGDllFunctions.Instance.ReadString(Handle, VPGTypeBProperty.VectorString.getValue(), result, index);\r\n"
        "        return result.getValue().getWideString(0);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void setVectorStringAt(long index, String value) {\r\n"
        "        Pointer valuePtr = new Memory(Native.WCHAR_SIZE * (value.length() + 1));\r\n"
        "        valuePtr.setWideString(0, value);\r\n"
        "        PointerByReference valueReference = new PointerByReference();\r\n"
        "        valueReference.setValue(valuePtr);\r\n"
        "        VPGDllFunctions.Instance.WriteString(Handle, VPGTypeBProperty.VectorString.getValue(), valueReference, index);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void insertVectorString(String value) {\r\n"
        "        insertVectorStringAt(-1, value);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void insertVectorStringAt(long index, String value) {\r\n"
        "        Pointer valuePtr = new Memory(Native.WCHAR_SIZE * (value.length() + 1));\r\n"
        "        valuePtr.setWideString(0, value);\r\n"
        "        PointerByReference valueReference = new PointerByReference();\r\n"
        "        valueReference.setValue(valuePtr);\r\n"
        "        VPGDllFunctions.Instance.InsertString(Handle, VPGTypeBProperty.VectorString.getValue(), valueReference, index);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void removeVectorStringAt(long index) {\r\n"
        "        VPGDllFunctions.Instance.RemoveContainerElement(Handle, VPGTypeBProperty.VectorString.getValue(), index);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void clearVectorString() {\r\n"
        "        VPGDllFunctions.Instance.ClearContainer(Handle, VPGTypeBProperty.VectorString.getValue());\r\n"
        "    }\r\n"
        "\r\n"
        "    public long getMapCount() {\r\n"
        "        return VPGDllFunctions.Instance.GetContainerCount(Handle, VPGTypeBProperty.Map.getValue());\r\n"
        "    }\r\n"
        "\r\n"
        "    public double getMapByKey(int key) {\r\n"
        "        Pointer keyPtr = new Memory(Native.getNativeSize(int.class));\r\n"
        "        keyPtr.setInt(0, key);\r\n"
        "        return VPGDllFunctions.Instance.ReadDoubleByKey(Handle, VPGTypeBProperty.Map.getValue(), keyPtr);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void setMapByKey(int key, double value) {\r\n"
        "        Pointer keyPtr = new Memory(Native.getNativeSize(int.class));\r\n"
        "        keyPtr.setInt(0, key);\r\n"
        "        VPGDllFunctions.Instance.WriteIntByKey(Handle, VPGTypeBProperty.Map.getValue(), value, keyPtr);\r\n"
        "    }\r\n"
        "\r\n"
        "    public Set<Integer> getMapKeys() {\r\n"
        "        Set<Integer> result = new HashSet<>();\r\n"
        "        Pointer ptrs = VPGDllFunctions.Instance.GetMapKeys(Handle, VPGTypeBProperty.Map.getValue());\r\n"
        "        long total = getMapCount();\r\n"
        "        for (var ptr : ptrs.getPointerArray(0)) {\r\n"
        "            if (ptr == null) {\r\n"
        "                break;\r\n"
        "            }\r\n"
        "            result.add(ptr.getInteger(0));\r\n"
        "            if (result.size() >= total) {\r\n"
        "                break;\r\n"
        "            }\r\n"
        "        }\r\n"
        "        return result;\r\n"
        "    }\r\n"
        "\r\n"
        "    public boolean isMapContainKey(int key) {\r\n"
        "        Pointer keyPtr = new Memory(Native.getNativeSize(int.class));\r\n"
        "        keyPtr.setInt(0, key);\r\n"
        "        return VPGDllFunctions.Instance.IsContainKey(Handle, VPGTypeBProperty.Map.getValue(), keyPtr);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void removeMapByKey(int key) {\r\n"
        "        Pointer keyPtr = new Memory(Native.getNativeSize(int.class));\r\n"
        "        keyPtr.setInt(0, key);\r\n"
        "        VPGDllFunctions.Instance.RemoveContainerElementByKey(Handle, VPGTypeBProperty.Map.getValue(), keyPtr);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void clearMap() {\r\n"
        "        VPGDllFunctions.Instance.ClearContainer(Handle, VPGTypeBProperty.Map.getValue());\r\n"
        "    }\r\n"
        "\r\n"
        "    public long getVectorObjectCount() {\r\n"
        "        return VPGDllFunctions.Instance.GetContainerCount(Handle, VPGTypeBProperty.VectorObject.getValue());\r\n"
        "    }\r\n"
        "\r\n"
        "    public VPGTypeB getVectorObjectAt(long index) {\r\n"
        "        return new VPGTypeB(VPGDllFunctions.Instance.ReadObject(Handle, VPGTypeBProperty.VectorObject.getValue(), index));\r\n"
        "    }\r\n"
        "\r\n"
        "    public void setVectorObjectAt(long index, VPGTypeB value) {\r\n"
        "        VPGDllFunctions.Instance.WriteObject(Handle, VPGTypeBProperty.VectorObject.getValue(), value.Handle, index);\r\n"
        "    }\r\n"
        "\r\n"
        "    public VPGTypeB addVectorObject() {\r\n"
        "        return addVectorObjectAt(-1);\r\n"
        "    }\r\n"
        "\r\n"
        "    public VPGTypeB addVectorObjectAt(long index) {\r\n"
        "        return new VPGTypeB(VPGDllFunctions.Instance.AddObject(Handle, VPGTypeBProperty.VectorObject.getValue(), VPGObjectType.TypeB.getValue(), index));\r\n"
        "    }\r\n"
        "\r\n"
        "    public void insertVectorObject(VPGTypeB value) {\r\n"
        "        insertVectorObjectAt(-1, value);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void insertVectorObjectAt(long index, VPGTypeB value) {\r\n"
        "        VPGDllFunctions.Instance.InsertObject(Handle, VPGTypeBProperty.VectorObject.getValue(), value.Handle, index);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void removeVectorObjectAt(long index) {\r\n"
        "        VPGDllFunctions.Instance.RemoveContainerElement(Handle, VPGTypeBProperty.VectorObject.getValue(), index);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void clearVectorObject() {\r\n"
        "        VPGDllFunctions.Instance.ClearContainer(Handle, VPGTypeBProperty.VectorObject.getValue());\r\n"
        "    }\r\n"
        "\r\n"
        "    public long getMapObjectCount() {\r\n"
        "        return VPGDllFunctions.Instance.GetContainerCount(Handle, VPGTypeBProperty.MapObject.getValue());\r\n"
        "    }\r\n"
        "\r\n"
        "    public VPGTypeB getMapObjectByKey(String key) {\r\n"
        "        Pointer keyPtr = new Memory(Native.WCHAR_SIZE * (key.length() + 1));\r\n"
        "        keyPtr.setWideString(0, key);\r\n"
        "        return new VPGTypeB(VPGDllFunctions.Instance.ReadObjectByKey(Handle, VPGTypeBProperty.MapObject.getValue(), keyPtr));\r\n"
        "    }\r\n"
        "\r\n"
        "    public void setMapObjectByKey(String key, VPGTypeB value) {\r\n"
        "        Pointer keyPtr = new Memory(Native.WCHAR_SIZE * (key.length() + 1));\r\n"
        "        keyPtr.setWideString(0, key);\r\n"
        "        VPGDllFunctions.Instance.WriteObjectByKey(Handle, VPGTypeBProperty.MapObject.getValue(), value.Handle, keyPtr);\r\n"
        "    }\r\n"
        "\r\n"
        "    public Set<String> getMapObjectKeys() {\r\n"
        "        Set<String> result = new HashSet<>();\r\n"
        "        Pointer ptrs = VPGDllFunctions.Instance.GetMapKeys(Handle, VPGTypeBProperty.MapObject.getValue());\r\n"
        "        long total = getMapObjectCount();\r\n"
        "        for (var ptr : ptrs.getPointerArray(0)) {\r\n"
        "            if (ptr == null) {\r\n"
        "                break;\r\n"
        "            }\r\n"
        "            result.add(ptr.getWideString(0));\r\n"
        "            if (result.size() >= total) {\r\n"
        "                break;\r\n"
        "            }\r\n"
        "        }\r\n"
        "        return result;\r\n"
        "    }\r\n"
        "\r\n"
        "    public boolean isMapObjectContainKey(String key) {\r\n"
        "        Pointer keyPtr = new Memory(Native.WCHAR_SIZE * (key.length() + 1));\r\n"
        "        keyPtr.setWideString(0, key);\r\n"
        "        return VPGDllFunctions.Instance.IsContainKey(Handle, VPGTypeBProperty.MapObject.getValue(), keyPtr);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void removeMapObjectByKey(String key) {\r\n"
        "        Pointer keyPtr = new Memory(Native.WCHAR_SIZE * (key.length() + 1));\r\n"
        "        keyPtr.setWideString(0, key);\r\n"
        "        VPGDllFunctions.Instance.RemoveContainerElementByKey(Handle, VPGTypeBProperty.MapObject.getValue(), keyPtr);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void clearMapObject() {\r\n"
        "        VPGDllFunctions.Instance.ClearContainer(Handle, VPGTypeBProperty.MapObject.getValue());\r\n"
        "    }\r\n"
        "\r\n"
        "    public long getOrderedMapObjectCount() {\r\n"
        "        return VPGDllFunctions.Instance.GetContainerCount(Handle, VPGTypeBProperty.OrderedMapObject.getValue());\r\n"
        "    }\r\n"
        "\r\n"
        "    public double getOrderedMapObjectAt(long index) {\r\n"
        "        return new double(VPGDllFunctions.Instance.ReadObject(Handle, VPGTypeBProperty.OrderedMapObject.getValue(), index));\r\n"
        "    }\r\n"
        "\r\n"
        "    public void setOrderedMapObjectAt(long index, double value) {\r\n"
        "        VPGDllFunctions.Instance.WriteObject(Handle, VPGTypeBProperty.OrderedMapObject.getValue(), value.Handle, index);\r\n"
        "    }\r\n"
        "\r\n"
        "    public double addOrderedMapObject() {\r\n"
        "        return addOrderedMapObjectAt(-1);\r\n"
        "    }\r\n"
        "\r\n"
        "    public double addOrderedMapObjectAt(long index) {\r\n"
        "        return new double(VPGDllFunctions.Instance.AddObject(Handle, VPGTypeBProperty.OrderedMapObject.getValue(), VPGObjectType.double.getValue(), index));\r\n"
        "    }\r\n"
        "\r\n"
        "    public VPGTypeB getOrderedMapObjectByKey(double key) {\r\n"
        "        Pointer keyPtr = new Memory(Native.getNativeSize(double.class));\r\n"
        "        keyPtr.setDouble(0, key);\r\n"
        "        return new VPGTypeB(VPGDllFunctions.Instance.ReadObjectByKey(Handle, VPGTypeBProperty.OrderedMapObject.getValue(), keyPtr));\r\n"
        "    }\r\n"
        "\r\n"
        "    public void setOrderedMapObjectByKey(double key, VPGTypeB value) {\r\n"
        "        Pointer keyPtr = new Memory(Native.getNativeSize(double.class));\r\n"
        "        keyPtr.setDouble(0, key);\r\n"
        "        VPGDllFunctions.Instance.WriteObjectByKey(Handle, VPGTypeBProperty.OrderedMapObject.getValue(), value.Handle, keyPtr);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void insertOrderedMapObject(double value) {\r\n"
        "        insertOrderedMapObjectAt(-1, value);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void insertOrderedMapObjectAt(long index, double value) {\r\n"
        "        VPGDllFunctions.Instance.InsertObject(Handle, VPGTypeBProperty.OrderedMapObject.getValue(), value.Handle, index);\r\n"
        "    }\r\n"
        "\r\n"
        "    public Set<Double> getOrderedMapObjectKeys() {\r\n"
        "        Set<Double> result = new HashSet<>();\r\n"
        "        Pointer ptrs = VPGDllFunctions.Instance.GetMapKeys(Handle, VPGTypeBProperty.OrderedMapObject.getValue());\r\n"
        "        long total = getOrderedMapObjectCount();\r\n"
        "        for (var ptr : ptrs.getPointerArray(0)) {\r\n"
        "            if (ptr == null) {\r\n"
        "                break;\r\n"
        "            }\r\n"
        "            result.add(ptr.getDouble(0));\r\n"
        "            if (result.size() >= total) {\r\n"
        "                break;\r\n"
        "            }\r\n"
        "        }\r\n"
        "        return result;\r\n"
        "    }\r\n"
        "\r\n"
        "    public boolean isOrderedMapObjectContainKey(double key) {\r\n"
        "        Pointer keyPtr = new Memory(Native.getNativeSize(double.class));\r\n"
        "        keyPtr.setDouble(0, key);\r\n"
        "        return VPGDllFunctions.Instance.IsContainKey(Handle, VPGTypeBProperty.OrderedMapObject.getValue(), keyPtr);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void removeOrderedMapObjectAt(long index) {\r\n"
        "        VPGDllFunctions.Instance.RemoveContainerElement(Handle, VPGTypeBProperty.OrderedMapObject.getValue(), index);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void removeOrderedMapObjectByKey(double key) {\r\n"
        "        Pointer keyPtr = new Memory(Native.getNativeSize(double.class));\r\n"
        "        keyPtr.setDouble(0, key);\r\n"
        "        VPGDllFunctions.Instance.RemoveContainerElementByKey(Handle, VPGTypeBProperty.OrderedMapObject.getValue(), keyPtr);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void clearOrderedMapObject() {\r\n"
        "        VPGDllFunctions.Instance.ClearContainer(Handle, VPGTypeBProperty.OrderedMapObject.getValue());\r\n"
        "    }\r\n"
        "    // </editor-fold>\r\n"
        "}\r\n");
}

TEST_F(VPGJavaGenerationServiceTest, GenerateForm)
{
    std::wstring enumClass =
        L"#param once\r\n"
        "// @@Form\r\n"
        "enum class VPGGitFormProperty {\r\n"
        "    Log // GETSET_SPTR_NULL(VPGGitLog, Log)\r\n"
        "};\r\n";

    std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
    this->GetReader()->Parse(enumClass, enumClassList);

    std::map<std::wstring, std::wstring> typeWorkspaceClassRelativePathMapObject, typeWorkspaceClassRelativePathMapForm;
    typeWorkspaceClassRelativePathMapObject.insert(std::make_pair(L"VPGGit", L"com.vcc.object"));
    std::wstring filePath = ConcatPaths({this->GetWorkspace(), this->GetJavaOption()->GetObjectDirectory(), L"VPGGitLog.java"});
    VPGJavaGenerationService::GenerateObject(this->GetLogConfig().get(), filePath, L"", enumClassList.at(0).get(), typeWorkspaceClassRelativePathMapObject, typeWorkspaceClassRelativePathMapForm, this->GetOption().get(), this->GetJavaOption().get());
    
    EXPECT_TRUE(IsFileExists(filePath));
    EXPECT_EQ(ReadFile(filePath),
        L"package com.vcc.form;\r\n"
        "\r\n"
        "import com.sun.jna.Pointer;\r\n"
        "import com.vcc.test.VPGDllFunctions;\r\n"
        //"import com.vcc.object.VPGGit;\r\n"
        "import com.vcc.type.VPGGitFormProperty;\r\n"
        "\r\n"
        "public class VPGGitForm {\r\n"
        "    public Pointer Handle = null;\r\n"
        "\r\n"
        "    public VPGGitForm(Pointer handle) {\r\n"
        "        this.Handle = handle;\r\n"
        "    }\r\n"
        "\r\n"
        "    public VPGGitForm() {\r\n"
        "        this.Handle = VPGDllFunctions.Instance.ApplicationCreateForm(VPGObjectType.GitForm.getValue());\r\n"
        "    }\r\n"
        "\r\n"
        "    // <editor-fold defaultstate=\"collapsed\" desc=\"Generated Properties\">\r\n"
        "    public VPGGitLog getLog() {\r\n"
        "        return new VPGGitLog(VPGDllFunctions.Instance.ReadObject(Handle, VPGGitFormProperty.Log.getValue(), -1));\r\n"
        "    }\r\n"
        "\r\n"
        "    public void setLog(VPGGitLog value) {\r\n"
        "        VPGDllFunctions.Instance.WriteObject(Handle, VPGGitFormProperty.Log.getValue(), value.Handle, -1);\r\n"
        "    }\r\n"
        "    // </editor-fold>\r\n"
        "\r\n"
        "    // <editor-fold defaultstate=\"collapsed\" desc=\"Generated Form Actions\">\r\n"
        "    public void close(boolean isForce) {\r\n"
        "        VPGDllFunctions.Instance.ApplicationCloseForm(Handle, isForce);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void initialize() {\r\n"
        "        VPGDllFunctions.Instance.ApplicationInitializeForm(Handle);\r\n"
        "    }\r\n"
        "\r\n"
        "    public boolean isClosable() {\r\n"
        "        return VPGDllFunctions.Instance.ApplicationIsFormClosable(Handle);\r\n"
        "    }\r\n"
        "\r\n"
        "    public boolean isClosed() {\r\n"
        "        return VPGDllFunctions.Instance.ApplicationIsFormClosed(Handle);\r\n"
        "    }\r\n"
        "\r\n"
        "    public void reload() {\r\n"
        "        VPGDllFunctions.Instance.ApplicationReloadForm(Handle);\r\n"
        "    }\r\n"
        "    // </editor-fold>\r\n"
        "}\r\n");
}
