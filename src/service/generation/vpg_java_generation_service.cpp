#include "vpg_java_generation_service.hpp"

#include <assert.h>
#include <filesystem>
#include <fstream>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_config.hpp"

#include "vpg_class_helper.hpp"
#include "vpg_file_generation_manager.hpp"
#include "vpg_config.hpp"
#include "vpg_config_type.hpp"
#include "vpg_property_accessor_generation_service.hpp"

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

std::shared_ptr<VPGConfigExport> VPGJavaGenerationService::getJavaOption(const VPGConfig *option)
{
    TRY
        for (auto element : option->getExports()) {
            if (element->getInterface() == VPGConfigInterfaceType::Java) {
                return element;
            }
        }
    CATCH
    return nullptr;
}

std::wstring VPGJavaGenerationService::getJavaPactkage(const std::wstring &path, const std::wstring &middlePath, const std::wstring &filePathName)
{
    std::wstring result = L"";
    TRY
        if (!vcc::isStartWith(path, JAVA_PROJECT_SOURCE_PARENT_FOLDER))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, filePathName + L" is not start with " + JAVA_PROJECT_SOURCE_PARENT_FOLDER);

        result = vcc::getRelativePath(path, JAVA_PROJECT_SOURCE_PARENT_FOLDER);
        if (!middlePath.empty())
            result = vcc::concatPaths({ result, middlePath });
        result = vcc::getLinuxPath(result);
        if (vcc::isStartWith(result, L"/"))
            result = result.substr(1);
        if (vcc::isEndWith(result, L"/"))
            result.pop_back();
        vcc::replaceAll(result, L"/", L".");
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::getJavaPactkageObject(const VPGEnumClass *enumClass, const VPGConfigExport *option, const std::wstring &middlePath)
{
    std::wstring result = L"";
    TRY
        std::wstring parentPath = option->getObjectDirectory();
        std::wstring filePathName = L"Object Directory";
        if (enumClass->getType() == VPGEnumClassType::Form) {
            if (!vcc::isBlank(option->getFormDirectory()))
                parentPath = option->getFormDirectory();
            filePathName = L"File Directory";
        }
        result = getJavaPactkage(parentPath, middlePath, filePathName);
    CATCH
    return result;
}

std::map<std::wstring, std::wstring> VPGJavaGenerationService::getImportFileMap(const std::wstring &projectPrefix, const VPGConfigExport *option, const std::map<std::wstring, std::wstring> &typeWorkspaceClassRelativePathMapObject, const std::map<std::wstring, std::wstring> &typeWorkspaceClassRelativePathMapForm)
{
    std::map<std::wstring, std::wstring> importFileMap;
    TRY
        for (auto const &relativePath : typeWorkspaceClassRelativePathMapObject) {
            std::wstring enumClassName = getTypeOrClassWithoutNamespace(relativePath.first);
            if (!projectPrefix .empty() && !vcc::isStartWith(enumClassName, projectPrefix))
                enumClassName = projectPrefix  + enumClassName;
            importFileMap.insert(std::make_pair(enumClassName, getJavaPactkage(option->getTypeDirectory(), relativePath.second, L"Type Directory")));
            if (vcc::isEndWith(enumClassName, propertyClassNameSuffix)) {
                std::wstring tmpObjectName = enumClassName.substr(0, enumClassName.size() - propertyClassNameSuffix.size());
                importFileMap.insert(std::make_pair(tmpObjectName, getJavaPactkage(option->getObjectDirectory(), relativePath.second, L"Object Directory")));
            }
        }
        for (auto const &relativePath : typeWorkspaceClassRelativePathMapForm) {
            std::wstring enumClassName = getTypeOrClassWithoutNamespace(relativePath.first);
            if (!projectPrefix .empty() && !vcc::isStartWith(enumClassName, projectPrefix ))
                enumClassName = projectPrefix  + enumClassName;
            importFileMap.insert(std::make_pair(enumClassName, getJavaPactkage(option->getTypeDirectory(), relativePath.second, L"Type Directory")));
            if (vcc::isEndWith(enumClassName, propertyClassNameSuffix)) {
                std::wstring tmpObjectName = enumClassName.substr(0, enumClassName.size() - propertyClassNameSuffix.size());
                std::wstring directory = !vcc::isBlank(option->getFormDirectory()) ? option->getFormDirectory() : option->getObjectDirectory();
                importFileMap.insert(std::make_pair(tmpObjectName, getJavaPactkage(directory, relativePath.second, L"Form Directory")));
            }
        }
    CATCH
    return importFileMap;
}

std::wstring VPGJavaGenerationService::getOperationResultFilePath(const std::wstring &projectPrefix, const VPGConfigExport *option)
{
    TRY
        return vcc::concatPaths({ option->getObjectDirectory(), projectPrefix + L"OperationResult.java" });
    CATCH
    return L"";
}

std::wstring VPGJavaGenerationService::getPropertyAccessorCppToJavaConvertedType(const std::wstring &cppType)
{
    std::wstring result = L"";
    TRY
        result = getJavaGetterSetterCppToJavaConvertedType(cppType);
        if (result == L"String")
            result = L"PointerByReference";
        
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::getJavaGetterSetterCppToJavaConvertedType(const std::wstring &cppType)
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
        } else if (vcc::isContain(cppType, L"int")
            || vcc::isContain(cppType, L"short")
            || vcc::isContain(cppType, L"long")
            || cppType == L"size_t"
            || cppType == L"time_t") {
            return L"long";
        } else if (cppType == L"float") {
            return L"float";
        } else if (cppType == L"double") {
            return L"double";
        } else if (cppType == L"string"
            || cppType == L"wstring") {
            return L"String";
        } else if (cppType == L"void**" || cppType == L"void **") {
            return L"PointerByReference";
        } else if (cppType == L"void*" || cppType == L"void *") {
            return L"Pointer";
        } else if (cppType == L"void") {
            return L"void";
        } else
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Unknown type: " + cppType);
    CATCH
    return L"";
}

std::wstring VPGJavaGenerationService::getJavaGetterSetterJavaTypeToJavaCaptialType(const std::wstring &javaType)
{
    TRY
        if (vcc::isCapital(javaType))
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
            vcc::toCapital(result);
            return result;
        }
    CATCH
    return L"";
}

