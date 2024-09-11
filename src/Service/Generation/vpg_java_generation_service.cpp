#include "vpg_java_generation_service.hpp"

#include <filesystem>
#include <fstream>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_property.hpp"

#include "vpg_file_generation_manager.hpp"
#include "vpg_generation_option.hpp"
#include "vpg_generation_option_interface_type.hpp"
#include "vpg_property_accessor_generation_service.hpp"

using namespace vcc;

#define LOG_ID L"Java Generation"
#define JAVA_BRIDGE_FILE_NAME L"DllFunctions.java"
#define JAVA_PROJECT_SOURCE_PARENT_FOLDER L"src/main/java"

const std::wstring propertyClassNameSuffix = L"Property";

const std::wstring dllExport = L"DLLEXPORT";
const std::wstring dllExportStart = L"extern \"C\"";
const std::wstring dllInterfaceExportPropertyAccessor = L"PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER";
const std::wstring dllInterfaceExportPropertyAccessorString = L"PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_STRING";
const std::wstring dllInterfaceExportPropertyAccessorObject = L"PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_OBJECT";
const std::wstring dllInterfaceExportPropertyAccessorContainer = L"PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_CONTAINER";

std::shared_ptr<VPGGenerationOptionExport> VPGJavaGenerationService::GetJavaOption(const VPGGenerationOption *option)
{
    std::shared_ptr<VPGGenerationOptionExport> result = nullptr;
    TRY
        for (auto element : option->GetExports()) {
            if (element->GetInterface() == VPGGenerationOptionInterfaceType::Java) {
                result = element;
                break;
            }
        }
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::GetJavaPactkage(const std::wstring &path, const std::wstring &middlePath, const std::wstring &filePathName)
{
    std::wstring result = L"";
    TRY
        if (!IsStartWith(path, JAVA_PROJECT_SOURCE_PARENT_FOLDER))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, filePathName + L" is not start with " + JAVA_PROJECT_SOURCE_PARENT_FOLDER);

        result = GetRelativePath(path, JAVA_PROJECT_SOURCE_PARENT_FOLDER);
        if (!middlePath.empty())
            result = ConcatPaths({ result, middlePath });
        result = GetLinuxPath(result);
        if (IsStartWith(result, L"/"))
            result = result.substr(1);
        if (IsEndWith(result, L"/"))
            result.pop_back();
        ReplaceAll(result, L"/", L".");
        ToLower(result);
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::GetPropertyAccessorCppToJavaConvertedType(const std::wstring &cppType)
{
    std::wstring result = L"";
    TRY
        result = GetJavaGetterSetterCppToJavaConvertedType(cppType);
        if (result == L"String")
            result = L"PointerByReference";
        
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::GetJavaGetterSetterCppToJavaConvertedType(const std::wstring &cppType)
{
    TRY
        if (cppType == L"bool") {
            return L"boolean";
        } else if (cppType == L"char"
            || cppType == L"byte") {
            return L"byte";
        } else if (cppType == L"wchar_t") {
            return L"char";
        } else if (cppType == L"int") {
            return L"int";
        } else if (IsContain(cppType, L"int")
            || IsContain(cppType, L"short")
            || IsContain(cppType, L"long")
            || cppType == L"size_t"
            || cppType == L"time_t") {
            return L"long";
        } else if (cppType == L"float") {
            return L"float";
        } else if (cppType == L"double") {
            return L"double";
        } else if (cppType == L"std::string"
            || cppType == L"std::wstring") {
            return L"String";
        } else if (cppType == L"void*" || cppType == L"void *") {
            return L"PointerByReference";
        } else if (cppType == L"void") {
            return L"void";
        } else
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Unknown type: " + cppType);
    CATCH
    return L"";
}

std::wstring VPGJavaGenerationService::GetJavaGetterSetterJavaTypeToJavaCaptialType(const std::wstring &javaType)
{
    TRY
        if (IsCaptial(javaType))
            return javaType;

        if (javaType == L"boolean") {
            return L"Boolean";
        } else if (javaType == L"byte") {
            return L"Byte";
        } else if (javaType == L"int") {
            return L"Integer";
        } else if (javaType == L"long") {
            return L"Long";
        } else if (javaType == L"float") {
            return L"Float";
        } else if (javaType == L"double") {
            return L"Double";
        } else if (javaType == L"void") {
            return L"void";
        } else {
            std::wstring result = javaType;
            ToCapital(result);
            return result;
        }
    CATCH
    return L"";
}

std::wstring VPGJavaGenerationService::GenerateJavaBridgeContent(const std::wstring &content, const VPGGenerationOption *option)
{
    std::wstring result = L"";
    TRY
        std::shared_ptr<VPGGenerationOptionExport> javaOption = VPGJavaGenerationService::GetJavaOption(option);
        if (javaOption == nullptr || IsBlank(javaOption->GetWorkspace()) || IsBlank(javaOption->GetDllBridgeDirectory()))
            return result;

        std::wstring filePrefix = option->GetProjectPrefix();
        Trim(filePrefix);
        ToUpper(filePrefix);

        std::set<std::wstring> importPackages;
        importPackages.insert(L"com.sun.jna.Library");
        importPackages.insert(L"com.sun.jna.Native");
        importPackages.insert(L"com.sun.jna.ptr.PointerByReference");
        std::wstring packageFolder = GetJavaPactkage(javaOption->GetDllBridgeDirectory(), L"", L"Dll Bridge Directory");

        size_t pos = Find(content, dllExportStart);
        if (pos == std::wstring::npos)
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"DllFunctions.h missing 'extern \"C\"'");

        pos += dllExportStart.size() - 1;
        while (pos < content.length()) {
            if (IsStartWith(content, dllExport, pos)) {
                // normal function
                pos += dllExport.length();
                size_t endPos1 = Find(content, L";", pos);
                size_t endPos2 = Find(content, L"{", pos);
                size_t endPos = 0;
                if (endPos1 == std::wstring::npos && endPos2 == std::wstring::npos)
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"DllFunctions.h DLLEXPORT missing ; or {");
                else if (endPos1 == std::wstring::npos)
                    endPos = endPos2;
                else if (endPos2 == std::wstring::npos)
                    endPos = endPos1;
                else
                    endPos = std::min(endPos1, endPos2);
                std::wstring cppFunction = content.substr(pos, endPos - pos);
                Trim(cppFunction);

                // 1. determine return is void * or not
                // 2. get function name
                // 3. pharse argument list
                size_t argumentStartPos = Find(cppFunction, L"(");
                if (argumentStartPos == std::wstring::npos)
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"DllFunctions.h DLLEXPORT function " + cppFunction + L" missing arguments");
                size_t argumentEndPos = Find(cppFunction, L")", argumentStartPos);
                if (argumentEndPos == std::wstring::npos)
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"DllFunctions.h DLLEXPORT function " + cppFunction + L" argument list missing ending )");
                std::wstring functionNameWithReturn = cppFunction.substr(0, argumentStartPos);
                std::wstring functionArgumentList = cppFunction.substr(argumentStartPos + 1, argumentEndPos - argumentStartPos - 1);
                
                // return type and function name
                std::wstring returnType = L"";
                std::wstring functionName = L"";
                size_t tmpPos = 0;
                if (IsContain(functionNameWithReturn, L"*")) {
                    returnType = L"void *";
                    tmpPos = Find(functionNameWithReturn, L"*");
                    tmpPos++;
                    functionName = functionNameWithReturn.substr(tmpPos);
                } else {
                    returnType = GetNextString(functionNameWithReturn, tmpPos, { L" ", L"\t", L"\r", L"\n" });
                    tmpPos++;
                    functionName = GetNextString(functionNameWithReturn, tmpPos, { L" ", L"\t", L"\r", L"\n" });
                }
                Trim(returnType);
                Trim(functionName);
                result += INDENT + GetPropertyAccessorCppToJavaConvertedType(returnType) + L" " + functionName + L"(";

                // argument
                Trim(functionArgumentList);
                std::wstring argumentStr = L"";
                std::vector<std::wstring> tokens =  SplitString(functionArgumentList, {L","});
                
                std::wstring argumentType = L"";
                std::wstring argumentName = L"";
                for (auto const &token : tokens) {
                    tmpPos = 0;
                    if (!argumentStr.empty())
                        argumentStr += L", ";

                    if (IsContain(token, L"*")) {
                        argumentType = L"void *";
                        tmpPos = token.find_last_of(L"*");
                        tmpPos++;
                        argumentName = token.substr(tmpPos);
                    } else {
                        argumentType = GetNextString(token, tmpPos, { L" ", L"\t", L"\r", L"\n" });
                        tmpPos++;
                        argumentName = GetNextString(token, tmpPos, { L" ", L"\t", L"\r", L"\n" });
                    }
                    Trim(argumentType);
                    Trim(argumentName);

                    std::wstring javaType = GetPropertyAccessorCppToJavaConvertedType(argumentType);
                    if (javaType == L"Pointer")
                        importPackages.insert(L"com.sun.jna.Pointer");
                    argumentStr += javaType + L" " + argumentName;
                }
                result += argumentStr + L");\r\n";
                pos = endPos;
            } else if (IsStartWith(content, dllInterfaceExportPropertyAccessorString, pos)) {
                importPackages.insert(L"com.sun.jna.Pointer");
                result += INDENT + L"void ReadString(PointerByReference ref, long property, PointerByReference value, long index);\r\n"
                    + INDENT + L"void ReadStringByKey(PointerByReference ref, long property, PointerByReference value, Pointer key);\r\n"
                    + INDENT + L"void WriteString(PointerByReference ref, long property, PointerByReference value, long index);\r\n"
                    + INDENT + L"void WriteStringByKey(PointerByReference ref, long property, PointerByReference value, Pointer key);\r\n"
                    + INDENT + L"void InsertString(PointerByReference ref, long property, PointerByReference value, long index);\r\n";
                pos += dllInterfaceExportPropertyAccessorString.length() - 1;
            } else if (IsStartWith(content, dllInterfaceExportPropertyAccessorObject, pos)) {
                importPackages.insert(L"com.sun.jna.Pointer");
                result += INDENT + L"PointerByReference ReadObject(PointerByReference ref, long property, long index);\r\n"
                    + INDENT + L"PointerByReference ReadObjectByKey(PointerByReference ref, long property, Pointer key);\r\n"
                    + INDENT + L"void WriteObject(PointerByReference ref, long property, PointerByReference value, long index);\r\n"
                    + INDENT + L"void WriteObjectByKey(PointerByReference ref, long property, PointerByReference value, Pointer key);\r\n"
                    + INDENT + L"PointerByReference AddObject(PointerByReference ref, long property, long objectType, long index);\r\n"
                    + INDENT + L"void InsertObject(PointerByReference ref, long property, PointerByReference value, long index);\r\n";
                pos += dllInterfaceExportPropertyAccessorObject.length() - 1;
            } else if (IsStartWith(content, dllInterfaceExportPropertyAccessorContainer, pos)) {
                importPackages.insert(L"com.sun.jna.Pointer");
                result += INDENT + L"long GetContainerCount(PointerByReference ref, long property);\r\n"
                    + INDENT + L"Pointer GetMapKeys(PointerByReference ref, long property);\r\n"
                    + INDENT + L"boolean IsContainKey(PointerByReference ref, long property, Pointer key);\r\n"
                    + INDENT + L"void RemoveContainerElement(PointerByReference ref, long property, long index);\r\n"
                    + INDENT + L"void RemoveContainerElementByKey(PointerByReference ref, long property, Pointer key);\r\n"
                    + INDENT + L"void ClearContainer(PointerByReference ref, long property);\r\n";
                pos += dllInterfaceExportPropertyAccessorContainer.length() - 1;
            } else if (IsStartWith(content, dllInterfaceExportPropertyAccessor, pos)) {
                importPackages.insert(L"com.sun.jna.Pointer");

                pos = Find(content, L"(", pos);
                if (pos == std::wstring::npos)
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"DllFunctions.h macro " + dllInterfaceExportPropertyAccessor + L" missing (");
                pos++;
                size_t endPos = Find(content, L")", pos);
                if (endPos == std::wstring::npos)
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"DllFunctions.h macro " + dllInterfaceExportPropertyAccessor + L" missing )");

                std::wstring subStr = content.substr(pos, endPos - pos);
                Trim(subStr);
                std::vector<std::wstring> tokens = SplitString(subStr, {L","});
                if (tokens.size() != 2)
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"DllFunctions.h macro " + dllInterfaceExportPropertyAccessor + L" missing does not have 2 arguments");
                std::wstring type = tokens[0];
                Trim(type);
                std::wstring name = tokens[1];
                Trim(name);
                std::wstring convectedType = GetPropertyAccessorCppToJavaConvertedType(type);
                result += INDENT + convectedType + L" Read" + name + L"(PointerByReference ref, long property, long index);\r\n"
                    + INDENT + convectedType + L" Read" + name + L"ByKey(PointerByReference ref, long property, Pointer key);\r\n"
                    + INDENT + L"void Write" + name + L"(PointerByReference ref, long property, " + convectedType + L" value, long index);\r\n"
                    + INDENT + L"void Write" + name + L"ByKey(PointerByReference ref, long property, " + convectedType + L" value, Pointer key);\r\n"
                    + INDENT + L"void Insert" + name + L"(PointerByReference ref, long property, " + convectedType + L" value, long index);\r\n";
                pos = endPos;
            } 
            GetNextCharPos(content, pos, false);
        }
        std::wstring importPackageStr = L"";
        for (auto const &str : importPackages)
            importPackageStr += L"import " + str + L";\r\n";

        result = L"package " + packageFolder + L";\r\n"
            "\r\n"
            + importPackageStr
            + L"\r\n"
            "public interface " + filePrefix + L"DllFunctions extends Library {\r\n"
            + INDENT + filePrefix + L"DllFunctions Instance = loadLibrary();\r\n"
            "\r\n"
            + INDENT + L"static private " + filePrefix + L"DllFunctions loadLibrary() {\r\n"
            + INDENT + INDENT + L"String prefix = System.getProperty(\"os.name\").startsWith(\"Windows\") ? \"lib\" : \"\";\r\n"
            + INDENT + INDENT + L"return (" + filePrefix + L"DllFunctions)Native.load(prefix + \"vpg\", " + filePrefix + L"DllFunctions.class);\r\n"
            + INDENT + L"}\r\n"
            "\r\n"
            + result
            + L"}\r\n";
    CATCH
    return result;
}

