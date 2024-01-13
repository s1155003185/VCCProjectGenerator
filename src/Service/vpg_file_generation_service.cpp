#include "vpg_file_generation_service.hpp"

#include <filesystem>
#include <fstream>
#include <numeric>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_service.hpp"
#include "string_helper.hpp"

#include "vpg_enum_class_reader.hpp"

const std::wstring classMacroFilePath = L"include/External/VCC/Core/Macro/class_macro.hpp";
const std::wstring logId = L"File Generation";

const std::wstring objectTypeHppFileName = L"object_type.hpp";
const std::wstring propertyFileSuffix = L"property.hpp";
const std::wstring proeprtyClassNameSuffix = L"Property";
const std::wstring propertyAccessorFileSuffixWithoutExtention = L"property_accessor";

std::set<std::wstring> VPGFileGenerationService::GetClassMacroList(const std::wstring &projWorkspace)
{
    std::set<std::wstring> results;
    TRY_CATCH(
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
    )
    return results;
}

void VPGFileGenerationService::GenerateObjectTypeFile(const LogProperty *logProperty, const std::wstring &classPrefix, const std::wstring &hppFilePath, const std::vector<std::wstring> &propertyTypeList)
{
    TRY_CATCH(
        LogService::LogInfo(logProperty, logId, L"Generate object type file: " + hppFilePath);
        std::wstring content = L"#pragma once\r\n";
        content += L"\r\n";
        content += L"enum class " + classPrefix + L"ObjectType {\r\n";
        for (size_t i = 0; i < propertyTypeList.size(); i++) {
            content += INDENT + propertyTypeList.at(i) + (i < propertyTypeList.size() - 1 ? L"," : L"") + L"\r\n";
        }
        content += L"};\r\n";
        WriteFile(hppFilePath, content, true);
        LogService::LogInfo(logProperty, logId, L"Generate object type file completed.");
    )
}

void VPGFileGenerationService::GeneratePropertyClassFile(const LogProperty *logProperty, const std::wstring &classPrefix, const std::wstring &hppFilePath, const std::vector<VPGEnumClass> &enumClassList)
{
    TRY_CATCH(
        LogService::LogInfo(logProperty, logId, L"Generate object class file: " + hppFilePath);
        std::wstring content = L"#pragma once\r\n";
        content += L"\r\n";
        content += L"#include \"base_object.hpp\"\r\n";
        content += L"#include \"class_marco.hpp\"\r\n";
        content += L"\r\n";
        content += L"using namespace vcc;\r\n";
        content += L"\r\n";
        
        // generate external class
        for (const VPGEnumClass &enumClass : enumClassList) {
            for (const VPGEnumClassProperty &property : *enumClass.GetProperties()) {
                // handle enum without macro case
                if (property.GetMacro().empty())
                    continue;
                // TODO: parse all class to get class list (include .hpp file?)
                // TODO: need to get type list to handle enum class
                if (std::iswupper(property.GetType()[0])) {
                    content += L"class " + property.GetType() + L";\r\n";
                }
            }
        }
        // generate class
        for (const VPGEnumClass &enumClass : enumClassList) {
            std::wstring className = enumClass.GetName().substr(!classPrefix.empty() ? classPrefix.length() : 0);
            className = className.substr(0, className.length() - proeprtyClassNameSuffix.length());

            content += L"class " + className + L" : public BaseObject\r\n";
            content += L"{\r\n";
            // generate properties
            for (const VPGEnumClassProperty &property : *enumClass.GetProperties()) {
                // handle enum without macro case
                if (property.GetMacro().empty())
                    continue;
                content += INDENT + property.GetMacro() + L"\r\n";
            }
            content += L"\r\n";
            content += INDENT + L"public:\r\n";
            content += INDENT + INDENT + className + L"() : BaseObject(ObjectType::" + className + L") {}\r\n";
            content += INDENT + INDENT + L"~" + className + L"() {}\r\n";
            content += L"\r\n";
            content += INDENT + INDENT +  L"virtual std::wstring GetKey() override { return L\"\"; }\r\n"; // TODO: not handle yet
            content += INDENT + INDENT +  L"virtual std::wstring ToString() override { return L\"\"; }\r\n"; // TODO: not handle yet
            content += INDENT + INDENT +  L"virtual std::shared_ptr<IObject> Clone() override { return make_shared<" + className + L">(*this); }\r\n"; // TODO: not handle yet
            content += L"};\r\n";
            content += L"\r\n";
        }


        WriteFile(hppFilePath, content, true);
        LogService::LogInfo(logProperty, logId, L"Generate object class file completed.");
    )
}

