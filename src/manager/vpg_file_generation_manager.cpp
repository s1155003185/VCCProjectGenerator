#include "vpg_file_generation_manager.hpp"

#include <filesystem>
#include <fstream>
#include <numeric>
#include <string>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "log_service.hpp"
#include "string_helper.hpp"

#include "vpg_class_helper.hpp"
#include "vpg_cpp_helper.hpp"
#include "vpg_dll_file_generation_service.hpp"
#include "vpg_enum_class.hpp"
#include "vpg_enum_class_reader.hpp"
#include "vpg_generation_option.hpp"
#include "vpg_include_path_service.hpp"
#include "vpg_java_generation_service.hpp"
#include "vpg_object_file_generation_service.hpp"
#include "vpg_object_factory_file_generation_service.hpp"
#include "vpg_object_type_file_generation_service.hpp"
#include "vpg_property_accessor_generation_service.hpp"
#include "vpg_property_accessor_factory_file_generation_service.hpp"

const std::wstring classMacroFilePath = L"include/external/vcc/core/macro/class_macro.hpp";
const std::wstring logId = L"File Generation";

const std::wstring objectTypeHppFileName = L"object_type.hpp";
const std::wstring objectFactoryFileNameHpp = L"object_factory.hpp";
const std::wstring objectFactoryFileNameCpp = L"object_factory.cpp";
const std::wstring propertyAccessorFactoryFileNameHpp = L"property_accessor_factory.hpp";
const std::wstring propertyAccessorFactoryFileNameCpp = L"property_accessor_factory.cpp";
const std::wstring propertyFileSuffix = L"_property.hpp";
const std::wstring propertyClassNameSuffix = L"Property";
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

std::wstring VPGFileGenerationManager::GetClassFilenameFromEnumClassFilename(const std::wstring &enumClassFileName)
{
    TRY
        std::wstring tmpFileName = enumClassFileName;
        ReplaceAll(tmpFileName, propertyFileSuffix, L".hpp");
        return tmpFileName;
    CATCH
    return enumClassFileName;
}

void VPGFileGenerationManager::GetFileList(const VPGEnumClassReader *reader, const std::wstring &directoryFullPath, const std::wstring &projectPrefix, const bool &isSeperateAction)
{
    /****************************************************************************************************
    ****************************** All Generated File should be added here ******************************
    ****************************************************************************************************/
    TRY
        _EnumClasses.clear();
        VPGIncludePathService::GetWorkspaceIncludePath(_Workspace, this->_ClassMacros, _IncludeFiles, _EnumClasses);

        std::map<std::wstring, std::wstring> enumClassFiles;
        std::map<std::wstring, std::wstring> classFiles;

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
                    if (enumClassFiles.count(enumClassName) > 0)
                        THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Enum Class " + enumClassName + L" duplicated:\r\n"
                            + enumClassFiles[enumClassName] + L"\r\n"
                            + fileName);
                    enumClassFiles.insert(std::make_pair(enumClassName, fileName));
                    if (_IncludeFiles.find(enumClassName) != _IncludeFiles.end()) {
                        std::wstring currentIncludeFile = _IncludeFiles[enumClassName];
                        if (currentIncludeFile != fileName) 
                            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Enum Class " + enumClassName + L" duplicated:\r\n"
                                + _IncludeFiles[enumClassName] + L"\r\n"
                                + fileName);
                    } else
                        _IncludeFiles.insert(std::make_pair(enumClassName, fileName));
                    // class
                    if (IsPropertyFile(fileName, projectPrefix) && IsPropertyClass(enumClassName, projectPrefix)) {
                        std::wstring className = GetClassNameFromPropertyClassName(enumClassName);
                        std::wstring classFileName = GetClassFilenameFromEnumClassFilename(fileName);
                        if (classFiles.count(className) > 0)
                            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Class " + className + L" duplicated:\r\n"
                                + classFiles[className] + L"\r\n"
                                + classFileName);
                        classFiles.insert(std::make_pair(className, classFileName));
                        if (_IncludeFiles.find(className) != _IncludeFiles.end()) {
                            std::wstring currentIncludeFile = _IncludeFiles[className];
                            if (currentIncludeFile != classFileName) 
                                THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Class " + className + L" duplicated:\r\n"
                                    + _IncludeFiles[className] + L"\r\n"
                                    + classFileName);
                        } else
                            _IncludeFiles.insert(std::make_pair(className, classFileName));

                        // action
                        if (isSeperateAction) {
                            for (auto const &property : enumClass->GetProperties()) {
                                if (property->GetPropertyType() == VPGEnumClassPropertyType::Action) {
                                    std::wstring actionClassName = GetActionClassName(enumClass.get(), property.get());
                                    std::wstring actionFileName = GetActionFileNameWithoutExtension(actionClassName, projectPrefix) + L".hpp";
                                    if (_IncludeFiles.find(actionClassName) == _IncludeFiles.end())
                                        _IncludeFiles.insert(std::make_pair(actionClassName, actionFileName));
                                }
                            }
                        }
                    }
                }
            CATCH_SLIENT
        }
    CATCH
}

