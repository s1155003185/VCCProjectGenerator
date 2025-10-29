#include "vpg_unittest_file_generation_service.hpp"

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_config.hpp"
#include "log_service.hpp"
#include "vpg_class_helper.hpp"
#include "vpg_enum_class.hpp"
#include "vpg_file_sync_service.hpp"
#include "vpg_tag_helper.hpp"

#include <string>

const std::wstring logId = L"Unittest File Generation";

std::wstring VPGUnittestFileGenerationService::generateUnittestClass(const std::wstring &className)
{
    TRY
        return  L"class " + className + L" : public testing::Test \r\n"
            + L"{\r\n"
            + INDENT + getVccTagHeaderCustomClassProperties(VPGCodeType::Cpp, className) + L"\r\n"
            + INDENT + getVccTagTailerCustomClassProperties(VPGCodeType::Cpp, className) + L"\r\n"
            + L"\r\n"
            + INDENT + L"public:\r\n"
            + INDENT + INDENT + className + L"() {}\r\n"
            + INDENT + INDENT + L"virtual ~" + className + L"() {}\r\n"
            + L"\r\n"
            + INDENT + INDENT + L"void SetUp() override\r\n"
            + INDENT + INDENT + L"{\r\n"
            + INDENT + INDENT + INDENT + getVccTagHeaderCustomClassCustomSetUp(VPGCodeType::Cpp, className) + L"\r\n"
            + INDENT + INDENT + INDENT + getVccTagTailerCustomClassCustomSetUp(VPGCodeType::Cpp, className) + L"\r\n"
            + INDENT + INDENT + L"}\r\n"
            + L"\r\n"
            + INDENT + INDENT + L"void TearDown() override\r\n"
            + INDENT + INDENT + L"{\r\n"
            + INDENT + INDENT + INDENT + getVccTagHeaderCustomClassCustomTearDown(VPGCodeType::Cpp, className) + L"\r\n"
            + INDENT + INDENT + INDENT + getVccTagTailerCustomClassCustomTearDown(VPGCodeType::Cpp, className) + L"\r\n"
            + INDENT + INDENT + L"}\r\n"
            + L"\r\n"
            + INDENT + INDENT + getVccTagHeaderCustomClassFunctions(VPGCodeType::Cpp, className) + L"\r\n"
            + INDENT + INDENT + getVccTagTailerCustomClassFunctions(VPGCodeType::Cpp, className) + L"\r\n"
            + L"};\r\n";
    CATCH
    return L"";
}

std::wstring VPGUnittestFileGenerationService::generateUnittestTestCase(const std::wstring &testClassName, const std::wstring &testName)
{
    TRY
        return L"\r\n"
            "TEST_F(" + testClassName + L", " + testName + L")\r\n"
            "{\r\n"
            + INDENT + getVccTagHeaderCustomClassCustomFunctions(VPGCodeType::Cpp, L"custom", testClassName + testName, L"") + L"\r\n"
            + INDENT + L"// TODO: Implement test case\r\n"
            + INDENT + getVccTagTailerCustomClassCustomFunctions(VPGCodeType::Cpp, L"custom", testClassName + testName, L"") + L"\r\n"
            + L"};\r\n";
    CATCH
    return L"";
}