std::wstring VPGJavaGenerationService::GenerateJavaBridgeContent(const std::wstring &content, const VPGConfig *option)
{
    std::wstring result = L"";
    TRY
        auto javaOption = VPGJavaGenerationService::getJavaOption(option);
        if (javaOption == nullptr || vcc::isBlank(javaOption->getWorkspace()) || vcc::isBlank(javaOption->getDllBridgeDirectory()))
            return result;

        std::wstring filePrefix = option->getProjectPrefix();
        vcc::trim(filePrefix);
        vcc::toUpper(filePrefix);

        std::set<std::wstring> importPackages;
        importPackages.insert(L"com.sun.jna.Library");
        importPackages.insert(L"com.sun.jna.Native");
        std::wstring packageFolder = getJavaPactkage(javaOption->getDllBridgeDirectory(), L"", L"Dll Bridge Directory");

        size_t pos = vcc::find(content, dllExportStart);
        if (pos == std::wstring::npos)
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"DllFunctions.h missing 'extern \"C\"'");

        pos += dllExportStart.size() - 1;
        while (pos < content.length()) {
            if (vcc::isStartWith(content, dllExport, pos)) {
                // normal function
                pos += dllExport.length();
                size_t endPos1 = vcc::find(content, L";", pos);
                size_t endPos2 = vcc::find(content, L"{", pos);
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
                vcc::trim(cppFunction);

                // 1. determine return is void * or not
                // 2. get function name
                // 3. pharse argument list
                size_t argumentStartPos = vcc::find(cppFunction, L"(");
                if (argumentStartPos == std::wstring::npos)
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"DllFunctions.h DLLEXPORT function " + cppFunction + L" missing arguments");
                size_t argumentEndPos = vcc::find(cppFunction, L")", argumentStartPos);
                if (argumentEndPos == std::wstring::npos)
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"DllFunctions.h DLLEXPORT function " + cppFunction + L" argument list missing ending )");
                std::wstring functionNameWithReturn = cppFunction.substr(0, argumentStartPos);
                std::wstring functionArgumentList = cppFunction.substr(argumentStartPos + 1, argumentEndPos - argumentStartPos - 1);
                
                // return type and function name
                std::wstring returnType = L"";
                std::wstring functionName = L"";
                size_t tmpPos = 0;
                if (vcc::isContain(functionNameWithReturn, L"**")) {
                    returnType = L"void **";
                    tmpPos = functionNameWithReturn.find_last_of(L"*");
                    tmpPos++;
                    functionName = functionNameWithReturn.substr(tmpPos);
                } else if (vcc::isContain(functionNameWithReturn, L"*")) {
                    returnType = L"void *";
                    tmpPos = functionNameWithReturn.find_last_of(L"*");
                    tmpPos++;
                    functionName = functionNameWithReturn.substr(tmpPos);
                } else {
                    returnType = vcc::getNextString(functionNameWithReturn, tmpPos, { L" ", L"\t", L"\r", L"\n" });
                    tmpPos++;
                    functionName = vcc::getNextString(functionNameWithReturn, tmpPos, { L" ", L"\t", L"\r", L"\n" });
                }
                vcc::trim(returnType);
                vcc::trim(functionName);
                result += INDENT + getPropertyAccessorCppToJavaConvertedType(returnType) + L" " + functionName + L"(";

                // argument
                vcc::trim(functionArgumentList);
                std::wstring argumentStr = L"";
                std::vector<std::wstring> tokens =  vcc::splitString(functionArgumentList, {L","});
                
                std::wstring argumentType = L"";
                std::wstring argumentName = L"";
                for (auto const &token : tokens) {
                    tmpPos = 0;
                    if (!argumentStr.empty())
                        argumentStr += L", ";

                    if (vcc::isContain(token, L"**")) {
                        argumentType = L"void **";
                        tmpPos = token.find_last_of(L"*");
                        tmpPos++;
                        argumentName = token.substr(tmpPos);
                    } else if (vcc::isContain(token, L"*")) {
                        argumentType = L"void *";
                        tmpPos = token.find_last_of(L"*");
                        tmpPos++;
                        argumentName = token.substr(tmpPos);
                    } else {
                        argumentType = vcc::getNextString(token, tmpPos, { L" ", L"\t", L"\r", L"\n" });
                        tmpPos++;
                        argumentName = vcc::getNextString(token, tmpPos, { L" ", L"\t", L"\r", L"\n" });
                    }
                    vcc::trim(argumentType);
                    vcc::trim(argumentName);

                    std::wstring javaType = getPropertyAccessorCppToJavaConvertedType(argumentType);
                    if (javaType == L"Pointer")
                        importPackages.insert(L"com.sun.jna.Pointer");
                    else if (javaType == L"PointerByReference")
                        importPackages.insert(L"com.sun.jna.ptr.PointerByReference");

                    argumentStr += javaType + L" " + argumentName;
                }
                result += argumentStr + L");\r\n";
                pos = endPos;
            } else if (vcc::isStartWith(content, dllInterfaceExportPropertyAccessorString, pos)) {
                importPackages.insert(L"com.sun.jna.Pointer");
                importPackages.insert(L"com.sun.jna.ptr.PointerByReference");
                result += INDENT + L"void readString(Pointer ref, long property, PointerByReference value);\r\n"
                    + INDENT + L"void readStringAtIndex(Pointer ref, long property, PointerByReference value, long index);\r\n"
                    + INDENT + L"void readStringAtKey(Pointer ref, long property, PointerByReference value, Pointer key);\r\n"
                    + INDENT + L"void writeString(Pointer ref, long property, PointerByReference value);\r\n"
                    + INDENT + L"void writeStringAtIndex(Pointer ref, long property, PointerByReference value, long index);\r\n"
                    + INDENT + L"void writeStringAtKey(Pointer ref, long property, PointerByReference value, Pointer key);\r\n"
                    + INDENT + L"void insertStringAtIndex(Pointer ref, long property, PointerByReference value, long index);\r\n";
                pos += dllInterfaceExportPropertyAccessorString.length() - 1;
            } else if (vcc::isStartWith(content, dllInterfaceExportPropertyAccessorObject, pos)) {
                importPackages.insert(L"com.sun.jna.Pointer");
                result += INDENT + L"Pointer readObject(Pointer ref, long property);\r\n"
                    + INDENT + L"Pointer readObjectAtIndex(Pointer ref, long property, long index);\r\n"
                    + INDENT + L"Pointer readObjectAtKey(Pointer ref, long property, Pointer key);\r\n"
                    + INDENT + L"void writeObject(Pointer ref, long property, Pointer value);\r\n"
                    + INDENT + L"void writeObjectAtIndex(Pointer ref, long property, Pointer value, long index);\r\n"
                    + INDENT + L"void writeObjectAtKey(Pointer ref, long property, Pointer value, Pointer key);\r\n"
                    + INDENT + L"Pointer addObjectAtIndex(Pointer ref, long property, long objectType, long index);\r\n"
                    + INDENT + L"void insertObjectAtIndex(Pointer ref, long property, Pointer value, long index);\r\n";
                pos += dllInterfaceExportPropertyAccessorObject.length() - 1;
            } else if (vcc::isStartWith(content, dllInterfaceExportPropertyAccessorContainer, pos)) {
                importPackages.insert(L"com.sun.jna.Pointer");
                result += INDENT + L"long getCount(Pointer ref, long property);\r\n"
                    + INDENT + L"Pointer getMapKeys(Pointer ref, long property);\r\n"
                    + INDENT + L"boolean isContainKey(Pointer ref, long property, Pointer key);\r\n"
                    + INDENT + L"void removeObject(Pointer ref, long property, Pointer value);\r\n"
                    + INDENT + L"void removeAtIndex(Pointer ref, long property, long index);\r\n"
                    + INDENT + L"void removeAtKey(Pointer ref, long property, Pointer key);\r\n"
                    + INDENT + L"void clear(Pointer ref, long property);\r\n";
                pos += dllInterfaceExportPropertyAccessorContainer.length() - 1;
            } else if (vcc::isStartWith(content, dllInterfaceExportPropertyAccessor, pos)) {
                importPackages.insert(L"com.sun.jna.ptr.PointerByReference");

                pos = vcc::find(content, L"(", pos);
                if (pos == std::wstring::npos)
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"DllFunctions.h macro " + dllInterfaceExportPropertyAccessor + L" missing (");
                pos++;
                size_t endPos = vcc::find(content, L")", pos);
                if (endPos == std::wstring::npos)
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"DllFunctions.h macro " + dllInterfaceExportPropertyAccessor + L" missing )");

                std::wstring subStr = content.substr(pos, endPos - pos);
                vcc::trim(subStr);
                std::vector<std::wstring> tokens = vcc::splitString(subStr, {L","});
                if (tokens.size() != 2)
                    THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"DllFunctions.h macro " + dllInterfaceExportPropertyAccessor + L" missing does not have 2 arguments");
                std::wstring type = tokens[0];
                vcc::trim(type);
                std::wstring name = tokens[1];
                vcc::trim(name);
                std::wstring convectedType = getPropertyAccessorCppToJavaConvertedType(type);
                result += INDENT + convectedType + L" read" + name + L"(Pointer ref, long property);\r\n"
                    + INDENT + convectedType + L" read" + name + L"AtIndex(Pointer ref, long property, long index);\r\n"
                    + INDENT + convectedType + L" read" + name + L"AtKey(Pointer ref, long property, Pointer key);\r\n"
                    + INDENT + L"void write" + name + L"(Pointer ref, long property, " + convectedType + L" value);\r\n"
                    + INDENT + L"void write" + name + L"AtIndex(Pointer ref, long property, " + convectedType + L" value, long index);\r\n"
                    + INDENT + L"void write" + name + L"AtKey(Pointer ref, long property, " + convectedType + L" value, Pointer key);\r\n"
                    + INDENT + L"void insert" + name + L"AtIndex(Pointer ref, long property, " + convectedType + L" value, long index);\r\n";
                pos = endPos;
            } 
            vcc::getNextCharPos(content, pos, false);
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
            + INDENT + INDENT + L"var lib = (" + filePrefix + L"DllFunctions)Native.load(prefix + \"vpg\", " + filePrefix + L"DllFunctions.class);\r\n"
            + INDENT + INDENT + L"lib.applicationStart();\r\n"
            + INDENT + INDENT + L"return lib;\r\n"
            + INDENT + L"}\r\n"
            "\r\n"
            + result
            + L"}\r\n";
    CATCH
    return result;
}

