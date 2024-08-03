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
        ReplaceAll(packageFolder, L"/", L".");
        result += L"package " + packageFolder + L"\r\n"
            "\r\n"
            "import com.sun.jna.Library;\r\n"
            "import com.sun.jna.Native;\r\n"
            "import com.sun.jna.ptr.PointerByReference;\r\n"
            "\r\n"
            "interface " + filePrefix + L"DllFunctions extends Library {\r\n"
            + INDENT + filePrefix + L"DllFunctions INSTANCE = (" + filePrefix + L"DllFunctions)Native.load(\"vpg\", " + filePrefix + L"DllFunctions.class);\r\n"
            "\r\n";

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