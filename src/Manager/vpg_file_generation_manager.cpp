#include "vpg_file_generation_manager.hpp"

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
const std::wstring propertyFileSuffix = L"_property.hpp";
const std::wstring proeprtyClassNameSuffix = L"Property";
const std::wstring propertyAccessorFileSuffixWithoutExtention = L"property_accessor";

void VPGFileGenerationManager::GetClassMacroList(const std::wstring &projWorkspace)
{
    TRY_CATCH(
        std::wstring filePath = ConcatPaths({projWorkspace, classMacroFilePath});
        std::wstring prefix = L"#define ";
        size_t prefixLen = wcslen(prefix.c_str());
        ReadFilePerLine(filePath, [prefix, prefixLen, this](std::wstring line) {
            Trim(line);
            if (line.starts_with(prefix) && line.find(L"(") != std::wstring::npos) {
                std::wstring type = line.substr(prefixLen, line.find(L"(") - prefixLen);
                Trim(type);
                this->_ClassMacros.insert(type);
            }
        });
        if (this->_ClassMacros.empty())
            THROW_EXCEPTION_MSG(ExceptionType::FileNotFound, classMacroFilePath + L" missing");
    )
}

std::wstring VPGFileGenerationManager::GetClassNameFromEnumClassName(const std::wstring &enumClassName)
{
    TRY_CATCH(
        if (enumClassName.ends_with(proeprtyClassNameSuffix) && enumClassName != proeprtyClassNameSuffix)
            return enumClassName.substr(0, enumClassName.length() - proeprtyClassNameSuffix.length());
    )
    return enumClassName;
}

std::wstring VPGFileGenerationManager::GetClassFilenameFromEnumClassFilename(const std::wstring &enumClassFileName)
{
    TRY_CATCH(
        std::wstring tmpFileName = enumClassFileName;
        ReplaceAll(tmpFileName, propertyFileSuffix, L".hpp");
        return tmpFileName;
    )
    return enumClassFileName;
}

void VPGFileGenerationManager::GetFileList(const VPGEnumClassReader *reader, const std::wstring &directoryFullPath, const std::wstring &projectPrefix)
{
    TRY_CATCH(
        for (auto const &filePath : std::filesystem::recursive_directory_iterator(PATH(directoryFullPath))) {
            try
            {
                std::wstring content = ReadFile(filePath.path().wstring());
                std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
                reader->Parse(content, enumClassList);
                for (auto const &enumClass : enumClassList) {
                    // enum
                    std::wstring enumClassName = enumClass->GetName();
                    std::wstring fileName = filePath.path().filename().wstring();
                    if (_EnumClassFiles.count(enumClassName) > 0)
                        THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Enum Class " + enumClassName + L" duplicated:\r\n"
                            + _EnumClassFiles[enumClassName] + L"\r\n"
                            + fileName);
                    _EnumClassFiles.insert(std::make_pair(enumClassName, fileName));
                    // class
                    if (IsClassEnumFile(fileName, projectPrefix) && IsClassEnum(enumClassName, projectPrefix)) {
                        std::wstring className = GetClassNameFromEnumClassName(enumClassName);
                        if (_ClassFiles.count(className) > 0)
                            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Class " + className + L" duplicated:\r\n"
                                + _ClassFiles[className] + L"\r\n"
                                + fileName);
                        _ClassFiles.insert(std::make_pair(className, GetClassFilenameFromEnumClassFilename(fileName)));
                    }
                }
            }
            catch(...)
            {
            }            
        }
    )
}

bool VPGFileGenerationManager::IsClassEnumFile(const std::wstring &filename, const std::wstring &projectPrefix)
{
    TRY_CATCH(
        std::wstring tmpFileName = filename;
        ToLower(tmpFileName);
        std::wstring tmpProjectPrefix = projectPrefix;
        ToLower(tmpProjectPrefix);
        return tmpFileName.ends_with(propertyFileSuffix) && (IsBlank(tmpProjectPrefix) || tmpFileName.starts_with(tmpProjectPrefix));
    )
    return false;
}

bool VPGFileGenerationManager::IsClassEnum(const std::wstring &enumClassName, const std::wstring &projectPrefix)
{
    TRY_CATCH(
        return enumClassName.ends_with(proeprtyClassNameSuffix) && enumClassName.length() > proeprtyClassNameSuffix.length()
            && (IsBlank(projectPrefix) || enumClassName.starts_with(projectPrefix));
    )
    return false;
}

void VPGFileGenerationManager::GenerateObjectTypeFile(const LogProperty *logProperty, const std::wstring &hppFilePath, const std::vector<std::wstring> &propertyTypeList)
{
    TRY_CATCH(
        LogService::LogInfo(logProperty, logId, L"Generate object type file: " + hppFilePath);
        std::wstring content = L"#pragma once\r\n";
        content += L"\r\n";
        content += L"enum class ObjectType {\r\n";
        content += INDENT + L"NA" + (!propertyTypeList.empty() ? L"," : L"") + L"\r\n";
        for (size_t i = 0; i < propertyTypeList.size(); i++) {
            content += INDENT + propertyTypeList.at(i) + (i < propertyTypeList.size() - 1 ? L"," : L"") + L"\r\n";
        }
        content += L"};\r\n";
        WriteFile(hppFilePath, content, true);
        LogService::LogInfo(logProperty, logId, L"Generate object type file completed.");
    )
}

