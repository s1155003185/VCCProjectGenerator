#include "vpg_dll_file_generation_service.hpp"

#include <set>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_service.hpp"
#include "set_helper.hpp"

#include "vpg_file_generation_service.hpp"

#define LOG_ID L"Dll File Generation"

std::wstring VPGDllFileGenerationService::generateApplicationHpp(const VPGDllFileGenerationServiceOption *option)
{
    std::wstring result = L"";
    TRY
        if (!option->getIsGenerateApplication())
            return result;

        std::map<std::wstring, std::wstring> functionMap;
        functionMap.insert(std::make_pair(L"applicationStart", L"DLLEXPORT void applicationStart();\r\n"));

        // Initialize Form
        functionMap.insert(std::make_pair(L"applicationCreateForm", L"DLLEXPORT void *applicationCreateForm(int64_t objectType);\r\n"));
        
        // Result
        functionMap.insert(std::make_pair(L"applicationGetResultErrorCode", L"DLLEXPORT int64_t applicationGetResultErrorCode(void *result);\r\n"));
        functionMap.insert(std::make_pair(L"applicationGetResultMessage", L"DLLEXPORT void applicationGetResultMessage(void *result, wchar_t **value);\r\n"));
        functionMap.insert(std::make_pair(L"applicationIsErrorResult", L"DLLEXPORT bool applicationIsErrorResult(void *result);\r\n"));
        functionMap.insert(std::make_pair(L"applicationIsWarningResult", L"DLLEXPORT bool applicationIsWarningResult(void *result);\r\n"));
        functionMap.insert(std::make_pair(L"applicationEraseResult", L"DLLEXPORT void applicationEraseResult(void *result);\r\n"));

        // Form Action
        functionMap.insert(std::make_pair(L"applicationCreateActionArgument", L"DLLEXPORT void *applicationCreateActionArgument(int64_t objectType);\r\n"));

        functionMap.insert(std::make_pair(L"applicationDoFormAction", L"DLLEXPORT void *applicationDoFormAction(void *form, int64_t formProperty, void *argument);\r\n"));
        functionMap.insert(std::make_pair(L"applicationGetFormActionCurrentSeqNo", L"DLLEXPORT int64_t applicationGetFormActionCurrentSeqNo(void *form);\r\n"));
        functionMap.insert(std::make_pair(L"applicationGetFormActionFirstSeqNo", L"DLLEXPORT int64_t applicationGetFormActionFirstSeqNo(void *form);\r\n"));
        functionMap.insert(std::make_pair(L"applicationGetFormActionLastSeqNo", L"DLLEXPORT int64_t applicationGetFormActionLastSeqNo(void *form);\r\n"));

        functionMap.insert(std::make_pair(L"applicationRedoFormAction", L"DLLEXPORT void applicationRedoFormAction(void *form, int64_t noOfStep);\r\n"));
        functionMap.insert(std::make_pair(L"applicationRedoFormActionToSeqNo", L"DLLEXPORT void applicationRedoFormActionToSeqNo(void *form, int64_t seqNo);\r\n"));
        
        functionMap.insert(std::make_pair(L"applicationUndoFormAction", L"DLLEXPORT void applicationUndoFormAction(void *form, int64_t noOfStep);\r\n"));
        functionMap.insert(std::make_pair(L"applicationUndoFormActionToSeqNo", L"DLLEXPORT void applicationUndoFormActionToSeqNo(void *form, int64_t seqNo);\r\n"));

        functionMap.insert(std::make_pair(L"applicationClearFormAction", L"DLLEXPORT int64_t applicationClearFormAction(void *form);\r\n"));
        functionMap.insert(std::make_pair(L"applicationTruncateFormAction", L"DLLEXPORT int64_t applicationTruncateFormAction(void *form);\r\n"));

        // Close Form
        functionMap.insert(std::make_pair(L"applicationIsFormClosable", L"DLLEXPORT bool applicationIsFormClosable(void *form);\r\n"));
        functionMap.insert(std::make_pair(L"applicationIsFormClosed", L"DLLEXPORT bool applicationIsFormClosed(void *form);\r\n"));
        functionMap.insert(std::make_pair(L"applicationCloseForm", L"DLLEXPORT bool applicationCloseForm(void *form, bool isForce);\r\n"));
                    
        for (auto const &action : functionMap) {
            std::vector<std::wstring> lines = vcc::splitStringByLine(action.second);
            for (auto &line : lines) {
                vcc::rTrim(line);
                result += line + L"\r\n";
            }
        }
    CATCH
    return result;
}