void VPGJavaGenerationService::GenerateJavaBridge(const LogProperty *logProperty, const std::wstring &targetWorkspace, const std::wstring &dllInterfacehppFilePath, const VPGGenerationOption *option)
{
    TRY
        assert(option != nullptr);
        if (!IsFileExists(dllInterfacehppFilePath))
            return;

        std::shared_ptr<VPGGenerationOptionExport> javaOption = VPGJavaGenerationService::GetJavaOption(option);
        if (javaOption == nullptr || IsBlank(javaOption->GetWorkspace()) || IsBlank(javaOption->GetDllBridgeDirectory()))
            return;
        
        std::wstring filePrefix = option->GetProjectPrefix();
        Trim(filePrefix);
        ToUpper(filePrefix);
        std::wstring javaFileName = filePrefix + JAVA_BRIDGE_FILE_NAME;
        std::wstring workspace = IsAbsolutePath(javaOption->GetWorkspace()) ? javaOption->GetWorkspace() : ConcatPaths({ targetWorkspace, javaOption->GetWorkspace() });
        std::wstring filePath = ConcatPaths({ workspace, javaOption->GetDllBridgeDirectory(), javaFileName });
        LogService::LogInfo(logProperty, LOG_ID, L"Generate Java Bridge: " + filePath);
        WriteFile(filePath, VPGJavaGenerationService::GenerateJavaBridgeContent(ReadFile(dllInterfacehppFilePath), option), true);
        LogService::LogInfo(logProperty, LOG_ID, L"Generate Java Bridge completed.");
    CATCH
}

