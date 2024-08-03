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
            THROW_EXCEPTION_MSG(ExceptionType::ParserError, L"DllFunctions.h is missing 'extern \"C\"'");

        pos += dllExportStart.size() - 1;
        while (pos < content.length()) {
            std::wstring subStr = content.substr(pos);
            if (IsStartWith(content, dllExport, pos)) {
                pos += dllExport.length() - 1;

            } else if (IsStartWith(content, dllInterfaceExportPropertyAccessor, pos)) {
                // pos = Find(content, L"(", pos);
                // size_t endPos = Find(content, L")", pos);
                // pos = endPos;
            } else if (IsStartWith(content, dllInterfaceExportPropertyAccessorString, pos)) {
                result += INDENT + L"void ReadString(PointerByReference ref, Integer property, PointerByReference value, Integer index);\r\n"
                    + INDENT + L"void ReadStringByKey(PointerByReference ref, Integer property, PointerByReference value, PointerByReference key);\r\n"
                    + INDENT + L"void WriteString(PointerByReference ref, Integer property, PointerByReference value, Integer index);\r\n"
                    + INDENT + L"void WriteStringByKey(PointerByReference ref, Integer property, PointerByReference value, PointerByReference key);\r\n";
                //pos += dllInterfaceExportPropertyAccessorString.length() - 1;
            } else if (IsStartWith(content, dllInterfaceExportPropertyAccessorObject, pos)) {
                result += INDENT + L"PointerByReference ReadObject(PointerByReference ref, Integer property, Integer index);\r\n"
                    + INDENT + L"PointerByReference ReadObjectByKey(PointerByReference ref, Integer property, PointerByReference key);\r\n"
                    + INDENT + L"void WriteObject(PointerByReference ref, Integer property, PointerByReference value, Integer index);\r\n"
                    + INDENT + L"void WriteObjectByKey(PointerByReference ref, Integer property, PointerByReference value, PointerByReference key);\r\n";
                //pos += dllInterfaceExportPropertyAccessorObject.length() - 1;
            } else if (IsStartWith(content, dllInterfaceExportPropertyAccessorContainer, pos)) {
                result += INDENT + L"Integer GetContainerCount(PointerByReference ref, Integer property);\r\n"
                    + INDENT + L"Boolean IsContainKey(PointerByReference ref, Integer property, PointerByReference key);\r\n"
                    + INDENT + L"void RemoveContainerElement(PointerByReference ref, Integer property, Integer index);\r\n"
                    + INDENT + L"void RemoveContainerElementByKey(PointerByReference ref, Integer property, PointerByReference key);\r\n"
                    + INDENT + L"void ClearContainer(PointerByReference ref, Integer property);\r\n";
                //pos += dllInterfaceExportPropertyAccessorContainer.length() - 1;
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