std::wstring VPGFileGenerationManager::GetConcatPath(const std::wstring &projWorkspace, const std::wstring &objWorkspace, const std::wstring &middlePath, const std::wstring &fileName) const
{
    std::vector<std::wstring> objectFilePaths = { projWorkspace, objWorkspace };
    if (!middlePath.empty())
        objectFilePaths.push_back(middlePath);
    objectFilePaths.push_back(fileName);
    return ConcatPaths(objectFilePaths);
}

void VPGFileGenerationManager::GernerateProperty(const LogConfig *logConfig, const VPGGenerationOption *option)
{
    TRY
        std::wstring projPrefix = option->GetProjectPrefix();
        std::wstring projWorkspace = _Workspace;
        std::wstring typeWorkspaceFullPath = ConcatPaths({projWorkspace, option->GetTypeWorkspace()});
        std::wstring propertyAccessorDirectoryHpp = option->GetPropertyAccessorDirectoryHpp();
        std::wstring propertyAccessorDirectoryCpp = option->GetPropertyAccessorDirectoryCpp();

        GetClassMacroList(projWorkspace);
        VPGEnumClassReader enumClassReader(_ClassMacros);
        GetFileList(&enumClassReader, typeWorkspaceFullPath, projPrefix, !IsBlank(option->GetActionDirectoryHpp()));

        // get all enum and enum class under typeWorkspace to get java import map
        // only contain 
        std::map<std::wstring, std::wstring> typeWorkspaceClassRelativePathMapObject, typeWorkspaceClassRelativePathMapForm;
        std::map<std::wstring, std::vector<std::wstring>> includeFileEnumClassMap;
        for (auto const &enumClassIncludeFilePair : _IncludeFiles) {
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
            std::wstring relativePath = GetRelativePath(filePathLinuxPath.substr(Find(filePathLinuxPath, GetLinuxPath(typeWorkspaceFullPath))), typeWorkspaceFullPath);
            relativePath = PATH(relativePath).parent_path().wstring();
            if (relativePath == L".")
                relativePath = L"";

            for (auto const &enumClassName : includeFileEnumClassMap.find(fileName)->second) {
                if (_EnumClasses.find(enumClassName) == _EnumClasses.end())
                    continue;
                if (_EnumClasses.at(enumClassName)->GetType() == VPGEnumClassType::Form)
                    typeWorkspaceClassRelativePathMapForm.insert(std::make_pair(enumClassName, relativePath));
                else
                    typeWorkspaceClassRelativePathMapObject.insert(std::make_pair(enumClassName, relativePath));
            }
        }

        //Generate Object Type, Object Class, PropertyAccessor,
        // 1. get all files from directory
        // 2. get all properties with enum class Prefix + Class + Property
        std::wstring filePrefix = projPrefix;
        ToLower(filePrefix);

        LogService::LogInfo(logConfig, logId, L"Generate property start.");
        std::set<std::wstring> objectTypes;
        std::set<std::wstring> objectFileNames, propertyAccessorFileNames;
        auto dllOption = std::make_shared<VPGDllFileGenerationServiceOption>();
        for (auto &filePath : std::filesystem::recursive_directory_iterator(PATH(typeWorkspaceFullPath))) {
            if (filePath.is_directory())
                continue;
            std::wstring path = GetLinuxPath(filePath.path().wstring());
            std::wstring fileName = filePath.path().filename().wstring();
            std::wstring middlePath = GetRelativePath(GetLinuxPath(filePath.path().parent_path().wstring()), GetLinuxPath(typeWorkspaceFullPath));
            if (middlePath == L".")
                middlePath = L"";

            if (!IsBlank(projPrefix) && !IsStartWith(fileName, filePrefix))
                LogService::LogWarning(logConfig, logId, L"Class Prefix " + projPrefix + L" missing. Skip: " + path);

            // ------------------------------------------------------------------------------------------ //
            //                                      Parse File Start                                      //
            // ------------------------------------------------------------------------------------------ //
            LogService::LogWarning(logConfig, logId, L"Parse file start: " + path);

            std::wstring fileContent = ReadFile(path);
            Trim(fileContent);
            if (fileContent.empty())
                continue;

            std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
            std::vector<std::shared_ptr<VPGEnumClass>> objectEnumClassList;
            enumClassReader.Parse(fileContent, enumClassList);
            for (auto const &enumClass : enumClassList) {
                std::wstring className = GetClassNameFromPropertyClassName(enumClass->GetName());
                if (projPrefix.empty() || IsPropertyClass(enumClass->GetName(), projPrefix)) {
                    objectTypes.insert(GetClassNameFromPropertyClassName(enumClass->GetName()).substr(projPrefix.size()));
                    objectEnumClassList.push_back(enumClass);
                } else {
                    std::wstring classPrefixStr = !IsBlank(projPrefix) ? (L"Prefix " + projPrefix + L" or ") : L"";
                    LogService::LogWarning(logConfig, logId, L"Class " + classPrefixStr + L"Suffix " + propertyClassNameSuffix + L"missing. Not generate object for " + enumClass->GetName());
                }
            
                // ------------------------------------------------------------------------------------------ //
                //                               JAVA Export File                                             //
                // ------------------------------------------------------------------------------------------ //
                std::wstring javaEnumClassName = enumClass->GetName();
                if (!projPrefix.empty() && !IsStartWith(javaEnumClassName, projPrefix))
                    javaEnumClassName = projPrefix + javaEnumClassName;

                for (auto const &javaOption : option->GetExports()) {
                    if (IsBlank(javaOption->GetWorkspace()) || javaOption->GetInterface() != VPGGenerationOptionInterfaceType::Java)
                        continue;
                    
                    std::wstring workspace = IsAbsolutePath(javaOption->GetWorkspace()) ? javaOption->GetWorkspace() : ConcatPaths({ _Workspace, javaOption->GetWorkspace() });

                    if (!IsBlank(javaOption->GetTypeDirectory()))
                        VPGJavaGenerationService::GenerateEnum(logConfig, GetConcatPath(workspace, javaOption->GetTypeDirectory(), middlePath, javaEnumClassName + L".java"), middlePath, enumClass.get(), option, javaOption.get());
                    
                    if (IsPropertyClass(enumClass->GetName(), projPrefix)) {
                        std::wstring objectDirectory = javaOption->GetObjectDirectory();
                        if (enumClass->GetType() == VPGEnumClassType::Form && !IsBlank(javaOption->GetFormDirectory()))
                            objectDirectory = javaOption->GetFormDirectory();
                        if (!IsBlank(objectDirectory))
                            VPGJavaGenerationService::GenerateObject(logConfig, GetConcatPath(workspace, objectDirectory, middlePath, className + L".java"), middlePath, enumClass.get(),
                                typeWorkspaceClassRelativePathMapObject, typeWorkspaceClassRelativePathMapForm,
                                option, javaOption.get());
                    }
                }
            }
            
            if (IsPropertyFile(fileName, filePrefix)) {
                // ------------------------------------------------------------------------------------------ //
                //                               Generate Object Class File                                   //
                // ------------------------------------------------------------------------------------------ //
                std::wstring objectFileName = fileName.substr(0, fileName.size() - propertyFileSuffix.size());
                if (objectFileName.ends_with(L"_"))
                    objectFileName.pop_back();

                std::wstring propertyAccessorFileName = objectFileName + L"_" + propertyAccessorFileSuffixWithoutExtention;
                
                if (!option->GetObjectDirectoryHpp().empty() && !option->GetObjectDirectoryCpp().empty()) {
                    objectFileNames.insert(objectFileName + L".hpp");
                    std::wstring fileObjectDirectoryHpp = !option->GetFormDirectoryHpp().empty() ? GetConcatPath(projWorkspace, option->GetFormDirectoryHpp(), middlePath, objectFileName + L".hpp") : L"";
                    std::wstring fileObjectDirectoryCpp = !option->GetFormDirectoryCpp().empty() ? GetConcatPath(projWorkspace, option->GetFormDirectoryCpp(), middlePath, objectFileName + L".cpp") : L"";
                    std::wstring actionFolderHpp = !option->GetActionDirectoryHpp().empty() ? GetConcatPath(projWorkspace, option->GetActionDirectoryHpp(), middlePath, L"") : L"";
                    std::wstring actionFolderCpp = !option->GetActionDirectoryCpp().empty() ? GetConcatPath(projWorkspace, option->GetActionDirectoryCpp(), middlePath, L"") : L"";
                    
                    VPGObjectFileGenerationService::GenerateHpp(logConfig, projPrefix, _IncludeFiles, GetConcatPath(projWorkspace, option->GetObjectDirectoryHpp(), middlePath, objectFileName + L".hpp"), fileObjectDirectoryHpp, actionFolderHpp, objectEnumClassList);
                    VPGObjectFileGenerationService::GenerateCpp(logConfig, projPrefix, _IncludeFiles, _EnumClasses, GetConcatPath(projWorkspace, option->GetObjectDirectoryCpp(), middlePath, objectFileName + L".cpp"), fileObjectDirectoryCpp, actionFolderCpp, objectEnumClassList);
                }
                if (!propertyAccessorDirectoryHpp.empty() && !propertyAccessorDirectoryCpp.empty()) {
                    propertyAccessorFileNames.insert(propertyAccessorFileName + L".hpp");
                    VPGPropertyAccessorGenerationService::GenerateHpp(logConfig, projPrefix, GetConcatPath(projWorkspace, propertyAccessorDirectoryHpp, middlePath, propertyAccessorFileName + L".hpp"), objectEnumClassList);
                    VPGPropertyAccessorGenerationService::GenerateCpp(logConfig, projPrefix, _IncludeFiles, GetConcatPath(projWorkspace, propertyAccessorDirectoryCpp, middlePath, propertyAccessorFileName + L".cpp"), objectEnumClassList);
                }
            }
            
            // ------------------------------------------------------------------------------------------ //
            //                                      Parse File End                                        //
            // ------------------------------------------------------------------------------------------ //
            LogService::LogInfo(logConfig, logId, L"Parse file completed: " + path);
        }
        // ------------------------------------------------------------------------------------------ //
        //                               Generate Object Type File                                    //
        // ------------------------------------------------------------------------------------------ //
        VPGObjectTypeFileGenerationService::Generate(logConfig, ConcatPaths({projWorkspace, option->GetObjectTypeDirectory(), objectTypeHppFileName}), objectTypes);

        // ------------------------------------------------------------------------------------------ //
        //                               Generate Object Factory File                                 //
        // ------------------------------------------------------------------------------------------ //
        if (!option->GetObjectFactoryDirectoryHpp().empty() && !option->GetObjectFactoryDirectoryCpp().empty()) {
            VPGObjectFactoryFileGenerationService::GenerateHpp(logConfig, ConcatPaths({projWorkspace, option->GetObjectFactoryDirectoryHpp(), objectFactoryFileNameHpp}));
            VPGObjectFactoryFileGenerationService::GenerateCpp(logConfig, projPrefix, objectFileNames, ConcatPaths({projWorkspace, option->GetObjectFactoryDirectoryCpp(), objectFactoryFileNameCpp}), objectTypes);
        }
        // ------------------------------------------------------------------------------------------ //
        //                               Generate Property Accessor Factory File                      //
        // ------------------------------------------------------------------------------------------ //
        if (!option->GetPropertyAccessorFactoryDirectoryHpp().empty() && !option->GetPropertyAccessorFactoryDirectoryCpp().empty()) {
            dllOption->SetIsGeneratePropertyAccessor(true);
            VPGPropertyAccessorFactoryFileGenerationService::GenerateHpp(logConfig, ConcatPaths({projWorkspace, option->GetPropertyAccessorFactoryDirectoryHpp(), propertyAccessorFactoryFileNameHpp}));
            VPGPropertyAccessorFactoryFileGenerationService::GenerateCpp(logConfig, projPrefix, propertyAccessorFileNames, ConcatPaths({projWorkspace, option->GetPropertyAccessorFactoryDirectoryCpp(), propertyAccessorFactoryFileNameCpp}), objectTypes);
        }

        // ------------------------------------------------------------------------------------------ //
        //                               Generate DLL Inteface File                                   //
        // ------------------------------------------------------------------------------------------ //
        VPGDllFileGenerationService::GenerateHpp(logConfig, ConcatPaths({projWorkspace, L"DllFunctions.h"}), dllOption.get());
        VPGDllFileGenerationService::GenerateCpp(logConfig, ConcatPaths({projWorkspace, L"DllFunctions.cpp"}), dllOption.get());

        // ------------------------------------------------------------------------------------------ //
        //                               Generate JAVA bridge                                         //
        // ------------------------------------------------------------------------------------------ //
        VPGJavaGenerationService::GenerateJavaBridge(logConfig, _Workspace, ConcatPaths({projWorkspace, L"DllFunctions.h"}), option);

        LogService::LogInfo(logConfig, logId, L"Generate Property Finished.");
    CATCH
}
