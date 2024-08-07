#include "vpg_java_generation_service.hpp"

#include <filesystem>
#include <fstream>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_property.hpp"

#include "vpg_file_generation_manager.hpp"
#include "vpg_generation_option.hpp"
#include "vpg_generation_option_interface_type.hpp"

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

std::wstring VPGJavaGenerationService::GetJavaPactkage(const std::wstring &path, const std::wstring &filePathName)
{
    std::wstring result = L"";
    TRY
        if (!IsStartWith(path, JAVA_PROJECT_SOURCE_PARENT_FOLDER))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, filePathName + L" is not start with " + JAVA_PROJECT_SOURCE_PARENT_FOLDER);

        result = GetRelativePath(path, JAVA_PROJECT_SOURCE_PARENT_FOLDER);
        result = GetLinuxPath(result);
        if (IsStartWith(result, L"/"))
            result = result.substr(1);
        if (IsEndWith(result, L"/"))
            result.pop_back();
        ReplaceAll(result, L"/", L".");
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::GetCppToJavaConvertedType(const std::wstring &cppType)
{
    TRY
        if (cppType == L"bool") {
            return L"Boolean";
        } else if (cppType == L"char"
            || cppType == L"byte") {
            return L"Byte";
        } else if (cppType == L"wchar_t") {
            return L"Char";
        } else if (IsContain(cppType, L"int")
            || IsContain(cppType, L"short")
            || IsContain(cppType, L"long")
            || cppType == L"size_t"
            || cppType == L"time_t") {
            return L"Integer";
        } else if (cppType == L"float") {
            return L"Float";
        } else if (cppType == L"double") {
            return L"Double";
        } else if (cppType == L"std::string"
            || cppType == L"std::wstring") {
            return L"PointerByReference";
        } else if (cppType == L"void*" || cppType == L"void *") {
            return L"PointerByReference";
        } else if (cppType == L"void") {
            return L"void";
        } else
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"Unknown type: " + cppType);
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

        std::wstring packageFolder = GetJavaPactkage(javaOption->GetDllBridgeDirectory(), L"Dll Bridge Directory");
        result += L"package " + packageFolder + L";\r\n"
            "\r\n"
            "import com.sun.jna.Library;\r\n"
            "import com.sun.jna.Native;\r\n"
            "import com.sun.jna.ptr.PointerByReference;\r\n"
            "\r\n"
            "interface " + filePrefix + L"DllFunctions extends Library {\r\n"
            + INDENT + filePrefix + L"DllFunctions INSTANCE = (" + filePrefix + L"DllFunctions)Native.load(\"vpg\", " + filePrefix + L"DllFunctions.class);\r\n"
            "\r\n";

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
                result += INDENT + GetCppToJavaConvertedType(returnType) + L" " + functionName + L"(";

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
                    argumentStr += GetCppToJavaConvertedType(argumentType) + L" " + argumentName;
                }
                result += argumentStr + L");\r\n";
                pos = endPos;
            } else if (IsStartWith(content, dllInterfaceExportPropertyAccessorString, pos)) {
                result += INDENT + L"void ReadString(PointerByReference ref, Integer property, PointerByReference value, Integer index);\r\n"
                    + INDENT + L"void ReadStringByKey(PointerByReference ref, Integer property, PointerByReference value, PointerByReference key);\r\n"
                    + INDENT + L"void WriteString(PointerByReference ref, Integer property, PointerByReference value, Integer index);\r\n"
                    + INDENT + L"void WriteStringByKey(PointerByReference ref, Integer property, PointerByReference value, PointerByReference key);\r\n";
                pos += dllInterfaceExportPropertyAccessorString.length() - 1;
            } else if (IsStartWith(content, dllInterfaceExportPropertyAccessorObject, pos)) {
                result += INDENT + L"PointerByReference ReadObject(PointerByReference ref, Integer property, Integer index);\r\n"
                    + INDENT + L"PointerByReference ReadObjectByKey(PointerByReference ref, Integer property, PointerByReference key);\r\n"
                    + INDENT + L"void WriteObject(PointerByReference ref, Integer property, PointerByReference value, Integer index);\r\n"
                    + INDENT + L"void WriteObjectByKey(PointerByReference ref, Integer property, PointerByReference value, PointerByReference key);\r\n";
                pos += dllInterfaceExportPropertyAccessorObject.length() - 1;
            } else if (IsStartWith(content, dllInterfaceExportPropertyAccessorContainer, pos)) {
                result += INDENT + L"Integer GetContainerCount(PointerByReference ref, Integer property);\r\n"
                    + INDENT + L"Boolean IsContainKey(PointerByReference ref, Integer property, PointerByReference key);\r\n"
                    + INDENT + L"void RemoveContainerElement(PointerByReference ref, Integer property, Integer index);\r\n"
                    + INDENT + L"void RemoveContainerElementByKey(PointerByReference ref, Integer property, PointerByReference key);\r\n"
                    + INDENT + L"void ClearContainer(PointerByReference ref, Integer property);\r\n";
                pos += dllInterfaceExportPropertyAccessorContainer.length() - 1;
            } else if (IsStartWith(content, dllInterfaceExportPropertyAccessor, pos)) {
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
                std::wstring convectedType = GetCppToJavaConvertedType(type);
                result += INDENT + convectedType + L" Read" + name + L"(PointerByReference ref, Integer property, Integer index);\r\n"
                    + INDENT + convectedType + L" Read" + name + L"ByKey(PointerByReference ref, Integer property, PointerByReference key);\r\n"
                    + INDENT + L"void Write" + name + L"(PointerByReference ref, Integer property, " + convectedType + L" value, Integer index);\r\n"
                    + INDENT + L"void Write" + name + L"ByKey(PointerByReference ref, Integer property, " + convectedType + L" value, PointerByReference key);\r\n";
                pos = endPos;
            } 
            GetNextCharPos(content, pos, false);
        }
        result += L"}\r\n";
    CATCH
    return result;
}