void VPGUnittestFileGenerationService::generateCppAction(const vcc::LogConfig *logConfig, const std::wstring &projectPrefix, const std::wstring &originalPropertyFilePath, const std::wstring &folderPathCpp, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList, const std::vector<std::wstring> &unittestNames, const bool &isSeperateFile)
{
    TRY
        if (unittestNames.empty())
            return;

        std::wstring fileName = originalPropertyFilePath;
        if (vcc::find(fileName, L"_property.hpp") != std::wstring::npos)
            vcc::replace(fileName, L"_property.hpp", L"_action_test.cpp");
        else {
            if (vcc::isEndWith(fileName, L".hpp"))
                vcc::replace(fileName, L".hpp", L"_action_test.cpp");
            else if (vcc::isEndWith(fileName, L".h"))
                vcc::replace(fileName, L".h", L"_action_test.cpp");
            else
                fileName += L"_action_test.cpp";
        }

        std::wstring header = L"// <vcc:vccproj sync=\"FULL\" gen=\"FULL\"/>\r\n"
            "#include <gtest/gtest.h>\r\n"
            "\r\n"
            + getVccTagHeaderCustomHeader(VPGCodeType::Cpp) + L"\r\n"
            + getVccTagTailerCustomHeader(VPGCodeType::Cpp) + L"\r\n";
        std::wstring tail = getVccTagHeaderCustomClassFunctions(VPGCodeType::Cpp, L"") + L"\r\n"
            + getVccTagTailerCustomClassFunctions(VPGCodeType::Cpp, L"") + L"\r\n";

        // Get grouped file Test class
        // If only one class, then use class name
        // If have multi class, then use file name convert to valid class name
        std::wstring testClassName = L"";
        std::wstring content = L"";
        if (!isSeperateFile) {
            if (enumClassList.size() == 1) {
                std::wstring className = getClassNameFromPropertyClassName(getTypeOrClassWithoutNamespace(enumClassList.front()->getName()));
                testClassName = className + L"ActionTest";
            } else {
                std::wstring baseFileName = fileName;
                vcc::replace(baseFileName, L".cpp", L"");
                vcc::replace(baseFileName, L".h", L"");
                vcc::replace(baseFileName, L".hpp", L"");
                testClassName = getClassNameFromFileName(originalPropertyFilePath, projectPrefix) + L"ActionTest";
            }
            content += header
                + L"\r\n"
                + VPGUnittestFileGenerationService::generateUnittestClass(testClassName);
        }

        bool isGenerated = false;
        for (auto const &enumClass : enumClassList) {
            std::wstring currentWorkspace = getNamespaceFromClassName(enumClass->getName());
            std::wstring className = getClassNameFromPropertyClassName(getTypeOrClassWithoutNamespace(enumClass->getName()));
            std::wstring unittestClassName = className + L"ActionTest";
        
            for (auto const &property : enumClass->getProperties()) {
                if (property->getPropertyType() != VPGEnumClassAttributeType::Action)
                    continue;

                isGenerated = true;

                std::wstring actionClassName = className + property->getPropertyName();

                if (isSeperateFile) {
                    unittestClassName = actionClassName + L"Test";
                    content = header
                        + L"\r\n"
                        + VPGUnittestFileGenerationService::generateUnittestClass(unittestClassName);
                }

                for (auto const &unittestName : unittestNames) {
                    if (isSeperateFile)
                        content += VPGUnittestFileGenerationService::generateUnittestTestCase(unittestClassName, unittestName);
                    else
                        content += VPGUnittestFileGenerationService::generateUnittestTestCase(unittestClassName, actionClassName + L"_" + unittestName);
                }
                
                if (isSeperateFile) {
                    // Generate to seperate files
                    std::wstring fileNameCpp = getActionFileNameWithoutExtension(getTypeOrClassWithoutNamespace(getClassNameFromPropertyClassName(enumClass->getName())) + property->getPropertyName(), projectPrefix) + L"_test.cpp";
                
                    // Generate File
                    std::wstring filePathCpp = vcc::concatPaths({folderPathCpp, fileNameCpp});
                    vcc::LogService::logInfo(logConfig, logId, L"Generate unittest action unittest file: " + filePathCpp);
                    if (vcc::isFilePresent(filePathCpp))
                        content = VPGFileSyncService::SyncFileContent(VPGFileContentSyncTagMode::Generation, content, vcc::readFile(filePathCpp), VPGFileContentSyncMode::Full, L"//");
                    vcc::lTrim(content);
                    vcc::writeFile(filePathCpp, content, true);
                    vcc::LogService::logInfo(logConfig, logId, L"Generate unittest action unittest file completed.");

                    isGenerated = false;
                }
            }

        }

        if (!isSeperateFile && isGenerated) {
            // Generate File
            std::wstring filePathCpp = vcc::concatPaths({folderPathCpp, fileName});
            vcc::LogService::logInfo(logConfig, logId, L"Generate action unittest file: " + filePathCpp);
            if (vcc::isFilePresent(filePathCpp))
                content = VPGFileSyncService::SyncFileContent(VPGFileContentSyncTagMode::Generation, content, vcc::readFile(filePathCpp), VPGFileContentSyncMode::Full, L"//");
            vcc::lTrim(content);
            vcc::writeFile(filePathCpp, content, true);
            vcc::LogService::logInfo(logConfig, logId, L"Generate action unittest file completed.");
        }
    CATCH
}