std::wstring VPGDllFileGenerationService::generateApplicationCpp(const VPGDllFileGenerationServiceOption *option, std::set<std::wstring> &customIncludeFiles)
{
    std::wstring result = L"";
    TRY
        if (!option->getIsGenerateApplication())
            return result;

        customIncludeFiles.insert(L"application.hpp");
        customIncludeFiles.insert(L"i_object.hpp");
        customIncludeFiles.insert(L"object_type.hpp");

        std::map<std::wstring, std::wstring> functionMap;
        functionMap.insert(std::make_pair(L"applicationStart",
            L"void applicationStart()\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        Application::Run();\r\n"
            "    CATCH\r\n"
            "}\r\n"));

        // Initialize Form
        functionMap.insert(std::make_pair(L"applicationCreateForm",
            L"void *applicationCreateForm(int64_t objectType)\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        return Application::createForm(static_cast<ObjectType>(objectType)).get();\r\n"
            "    CATCH\r\n"
            "    return nullptr;\r\n"
            "}\r\n"));
            
        // Result
        functionMap.insert(std::make_pair(L"applicationGetResultErrorCode",
            L"int64_t applicationGetResultErrorCode(void *result)\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        return Application::getResultErrorCode(static_cast<vcc::IObject *>(result));\r\n"
            "    CATCH\r\n"
            "    return 0;\r\n"
            "}\r\n"));
        functionMap.insert(std::make_pair(L"applicationGetResultMessage",
            L"void applicationGetResultMessage(void *result, wchar_t **value)\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        std::wstring message = Application::getResultMessage(static_cast<vcc::IObject *>(result));\r\n"
            "        size_t size = (message.length() + 1) * sizeof(wchar_t);\r\n"
            "        *value = static_cast<wchar_t*>(malloc(size));\r\n"
            "        wcscpy(*value, message.c_str());\r\n"
            "    CATCH\r\n"
            "}\r\n"));
        
        functionMap.insert(std::make_pair(L"applicationIsErrorResult",
            L"bool applicationIsErrorResult(void *result)\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        return Application::isErrorResult(static_cast<vcc::IObject *>(result));\r\n"
            "    CATCH\r\n"
            "    return false;\r\n"
            "}\r\n"
        ));
        functionMap.insert(std::make_pair(L"applicationIsWarningResult",
            L"bool applicationIsWarningResult(void *result)\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        return Application::isWarningResult(static_cast<vcc::IObject *>(result));\r\n"
            "    CATCH\r\n"
            "    return false;\r\n"
            "}\r\n"
        ));
        functionMap.insert(std::make_pair(L"applicationEraseResult",
            L"void applicationEraseResult(void *result)\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        Application::eraseResult(static_cast<vcc::IObject *>(result));\r\n"
            "    CATCH\r\n"
            "}\r\n"));
            
        // Form Action
        functionMap.insert(std::make_pair(L"applicationCreateActionArgument",
            L"void *applicationCreateActionArgument(int64_t objectType)\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        return Application::createActionArgument(static_cast<ObjectType>(objectType)).get();\r\n"
            "    CATCH\r\n"
            "    return nullptr;\r\n"
            "}\r\n"));

        functionMap.insert(std::make_pair(L"applicationDoFormAction", 
            L"void *applicationDoFormAction(void *form, int64_t formProperty, void *argument)\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        return Application::doFormAction(static_cast<vcc::IObject *>(form), formProperty, static_cast<vcc::IObject *>(argument)).get();\r\n"
            "    CATCH\r\n"
            "    return nullptr;\r\n"
            "}\r\n"));
        functionMap.insert(std::make_pair(L"applicationGetFormActionCurrentSeqNo",
                L"int64_t applicationGetFormActionCurrentSeqNo(void *form)\r\n"
                "{\r\n"
                "    TRY\r\n"
                "        return Application::getFormActionCurrentSeqNo(static_cast<vcc::IObject *>(form));\r\n"
                "    CATCH\r\n"
                "    return -1;\r\n"
                "}\r\n"));
        functionMap.insert(std::make_pair(L"applicationGetFormActionFirstSeqNo",
            L"int64_t applicationGetFormActionFirstSeqNo(void *form)\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        return Application::getFormActionFirstSeqNo(static_cast<vcc::IObject *>(form));\r\n"
            "    CATCH\r\n"
            "    return -1;\r\n"
            "}\r\n"));
        functionMap.insert(std::make_pair(L"applicationGetFormActionLastSeqNo",
            L"int64_t applicationGetFormActionLastSeqNo(void *form)\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        return Application::getFormActionLastSeqNo(static_cast<vcc::IObject *>(form));\r\n"
            "    CATCH\r\n"
            "    return -1;\r\n"
            "}\r\n"));
            
        functionMap.insert(std::make_pair(L"applicationRedoFormAction",
            L"void applicationRedoFormAction(void *form, int64_t noOfStep)\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        Application::redoFormAction(static_cast<vcc::IObject *>(form), noOfStep);\r\n"
            "    CATCH\r\n"
            "}\r\n"));
        functionMap.insert(std::make_pair(L"applicationRedoFormActionToSeqNo",
            L"void applicationRedoFormActionToSeqNo(void *form, int64_t seqNo)\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        Application::redoFormActionToSeqNo(static_cast<vcc::IObject *>(form), seqNo);\r\n"
            "    CATCH\r\n"
            "}\r\n"));
            
            
        functionMap.insert(std::make_pair(L"applicationUndoFormAction",
            L"void applicationUndoFormAction(void *form, int64_t noOfStep)\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        Application::undoFormAction(static_cast<vcc::IObject *>(form), noOfStep);\r\n"
            "    CATCH\r\n"
            "}\r\n"));
        functionMap.insert(std::make_pair(L"applicationUndoFormActionToSeqNo",
            L"void applicationUndoFormActionToSeqNo(void *form, int64_t seqNo)\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        Application::undoFormActionToSeqNo(static_cast<vcc::IObject *>(form), seqNo);\r\n"
            "    CATCH\r\n"
            "}\r\n"));

        functionMap.insert(std::make_pair(L"applicationClearFormAction",
            L"int64_t applicationClearFormAction(void *form)\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        return Application::clearFormAction(static_cast<vcc::IObject *>(form));\r\n"
            "    CATCH\r\n"
            "    return -1;\r\n"
            "}\r\n"));
        functionMap.insert(std::make_pair(L"applicationTruncateFormAction",
            L"int64_t applicationTruncateFormAction(void *form)\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        return Application::truncateFormAction(static_cast<vcc::IObject *>(form));\r\n"
            "    CATCH\r\n"
            "    return -1;\r\n"
            "}\r\n"));
            
        // Close Form
        functionMap.insert(std::make_pair(L"applicationIsFormClosable",
            L"bool applicationIsFormClosed(void *form)\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        return Application::isFormClosed(static_cast<vcc::IObject *>(form));\r\n"
            "    CATCH\r\n"
            "    return false;\r\n"
            "}\r\n"));
        functionMap.insert(std::make_pair(L"applicationIsFormClosed",
            L"bool applicationIsFormClosable(void *form)\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        return Application::isFormClosable(static_cast<vcc::IObject *>(form));\r\n"
            "    CATCH\r\n"
            "    return false;\r\n"
            "}\r\n"));
        functionMap.insert(std::make_pair(L"applicationCloseForm",
            L"bool applicationCloseForm(void *form, bool isForce)\r\n"
            "{\r\n"
            "    TRY\r\n"
            "        return Application::closeForm(static_cast<vcc::IObject *>(form), isForce);\r\n"
            "    CATCH\r\n"
            "    return false;\r\n"
            "}\r\n"));

        for (auto const &action : functionMap) {
            std::vector<std::wstring> lines = vcc::splitStringByLine(action.second);
            result += L"\r\n";
            for (auto &line : lines) {
                vcc::rTrim(line);
                result += line + L"\r\n";
            }
        }
    CATCH
    return result;
}