std::wstring VPGJavaGenerationService::GenerateEnumContent(const std::wstring &projectPrefix, const VPGEnumClass *enumClass, const std::wstring &middlePath, const VPGGenerationOptionExport *option)
{
    std::wstring result = L"";
    TRY
        std::wstring enumClassName = (!IsStartWith(enumClass->GetName(), projectPrefix) ? projectPrefix  : L"") + enumClass->GetName();
        std::wstring packageFolder = GetJavaPactkage(option->GetTypeDirectory(), middlePath, L"Type Directory");
        result += L"package " + packageFolder + L";\r\n"
            "\r\n"
            "import java.util.Objects;\r\n"
            "\r\n"
            "public enum " + enumClassName + L" {\r\n";
        int64_t enumValue = 0;
        for (size_t i = 0; i < enumClass->GetProperties().size(); i++) {
            auto const &property = enumClass->GetProperties().at(i);
            if (property->GetEnumValue() > -1)
                enumValue = property->GetEnumValue();
            result += INDENT + property->GetEnum() + L"(" + std::to_wstring(enumValue) + L")" 
                + (i == enumClass->GetProperties().size() - 1 ? L";" : L",") + L"\r\n";
            enumValue++;
        }
        result += L"\r\n"
            + INDENT + L"public final Integer value;\r\n"
            "\r\n"
            + INDENT + enumClassName + L"(Integer value) {\r\n"
            + INDENT + INDENT + L"this.value = value;\r\n"
            + INDENT + L"}\r\n"
            "\r\n"
            + INDENT + L"public Integer getValue() {\r\n"
            + INDENT + INDENT + L"return value;\r\n"
            + INDENT + L"}\r\n"
            "\r\n"
            + INDENT + L"public static " + enumClassName + L" parse(Integer value) {\r\n"
            + INDENT + INDENT + L"for (" + enumClassName + L" type : values()) {\r\n"
            + INDENT + INDENT + INDENT + L"if (Objects.equals(type.value, value)) {\r\n"
            + INDENT + INDENT + INDENT + INDENT + L"return type;\r\n"
            + INDENT + INDENT + INDENT + L"}\r\n"
            + INDENT + INDENT + L"}\r\n"
            + INDENT + INDENT + L"throw new IllegalArgumentException(\"Invalid " + enumClassName + L" value: \" + value);\r\n"
            + INDENT + L"}\r\n"
            "\r\n"
            "}\r\n";
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::GetGetterSetterMapKeyContent(const std::wstring &classPropertyEnum, const std::wstring &dllInstantPrefix, const std::wstring &javaType1,
    const std::wstring &orginalFunction, const bool &isReturnNeeded)
{
    std::wstring result = L"";
    TRY
        if (javaType1 == L"String")
            result += INDENT + INDENT + L"Pointer keyPtr = new Memory(Native.WCHAR_SIZE * (key.length() + 1));\r\n"
                + INDENT + INDENT + L"keyPtr.setWideString(0, key);\r\n";
        else if (IsCaptial(javaType1)) {
            // Enum
            result += INDENT + INDENT + L"Pointer keyPtr = new Memory(Native.getNativeSize(long.class));\r\n"
                + INDENT + INDENT + L"keyPtr.setLong(0, (long)key.getValue());\r\n";
        } else {
            std::wstring capitialType = javaType1;
            ToCapital(capitialType);
            result += INDENT + INDENT + L"Pointer keyPtr = new Memory(Native.getNativeSize(" + javaType1 + L".class));\r\n"
                + INDENT + INDENT + L"keyPtr.set" + capitialType + L"(0, key);\r\n";
        }
        result += INDENT + INDENT + (isReturnNeeded ? L"return " : L"") + dllInstantPrefix + orginalFunction + L"(Reference, " + classPropertyEnum + L", keyPtr);\r\n";
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::GenerateObjectGetterSetterContainerCount(const VPGEnumClassProperty *property, const std::wstring &projectPrefix, const std::wstring &objectProperty, bool isVector, bool isMap, bool isSet)
{
    std::wstring result = L"";
    TRY
        if (!isVector && !isMap && !isSet)
            return result;

        std::wstring dllInstantPrefix = projectPrefix + L"DllFunctions.Instance.";
        std::wstring classPropertyEnum = objectProperty + L"." + property->GetEnum() + L".getValue()";

        result += L"\r\n"
        + INDENT + L"public long get" + property->GetPropertyName() + L"Count() {\r\n"
        + INDENT + INDENT + L"return " + dllInstantPrefix + L"GetContainerCount(Reference, " + classPropertyEnum + L");\r\n"
        + INDENT + L"}\r\n";
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::GenerateObjectGetterSetterContainer(const VPGEnumClassProperty *property, const std::wstring &projectPrefix, const std::wstring &objectProperty,
    const std::wstring &cppType1, const std::wstring &javaType1,
    bool isVector, bool isMap, bool isSet, std::set<std::wstring> &importFiles)
{
    std::wstring result = L"";
    TRY
        if (!isVector && !isMap && !isSet)
            return result;
        
        bool isAllowWrite = property->GetAccessMode() == VPGEnumClassPropertyAccessMode::ReadWrite || property->GetAccessMode() == VPGEnumClassPropertyAccessMode::WriteOnly;
        std::wstring dllInstantPrefix = projectPrefix + L"DllFunctions.Instance.";
        std::wstring classPropertyEnum = objectProperty + L"." + property->GetEnum() + L".getValue()";
        std::wstring macro = property->GetMacro().substr(0, property->GetMacro().find(L"("));

        if (isMap) {
            importFiles.insert(L"java.util.HashSet");
            importFiles.insert(L"java.util.Set");

            std::wstring javaCaptitalType = GetJavaGetterSetterJavaTypeToJavaCaptialType(javaType1);
            std::wstring getFromPointer = L"ptr.get" + javaCaptitalType + L"(0)";
            if (cppType1 == L"std::wstring")
                getFromPointer = L"ptr.getWideString(0)";
            else if (IsCaptial(cppType1)) {
                if (IsContain(macro, L"SPTR"))
                    getFromPointer = L"ptr.getLong(0)";
                else
                    getFromPointer = javaType1 + L".parse((int)ptr.getLong(0))";
            }
            result +=  L"\r\n"
                + INDENT + L"public Set<" + javaCaptitalType + L"> get" + property->GetPropertyName() + L"Keys() {\r\n"
                + INDENT + INDENT + L"Set<" + javaCaptitalType + L"> result = new HashSet<>();\r\n"
                + INDENT + INDENT + L"Pointer ptrs = VPGDllFunctions.Instance.GetMapKeys(Reference, " + classPropertyEnum + L");\r\n"
                + INDENT + INDENT + L"long total = get" + property->GetPropertyName() + L"Count();\r\n"
                + INDENT + INDENT + L"for (var ptr : ptrs.getPointerArray(0)) {\r\n"
                + INDENT + INDENT + INDENT + L"if (ptr == null) {\r\n"
                + INDENT + INDENT + INDENT + INDENT + L"break;\r\n"
                + INDENT + INDENT + INDENT + L"}\r\n"
                + INDENT + INDENT + INDENT + L"result.add(" + getFromPointer + L");\r\n"
                + INDENT + INDENT + INDENT + L"if (result.size() >= total) {\r\n"
                + INDENT + INDENT + INDENT + INDENT + L"break;\r\n"
                + INDENT + INDENT + INDENT + L"}\r\n"
                + INDENT + INDENT + L"}\r\n"
                + INDENT + INDENT + L"return result;\r\n"
                + INDENT + L"}\r\n"
                "\r\n"
                + INDENT + L"public boolean is" + property->GetPropertyName() + L"ContainKey(" + javaType1 + L" key) {\r\n"
                + GetGetterSetterMapKeyContent(classPropertyEnum, dllInstantPrefix, javaType1, L"IsContainKey", true)
                + INDENT + L"}\r\n";            
        }

        if (isVector && isAllowWrite)
            result += L"\r\n"
                + INDENT + L"public void remove" + property->GetPropertyName() + L"At(long index) {\r\n"
                + INDENT + INDENT + dllInstantPrefix + L"RemoveContainerElement(Reference, " + classPropertyEnum + L", index);\r\n"
                + INDENT + L"}\r\n";

        if (isMap && isAllowWrite) {
            result += L"\r\n"
                + INDENT + L"public void remove" + property->GetPropertyName() + L"ByKey(" + javaType1 + L" key) {\r\n"
                + GetGetterSetterMapKeyContent(classPropertyEnum, dllInstantPrefix, javaType1, L"RemoveContainerElementByKey", false)
                + INDENT + L"}\r\n";
        }
            
        if (isSet) {
            // TODO: no handle at the moment
        }
        
        if (isAllowWrite)
            result += L"\r\n"
                + INDENT + L"public void clear" + property->GetPropertyName() + L"() {\r\n"
                + INDENT + INDENT + dllInstantPrefix + L"ClearContainer(Reference, " + classPropertyEnum + L");\r\n"
                + INDENT + L"}\r\n";
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::GenerateObjectGetterSetterRead(const VPGEnumClassProperty *property, const std::wstring &projectPrefix, const std::wstring &objectProperty,
    const std::wstring &macro, const std::wstring &cppType1, const std::wstring &javaType1, const std::wstring &cppType2, const std::wstring &javaType2,
    bool isVector, bool isMap, bool isSet, std::set<std::wstring> &importFiles)
{
    std::wstring result = L"";
    TRY
        bool isAllowRead = property->GetAccessMode() == VPGEnumClassPropertyAccessMode::ReadWrite || property->GetAccessMode() == VPGEnumClassPropertyAccessMode::ReadOnly;
        if (!isAllowRead)
            return result;

        std::wstring dllInstantPrefix = projectPrefix + L"DllFunctions.Instance.";
        std::wstring classPropertyEnum = objectProperty + L"." + property->GetEnum() + L".getValue()";
        
        std::wstring javaFunctionNameSuffix = L"";
        std::wstring javaFunctionArgument = L"";
        std::wstring dllFunctionIndex = L"-1";
        if (isVector) {
            javaFunctionNameSuffix = L"At";
            javaFunctionArgument = L"long index";
            dllFunctionIndex = L"index";
        } else if (isMap) {
            javaFunctionNameSuffix = L"ByKey";
            javaFunctionArgument = javaType1 + L" key";
            dllFunctionIndex = L"keyPtr";
            
            importFiles.insert(L"com.sun.jna.Memory");
            importFiles.insert(L"com.sun.jna.Native");
            importFiles.insert(L"com.sun.jna.Pointer");
        } else if (isSet) {
            // not support set
            return result;
        }
        
        std::wstring returnJavaType = isMap ? javaType2 : javaType1;
        std::wstring returnCppType = isMap ? cppType2 : cppType1;

        std::wstring convertedType = L"";
        std::wstring convertedName = L"";
        std::wstring returnResult = L"";
        VPGPropertyAccessorGenerationService::GetPropertyAccessorTypeName(returnCppType, convertedType, convertedName, returnResult);

        result += L"\r\n"
            + INDENT + L"public " + returnJavaType + L" get" + property->GetPropertyName() + javaFunctionNameSuffix + L"(" + javaFunctionArgument + L") {\r\n";
        if (isMap) {
            if (javaType1 == L"String")
                result += INDENT + INDENT + L"Pointer keyPtr = new Memory(Native.WCHAR_SIZE * (key.length() + 1));\r\n"
                    + INDENT + INDENT + L"keyPtr.setWideString(0, key);\r\n";
            else if (IsCaptial(javaType1)) {
                // Enum
                result += INDENT + INDENT + L"Pointer keyPtr = new Memory(Native.getNativeSize(long.class));\r\n"
                    + INDENT + INDENT + L"keyPtr.setLong(0, (long)key.getValue());\r\n";
            } else {
                std::wstring capitialType = javaType1;
                ToCapital(capitialType);
                result += INDENT + INDENT + L"Pointer keyPtr = new Memory(Native.getNativeSize(" + javaType1 + L".class));\r\n"
                    + INDENT + INDENT + L"keyPtr.set" + capitialType + L"(0, key);\r\n";
            }
        }
        if (returnJavaType == L"String") {
            result += INDENT + INDENT + L"PointerByReference result = new PointerByReference();\r\n"
                + INDENT + INDENT + dllInstantPrefix + L"ReadString" + (isMap ? L"ByKey" : L"") + L"(Reference, " + classPropertyEnum + L", result, " + dllFunctionIndex + L");\r\n"
                + INDENT + INDENT + L"return result.getValue().getWideString(0);\r\n"; 
        } else if (IsContain(macro, L"SPTR")) {
            // Object
            result += INDENT + INDENT + L"return new " + returnJavaType + L"(" + dllInstantPrefix + L"ReadObject" + (isMap ? L"ByKey" : L"") + L"(Reference, " + classPropertyEnum + L", " + dllFunctionIndex + L"));\r\n";
        } else if (IsCaptial(returnJavaType)) {
            // Enum
            result += INDENT + INDENT + L"return " + returnJavaType + L".parse((int)" + dllInstantPrefix + L"ReadLong" + (isMap ? L"ByKey" : L"") + L"(Reference, " + classPropertyEnum + L", " + dllFunctionIndex + L"));\r\n";
        } else {
            // Normal Type
            result += INDENT + INDENT + L"return " + dllInstantPrefix + L"Read" + convertedName + (isMap ? L"ByKey" : L"") + L"(Reference, " + classPropertyEnum + L", " + dllFunctionIndex + L");\r\n";
        }
        result += INDENT + L"}\r\n";
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::GenerateObjectGetterSetterWrite(const VPGEnumClassProperty *property, const std::wstring &projectPrefix, const std::wstring &objectProperty,
    const std::wstring &macro, const std::wstring &cppType1, const std::wstring &javaType1, const std::wstring &cppType2, const std::wstring &javaType2,
    bool isVector, bool isMap, bool isSet,
    const std::map<std::wstring, std::wstring> &importFileMap, std::set<std::wstring> &importFiles)
{
    std::wstring result= L"";
    TRY
        bool isAllowWrite = property->GetAccessMode() == VPGEnumClassPropertyAccessMode::ReadWrite || property->GetAccessMode() == VPGEnumClassPropertyAccessMode::WriteOnly;
        if (!isAllowWrite)
            return result;

        std::wstring dllInstantPrefix = projectPrefix + L"DllFunctions.Instance.";
        std::wstring classPropertyEnum = objectProperty + L"." + property->GetEnum() + L".getValue()";
        
        std::wstring convertedType = L"";
        std::wstring convertedName = L"";
        std::wstring returnResult = L"";
        VPGPropertyAccessorGenerationService::GetPropertyAccessorTypeName(cppType1, convertedType, convertedName, returnResult);
        
        std::wstring javaFunctionNameSuffix = L"";
        std::wstring javaFunctionArgument = L"";
        std::wstring dllFunctionIndex = L"-1";
        if (isVector) {
            javaFunctionNameSuffix = L"At";
            javaFunctionArgument = L"long index, ";
            dllFunctionIndex = L"index";
            
            if (javaType1 == L"String") {
                importFiles.insert(L"com.sun.jna.Memory");
                importFiles.insert(L"com.sun.jna.Native");
                importFiles.insert(L"com.sun.jna.Pointer");
            }
        } else if (isMap) {
            javaFunctionNameSuffix = L"ByKey";
            javaFunctionArgument = javaType1 + L" key, ";
            dllFunctionIndex = L"keyPtr";
            
            importFiles.insert(L"com.sun.jna.Memory");
            importFiles.insert(L"com.sun.jna.Native");
            importFiles.insert(L"com.sun.jna.Pointer");
        } else if (isSet) {
            // not support set
            return result;
        } else {
            if (javaType1 == L"String") {
                importFiles.insert(L"com.sun.jna.Memory");
                importFiles.insert(L"com.sun.jna.Native");
                importFiles.insert(L"com.sun.jna.Pointer");
            }
        }

        std::wstring javaType = isMap ? javaType2 : javaType1;
        std::wstring cppType = isMap ? cppType2 : cppType1;

        result += L"\r\n"
            + INDENT + L"public void set" + property->GetPropertyName() + javaFunctionNameSuffix + L"(" + javaFunctionArgument + javaType + L" value) {\r\n";
        if (isMap) {
            if (javaType1 == L"String")
                result += INDENT + INDENT + L"Pointer keyPtr = new Memory(Native.WCHAR_SIZE * (key.length() + 1));\r\n"
                    + INDENT + INDENT + L"keyPtr.setWideString(0, key);\r\n";
            else if (IsCaptial(javaType1)) {
                // Enum
                result += INDENT + INDENT + L"Pointer keyPtr = new Memory(Native.getNativeSize(long.class));\r\n"
                    + INDENT + INDENT + L"keyPtr.setLong(0, (long)key.getValue());\r\n";
            } else {
                std::wstring capitialType = javaType1;
                ToCapital(capitialType);
                result += INDENT + INDENT + L"Pointer keyPtr = new Memory(Native.getNativeSize(" + javaType1 + L".class));\r\n"
                    + INDENT + INDENT + L"keyPtr.set" + capitialType + L"(0, key);\r\n";
            }
        }
        if (javaType == L"String") {
            result += INDENT + INDENT + L"Pointer valuePtr = new Memory(Native.WCHAR_SIZE * (value.length() + 1));\r\n"
                + INDENT + INDENT + L"valuePtr.setWideString(0, value);\r\n"
                + INDENT + INDENT + L"PointerByReference valueReference = new PointerByReference();\r\n"
                + INDENT + INDENT + L"valueReference.setValue(valuePtr);\r\n"
                + INDENT + INDENT + dllInstantPrefix + L"WriteString" + (isMap ? L"ByKey" : L"") + L"(Reference, " + classPropertyEnum + L", valueReference, " + dllFunctionIndex + L");\r\n";
        } else if (IsContain(macro, L"SPTR")) {
            // Object
            result += INDENT + INDENT + dllInstantPrefix + L"WriteObject" + (isMap ? L"ByKey" : L"") + L"(Reference, " + classPropertyEnum + L", value.Reference, " + dllFunctionIndex + L");\r\n";
        } else if (IsCaptial(cppType)) {
            // Enum
            result += INDENT + INDENT + dllInstantPrefix + L"WriteLong" + (isMap ? L"ByKey" : L"") + L"(Reference, " + classPropertyEnum + L", value.getValue(), " + dllFunctionIndex + L");\r\n";
        } else {
            result += INDENT + INDENT + dllInstantPrefix + L"Write" + convertedName + (isMap ? L"ByKey" : L"") + L"(Reference, " + classPropertyEnum + L", value, " + dllFunctionIndex + L");\r\n";
        }
        result += INDENT + L"}\r\n";

        if (isVector) {
            if ((IsContain(macro, L"SPTR"))) {
                std::wstring objectTypeClass = projectPrefix + L"ObjectType";
                if (importFileMap.find(L"ObjectType") != importFileMap.end())
                    importFiles.insert(importFileMap.find(L"ObjectType")->second + L"." + objectTypeClass);

                std::wstring objectType = cppType1;
                if (IsStartWith(cppType1, projectPrefix))
                    objectType = cppType1.substr(projectPrefix.length());
                    
                result += L"\r\n"
                    + INDENT + L"public " + javaType1 + L" add" + property->GetPropertyName() + L"() {\r\n"
                    + INDENT + INDENT + L"return add" + property->GetPropertyName() + javaFunctionNameSuffix + L"(-1);\r\n"
                    + INDENT + L"}\r\n"
                    "\r\n"
                    + INDENT + L"public " + javaType1 + L" add" + property->GetPropertyName() + javaFunctionNameSuffix + L"(long index) {\r\n"
                    + INDENT + INDENT + L"return new " + cppType1 + L"(" + dllInstantPrefix + L"AddObject(Reference, " + classPropertyEnum + L", " + objectTypeClass + L"." + objectType + L".getValue(), " + dllFunctionIndex + L"));\r\n"
                    + INDENT + L"}\r\n";
            }
        }
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::GenerateObjectGetterSetterInsert(const VPGEnumClassProperty *property, const std::wstring &projectPrefix, const std::wstring &objectProperty,
    const std::wstring &macro, const std::wstring &cppType1, const std::wstring &javaType1,
    bool isVector, std::set<std::wstring> &importFiles)
{
    std::wstring result = L"";
    TRY
        if (!isVector)
            return result;
        
        bool isAllowWrite = property->GetAccessMode() == VPGEnumClassPropertyAccessMode::ReadWrite || property->GetAccessMode() == VPGEnumClassPropertyAccessMode::WriteOnly;
        if (!isAllowWrite)
            return result;

        std::wstring dllInstantPrefix = projectPrefix + L"DllFunctions.Instance.";
        std::wstring classPropertyEnum = objectProperty + L"." + property->GetEnum() + L".getValue()";
        
        std::wstring convertedType = L"";
        std::wstring convertedName = L"";
        std::wstring returnResult = L"";
        VPGPropertyAccessorGenerationService::GetPropertyAccessorTypeName(cppType1, convertedType, convertedName, returnResult);

        std::wstring javaFunctionNameSuffix = L"At";
        std::wstring dllFunctionIndex = L"index";

        result += L"\r\n"
            + INDENT + L"public void insert" + property->GetPropertyName() + L"(" + javaType1 + L" value) {\r\n"
            + INDENT+ INDENT + L"insert" + property->GetPropertyName() + javaFunctionNameSuffix + L"(-1, value);\r\n"
            + INDENT + L"}\r\n"
            "\r\n";
        result += INDENT + L"public void insert" + property->GetPropertyName() + javaFunctionNameSuffix + L"(long index, " + javaType1 + L" value) {\r\n";
        if (javaType1 == L"String") {
            importFiles.insert(L"com.sun.jna.Memory");
            importFiles.insert(L"com.sun.jna.Native");
            importFiles.insert(L"com.sun.jna.Pointer");

            result += INDENT + INDENT + L"Pointer valuePtr = new Memory(Native.WCHAR_SIZE * (value.length() + 1));\r\n"
                + INDENT + INDENT + L"valuePtr.setWideString(0, value);\r\n"
                + INDENT + INDENT + L"PointerByReference valueReference = new PointerByReference();\r\n"
                + INDENT + INDENT + L"valueReference.setValue(valuePtr);\r\n"
                + INDENT + INDENT + dllInstantPrefix + L"InsertString(Reference, " + classPropertyEnum + L", valueReference, " + dllFunctionIndex + L");\r\n";
        } else if (IsContain(macro, L"SPTR")) {
            // Object
            result += INDENT + INDENT + dllInstantPrefix + L"InsertObject(Reference, " + classPropertyEnum + L", value.Reference, " + dllFunctionIndex + L");\r\n";
        } else if (IsCaptial(cppType1)) {
            // Enum
            result += INDENT + INDENT + dllInstantPrefix + L"InsertLong(Reference, " + classPropertyEnum + L", value.getValue(), " + dllFunctionIndex + L");\r\n";
        } else {
            result += INDENT + INDENT + dllInstantPrefix + L"Insert" + convertedName + L"(Reference, " + classPropertyEnum + L", value, " + dllFunctionIndex + L");\r\n";
        }
        result += INDENT + L"}\r\n";
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::GenerateObjectGetterSetter(const std::wstring &projectPrefix, const std::wstring &objectProperty, const VPGEnumClassProperty *property, const std::map<std::wstring, std::wstring> &importFileMap, std::set<std::wstring> &importFiles)
{
    std::wstring result = L"";
    TRY
        if (IsBlank(property->GetMacro()) || property->GetAccessMode() == VPGEnumClassPropertyAccessMode::NoAccess)
            return result;
        std::wstring macro = property->GetMacro().substr(0, property->GetMacro().find(L"("));
        std::wstring dllInstantPrefix = projectPrefix + L"DllFunctions.Instance.";
        bool isAllowRead = property->GetAccessMode() == VPGEnumClassPropertyAccessMode::ReadWrite || property->GetAccessMode() == VPGEnumClassPropertyAccessMode::ReadOnly;
        bool isAllowWrite = property->GetAccessMode() == VPGEnumClassPropertyAccessMode::ReadWrite || property->GetAccessMode() == VPGEnumClassPropertyAccessMode::WriteOnly;

        if (!isAllowRead && !isAllowWrite)
            return result;
        
        std::wstring cppType1 = property->GetType1();
        if (IsBlank(cppType1))
            return result;

        std::wstring javaType1 = std::iswupper(cppType1[0]) ? cppType1 : GetJavaGetterSetterCppToJavaConvertedType(cppType1);
        std::wstring cppType2 = property->GetType2();
        std::wstring javaType2 = !IsBlank(cppType2) ? (std::iswupper(cppType2[0]) ? cppType2 : GetJavaGetterSetterCppToJavaConvertedType(cppType2)) : cppType2;
        if (!IsBlank(cppType1) && std::iswupper(cppType1[0]) && importFileMap.find(javaType1) != importFileMap.end())
            importFiles.insert(importFileMap.find(javaType1)->second + L"." + javaType1);
        if (!IsBlank(cppType2) && std::iswupper(cppType2[0]) && importFileMap.find(javaType2) != importFileMap.end())
            importFiles.insert(importFileMap.find(javaType2)->second + L"." + javaType2);
        
        // add import file
        if (!javaType1.empty() && std::iswupper(javaType1[0])) {
            if (importFileMap.find(javaType1) != importFileMap.end())
                importFiles.insert(importFileMap.find(javaType1)->second + L"." + javaType1);
        }
        if (!javaType2.empty() && std::iswupper(javaType2[0])) {
            if (importFileMap.find(javaType2) != importFileMap.end())
                importFiles.insert(importFileMap.find(javaType2)->second + L"." + javaType2);
        }
        // not support set
        // container
        bool isVector = IsStartWith(macro, L"VECTOR") || IsStartWith(macro, L"ORDERED_MAP");
        bool isMap = IsStartWith(macro, L"MAP") || IsStartWith(macro, L"ORDERED_MAP");
        bool isSet = IsStartWith(macro, L"SET");
        result += GenerateObjectGetterSetterContainerCount(property, projectPrefix, objectProperty, isVector, isMap, isSet);

        if (!isVector && !isMap && !isSet) {
            result += GenerateObjectGetterSetterRead(property, projectPrefix, objectProperty,
                macro, cppType1, javaType1, cppType2, javaType2,
                false, false, false,
                importFiles);
            result += GenerateObjectGetterSetterWrite(property, projectPrefix, objectProperty,
                macro, cppType1, javaType1, cppType2, javaType2,
                false, false, false,
                importFileMap, importFiles);
        } else {
            if (isVector) {
                result += GenerateObjectGetterSetterRead(property, projectPrefix, objectProperty,
                    macro, cppType1, javaType1, cppType2, javaType2,
                    true, false, false,
                    importFiles);
                result += GenerateObjectGetterSetterWrite(property, projectPrefix, objectProperty,
                    macro, cppType1, javaType1, cppType2, javaType2,
                    true, false, false,
                    importFileMap, importFiles);
            }
            if (isMap) {
                result += GenerateObjectGetterSetterRead(property, projectPrefix, objectProperty,
                    macro, cppType1, javaType1, cppType2, javaType2,
                    false, true, false,
                    importFiles);
                result += GenerateObjectGetterSetterWrite(property, projectPrefix, objectProperty,
                    macro, cppType1, javaType1, cppType2, javaType2,
                    false, true, false,
                    importFileMap, importFiles);
            }
            if (isSet) {
                result += GenerateObjectGetterSetterRead(property, projectPrefix, objectProperty,
                    macro, cppType1, javaType1, cppType2, javaType2,
                    false, false, true,
                    importFiles);
                result += GenerateObjectGetterSetterWrite(property, projectPrefix, objectProperty,
                    macro, cppType1, javaType1, cppType2, javaType2,
                    false, false, true,
                    importFileMap, importFiles);
            }
        }

        result += GenerateObjectGetterSetterInsert(property, projectPrefix, objectProperty,
            macro, cppType1, javaType1,
            isVector, importFiles);

        // container
        result += GenerateObjectGetterSetterContainer(property, projectPrefix, objectProperty,
            cppType1, javaType1,
            isVector, isMap, isSet,
            importFiles);
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::GenerateObjectContent(const std::wstring &projectPrefix, const VPGEnumClass *enumClass, const std::wstring &middlePath, const std::map<std::wstring, std::wstring> &importFileMap, const VPGGenerationOptionExport *option)
{
    std::wstring result = L"";
    TRY
        std::wstring packageFolder = GetJavaPactkage(option->GetObjectDirectory(), middlePath, L"Object Directory");
        std::wstring objectName = enumClass->GetName();
        objectName = (!IsStartWith(objectName, projectPrefix) ? projectPrefix : L"") + objectName.substr(0, objectName.size() - propertyClassNameSuffix.size());
        
        std::set<std::wstring> importFiles;
        importFiles.insert(L"com.sun.jna.ptr.PointerByReference");
        importFiles.insert(GetJavaPactkage(option->GetDllBridgeDirectory(), L"", L"DLL Interface Directory") + L"." + (!projectPrefix.empty() ? projectPrefix : L"") + L"DllFunctions");
        importFiles.insert(GetJavaPactkage(option->GetTypeDirectory(), middlePath, L"Type Directory") + L"." + enumClass->GetName());

        std::wstring getterSetterStr = L"";
        for (auto const &property : enumClass->GetProperties())
            getterSetterStr += VPGJavaGenerationService::GenerateObjectGetterSetter(projectPrefix, enumClass->GetName(), property.get(), importFileMap, importFiles);
        
        result += L"package " + packageFolder + L";\r\n"
            "\r\n";
        for (auto const &str : importFiles) {
            // same package no need to import
            if (IsStartWith(str, packageFolder + L".")
                && CountSubstring(str.substr(packageFolder.size()), L".") < 2)
                continue;
            result += L"import " + str + L";\r\n";
        }
        result += L"\r\n"
            "public class " + objectName + L" {\r\n"
            + INDENT + L"public PointerByReference Reference = null;\r\n"
            "\r\n"
            + INDENT + L"public " + objectName + L"(PointerByReference reference) {\r\n"
            + INDENT + INDENT + L"this.Reference = reference;\r\n"
            + INDENT + L"}\r\n"
            + getterSetterStr
            + L"}\r\n";
    CATCH
    return result;
}

void VPGJavaGenerationService::GenerateEnum(const LogProperty *logProperty, const std::wstring &filePath, const std::wstring &cppMiddlePath, const VPGEnumClass *enumClass, const VPGGenerationOption *option, const VPGGenerationOptionExport *javaOption)
{
    TRY
        assert(option != nullptr);
        assert(javaOption != nullptr);
        if (filePath.empty())
            return;
        
        std::wstring tmpFilePath = GetParentPath(filePath);
        ToLower(tmpFilePath);
        tmpFilePath = ConcatPaths({ tmpFilePath, GetFileName(filePath) });

        LogService::LogInfo(logProperty, LOG_ID, L"Generate Java Enum: " + tmpFilePath);
        WriteFile(tmpFilePath, VPGJavaGenerationService::GenerateEnumContent(option->GetProjectPrefix(), enumClass, cppMiddlePath, javaOption), true);
        LogService::LogInfo(logProperty, LOG_ID, L"Generate Java Enum completed.");
    CATCH
}

void VPGJavaGenerationService::GenerateObject(const LogProperty *logProperty, const std::wstring &filePath, const std::wstring &cppMiddlePath, const VPGEnumClass *enumClass,
    const std::map<std::wstring, std::wstring> &typeWorkspaceClassRelativePathMap,
    const VPGGenerationOption *option, const VPGGenerationOptionExport *javaOption)
{
    TRY
        assert(option != nullptr);
        assert(javaOption != nullptr);
        if (filePath.empty())
            return;
            
        std::wstring tmpFilePath = GetParentPath(filePath);
        ToLower(tmpFilePath);
        tmpFilePath = ConcatPaths({ tmpFilePath, GetFileName(filePath) });
       
        std::wstring objectName = enumClass->GetName();
        if (!IsEndWith(objectName, propertyClassNameSuffix))
            return;

        std::map<std::wstring, std::wstring> importFileMap;
        for (auto const &relativePath : typeWorkspaceClassRelativePathMap) {
            std::wstring enumClassName = relativePath.first;
            if (!option->GetProjectPrefix().empty() && !IsStartWith(enumClassName, enumClassName))
                enumClassName = option->GetProjectPrefix() + enumClassName;
            importFileMap.insert(std::make_pair(enumClassName, GetJavaPactkage(javaOption->GetTypeDirectory(), relativePath.second, L"Type Directory")));
            if (IsEndWith(enumClassName, propertyClassNameSuffix)) {
                std::wstring tmpObjectName = enumClassName.substr(0, enumClassName.size() - propertyClassNameSuffix.size());
                importFileMap.insert(std::make_pair(tmpObjectName, GetJavaPactkage(javaOption->GetObjectDirectory(), relativePath.second, L"Object Directory")));
            }
        }
        LogService::LogInfo(logProperty, LOG_ID, L"Generate Java Class: " + tmpFilePath);
        WriteFile(tmpFilePath, VPGJavaGenerationService::GenerateObjectContent(option->GetProjectPrefix(), enumClass, cppMiddlePath, importFileMap, javaOption), true);
        LogService::LogInfo(logProperty, LOG_ID, L"Generate Java Class completed.");
    CATCH
}