void VPGFileGenerationManager::GeneratePropertyClassFile(const LogProperty *logProperty, const std::wstring &classPrefix, const std::wstring &hppFilePath, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList)
{
    TRY_CATCH(
        LogService::LogInfo(logProperty, logId, L"Generate object class file: " + hppFilePath);

        // TODO: need to enable to check all systemn function
        std::set<std::wstring> systemFileList;
        std::set<std::wstring> projectFileList;
        std::set<std::wstring> abstractClassList;

        projectFileList.insert(L"#include \"base_object.hpp\"");
        projectFileList.insert(L"#include \"class_macro.hpp\"");
        projectFileList.insert(L"#include \"object_type.hpp\"");
        // generate external class
        for (auto const &enumClass : enumClassList) {
            for (std::shared_ptr<VPGEnumClassProperty> property : enumClass->GetProperties()) {
                // handle enum without macro case
                if (property->GetMacro().empty())
                    continue;
                std::wstring type = property->GetType();
                if (std::iswupper(type[0])) {
                    if (property->GetMacro().substr(0, property->GetMacro().find(L"(")).find(L"SPTR") != std::wstring::npos) {
                        if (_ClassFiles.contains(type))
                            projectFileList.insert(L"#include " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, _ClassFiles[type]));
                        else
                            abstractClassList.insert(L"class " + type + L";");
                    } else {
                        if (_EnumClassFiles.contains(type))
                            projectFileList.insert(L"#include " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, _EnumClassFiles[type]));
                        else
                            abstractClassList.insert(L"enum class " + property->GetType() + L";");
                    }
                } else {
                    // TODO: need to enable to check all systemn function
                    // system type
                    if (CountSubstring(type, L"string") > 0)
                        systemFileList.insert(L"#include <string>");
                }
            }
        }
        std::wstring systemFileListStr = L"";
        std::wstring projectFileListStr = L"";
        for (auto const &str : systemFileList)
            systemFileListStr += str + L"\r\n";
        for (auto const &str : projectFileList)
            projectFileListStr += str + L"\r\n";

        std::wstring content = L"#pragma once\r\n";
        content += L"\r\n";
        content += !systemFileListStr.empty() ? (systemFileListStr + L"\r\n") : L"";
        content += !projectFileListStr.empty() ? (projectFileListStr + L"\r\n") : L"";
        
        // as using base_object and macro, must have namespace vcc
        content += L"using namespace vcc;\r\n";
        content += L"\r\n";
        // for those class that cannot be found in file list
        for (auto const &str : abstractClassList)
            content += str + L"\r\n";
            
        // generate class
        for (auto const &enumClass : enumClassList) {
            std::wstring className = enumClass->GetName().substr(0, enumClass->GetName().length() - proeprtyClassNameSuffix.length());
            content += L"class " + className + L" : public BaseObject<" + className + L">\r\n";
            content += L"{\r\n";
            // generate properties
            for (std::shared_ptr<VPGEnumClassProperty> property : enumClass->GetProperties()) {
                // handle enum without macro case
                if (property->GetMacro().empty())
                    continue;
                content += INDENT + property->GetMacro() + L"\r\n";
            }
            content += L"\r\n";
            content += INDENT + L"public:\r\n";
            content += INDENT + INDENT + className + L"() : BaseObject(ObjectType::" + className.substr(!classPrefix.empty() ? classPrefix.length() : 0) + L") {}\r\n";
            content += INDENT + INDENT + L"virtual ~" + className + L"() {}\r\n";

            bool isPtrExists = false;
            for (auto const &property : enumClass->GetProperties()) {
                // handle enum without macro case
                if (property->GetMacro().empty())
                    continue;
                if (std::iswupper(property->GetType()[0])) {
                    if (property->GetMacro().substr(0, property->GetMacro().find(L"(")).find(L"SPTR") != std::wstring::npos) {
                        isPtrExists = true;
                        break;
                    }
                }
            }
            if (isPtrExists) {
                content += L"\r\n";                
                content += INDENT + INDENT + L"virtual std::shared_ptr<IObject> Clone() const override {\r\n";
                content += INDENT + INDENT + INDENT + L"std::shared_ptr<" + className + L"> obj = std::make_shared<" + className + L">(*this);\r\n";
                for (auto const &property : enumClass->GetProperties()) {
                    // handle enum without macro case
                    if (property->GetMacro().empty())
                        continue;
                    if (std::iswupper(property->GetType()[0])) {
                        if (property->GetMacro().find(L"SPTR")) {
                            content += INDENT + INDENT + INDENT + L"obj->Clone" + property->GetPropertyName() + L"(this->_" + property->GetPropertyName() + L");\r\n";
                        }
                    }
                }
                content += INDENT + INDENT + INDENT + L"return obj;\r\n";
                content += INDENT + INDENT + L"}\r\n";
            }
            content += L"};\r\n";
            content += L"\r\n";
        }
        WriteFile(hppFilePath, content, true);
        LogService::LogInfo(logProperty, logId, L"Generate object class file completed.");
    )
}

