#include "vpg_file_generation_manager.hpp"

#include <filesystem>
#include <fstream>
#include <numeric>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_service.hpp"
#include "string_helper.hpp"

#include "vpg_cpp_helper.hpp"
#include "vpg_enum_class_reader.hpp"
#include "vpg_include_path_service.hpp"
#include "vpg_object_type_file_generation_service.hpp"

const std::wstring classMacroFilePath = L"include/External/VCC/Core/Macro/class_macro.hpp";
const std::wstring logId = L"File Generation";

const std::wstring objectTypeHppFileName = L"object_type.hpp";
const std::wstring propertyFileSuffix = L"_property.hpp";
const std::wstring proeprtyClassNameSuffix = L"Property";
const std::wstring propertyAccessorFileSuffixWithoutExtention = L"property_accessor";

void VPGFileGenerationManager::GetClassMacroList(const std::wstring &projWorkspace)
{
    TRY
        std::wstring filePath = ConcatPaths({projWorkspace, classMacroFilePath});
        std::wstring prefix = L"#define ";
        size_t prefixLen = wcslen(prefix.c_str());
        ReadFilePerLine(filePath, [prefix, prefixLen, this](std::wstring line) {
            Trim(line);
            if (line.starts_with(prefix) && Find(line, L"(") != std::wstring::npos) {
                std::wstring type = line.substr(prefixLen, Find(line, L"(") - prefixLen);
                Trim(type);
                this->_ClassMacros.insert(type);
            }
        });
        if (this->_ClassMacros.empty())
            THROW_EXCEPTION_MSG(ExceptionType::FileNotFound, classMacroFilePath + L" missing");
    CATCH
}

std::wstring VPGFileGenerationManager::GetProjectClassIncludeFile(const std::wstring &className)
{
    TRY
        if (_ProjectClassIncludeFiles.empty())
            VPGIncludePathService::GetWorkspaceIncludePath(L"", _ProjectClassIncludeFiles);
        // enum generate class
        if (_ClassFiles.contains(className))
            return _ClassFiles[className];
        else if (_ProjectClassIncludeFiles.contains(className))
            return _ProjectClassIncludeFiles[className];
        else if (_ProjectClassIncludeFiles.contains(L"vcc::" + className))
            return _ProjectClassIncludeFiles[L"vcc::" + className];
    CATCH
    return L"";
}

std::wstring VPGFileGenerationManager::GetProjectEnumClassIncludeFile(const std::wstring &className)
{
    TRY
        if (_ProjectClassIncludeFiles.empty())
            VPGIncludePathService::GetWorkspaceIncludePath(L"", _ProjectClassIncludeFiles);
        // enum generate class
        if (_EnumClassFiles.contains(className))
            return _EnumClassFiles[className];
        else if (_ProjectClassIncludeFiles.contains(className))
            return _ProjectClassIncludeFiles[className];
        else if (_ProjectClassIncludeFiles.contains(L"vcc::" + className))
            return _ProjectClassIncludeFiles[L"vcc::" + className];
    CATCH
    return L"";
}

std::wstring VPGFileGenerationManager::GetClassNameFromEnumClassName(const std::wstring &enumClassName)
{
    TRY
        if (enumClassName.ends_with(proeprtyClassNameSuffix) && enumClassName != proeprtyClassNameSuffix)
            return enumClassName.substr(0, enumClassName.length() - proeprtyClassNameSuffix.length());
    CATCH
    return enumClassName;
}

std::wstring VPGFileGenerationManager::GetClassFilenameFromEnumClassFilename(const std::wstring &enumClassFileName)
{
    TRY
        std::wstring tmpFileName = enumClassFileName;
        ReplaceAll(tmpFileName, propertyFileSuffix, L".hpp");
        return tmpFileName;
    CATCH
    return enumClassFileName;
}

void VPGFileGenerationManager::GetFileList(const VPGEnumClassReader *reader, const std::wstring &directoryFullPath, const std::wstring &projectPrefix)
{
    TRY
        for (auto const &filePath : std::filesystem::recursive_directory_iterator(PATH(directoryFullPath))) {
            TRY
                if (filePath.is_directory())
                    continue;
                std::wstring content = ReadFile(filePath.path().wstring());
                std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
                reader->Parse(GetSimpleCode(content), enumClassList);
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
            CATCH_SLIENT
        }
    CATCH
}

bool VPGFileGenerationManager::IsClassEnumFile(const std::wstring &filename, const std::wstring &projectPrefix)
{
    TRY
        std::wstring tmpFileName = filename;
        ToLower(tmpFileName);
        std::wstring tmpProjectPrefix = projectPrefix;
        ToLower(tmpProjectPrefix);
        return tmpFileName.ends_with(propertyFileSuffix) && (IsBlank(tmpProjectPrefix) || tmpFileName.starts_with(tmpProjectPrefix));
    CATCH
    return false;
}

bool VPGFileGenerationManager::IsClassEnum(const std::wstring &enumClassName, const std::wstring &projectPrefix)
{
    TRY
        return enumClassName.ends_with(proeprtyClassNameSuffix) && enumClassName.length() > proeprtyClassNameSuffix.length()
            && (IsBlank(projectPrefix) || enumClassName.starts_with(projectPrefix));
    CATCH
    return false;
}