void VPGJavaGenerationService::GenerateJavaBridge(const LogProperty *logProperty, const std::wstring &dllInterfacehppFilePath, const VPGGenerationOption *option)
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
        std::wstring filePath = ConcatPaths({ javaOption->GetWorkspace(), javaOption->GetDllBridgeDirectory(), javaFileName });
        LogService::LogInfo(logProperty, LOG_ID, L"Generate Java Bridge: " + filePath);
        WriteFile(filePath, VPGJavaGenerationService::GenerateJavaBridgeContent(ReadFile(dllInterfacehppFilePath), option), true);
        LogService::LogInfo(logProperty, LOG_ID, L"Generate Java Bridge completed.");
    CATCH
}

std::wstring VPGJavaGenerationService::GenerateEnumContent(const VPGEnumClass *enumClass, const VPGGenerationOptionExport *option)
{
    std::wstring result = L"";
    TRY
        std::wstring packageFolder = GetJavaPactkage(option->GetTypeDirectory(), L"Type Directory");
        result += L"package " + packageFolder + L";\r\n"
            "\r\n"
            "public enum " + enumClass->GetName() + L" {\r\n";
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
            + INDENT + enumClass->GetName() + L"(Integer value) {\r\n"
            + INDENT + INDENT + L"this.value = value;\r\n"
            + INDENT + L"}\r\n"
            "\r\n"
            + INDENT + L"public Integer getValue() {\r\n"
            + INDENT + INDENT + L"return value;\r\n"
            + INDENT + L"}\r\n"
            "}\r\n";
    CATCH
    return result;
}

std::wstring VPGJavaGenerationService::GenerateObjectContent(const VPGEnumClass *enumClass, const VPGGenerationOptionExport *option)
{
    std::wstring result = L"";
    TRY
        std::wstring packageFolder = GetJavaPactkage(option->GetObjectDirectory(), L"Object Directory");
        std::wstring objectName = enumClass->GetName();
        objectName = objectName.substr(0, objectName.size() - propertyClassNameSuffix.size());
        result += L"package " + packageFolder + L";\r\n"
            "\r\n"
            "public enum " + objectName + L" {\r\n";

        result += L"}\r\n";
    CATCH
    return result;
}