void VPGJavaGenerationService::GenerateJavaBridge(const vcc::LogConfig *logConfig, const std::wstring &targetWorkspace, const std::wstring &dllInterfacehppFilePath, const VPGConfig *option)
{
    TRY
        assert(option != nullptr);
        if (!vcc::isFilePresent(dllInterfacehppFilePath))
            return;

        auto javaOption = VPGJavaGenerationService::getJavaOption(option);
        if (javaOption == nullptr || vcc::isBlank(javaOption->getWorkspace()) || vcc::isBlank(javaOption->getDllBridgeDirectory()))
            return;
        
        std::wstring filePrefix = option->getProjectPrefix();
        vcc::trim(filePrefix);
        vcc::toUpper(filePrefix);
        std::wstring javaFileName = filePrefix + JAVA_BRIDGE_FILE_NAME;
        std::wstring workspace = vcc::isAbsolutePath(javaOption->getWorkspace()) ? javaOption->getWorkspace() : vcc::concatPaths({ targetWorkspace, javaOption->getWorkspace() });
        std::wstring filePath = vcc::concatPaths({ workspace, javaOption->getDllBridgeDirectory(), javaFileName });
        vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate Java Bridge: " + filePath);
        vcc::writeFile(filePath, VPGJavaGenerationService::GenerateJavaBridgeContent(vcc::readFile(dllInterfacehppFilePath), option), true);
        vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate Java Bridge completed.");
    CATCH
}