void VPGFileGenerationManager::GeneratePropertyClassFile(const LogProperty *logProperty, const std::wstring &classPrefix, const std::wstring &hppFilePath, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList)
{
    TRY
        LogService::LogInfo(logProperty, logId, L"Generate object class file: " + hppFilePath);

        // TODO: need to enable to check all systemn function
        // need to skip all code in command
        // Mac: all next tokens after class _LIBCPP_TEMPLATE_VIS under path /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/V1
        // Window: C:\msys64\mingw64\include\c++\12.2.0

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
                    if (Find(property->GetMacro().substr(0, Find(property->GetMacro(), L"(")), L"SPTR") != std::wstring::npos) {

                        std::wstring includeFile = GetProjectClassIncludeFile(type);
                        if (!includeFile.empty())
                            projectFileList.insert(L"#include " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, includeFile));
                        else
                            abstractClassList.insert(L"class " + type + L";");
                    } else {
                        std::wstring includeFile = GetProjectEnumClassIncludeFile(type);
                        if (!includeFile.empty())
                            projectFileList.insert(L"#include " + GetEscapeStringWithQuote(EscapeStringType::DoubleQuote, includeFile));
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
                    if (Find(property->GetMacro().substr(0, Find(property->GetMacro(), L"(")), L"SPTR") != std::wstring::npos) {
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
                        if (Find(property->GetMacro(), L"SPTR") != std::wstring::npos) {
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
    CATCH
}

void VPGFileGenerationManager::GeneratePropertyPropertyAccessorFile(const LogProperty *logProperty, const std::wstring &classPrefix, const std::wstring &hppFilePath, const std::wstring &cppFilePath, const std::vector<std::shared_ptr<VPGEnumClass>> &enumClassList)
{
    TRY
        LogService::LogInfo(logProperty, logId, L"Generate property access file: " + hppFilePath + L" " + cppFilePath);

        LogService::LogInfo(logProperty, logId, L"Generate property access file: " + hppFilePath + L" " + cppFilePath + L" complete.");
    CATCH
}

void VPGFileGenerationManager::GernerateProperty(const LogProperty *logProperty, const std::wstring &projPrefix, const std::wstring &projWorkspace, const std::wstring &typeWorkspace, 
    const std::wstring &objTypeDirectoryHpp, const std::wstring &objDirectoryHpp, const std::wstring &propertyAccessorDirectoryHpp, const std::wstring &propertyAccessorDirectoryCpp)
{
    TRY
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
        std::set<std::wstring> objectTypes;
        for (auto &filePath : std::filesystem::recursive_directory_iterator(PATH(typeWorkspaceFullPath))) {
            std::wstring path = GetLinuxPath(filePath.path().wstring());
            std::wstring fileName = filePath.path().filename().wstring();
            std::wstring middlePath = GetRelativePath(GetLinuxPath(filePath.path().parent_path().wstring()), GetLinuxPath(typeWorkspaceFullPath));
            if (middlePath == L".")
                middlePath = L"";
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
                objectTypes.insert(GetClassNameFromEnumClassName(classNameWithoutPrefix));
            
                // ------------------------------------------------------------------------------------------ //
                //                               Generate Object Class File                                   //
                // ------------------------------------------------------------------------------------------ //
                std::wstring objectFileName = fileName.substr(0, fileName.size() - propertyFileSuffix.size());
                if (objectFileName.ends_with(L"_"))
                    objectFileName.pop_back();

                std::wstring propertyAccessorFileName = objectFileName + L"_" + propertyAccessorFileSuffixWithoutExtention;

                std::vector<std::wstring> objectFilePaths = { projWorkspace, objDirectoryHpp };
                if (!middlePath.empty())
                    objectFilePaths.push_back(middlePath);
                objectFilePaths.push_back(objectFileName + L".hpp");
                GeneratePropertyClassFile(logProperty, projPrefix, ConcatPaths(objectFilePaths), enumClassList);
                GeneratePropertyPropertyAccessorFile(logProperty, projPrefix, 
                    ConcatPaths({projWorkspace, propertyAccessorDirectoryHpp, propertyAccessorFileName + L".hpp"}), 
                    ConcatPaths({projWorkspace, propertyAccessorDirectoryCpp, propertyAccessorFileName + L".cpp"}), enumClassList);
            }
            LogService::LogInfo(logProperty, logId, L"Parse file completed: " + path);
            // ------------------------------------------------------------------------------------------ //
            //                                      Parse File End                                        //
            // ------------------------------------------------------------------------------------------ //
        }
        // ------------------------------------------------------------------------------------------ //
        //                               Generate Object Type File                                    //
        // ------------------------------------------------------------------------------------------ //
        VPGObjectTypeFileGenerationSerive::Generate(logProperty, ConcatPaths({projWorkspace, objTypeDirectoryHpp, objectTypeHppFileName}), objectTypes);

        LogService::LogInfo(logProperty, logId, L"Generate Property Finished.");
    CATCH
}
