#include "vpg_java_generation_service.hpp"

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_property.hpp"

#include "vpg_generation_option.hpp"
#include "vpg_generation_option_interface_type.hpp"

using namespace vcc;

#define LOG_ID L"Java Generation"
#define JAVA_BRIDGE_FILE_NAME L"DllFunctions.java"
#define JAVA_PROJECT_SOURCE_PARENT_FOLDER L"src/main/java"

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

        if (!IsStartWith(javaOption->GetDllBridgeDirectory(), JAVA_PROJECT_SOURCE_PARENT_FOLDER))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Dll Bridge Directory is not start with " + JAVA_PROJECT_SOURCE_PARENT_FOLDER);

        std::wstring packageFolder = GetRelativePath(javaOption->GetDllBridgeDirectory(), JAVA_PROJECT_SOURCE_PARENT_FOLDER);
        packageFolder = GetLinuxPath(packageFolder);
        if (IsStartWith(packageFolder, L"/"))
            packageFolder = packageFolder.substr(1);
        if (IsEndWith(packageFolder, L"/"))
            packageFolder.pop_back();
        ReplaceAll(packageFolder, L"/", L".");
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