std::wstring VPGDllFileGenerationService::generatePropertyAccessorHpp(const VPGDllFileGenerationServiceOption *option, std::set<std::wstring> &customIncludeFiles)
{
    std::wstring result = L"";
    TRY
        if (!option->getIsGeneratePropertyAccessor())
            return result;

        customIncludeFiles.insert(L"object_factory.hpp");
        customIncludeFiles.insert(L"property_accessor_factory.hpp");
        customIncludeFiles.insert(L"property_accessor_macro.hpp");

        result +=
            L"PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(bool, Bool)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(char, Char)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(wchar_t, Wchar)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(int, Int8)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(int, Uint8)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(int, Short)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(int, UnsignedShort)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(long, Int)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(long, UnsignedInt)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(long, Long)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(long, UnsignedLong)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(float, Float)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER(double, Double)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_STRING\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_OBJECT\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_HEADER_CONTAINER\r\n";
    CATCH
    return result;
}

std::wstring VPGDllFileGenerationService::generatePropertyAccessorCpp(const VPGDllFileGenerationServiceOption *option, std::set<std::wstring> &customIncludeFiles)
{
    std::wstring result = L"";
    TRY
        if (!option->getIsGeneratePropertyAccessor())
            return result;

        customIncludeFiles.insert(L"exception_macro.hpp");
        customIncludeFiles.insert(L"i_object.hpp");
        customIncludeFiles.insert(L"lock_type.hpp");
        customIncludeFiles.insert(L"property_accessor_factory.hpp");
        customIncludeFiles.insert(L"property_accessor_macro.hpp");

        result +=
            L"PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(bool, Bool, false)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(char, Char, L'\\0')\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(wchar_t, Wchar, L'\\0')\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(int, Int8, 0)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(int, Uint8, 0)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(int, Short, 0)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(int, UnsignedShort, 0)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(long, Int, 0)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(long, UnsignedInt, 0)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(long, Long, 0)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(long, UnsignedLong, 0)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(float, Float, 0)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL(double, Double, 0)\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_STRING\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_OBJECT\r\n"
            "PROPERTY_ACCESSOR_DLL_EXPORT_MACRO_DETAIL_CONTAINER\r\n";
    CATCH
    return result;
}