void VPGFileGenerationService::GeneratePropertyPropertyAccessorFile(const LogProperty *logProperty, const std::wstring &classPrefix, const std::wstring &hppFilePath, const std::wstring &cppFilePath, const std::vector<VPGEnumClass> &enumClassList)
{
    TRY_CATCH(
        LogService::LogInfo(logProperty, logId, L"Generate property accessor file: " + hppFilePath);
        std::wstring content = L"";
        WriteFile(hppFilePath, content, true);
        LogService::LogInfo(logProperty, logId, L"Generate property accessor file completed.");
        
        LogService::LogInfo(logProperty, logId, L"Generate property accessor file: " + cppFilePath);
        content = L"";
        WriteFile(cppFilePath, content, true);
        LogService::LogInfo(logProperty, logId, L"Generate property accessor file completed.");
    )
}

void VPGFileGenerationService::GernerateProperty(const LogProperty *logProperty, const std::wstring &projPrefix, const std::wstring &projWorkspace, const std::wstring &typeWorkspace, 
    const std::wstring &objTypeDirectoryHpp, const std::wstring &objDirectoryHpp, const std::wstring &propertyAccessorDirectoryHpp, const std::wstring &propertyAccessorDirectoryCpp)
{
    TRY_CATCH(
        std::set<std::wstring> classMacroList = VPGFileGenerationService::GetClassMacroList(projWorkspace);
        if (classMacroList.empty())
            THROW_EXCEPTION_MSG(ExceptionType::FileNotFound, classMacroFilePath + L" missing");

        //Generate Object Type, Object Class, PropertyAccessor,
        // 1. get all files from directory
        // 2. get all properties with enum class Prefix + Class + Property

        VPGEnumClassReader enumClassReader(classMacroList);
        std::wstring filePrefix = projPrefix;
        ToLower(filePrefix);
        std::wstring classSuffix = L"Property";

        LogService::LogInfo(logProperty, logId, L"Generate property start.");
        std::vector<std::wstring> objectTypeList;
        for (auto &filePath : std::filesystem::recursive_directory_iterator(PATH(typeWorkspace))) {
            std::wstring path = filePath.path().wstring();
            ToLower(path);
            if (path.ends_with(propertyFileSuffix)) {
                std::wstring fileName = filePath.path().filename().wstring();
                if (!filePrefix.empty()) {
                    ToLower(fileName);
                    if (!HasPrefix(fileName, filePrefix)) {
                        LogService::LogWarning(logProperty, logId, L"File prefix " + projPrefix + L" missing. Skip: " + path);
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
                    if (!projPrefix.empty() && !HasPrefix(enumClass.GetName(), projPrefix)) {
                        LogService::LogWarning(logProperty, logId, L"Class Prefix " + projPrefix + L" missing. Skip: " + enumClass.GetName());
                        continue;
                    }
                    if (!enumClass.GetName().ends_with(classSuffix)) {
                        LogService::LogWarning(logProperty, logId, L"Class Suffix " + classSuffix + L" missing. Skip: " + enumClass.GetName());
                        continue;
                    }
                    std::wstring classNameWithoutPrefix = enumClass.GetName().substr(!projPrefix.empty() ? projPrefix.size() : 0);
                    classNameWithoutPrefix = classNameWithoutPrefix.substr(0, classNameWithoutPrefix.size() - classSuffix.size());
                    objectTypeList.push_back(classNameWithoutPrefix);
                    
                    // ------------------------------------------------------------------------------------------ //
                    //                               Generate Object Type File                                    //
                    // ------------------------------------------------------------------------------------------ //                    
                    std::wstring objectFileName = fileName.substr(0, fileName.size() - propertyFileSuffix.size());
                    if (objectFileName.ends_with(L"_"))
                        objectFileName.pop_back();

                    std::wstring propertyAccessorFileName = objectFileName + L"_" + propertyAccessorFileSuffixWithoutExtention;

                    VPGFileGenerationService::GeneratePropertyClassFile(logProperty, projPrefix, ConcatPath(objDirectoryHpp, objectFileName + L".hpp"), enumClassList);
                    VPGFileGenerationService::GeneratePropertyPropertyAccessorFile(logProperty, projPrefix, 
                        ConcatPath(propertyAccessorDirectoryHpp, propertyAccessorFileName + L".hpp"), 
                        ConcatPath(propertyAccessorDirectoryCpp, propertyAccessorFileName + L".cpp"), enumClassList);
                }
                LogService::LogInfo(logProperty, logId, L"Parse file completed: " + path);
                // ------------------------------------------------------------------------------------------ //
                //                                      Parse File End                                        //
                // ------------------------------------------------------------------------------------------ //
            } else {
                LogService::LogWarning(logProperty, logId, L"Suffix " + propertyFileSuffix + L" missing. Skip: " + path);
            }
        }
        // ------------------------------------------------------------------------------------------ //
        //                               Generate Object Type File                                    //
        // ------------------------------------------------------------------------------------------ //
        VPGFileGenerationService::GenerateObjectTypeFile(logProperty, projPrefix, ConcatPath(objTypeDirectoryHpp, (!filePrefix.empty() ? (filePrefix + L"_") : L"") + objectTypeHppFileName), objectTypeList);

        LogService::LogInfo(logProperty, logId, L"Generate Property Finished.");
    )
}