void VPGFileGenerationManager::GernerateProperty(const LogProperty *logProperty, const std::wstring &projPrefix, const std::wstring &projWorkspace, const std::wstring &typeWorkspace, 
    const std::wstring &objTypeDirectoryHpp, const std::wstring &objDirectoryHpp, const std::wstring &propertyAccessorDirectoryHpp, const std::wstring &propertyAccessorDirectoryCpp)
{
    TRY_CATCH(
        std::wstring typeWorkspaceFullPath = ConcatPaths({projWorkspace, typeWorkspace});
        GetClassMacroList(projWorkspace);
        VPGEnumClassReader enumClassReader(_ClassMacros);
        GetFileList(&enumClassReader, typeWorkspaceFullPath, projPrefix);

        //Generate Object Type, Object Class, PropertyAccessor,
        // 1. get all files from directory
        // 2. get all properties with enum class Prefix + Class + Property
        std::wstring filePrefix = projPrefix;
        ToLower(filePrefix);

        LogService::LogInfo(logProperty, logId, L"Generate property start.");
        std::vector<std::wstring> objectTypeList;
        for (auto &filePath : std::filesystem::recursive_directory_iterator(PATH(typeWorkspaceFullPath))) {
            std::wstring path = filePath.path().wstring();
            std::wstring fileName = filePath.path().filename().wstring();
            // ------------------------------------------------------------------------------------------ //
            //                                      Validation                                            //
            // ------------------------------------------------------------------------------------------ //
            if (!IsClassEnumFile(fileName, filePrefix)) {
                std::wstring filePrefixStr = !IsBlank(filePrefix) ? (L"Prefix " + filePrefix + L" or ") : L"";
                LogService::LogWarning(logProperty, logId, L"File " + filePrefixStr + L"Suffix " + propertyFileSuffix + L" missing. Skip: " + path);
                continue;
            }
            // ------------------------------------------------------------------------------------------ //
            //                                      Parse File Start                                      //
            // ------------------------------------------------------------------------------------------ //
            LogService::LogWarning(logProperty, logId, L"Parse file start: " + path);

            std::wstring fileContent = ReadFile(path);
            Trim(fileContent);
            if (fileContent.empty())
                continue;

            std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
            enumClassReader.Parse(fileContent, enumClassList);
            for (auto const &enumClass : enumClassList) {
                if (!IsClassEnum(enumClass->GetName(), projPrefix)) {
                    std::wstring classPrefixStr = !IsBlank(projPrefix) ? (L"Prefix " + projPrefix + L" or ") : L"";
                    LogService::LogWarning(logProperty, logId, L"Class " + classPrefixStr + L"Suffix " + proeprtyClassNameSuffix + L"missing. Skip: " + enumClass->GetName());
                    continue;
                }
                std::wstring classNameWithoutPrefix = enumClass->GetName().substr(!projPrefix.empty() ? projPrefix.size() : 0);
                classNameWithoutPrefix = GetClassNameFromEnumClassName(classNameWithoutPrefix);
                objectTypeList.push_back(GetClassNameFromEnumClassName(classNameWithoutPrefix));
            
                // ------------------------------------------------------------------------------------------ //
                //                               Generate Object Class File                                   //
                // ------------------------------------------------------------------------------------------ //
                std::wstring objectFileName = fileName.substr(0, fileName.size() - propertyFileSuffix.size());
                if (objectFileName.ends_with(L"_"))
                    objectFileName.pop_back();

                std::wstring propertyAccessorFileName = objectFileName + L"_" + propertyAccessorFileSuffixWithoutExtention;

                GeneratePropertyClassFile(logProperty, projPrefix, ConcatPaths({projWorkspace, objDirectoryHpp, objectFileName + L".hpp"}), enumClassList);
                // GeneratePropertyPropertyAccessorFile(logProperty, projPrefix, 
                //     ConcatPaths({projWorkspace, propertyAccessorDirectoryHpp, propertyAccessorFileName + L".hpp"}), 
                //     ConcatPaths({projWorkspace, propertyAccessorDirectoryCpp, propertyAccessorFileName + L".cpp"}), enumClassList);
            }
            LogService::LogInfo(logProperty, logId, L"Parse file completed: " + path);
            // ------------------------------------------------------------------------------------------ //
            //                                      Parse File End                                        //
            // ------------------------------------------------------------------------------------------ //
        }
        // ------------------------------------------------------------------------------------------ //
        //                               Generate Object Type File                                    //
        // ------------------------------------------------------------------------------------------ //
        VPGFileGenerationManager::GenerateObjectTypeFile(logProperty, ConcatPaths({projWorkspace, objTypeDirectoryHpp, objectTypeHppFileName}), objectTypeList);

        LogService::LogInfo(logProperty, logId, L"Generate Property Finished.");
    )
}