void VPGDllFileGenerationService::generateHpp(const vcc::LogConfig *logConfig, const std::wstring &filePathHpp, const VPGDllFileGenerationServiceOption *option)
{
    TRY
        assert(option != nullptr);
        if (!vcc::isFilePresent(filePathHpp))
            return;
        
        vcc::LogService::logInfo(logConfig, LOG_ID, L"Modify DllFunctions.hpp file: " + filePathHpp);

        // header
        std::wstring content = L"";
        std::set<std::wstring> customIncludeFiles;
        std::wstring applicationStr = generateApplicationHpp(option);
        std::wstring propertyAccessorStr = generatePropertyAccessorHpp(option, customIncludeFiles);
        if (!customIncludeFiles.empty()) {
            for (auto const &file : customIncludeFiles)
                content += L"#include " + vcc::getEscapeStringWithQuote(vcc::EscapeStringType::DoubleQuote, file) + L"\r\n";
        }
        vcc::writeFile(filePathHpp, VPGFileGenerationService::generateFileContent(vcc::readFile(filePathHpp), L"vcc:dllInterfaceHeader", content, L"//"), true);

        // content
        content = applicationStr;
        content += propertyAccessorStr;

        vcc::writeFile(filePathHpp, VPGFileGenerationService::generateFileContent(vcc::readFile(filePathHpp), L"vcc:dllInterface", content, L"//"), true);
        vcc::LogService::logInfo(logConfig, LOG_ID, L"Modify DllFunctions.hpp file completed.");
    CATCH
}

void VPGDllFileGenerationService::generateCpp(const vcc::LogConfig *logConfig, const std::wstring &filePathCpp, const VPGDllFileGenerationServiceOption *option)
{
    TRY
        assert(option != nullptr);
        if (!vcc::isFilePresent(filePathCpp))
            return;

        vcc::LogService::logInfo(logConfig, LOG_ID, L"Modify DllFunctions.cpp file: " + filePathCpp);
        // header
        std::wstring content = L"";
        std::set<std::wstring> customIncludeFiles;
        std::wstring applicationStr = generateApplicationCpp(option, customIncludeFiles);
        std::wstring propertyAccessorStr = generatePropertyAccessorCpp(option, customIncludeFiles);

        if (!customIncludeFiles.empty()) {
            for (auto const &file : customIncludeFiles)
                content += L"#include " + vcc::getEscapeStringWithQuote(vcc::EscapeStringType::DoubleQuote, file) + L"\r\n";
        }
        vcc::writeFile(filePathCpp, VPGFileGenerationService::generateFileContent(vcc::readFile(filePathCpp), L"vcc:dllInterfaceHeader", content, L"//"), true);

        // content
        content = applicationStr;
        content += propertyAccessorStr;
        vcc::writeFile(filePathCpp, VPGFileGenerationService::generateFileContent(vcc::readFile(filePathCpp), L"vcc:dllInterface", content, L"//"), true);

        vcc::LogService::logInfo(logConfig, LOG_ID, L"Modify DllFunctions.cpp completed.");
    CATCH
}