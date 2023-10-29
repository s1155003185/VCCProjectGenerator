#include "vpg_file_generation_service.hpp"

#include <filesystem>
#include <fstream>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_service.hpp"
#include "string_helper.hpp"

#include "vpg_enum_class_reader.hpp"

constexpr auto classMacroFilePath = L"include/External/VCC/Core/Macro/class_macro.hpp";
constexpr auto logId = L"File Generation";

constexpr auto objectTypeHppFileName = L"object_type.hpp";

std::set<std::wstring> VPGFileGenerationService::GetClassMacroList(const std::wstring &projWorkspace)
{
    std::set<std::wstring> results;
    try {
        std::wstring filePath = ConcatPath(projWorkspace, classMacroFilePath);
        std::wstring prefix = L"#define ";
        size_t prefixLen = wcslen(prefix.c_str());
        ReadFilePerLine(filePath, [prefix, prefixLen, &results](std::wstring line) {
            Trim(line);
            if (line.starts_with(prefix) && line.find(L"(") != std::wstring::npos) {
                std::wstring type = line.substr(prefixLen, line.find(L"(") - prefixLen);
                Trim(type);
                results.insert(type);
            }
        });
    } catch (std::exception &ex) {
        THROW_EXCEPTION(ex);
    }
    return results;
}

void VPGFileGenerationService::GenerateObjectTypeFile(LogProperty &logProperty, const std::wstring &classPrefix, const std::wstring &hppFilePath, const std::vector<std::wstring> &propertyTypeList)
{
    try {
        LogService::LogInfo(logProperty, logId, L"Generate object type file: " + hppFilePath);

        LogService::LogInfo(logProperty, logId, L"Generate object type file completed.");
    } catch (std::exception &ex) {
        THROW_EXCEPTION(ex);
    }
}

void VPGFileGenerationService::GeneratePropertyClassFile(LogProperty &logProperty, const std::wstring &classPrefix, const std::wstring &hppFilePath, const std::vector<VPGEnumClass> &enumClassList)
{
    try {
        LogService::LogInfo(logProperty, logId, L"Generate object type file: " + hppFilePath);

        LogService::LogInfo(logProperty, logId, L"Generate object type file completed.");
    } catch (std::exception &ex) {
        THROW_EXCEPTION(ex);
    }
}

void VPGFileGenerationService::GeneratePropertyPropertyAccessorFile(LogProperty &logProperty, const std::wstring &classPrefix, const std::wstring &hppFilePath, const std::wstring &cppFilePath, const std::vector<VPGEnumClass> &enumClassList)
{
    try {
        LogService::LogInfo(logProperty, logId, L"Generate property accessor file: " + hppFilePath);

        LogService::LogInfo(logProperty, logId, L"Generate property accessor file completed.");
        LogService::LogInfo(logProperty, logId, L"Generate property accessor file: " + cppFilePath);

        LogService::LogInfo(logProperty, logId, L"Generate property accessor file completed.");
    } catch (std::exception &ex) {
        THROW_EXCEPTION(ex);
    }
}

void VPGFileGenerationService::GernerateProperty(LogProperty &logProperty, const std::wstring &projPrefix, const std::wstring &projWorkspace, const std::wstring &typeWorkspace, 
    const std::wstring &objTypeDirectory, const std::wstring &objDirectory, const std::wstring &propertyAccessorDirectory)
{
    try {
        std::set<std::wstring> classMacroList = VPGFileGenerationService::GetClassMacroList(projWorkspace);
        if (classMacroList.empty())
            THROW_EXCEPTION_M(ExceptionType::FILE_NOT_FOUND, classMacroFilePath + L" missing");

        //Generate Object Type, Object Class, PropertyAccessor,
        // 1. get all files from directory
        // 2. get all properties with enum class Prefix + Class + Property

        VPGEnumClassReader enumClassReader;
        std::wstring filePrefix = projPrefix;
        ToLower(filePrefix);
        std::wstring fileSuffix = L"";

        LogService::LogInfo(logProperty, logId, L"Generate property start.");
        std::vector<std::wstring> objectTypeList;
        for (auto &filePath : std::filesystem::recursive_directory_iterator(PATH(typeWorkspace))) {
            std::wstring path = filePath.path().wstring();
            ToLower(path);
            if (path.ends_with(L"property.hpp")) {
                if (!filePrefix.empty()) {
                    std::wstring fileName = filePath.path().filename().wstring();
                    ToLower(fileName);
                    if (!HasPrefix(fileName, filePrefix)) {
                        LogService::LogWarning(logProperty, logId, L"File suffix " + projPrefix + L" missing. Skip: " + path);
                        continue;
                    }
                }
                // ------------------------------------------------------------------------------------------ //
                //                                      Parse File Start                                      //
                // ------------------------------------------------------------------------------------------ //
                LogService::LogWarning(logProperty, logId, L"Parse file start: " + path);

                std::wstring fileContent = ReadFile(path);
                Trim(fileContent);
                if (fileContent.empty())
                    continue;

                std::vector<VPGEnumClass> enumClassList;
                enumClassReader.Parse(fileContent, enumClassList);
                for (const VPGEnumClass &enumClass : enumClassList) {
                    if (!projPrefix.empty() && !HasPrefix(enumClass.Name, projPrefix)) {
                        LogService::LogWarning(logProperty, logId, L"Suffix property.hpp missing. Skip: " + path);
                        continue;
                    }
                    objectTypeList.push_back(enumClass.Name);

                    // ------------------------------------------------------------------------------------------ //
                    //                               Generate Object Type File                                    //
                    // ------------------------------------------------------------------------------------------ //
                    VPGFileGenerationService::GeneratePropertyClassFile(logProperty, projPrefix, objDirectory, enumClassList);
                    VPGFileGenerationService::GeneratePropertyPropertyAccessorFile(logProperty, projPrefix, propertyAccessorDirectory, propertyAccessorDirectory, enumClassList);
                }
                LogService::LogInfo(logProperty, logId, L"Parse file completed: " + path);
                // ------------------------------------------------------------------------------------------ //
                //                                      Parse File End                                        //
                // ------------------------------------------------------------------------------------------ //
            } else {
                LogService::LogWarning(logProperty, logId, L"Suffix property.hpp missing. Skip: " + path);
            }
        }
        // ------------------------------------------------------------------------------------------ //
        //                               Generate Object Type File                                    //
        // ------------------------------------------------------------------------------------------ //
        VPGFileGenerationService::GenerateObjectTypeFile(logProperty, projPrefix, ConcatPath(objTypeDirectory, (!filePrefix.empty() ? (filePrefix + L"_") : L"") + objectTypeHppFileName), objectTypeList);

        LogService::LogInfo(logProperty, logId, L"Generate Property Finished.");
    } catch (std::exception &ex) {
        THROW_EXCEPTION(ex);
    }
}