std::wstring VPGJavaGenerationService::GenerateEnumContent(const std::wstring &projectPrefix, const VPGEnumClass *enumClass, const std::wstring &middlePath, const VPGConfigExport *option)
{
    std::wstring result = L"";
    TRY
        std::wstring enumClassName = getTypeOrClassWithoutNamespace(enumClass->getName());
        enumClassName = (!vcc::isStartWith(enumClassName, projectPrefix) ? projectPrefix  : L"") + enumClassName;
        std::wstring packageFolder = getJavaPactkage(option->getTypeDirectory(), middlePath, L"Type Directory");
        result += L"package " + packageFolder + L";\r\n"
            "\r\n"
            "import java.util.Objects;\r\n"
            "\r\n"
            "public enum " + enumClassName + L" {\r\n";
        int64_t enumValue = 0;
        // Need to remove duplicate enum value as there is GETCUSTOM and SETCUSTOM
        std::set<std::wstring> enumValueSet;
        for (size_t i = 0; i < enumClass->getProperties().size(); i++) {
            auto const &property = enumClass->getProperties().at(i);
            if (enumValueSet.find(property->getEnum()) != enumValueSet.end()) {
                continue;
            }
            enumValueSet.insert(property->getEnum());
            if (property->getEnumValue() > -1)
                enumValue = property->getEnumValue();
            result += INDENT + property->getEnum() + L"(" + std::to_wstring(enumValue) + L")" 
                + (i == enumClass->getProperties().size() - 1 ? L";" : L",") + L"\r\n";
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

std::wstring VPGJavaGenerationService::getGetterSetterMapKeyContent(const std::wstring &classPropertyEnum, const std::wstring &dllInstantPrefix, const std::wstring &javaType1,
    const std::wstring &orginalFunction, const bool &isReturnNeeded)
{
    std::wstring result = L"";
    TRY
        if (javaType1 == L"String")
            result += INDENT + INDENT + L"Pointer keyPtr = new Memory(Native.WCHAR_SIZE * (key.length() + 1));\r\n"
                + INDENT + INDENT + L"keyPtr.setWideString(0, key);\r\n";
        else if (vcc::isCapital(javaType1)) {
            // Enum
            result += INDENT + INDENT + L"Pointer keyPtr = new Memory(Native.getNativeSize(long.class));\r\n"
                + INDENT + INDENT + L"keyPtr.setLong(0, (long)key.getValue());\r\n";
        } else {
            std::wstring capitialType = javaType1;
            vcc::toCapital(capitialType);
            result += INDENT + INDENT + L"Pointer keyPtr = new Memory(Native.getNativeSize(" + javaType1 + L".class));\r\n"
                + INDENT + INDENT + L"keyPtr.set" + capitialType + L"(0, key);\r\n";
        }
        result += INDENT + INDENT + (isReturnNeeded ? L"return " : L"") + dllInstantPrefix + orginalFunction + L"(Handle, " + classPropertyEnum + L", keyPtr);\r\n";
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::GenerateObjectGetterSetterContainerCount(const VPGEnumClassAttribute *property, const std::wstring &projectPrefix, const std::wstring &objectProperty, bool isVector, bool isMap, bool isSet)
{
    std::wstring result = L"";
    TRY
        if (!isVector && !isMap && !isSet)
            return result;

        std::wstring dllInstantPrefix = projectPrefix + L"DllFunctions.Instance.";
        std::wstring classPropertyEnum = objectProperty + L"." + property->getEnum() + L".getValue()";

        result += L"\r\n"
        + INDENT + L"public long get" + property->getPropertyName() + L"Count() {\r\n"
        + INDENT + INDENT + L"return " + dllInstantPrefix + L"getCount(Handle, " + classPropertyEnum + L");\r\n"
        + INDENT + L"}\r\n";
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::GenerateObjectGetterSetterContainer(const VPGEnumClassAttribute *property, const std::wstring &projectPrefix, const std::wstring &objectProperty,
    const std::wstring &cppType1, const std::wstring &javaType1,
    bool isVector, bool isMap, bool isSet, std::set<std::wstring> &importFiles)
{
    std::wstring result = L"";
    TRY
        if (!isVector && !isMap && !isSet)
            return result;
        
        bool isAllowWrite = property->getAccessMode() == VPGEnumClassAttributeAccessMode::ReadWrite || property->getAccessMode() == VPGEnumClassAttributeAccessMode::WriteOnly;
        std::wstring dllInstantPrefix = projectPrefix + L"DllFunctions.Instance.";
        std::wstring classPropertyEnum = objectProperty + L"." + property->getEnum() + L".getValue()";
        std::wstring macro = property->getMacro().substr(0, property->getMacro().find(L"("));

        if (isMap) {
            importFiles.insert(L"java.util.HashSet");
            importFiles.insert(L"java.util.Set");

            std::wstring javaCaptitalType = getJavaGetterSetterJavaTypeToJavaCaptialType(javaType1);
            std::wstring getFromPointer = L"ptr.get" + javaCaptitalType + L"(0)";
            if (cppType1 == L"wstring")
                getFromPointer = L"ptr.getWideString(0)";
            else if (vcc::isCapital(cppType1)) {
                // not support Left type is pointer
                // if (vcc::isContain(macro, L"SPTR"))
                //     getFromPointer = L"ptr.getLong(0)";
                // else
                    getFromPointer = javaType1 + L".parse((int)ptr.getLong(0))";
            }
            result +=  L"\r\n"
                + INDENT + L"public Set<" + javaCaptitalType + L"> get" + property->getPropertyName() + L"Keys() {\r\n"
                + INDENT + INDENT + L"Set<" + javaCaptitalType + L"> result = new HashSet<>();\r\n"
                + INDENT + INDENT + L"Pointer ptrs = VPGDllFunctions.Instance.getMapKeys(Handle, " + classPropertyEnum + L");\r\n"
                + INDENT + INDENT + L"long total = get" + property->getPropertyName() + L"Count();\r\n"
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
                + INDENT + L"public boolean is" + property->getPropertyName() + L"ContainKey(" + javaType1 + L" key) {\r\n"
                + getGetterSetterMapKeyContent(classPropertyEnum, dllInstantPrefix, javaType1, L"isContainKey", true)
                + INDENT + L"}\r\n";            
        }

        if (isVector && isAllowWrite) {
            if (property->getIsObject() && property->getIsVector() && !property->getIsOrderedMap())
                result += L"\r\n"
                    + INDENT + L"public void remove" + property->getPropertyName() + L"(" + javaType1 + L" value) {\r\n"
                    + INDENT + INDENT + dllInstantPrefix + L"removeObject(Handle, " + classPropertyEnum + L", value.Handle);\r\n"
                    + INDENT + L"}\r\n";
            result += L"\r\n"
                + INDENT + L"public void remove" + property->getPropertyName() + L"AtIndex(long index) {\r\n"
                + INDENT + INDENT + dllInstantPrefix + L"removeAtIndex(Handle, " + classPropertyEnum + L", index);\r\n"
                + INDENT + L"}\r\n";
        }

        if (isMap && isAllowWrite) {
            result += L"\r\n"
                + INDENT + L"public void remove" + property->getPropertyName() + L"AtKey(" + javaType1 + L" key) {\r\n"
                + getGetterSetterMapKeyContent(classPropertyEnum, dllInstantPrefix, javaType1, L"removeAtKey", false)
                + INDENT + L"}\r\n";
        }
            
        if (isSet) {
            // TODO: no handle at the moment
        }
        
        if (isAllowWrite)
            result += L"\r\n"
                + INDENT + L"public void clear" + property->getPropertyName() + L"() {\r\n"
                + INDENT + INDENT + dllInstantPrefix + L"clear(Handle, " + classPropertyEnum + L");\r\n"
                + INDENT + L"}\r\n";
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::GenerateObjectGetterSetterRead(const VPGEnumClassAttribute *property, const std::wstring &projectPrefix, const std::wstring &objectProperty,
    const std::wstring &macro, const std::wstring &cppType1, const std::wstring &javaType1, const std::wstring &cppType2, const std::wstring &javaType2,
    bool isVector, bool isMap, bool isSet, std::set<std::wstring> &importFiles)
{
    std::wstring result = L"";
    TRY
        bool isAllowRead = property->getAccessMode() == VPGEnumClassAttributeAccessMode::ReadWrite || property->getAccessMode() == VPGEnumClassAttributeAccessMode::ReadOnly;
        if (!isAllowRead)
            return result;

        std::wstring dllInstantPrefix = projectPrefix + L"DllFunctions.Instance.";
        std::wstring classPropertyEnum = objectProperty + L"." + property->getEnum() + L".getValue()";
        
        std::wstring javaFunctionNameSuffix = L"";
        std::wstring javaFunctionArgument = L"";
        std::wstring dllFunctionIndex = L"";
        if (isVector) {
            javaFunctionNameSuffix = L"AtIndex";
            javaFunctionArgument = L"long index";
            dllFunctionIndex = L", index";
        } else if (isMap) {
            javaFunctionNameSuffix = L"AtKey";
            javaFunctionArgument = javaType1 + L" key";
            dllFunctionIndex = L", keyPtr";
            
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
        VPGPropertyAccessorGenerationService::getPropertyAccessorTypeName(returnCppType, convertedType, convertedName, returnResult);

        result += L"\r\n"
            + INDENT + L"public " + returnJavaType + L" get" + property->getPropertyName() + javaFunctionNameSuffix + L"(" + javaFunctionArgument + L") {\r\n";
        if (isMap) {
            if (javaType1 == L"String")
                result += INDENT + INDENT + L"Pointer keyPtr = new Memory(Native.WCHAR_SIZE * (key.length() + 1));\r\n"
                    + INDENT + INDENT + L"keyPtr.setWideString(0, key);\r\n";
            else if (vcc::isCapital(javaType1)) {
                // Enum
                result += INDENT + INDENT + L"Pointer keyPtr = new Memory(Native.getNativeSize(long.class));\r\n"
                    + INDENT + INDENT + L"keyPtr.setLong(0, (long)key.getValue());\r\n";
            } else {
                std::wstring capitialType = javaType1;
                vcc::toCapital(capitialType);
                result += INDENT + INDENT + L"Pointer keyPtr = new Memory(Native.getNativeSize(" + javaType1 + L".class));\r\n"
                    + INDENT + INDENT + L"keyPtr.set" + capitialType + L"(0, key);\r\n";
            }
        }
        if (returnJavaType == L"String") {
            result += INDENT + INDENT + L"PointerByReference result = new PointerByReference();\r\n"
                + INDENT + INDENT + dllInstantPrefix + L"readString" + (isMap ? L"AtKey" : (isVector ? L"AtIndex" : L"")) + L"(Handle, " + classPropertyEnum + L", result" + dllFunctionIndex + L");\r\n"
                + INDENT + INDENT + L"return result.getValue().getWideString(0);\r\n"; 
        } else if (vcc::isContain(macro, L"SPTR")) {
            // Object
            result += INDENT + INDENT + L"return new " + returnJavaType + L"(" + dllInstantPrefix + L"readObject" + (isMap ? L"AtKey" : (isVector ? L"AtIndex" : L"")) + L"(Handle, " + classPropertyEnum + dllFunctionIndex + L"));\r\n";
        } else if (vcc::isCapital(returnJavaType)) {
            // Enum
            result += INDENT + INDENT + L"return " + returnJavaType + L".parse((int)" + dllInstantPrefix + L"readLong" + (isMap ? L"AtKey" : (isVector ? L"AtIndex" : L"")) + L"(Handle, " + classPropertyEnum + dllFunctionIndex + L"));\r\n";
        } else {
            // Normal Type
            result += INDENT + INDENT + L"return " + dllInstantPrefix + L"read" + convertedName + (isMap ? L"AtKey" : (isVector ? L"AtIndex" : L"")) + L"(Handle, " + classPropertyEnum + dllFunctionIndex + L");\r\n";
        }
        result += INDENT + L"}\r\n";
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::GenerateObjectGetterSetterWrite(const VPGEnumClassAttribute *property, const std::wstring &projectPrefix, const std::wstring &objectProperty,
    const std::wstring &macro, const std::wstring &cppType1, const std::wstring &javaType1, const std::wstring &cppType2, const std::wstring &javaType2,
    bool isVector, bool isMap, bool isSet,
    const std::map<std::wstring, std::wstring> &importFileMap, std::set<std::wstring> &importFiles)
{
    std::wstring result= L"";
    TRY
        bool isAllowWrite = property->getAccessMode() == VPGEnumClassAttributeAccessMode::ReadWrite || property->getAccessMode() == VPGEnumClassAttributeAccessMode::WriteOnly;
        if (!isAllowWrite)
            return result;

        std::wstring dllInstantPrefix = projectPrefix + L"DllFunctions.Instance.";
        std::wstring classPropertyEnum = objectProperty + L"." + property->getEnum() + L".getValue()";
        
        std::wstring convertedType = L"";
        std::wstring convertedName = L"";
        std::wstring returnResult = L"";
        VPGPropertyAccessorGenerationService::getPropertyAccessorTypeName(cppType1, convertedType, convertedName, returnResult);
        
        std::wstring javaFunctionNameSuffix = L"";
        std::wstring javaFunctionArgument = L"";
        std::wstring dllFunctionIndex = L"";
        if (isVector) {
            javaFunctionNameSuffix = L"AtIndex";
            javaFunctionArgument = L"long index, ";
            dllFunctionIndex = L", index";
            
            if (javaType1 == L"String") {
                importFiles.insert(L"com.sun.jna.Memory");
                importFiles.insert(L"com.sun.jna.Native");
                importFiles.insert(L"com.sun.jna.Pointer");
            }
        } else if (isMap) {
            javaFunctionNameSuffix = L"AtKey";
            javaFunctionArgument = javaType1 + L" key, ";
            dllFunctionIndex = L", keyPtr";
            
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
            + INDENT + L"public void set" + property->getPropertyName() + javaFunctionNameSuffix + L"(" + javaFunctionArgument + javaType + L" value) {\r\n";
        if (isMap) {
            if (javaType1 == L"String")
                result += INDENT + INDENT + L"Pointer keyPtr = new Memory(Native.WCHAR_SIZE * (key.length() + 1));\r\n"
                    + INDENT + INDENT + L"keyPtr.setWideString(0, key);\r\n";
            else if (vcc::isCapital(javaType1)) {
                // Enum
                result += INDENT + INDENT + L"Pointer keyPtr = new Memory(Native.getNativeSize(long.class));\r\n"
                    + INDENT + INDENT + L"keyPtr.setLong(0, (long)key.getValue());\r\n";
            } else {
                std::wstring capitialType = javaType1;
                vcc::toCapital(capitialType);
                result += INDENT + INDENT + L"Pointer keyPtr = new Memory(Native.getNativeSize(" + javaType1 + L".class));\r\n"
                    + INDENT + INDENT + L"keyPtr.set" + capitialType + L"(0, key);\r\n";
            }
        }
        if (javaType == L"String") {
            importFiles.insert(L"com.sun.jna.ptr.PointerByReference");

            result += INDENT + INDENT + L"Pointer valuePtr = new Memory(Native.WCHAR_SIZE * (value.length() + 1));\r\n"
                + INDENT + INDENT + L"valuePtr.setWideString(0, value);\r\n"
                + INDENT + INDENT + L"PointerByReference valueReference = new PointerByReference();\r\n"
                + INDENT + INDENT + L"valueReference.setValue(valuePtr);\r\n"
                + INDENT + INDENT + dllInstantPrefix + L"writeString" + (isMap ? L"AtKey" : (isVector ? L"AtIndex" : L"")) + L"(Handle, " + classPropertyEnum + L", valueReference" + dllFunctionIndex + L");\r\n";
        } else if (vcc::isContain(macro, L"SPTR")) {
            // Object
            result += INDENT + INDENT + dllInstantPrefix + L"writeObject" + (isMap ? L"AtKey" : (isVector ? L"AtIndex" : L""))  + L"(Handle, " + classPropertyEnum + L", value.Handle" + dllFunctionIndex + L");\r\n";
        } else if (vcc::isCapital(cppType)) {
            // Enum
            result += INDENT + INDENT + dllInstantPrefix + L"writeLong" + (isMap ? L"AtKey" : (isVector ? L"AtIndex" : L""))  + L"(Handle, " + classPropertyEnum + L", value.getValue()" + dllFunctionIndex + L");\r\n";
        } else {
            result += INDENT + INDENT + dllInstantPrefix + L"write" + convertedName + (isMap ? L"AtKey" : (isVector ? L"AtIndex" : L"")) + L"(Handle, " + classPropertyEnum + L", value" + dllFunctionIndex + L");\r\n";
        }
        result += INDENT + L"}\r\n";

        if (isVector) {
            if (property->getIsObject()) {
                std::wstring objectTypeClass = projectPrefix + L"ObjectType";
                if (importFileMap.find(objectTypeClass) != importFileMap.end())
                    importFiles.insert(importFileMap.find(objectTypeClass)->second + L"." + objectTypeClass);

                std::wstring objectType = cppType1;
                if (vcc::isStartWith(cppType1, projectPrefix))
                    objectType = cppType1.substr(projectPrefix.length());
                    
                result += L"\r\n"
                    + INDENT + L"public " + javaType1 + L" add" + property->getPropertyName() + L"() {\r\n"
                    + INDENT + INDENT + L"return add" + property->getPropertyName() + javaFunctionNameSuffix + L"(-1);\r\n"
                    + INDENT + L"}\r\n"
                    "\r\n"
                    + INDENT + L"public " + javaType1 + L" add" + property->getPropertyName() + javaFunctionNameSuffix + L"(long index) {\r\n"
                    + INDENT + INDENT + L"return new " + cppType1 + L"(" + dllInstantPrefix + L"addObjectAtIndex(Handle, " + classPropertyEnum + L", " + objectTypeClass + L"." + objectType + L".getValue()" + dllFunctionIndex + L"));\r\n"
                    + INDENT + L"}\r\n";
            }
        }
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::GenerateObjectGetterSetterInsert(const VPGEnumClassAttribute *property, const std::wstring &projectPrefix, const std::wstring &objectProperty,
    const std::wstring &cppType1, const std::wstring &javaType1,
    bool isVector, std::set<std::wstring> &importFiles)
{
    std::wstring result = L"";
    TRY
        if (!isVector)
            return result;
        
        bool isAllowWrite = property->getAccessMode() == VPGEnumClassAttributeAccessMode::ReadWrite || property->getAccessMode() == VPGEnumClassAttributeAccessMode::WriteOnly;
        if (!isAllowWrite)
            return result;

        std::wstring dllInstantPrefix = projectPrefix + L"DllFunctions.Instance.";
        std::wstring classPropertyEnum = objectProperty + L"." + property->getEnum() + L".getValue()";
        
        std::wstring convertedType = L"";
        std::wstring convertedName = L"";
        std::wstring returnResult = L"";
        VPGPropertyAccessorGenerationService::getPropertyAccessorTypeName(cppType1, convertedType, convertedName, returnResult);

        std::wstring javaFunctionNameSuffix = L"AtIndex";
        std::wstring dllFunctionIndex = L"index";

        result += L"\r\n"
            + INDENT + L"public void insert" + property->getPropertyName() + L"(" + javaType1 + L" value) {\r\n"
            + INDENT+ INDENT + L"insert" + property->getPropertyName() + javaFunctionNameSuffix + L"(-1, value);\r\n"
            + INDENT + L"}\r\n"
            "\r\n";
        result += INDENT + L"public void insert" + property->getPropertyName() + javaFunctionNameSuffix + L"(long index, " + javaType1 + L" value) {\r\n";
        if (javaType1 == L"String") {
            importFiles.insert(L"com.sun.jna.Memory");
            importFiles.insert(L"com.sun.jna.Native");
            importFiles.insert(L"com.sun.jna.Pointer");

            result += INDENT + INDENT + L"Pointer valuePtr = new Memory(Native.WCHAR_SIZE * (value.length() + 1));\r\n"
                + INDENT + INDENT + L"valuePtr.setWideString(0, value);\r\n"
                + INDENT + INDENT + L"PointerByReference valueReference = new PointerByReference();\r\n"
                + INDENT + INDENT + L"valueReference.setValue(valuePtr);\r\n"
                + INDENT + INDENT + dllInstantPrefix + L"insertStringAtIndex(Handle, " + classPropertyEnum + L", valueReference, " + dllFunctionIndex + L");\r\n";
        } else if (property->getIsObject()) {
            // Object
            result += INDENT + INDENT + dllInstantPrefix + L"insertObjectAtIndex(Handle, " + classPropertyEnum + L", value.Handle, " + dllFunctionIndex + L");\r\n";
        } else if (vcc::isCapital(cppType1)) {
            // Enum
            result += INDENT + INDENT + dllInstantPrefix + L"insertLongAtIndex(Handle, " + classPropertyEnum + L", value.getValue(), " + dllFunctionIndex + L");\r\n";
        } else {
            result += INDENT + INDENT + dllInstantPrefix + L"insert" + convertedName + L"AtIndex(Handle, " + classPropertyEnum + L", value, " + dllFunctionIndex + L");\r\n";
        }
        result += INDENT + L"}\r\n";
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::GenerateObjectGetterSetter(const std::wstring &projectPrefix, const std::wstring &objectProperty, const VPGEnumClassAttribute *property, const std::map<std::wstring, std::wstring> &importFileMap, std::set<std::wstring> &importFiles)
{
    std::wstring result = L"";
    TRY
        if (property->getPropertyType() != VPGEnumClassAttributeType::Property || property->getAccessMode() == VPGEnumClassAttributeAccessMode::NoAccess)
            return result;
        std::wstring macro = property->getMacro().substr(0, property->getMacro().find(L"("));
        std::wstring dllInstantPrefix = projectPrefix + L"DllFunctions.Instance.";
        bool isAllowRead = property->getAccessMode() == VPGEnumClassAttributeAccessMode::ReadWrite || property->getAccessMode() == VPGEnumClassAttributeAccessMode::ReadOnly;
        bool isAllowWrite = property->getAccessMode() == VPGEnumClassAttributeAccessMode::ReadWrite || property->getAccessMode() == VPGEnumClassAttributeAccessMode::WriteOnly;

        if (!isAllowRead && !isAllowWrite)
            return result;
        
        std::wstring cppType1 = getTypeOrClassWithoutNamespace(property->getType1());
        if (vcc::isBlank(cppType1))
            return result;

        std::wstring javaType1 = vcc::isCapital(cppType1) ? cppType1 : getJavaGetterSetterCppToJavaConvertedType(cppType1);
        std::wstring cppType2 = getTypeOrClassWithoutNamespace(property->getType2());
        std::wstring javaType2 = !vcc::isBlank(cppType2) ? (vcc::isCapital(cppType2) ? cppType2 : getJavaGetterSetterCppToJavaConvertedType(cppType2)) : cppType2;
        if (!vcc::isBlank(cppType1) && vcc::isCapital(cppType1) && importFileMap.find(javaType1) != importFileMap.end())
            importFiles.insert(importFileMap.find(javaType1)->second + L"." + javaType1);
        if (!vcc::isBlank(cppType2) && vcc::isCapital(cppType2) && importFileMap.find(javaType2) != importFileMap.end())
            importFiles.insert(importFileMap.find(javaType2)->second + L"." + javaType2);
        
        // add import file
        if (!javaType1.empty() && vcc::isCapital(javaType1)) {
            if (importFileMap.find(javaType1) != importFileMap.end())
                importFiles.insert(importFileMap.find(javaType1)->second + L"." + javaType1);
        }
        if (!javaType2.empty() && vcc::isCapital(javaType2)) {
            if (importFileMap.find(javaType2) != importFileMap.end())
                importFiles.insert(importFileMap.find(javaType2)->second + L"." + javaType2);
        }
        // not support set
        // container
        bool isVector = property->getIsVector();
        bool isMap = property->getIsMap() || property->getIsOrderedMap();
        bool isSet = property->getIsSet();
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
            cppType1, javaType1,
            isVector, importFiles);

        // container
        result += GenerateObjectGetterSetterContainer(property, projectPrefix, objectProperty,
            cppType1, javaType1,
            isVector, isMap, isSet,
            importFiles);
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::GenerateObjectContent(const std::wstring &projectPrefix, const VPGEnumClass *enumClass, const std::wstring &middlePath, const std::map<std::wstring, std::wstring> &importFileMap, const VPGConfigExport *option)
{
    std::wstring result = L"";
    TRY
        std::wstring packageFolder = getJavaPactkageObject(enumClass, option, middlePath);
        std::wstring objectPropertyName = getTypeOrClassWithoutNamespace(enumClass->getName());
        std::wstring objectName = objectPropertyName;
        std::wstring classObjectType = objectName.substr(0, objectName.size() - propertyClassNameSuffix.size());
        classObjectType = vcc::isStartWith(objectName, projectPrefix) ? classObjectType.substr(projectPrefix.length()) : classObjectType;
        objectName = (!vcc::isStartWith(objectName, projectPrefix) ? projectPrefix : L"") + objectName.substr(0, objectName.size() - propertyClassNameSuffix.size());
        
        std::set<std::wstring> importFiles;
        importFiles.insert(L"com.sun.jna.Pointer");
        importFiles.insert(getJavaPactkage(option->getDllBridgeDirectory(), L"", L"DLL interface Directory") + L"." + projectPrefix  + L"DllFunctions");
        importFiles.insert(getJavaPactkage(option->getTypeDirectory(), middlePath, L"Type Directory") + L"." + objectPropertyName);

        if (enumClass->getType() == VPGEnumClassType::Form || enumClass->getType() == VPGEnumClassType::ActionArgument) {
            std::wstring objectTypeClass = projectPrefix + L"ObjectType";
            if (importFileMap.find(objectTypeClass) != importFileMap.end())
                importFiles.insert(importFileMap.find(objectTypeClass)->second + L"." + objectTypeClass);
        }

        std::wstring operationResultClass = projectPrefix  + L"OperationResult";
        if (enumClass->getType() == VPGEnumClassType::Result) {
            importFiles.insert(getJavaPactkage(option->getObjectDirectory(), L"", L"Object Directory") + L"." + operationResultClass);
        }

        // Property
        std::wstring getterSetterStr = L"";
        for (auto const &property : enumClass->getProperties())
            getterSetterStr += VPGJavaGenerationService::GenerateObjectGetterSetter(projectPrefix, objectPropertyName, property.get(), importFileMap, importFiles);
        if (!getterSetterStr.empty()) {
            vcc::lTrim(getterSetterStr);

            getterSetterStr = L"\r\n"
                + INDENT + L"// <editor-fold defaultstate=\"collapsed\" desc=\"Generated Properties\">\r\n"
                + INDENT + getterSetterStr
                + INDENT + L"// </editor-fold>\r\n";
        }
        
        // Form Action
        std::wstring  formActionStr = VPGJavaGenerationService::GenerateFormAction(projectPrefix, enumClass);
        if (!formActionStr.empty()) {
            vcc::lTrim(formActionStr);
            formActionStr = L"\r\n"
                + INDENT + L"// <editor-fold defaultstate=\"collapsed\" desc=\"Generated Form Actions\">\r\n"
                + INDENT + formActionStr
                + INDENT + L"// </editor-fold>\r\n";
        }
        std::wstring  formCustomActionStr = VPGJavaGenerationService::GenerateFormCustomAction(projectPrefix, enumClass, option->getObjectDirectory(), importFileMap, importFiles);
        if (!formCustomActionStr.empty()) {
            vcc::lTrim(formCustomActionStr);
            formCustomActionStr = L"\r\n"
                + INDENT + L"// <editor-fold defaultstate=\"collapsed\" desc=\"Generated Form Custom Actions\">\r\n"
                + INDENT + formCustomActionStr
                + INDENT + L"// </editor-fold>\r\n";
        }
        
        result += L"package " + packageFolder + L";\r\n"
            "\r\n";
        for (auto const &str : importFiles) {
            // same package no need to import
            if (vcc::isStartWith(str, packageFolder + L".")
                && vcc::countSubstring(str.substr(packageFolder.size()), L".") < 2)
                continue;
            result += L"import " + str + L";\r\n";
        }

        if (enumClass->getType() == VPGEnumClassType::Result) {
            result += L"\r\n"
                "public class " + objectName + L" extends " + operationResultClass + L" {\r\n"
                "\r\n"
                + INDENT + L"public " + objectName + L"(Pointer handle) {\r\n"
                + INDENT + INDENT + L"super(handle);\r\n"
                + INDENT + L"}\r\n";
        } else {
            result += L"\r\n"
                "public class " + objectName + L" {\r\n"
                "\r\n"
                + INDENT + L"public Pointer Handle = null;\r\n"
                "\r\n"
                + INDENT + L"public " + objectName + L"(Pointer handle) {\r\n"
                + INDENT + INDENT + L"this.Handle = handle;\r\n"
                + INDENT + L"}\r\n";
        }

        // Special Constructor
        switch (enumClass->getType())
        {
        case VPGEnumClassType::Form:
            result += L"\r\n"
                + INDENT + L"public " + objectName + L"() {\r\n"
                + INDENT + INDENT + L"this.Handle = " + projectPrefix + L"DllFunctions.Instance.applicationCreateForm(" + projectPrefix + L"ObjectType." + classObjectType + L".getValue());\r\n"
                + INDENT + L"}\r\n";
            break;
        case VPGEnumClassType::ActionArgument:
            result += L"\r\n"
                + INDENT + L"public " + objectName + L"() {\r\n"
                + INDENT + INDENT + L"this.Handle = " + projectPrefix + L"DllFunctions.Instance.applicationCreateActionArgument(" + projectPrefix + L"ObjectType." + classObjectType + L".getValue());\r\n"
                + INDENT + L"}\r\n";
            break;
        default:
            break;
        }
        result += getterSetterStr
            + formActionStr
            + formCustomActionStr
            + L"}\r\n";
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::GenerateOperationResultContent(const std::wstring &projectPrefix, const VPGConfigExport *option, const std::map<std::wstring, std::wstring> &importFileMap)
{
    std::wstring result = L"";
    TRY
        std::set<std::wstring> importFiles;
        importFiles.insert(L"com.sun.jna.Pointer");
        importFiles.insert(L"com.sun.jna.ptr.PointerByReference");

        std::wstring dllInterfaceClass = projectPrefix  + L"DllFunctions";
        importFiles.insert(getJavaPactkage(option->getDllBridgeDirectory(), L"", L"DLL interface Directory") + L"." + dllInterfaceClass);
        
        std::wstring exceptionTypeClass = projectPrefix  + L"ExceptionType";
        if (importFileMap.find(exceptionTypeClass) != importFileMap.end())
            importFiles.insert(importFileMap.find(exceptionTypeClass)->second + L"." + exceptionTypeClass);

        result = L"package " + getJavaPactkage(option->getObjectDirectory(), L"", L"Object Directory") + L";\r\n"
            "\r\n";
            
        for (auto const &str : importFiles)
            result += L"import " + str + L";\r\n";

        result += L"\r\n"
            "public class " + projectPrefix + L"OperationResult {\r\n"
            + INDENT + L"public Pointer Handle = null;\r\n"
            "\r\n"
            + INDENT + L"public " + projectPrefix + L"OperationResult(Pointer handle) {\r\n"
            + INDENT + INDENT + L"this.Handle = handle;\r\n"
            + INDENT + L"}\r\n"
            "\r\n"
            + INDENT + L"public " + exceptionTypeClass + L" getExceptionType() {\r\n"
            + INDENT + INDENT + L"return " + exceptionTypeClass + L".parse((int)" + dllInterfaceClass + L".Instance.applicationGetResultErrorCode(Handle));\r\n"
            + INDENT + L"}\r\n"
            "\r\n"
            + INDENT + L"public String getMessage() {\r\n"
            + INDENT + INDENT + L"PointerByReference result = new PointerByReference();\r\n"
            + INDENT + INDENT + dllInterfaceClass + L".Instance.applicationGetResultMessage(Handle, result);\r\n"
            + INDENT + INDENT + L"return result.getValue().getWideString(0);\r\n"
            + INDENT + L"}\r\n"
            + INDENT + L"public boolean isError() {\r\n"
            + INDENT + INDENT + L"return " + dllInterfaceClass + L".Instance.applicationIsErrorResult(Handle);\r\n"
            + INDENT + L"}\r\n"
            "\r\n"
            + INDENT + L"public boolean isWarning() {\r\n"
            + INDENT + INDENT + L"return " + dllInterfaceClass + L".Instance.applicationIsWarningResult(Handle);\r\n"
            + INDENT + L"}\r\n"
            "\r\n"
            + INDENT + L"public void close() {\r\n"
            + INDENT + INDENT + dllInterfaceClass + L".Instance.applicationEraseResult(Handle);\r\n"
            + INDENT + L"}\r\n"
            "}\r\n";
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::GenerateFormAction(const std::wstring &projectPrefix, const VPGEnumClass *enumClass)
{
    assert(enumClass != nullptr);
    if (enumClass->getType() != VPGEnumClassType::Form)
        return L"";
    
    std::wstring result = L"";
    TRY
        std::map<std::wstring, std::wstring> formActions;
        // Form Action
        formActions.insert(std::make_pair(L"getActionCurrentSeqNo",
            L"public long getActionCurrentSeqNo() {\r\n"
            + INDENT + L"return " + projectPrefix + L"DllFunctions.Instance.applicationGetFormActionCurrentSeqNo(Handle);\r\n"
            "}\r\n"));
        formActions.insert(std::make_pair(L"getActionFirstSeqNo",
            L"public long getActionFirstSeqNo() {\r\n"
            + INDENT + L"return " + projectPrefix + L"DllFunctions.Instance.applicationGetFormActionFirstSeqNo(Handle);\r\n"
            "}\r\n"));
        formActions.insert(std::make_pair(L"getActionLastSeqNo",
            L"public long getActionLastSeqNo() {\r\n"
            + INDENT + L"return " + projectPrefix + L"DllFunctions.Instance.applicationGetFormActionLastSeqNo(Handle);\r\n"
            "}\r\n"));

        formActions.insert(std::make_pair(L"redo", 
            L"public void redo(long noOfStep) {\r\n"
            + INDENT + projectPrefix + L"DllFunctions.Instance.applicationRedoFormAction(Handle, noOfStep);\r\n"
            "}\r\n"));
        formActions.insert(std::make_pair(L"redoToSeqNo",
            L"public void redoToSeqNo(long seqNo) {\r\n"
            + INDENT + projectPrefix + L"DllFunctions.Instance.applicationRedoFormActionToSeqNo(Handle, seqNo);\r\n"
            "}\r\n"));
    
        formActions.insert(std::make_pair(L"undo", 
            L"public void undo(long noOfStep) {\r\n"
            + INDENT + projectPrefix + L"DllFunctions.Instance.applicationUndoFormAction(Handle, noOfStep);\r\n"
            "}\r\n"));
        formActions.insert(std::make_pair(L"undoToSeqNo",
            L"public void undoToSeqNo(long seqNo) {\r\n"
            + INDENT + projectPrefix + L"DllFunctions.Instance.applicationUndoFormActionToSeqNo(Handle, seqNo);\r\n"
            "}\r\n"));
            
        formActions.insert(std::make_pair(L"clearAction", 
            L"public long clearAction() {\r\n"
            + INDENT + L"return " + projectPrefix + L"DllFunctions.Instance.applicationClearFormAction(Handle);\r\n"
            "}\r\n"));
        formActions.insert(std::make_pair(L"truncateAction",
            L"public long truncateAction() {\r\n"
            + INDENT + L"return " + projectPrefix + L"DllFunctions.Instance.applicationTruncateFormAction(Handle);\r\n"
            "}\r\n"));
        
        // Close
        formActions.insert(std::make_pair(L"isClosable",
            L"public boolean isClosable() {\r\n"
            + INDENT + L"return " + projectPrefix + L"DllFunctions.Instance.applicationIsFormClosable(Handle);\r\n"
            "}\r\n"));

        formActions.insert(std::make_pair(L"isClosed",
            L"public boolean isClosed() {\r\n"
            + INDENT + L"return " + projectPrefix + L"DllFunctions.Instance.applicationIsFormClosed(Handle);\r\n"
            "}\r\n"));

        formActions.insert(std::make_pair(L"close",
            L"public void close(boolean isForce) {\r\n"
            + INDENT + projectPrefix + L"DllFunctions.Instance.applicationCloseForm(Handle, isForce);\r\n"
            "}\r\n"));
        
        for (auto const &action : formActions) {
            std::vector<std::wstring> lines = vcc::splitStringByLine(action.second);
            result += L"\r\n";
            for (auto &line : lines) {
                vcc::rTrim(line);
                result += INDENT + line + L"\r\n";
            }
        }
    CATCH
    return result;
}

#include <iostream>
std::wstring VPGJavaGenerationService::GenerateFormCustomAction(const std::wstring &projectPrefix, const VPGEnumClass *enumClass, const std::wstring &optionResultParent,
    const std::map<std::wstring, std::wstring> &importFileMap, std::set<std::wstring> &importFiles)
{
    assert(enumClass != nullptr);
    if (enumClass->getType() != VPGEnumClassType::Form)
        return L"";
    
    std::wstring result = L"";
    TRY
        std::map<std::wstring, std::wstring> formActions;
        for (auto const &property : enumClass->getProperties()) {
            if (property->getPropertyType() != VPGEnumClassAttributeType::Action)
                continue;

            // Only support SPTR
            std::wstring type = getTypeOrClassWithoutNamespace(property->getType1());
            if (!vcc::isBlank(type) && vcc::isCapital(type) && importFileMap.find(type) != importFileMap.end()) {
                importFiles.insert(importFileMap.find(type)->second + L"." + type);

                std::wstring objectTypeClass = projectPrefix + L"ObjectType";
                if (importFileMap.find(objectTypeClass) != importFileMap.end())
                    importFiles.insert(importFileMap.find(objectTypeClass)->second + L"." + objectTypeClass);
            }

            std::wstring functionName = L"do" + property->getPropertyName();
            std::wstring resultClass = L"";
            auto redoClassWithoutNamespace = getTypeOrClassWithoutNamespace(property->getActionResultRedoClass());
            if (!vcc::isBlank(redoClassWithoutNamespace) && redoClassWithoutNamespace != L"OperationResult" && redoClassWithoutNamespace != projectPrefix + L"OperationResult") {
                resultClass = property->getActionResultRedoClass();
                if (importFileMap.find(resultClass) != importFileMap.end())
                    importFiles.insert(importFileMap.find(resultClass)->second + L"." + redoClassWithoutNamespace);
                resultClass = redoClassWithoutNamespace;
            } else {
                resultClass = projectPrefix + L"OperationResult";
                importFiles.insert(getJavaPactkage(optionResultParent, L"", L"Object Directory") + L"." + resultClass);
            }

            formActions.insert(std::make_pair(functionName,
                L"public " + resultClass + L" " + functionName + L"(" + (!type.empty() ? (type + L" argument") : L"") + L") {\r\n"
                + INDENT + L"return new " + resultClass + L"(" + projectPrefix + L"DllFunctions.Instance.applicationDoFormAction(Handle, " + getTypeOrClassWithoutNamespace(enumClass->getName()) + L"." + property->getEnum() + L".getValue(), " + (!type.empty() ? L"argument.Handle" : L"null") + L"));\r\n"
                "}\r\n"));
        }
        
        for (auto const &action : formActions) {
            std::vector<std::wstring> lines = vcc::splitStringByLine(action.second);
            result += L"\r\n";
            for (auto &line : lines) {
                vcc::rTrim(line);
                result += INDENT + line + L"\r\n";
            }
        }
    CATCH
    return result;
}

void VPGJavaGenerationService::GenerateEnum(const vcc::LogConfig *logConfig, const std::wstring &filePath, const std::wstring &cppMiddlePath, const VPGEnumClass *enumClass, const VPGConfig *option, const VPGConfigExport *javaOption)
{
    TRY
        assert(option != nullptr);
        assert(javaOption != nullptr);
        if (filePath.empty())
            return;
        
        std::wstring tmpFilePath = vcc::getParentPath(filePath);
        tmpFilePath = vcc::concatPaths({ tmpFilePath, vcc::getFileName(filePath) });

        vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate Java Enum: " + tmpFilePath);
        vcc::writeFile(tmpFilePath, VPGJavaGenerationService::GenerateEnumContent(option->getProjectPrefix(), enumClass, cppMiddlePath, javaOption), true);
        vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate Java Enum completed.");
    CATCH
}

void VPGJavaGenerationService::GenerateObject(const vcc::LogConfig *logConfig, const std::wstring &filePath, const std::wstring &cppMiddlePath, const VPGEnumClass *enumClass,
    const std::map<std::wstring, std::wstring> &typeWorkspaceClassRelativePathMapObject, const std::map<std::wstring, std::wstring> &typeWorkspaceClassRelativePathMapForm,
    const VPGConfig *option, const VPGConfigExport *javaOption)
{
    TRY
        assert(option != nullptr);
        assert(javaOption != nullptr);
        if (filePath.empty())
            return;
            
        std::wstring tmpFilePath = vcc::getParentPath(filePath);
        tmpFilePath = vcc::concatPaths({ tmpFilePath, vcc::getFileName(filePath) });
       
        std::wstring objectName = getTypeOrClassWithoutNamespace(enumClass->getName());
        if (!vcc::isEndWith(objectName, propertyClassNameSuffix))
            return;
        vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate Java Class: " + tmpFilePath);
        vcc::writeFile(tmpFilePath, VPGJavaGenerationService::GenerateObjectContent(option->getProjectPrefix(), enumClass, cppMiddlePath, getImportFileMap(option->getProjectPrefix(), javaOption, typeWorkspaceClassRelativePathMapObject, typeWorkspaceClassRelativePathMapForm), javaOption), true);
        vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate Java Class completed.");
    CATCH
}

void VPGJavaGenerationService::GenerateOperationResult(const vcc::LogConfig *logConfig, const std::wstring &projectPrefix, const VPGConfigExport *option,
    const std::map<std::wstring, std::wstring> &typeWorkspaceClassRelativePathMapObject, const std::map<std::wstring, std::wstring> &typeWorkspaceClassRelativePathMapForm)
{
    TRY
        if (option == nullptr || option->getInterface() != VPGConfigInterfaceType::Java || vcc::isBlank(option->getObjectDirectory()))
            return;
        std::wstring filePath = vcc::concatPaths({option->getWorkspace(), getOperationResultFilePath(projectPrefix, option)});
        vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate Java Class: " + filePath);
        vcc::writeFile(filePath, GenerateOperationResultContent(projectPrefix, option, getImportFileMap(projectPrefix, option, typeWorkspaceClassRelativePathMapObject, typeWorkspaceClassRelativePathMapForm)), true);
        vcc::LogService::logInfo(logConfig, LOG_ID, L"Generate Java Class completed.");
        return;
    CATCH
}