void VPGJavaGenerationService::GenerateEnumAndObject(const LogProperty *logProperty, const std::wstring &workspace, const VPGGenerationOption *option)
{
    // need to use class name as file name
    // java project only allow one class in one file with the name same as file
    TRY
        assert(option != nullptr);
        
        std::shared_ptr<VPGGenerationOptionExport> javaOption = VPGJavaGenerationService::GetJavaOption(option);
        if (javaOption == nullptr || IsBlank(javaOption->GetWorkspace()) 
            || (IsBlank(javaOption->GetTypeDirectory()) && IsBlank(javaOption->GetObjectDirectory())))
            return;

        std::wstring typeWorkspaceFullPath = GetLinuxPath(ConcatPaths({workspace, option->GetTypeWorkspace()}));
        DECLARE_UPTR(VPGFileGenerationManager, manager, nullptr, workspace);
        manager->GetClassMacroList(workspace);
        DECLARE_UPTR(VPGEnumClassReader, reader, manager->GetClassMacros());
        manager->GetFileList(reader.get(), typeWorkspaceFullPath, option->GetProjectPrefix());

        std::map<std::wstring, std::vector<std::wstring>> includeFileEnumClassMap;
        for (auto const &enumClassIncludeFilePair : manager->GetIncludeFiles()) {
            if (includeFileEnumClassMap.find(enumClassIncludeFilePair.second) == includeFileEnumClassMap.end()) {
                std::vector<std::wstring> emptyList;
                includeFileEnumClassMap.insert(std::make_pair(enumClassIncludeFilePair.second, emptyList));
            }
            includeFileEnumClassMap.find(enumClassIncludeFilePair.second)->second.push_back(enumClassIncludeFilePair.first);
        }
        
        for (auto const &filePath : std::filesystem::recursive_directory_iterator(PATH(typeWorkspaceFullPath))) {
            if (filePath.is_directory())
                continue;
            std::wstring fileName = filePath.path().filename().wstring();
            if (includeFileEnumClassMap.find(fileName) == includeFileEnumClassMap.end())
                continue;
            
            std::wstring filePathLinuxPath = GetLinuxPath(filePath.path().wstring());
            std::wstring relativePath = GetRelativePath(filePathLinuxPath.substr(Find(filePathLinuxPath, typeWorkspaceFullPath)), typeWorkspaceFullPath);
            relativePath = PATH(relativePath).parent_path().wstring();

            for (auto const &enumClassName : includeFileEnumClassMap.find(fileName)->second) {
                if (manager->GetEnumClasses().find(enumClassName) == manager->GetEnumClasses().end())
                    continue;

                if (!IsBlank(javaOption->GetTypeDirectory())) {
                    std::wstring javaFileName = enumClassName + L".java";
                    std::wstring javaEnumFilePath = ConcatPaths({ javaOption->GetWorkspace(), javaOption->GetTypeDirectory(), relativePath, javaFileName });
                    LogService::LogInfo(logProperty, LOG_ID, L"Generate Java Enum: " + javaFileName);
                    WriteFile(javaEnumFilePath, VPGJavaGenerationService::GenerateEnumContent(manager->GetEnumClasses().find(enumClassName)->second.get(), javaOption.get()), true);
                    LogService::LogInfo(logProperty, LOG_ID, L"Generate Java Enum completed.");
                }
                if (!IsBlank(javaOption->GetObjectDirectory())) {
                    std::wstring objectName = enumClassName;
                    if (IsEndWith(objectName, propertyClassNameSuffix)) {
                        objectName = objectName.substr(0, objectName.size() - propertyClassNameSuffix.size());
                        std::wstring javaFileName = objectName + L".java";
                        std::wstring javaObjectFilePath = ConcatPaths({ javaOption->GetWorkspace(), javaOption->GetObjectDirectory(), relativePath, javaFileName });
                        LogService::LogInfo(logProperty, LOG_ID, L"Generate Java Object: " + javaFileName);
                        WriteFile(javaObjectFilePath, VPGJavaGenerationService::GenerateObjectContent(manager->GetEnumClasses().find(enumClassName)->second.get(), javaOption.get()), true);
                        LogService::LogInfo(logProperty, LOG_ID, L"Generate Java Object completed.");
                    }
                }
            }
        }
    CATCH
}