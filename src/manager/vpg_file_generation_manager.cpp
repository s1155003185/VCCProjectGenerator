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
#include "vpg_config.hpp"
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
        std::wstring filePath = vcc::ConcatPaths({projWorkspace, classMacroFilePath});
        std::wstring prefix = L"#define ";
        size_t prefixLen = wcslen(prefix.c_str());
        vcc::ReadFilePerLine(filePath, [prefix, prefixLen, this](std::wstring line) {
            vcc::Trim(line);
            if (vcc::IsStartWith(line, prefix) && vcc::IsContain(line, L"(")) {
                std::wstring type = line.substr(prefixLen, vcc::Find(line, L"(") - prefixLen);
                vcc::Trim(type);
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
        vcc::ReplaceAll(tmpFileName, propertyFileSuffix, L".hpp");
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
                std::wstring content = vcc::ReadFile(filePath.path().wstring());
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
                                if (property->GetPropertyType() == VPGEnumClassAttributeType::Action) {
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
    return vcc::ConcatPaths(objectFilePaths);
}

void VPGFileGenerationManager::GernerateProperty(const vcc::LogConfig *logConfig, const VPGConfig *option)
{
    TRY
        std::wstring projPrefix = option->GetProjectPrefix();
        std::wstring projWorkspace = _Workspace;
        std::wstring typeWorkspaceFullPath = vcc::ConcatPaths({projWorkspace, option->GetInputTypeWorkspace()});
        std::wstring propertyAccessorDirectoryHpp = option->GetOutputPropertyAccessorDirectoryHpp();
        std::wstring propertyAccessorDirectoryCpp = option->GetOutputPropertyAccessorDirectoryCpp();
        std::wstring actionDirectoryHpp = option->GetOutputActionDirectoryHpp();
        std::wstring actionDirectoryCpp = option->GetOutputActionDirectoryCpp();
        std::wstring formDirectoryHpp = option->GetOutputFormDirectoryHpp();
        std::wstring formDirectoryCpp = option->GetOutputFormDirectoryCpp();
        std::wstring objectDirectoryHpp = option->GetOutputObjectDirectoryHpp();
        std::wstring objectDirectoryCpp = option->GetOutputObjectDirectoryCpp();
        std::wstring objectTypeDirectory = option->GetOutputObjectTypeDirectory();
        std::wstring objectFactoryDirectoryHpp = option->GetOutputObjectFactoryDirectoryHpp();
        std::wstring objectFactoryDirectoryCpp = option->GetOutputObjectFactoryDirectoryCpp();
        std::wstring propertyAccessorFactoryDirectoryHpp = option->GetOutputPropertyAccessorFactoryDirectoryHpp();
        std::wstring propertyAccessorFactoryDirectoryCpp = option->GetOutputPropertyAccessorFactoryDirectoryCpp();
        
        GetClassMacroList(projWorkspace);
        VPGEnumClassReader enumClassReader(_ClassMacros);
        GetFileList(&enumClassReader, typeWorkspaceFullPath, projPrefix, !vcc::IsBlank(actionDirectoryHpp));

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

            std::wstring filePathLinuxPath = vcc::GetLinuxPath(filePath.path().wstring());
            std::wstring relativePath = vcc::GetRelativePath(filePathLinuxPath.substr(vcc::Find(filePathLinuxPath, vcc::GetLinuxPath(typeWorkspaceFullPath))), typeWorkspaceFullPath);
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
        vcc::LogService::LogInfo(logConfig, logId, L"Generate property start.");
        
        // Generate OperationResult
        for (auto const &exportOption : option->GetExports()) {
            if (exportOption->GetInterface() == VPGConfigInterfaceType::Java) {
                VPGJavaGenerationService::GenerateOperationResult(logConfig, projPrefix, exportOption.get(),
                    typeWorkspaceClassRelativePathMapObject, typeWorkspaceClassRelativePathMapForm);
            }
        }
        std::wstring filePrefix = projPrefix;
        vcc::ToLower(filePrefix);        
        std::set<std::wstring> objectTypes;
        std::set<std::wstring> objectFileNames, propertyAccessorFileNames;
        auto dllOption = std::make_shared<VPGDllFileGenerationServiceOption>();
        for (auto &filePath : std::filesystem::recursive_directory_iterator(PATH(typeWorkspaceFullPath))) {
            if (filePath.is_directory())
                continue;
            std::wstring path = vcc::GetLinuxPath(filePath.path().wstring());
            std::wstring fileName = filePath.path().filename().wstring();
            std::wstring middlePath = vcc::GetRelativePath(vcc::GetLinuxPath(filePath.path().parent_path().wstring()), vcc::GetLinuxPath(typeWorkspaceFullPath));
            if (middlePath == L".")
                middlePath = L"";

            if (!vcc::IsBlank(projPrefix) && !vcc::IsStartWith(fileName, filePrefix))
                vcc::LogService::LogWarning(logConfig, logId, L"Class Prefix " + projPrefix + L" missing. Skip: " + path);

            // ------------------------------------------------------------------------------------------ //
            //                                      Parse File Start                                      //
            // ------------------------------------------------------------------------------------------ //
            vcc::LogService::LogWarning(logConfig, logId, L"Parse file start: " + path);

            std::wstring fileContent = vcc::ReadFile(path);
            vcc::Trim(fileContent);
            if (fileContent.empty())
                continue;

            std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
            std::vector<std::shared_ptr<VPGEnumClass>> objectEnumClassList;
            enumClassReader.Parse(fileContent, enumClassList);
            // ------------------------------------------------------------------------------------------ //
            //                              Override Enum Class based on vcc.json                         //
            // ------------------------------------------------------------------------------------------ //
            if (option->GetBehavior()->GetActionHistoryType() == VPGConfigActionHistoryType::NoHistory) {
                for (auto enumClass : enumClassList)
                    for (auto property : enumClass->GetProperties())
                        property->SetIsNoHistory(true);
            }
            // ------------------------------------------------------------------------------------------ //
            //                              Procedure Start                                               //
            // ------------------------------------------------------------------------------------------ //
            for (auto const &enumClass : enumClassList) {
                std::wstring propertyClassNameWithoutNamespace = GetTypeOrClassWithoutNamespace(enumClass->GetName());
                std::wstring classNameWithoutNamespace = GetClassNameFromPropertyClassName(enumClass->GetName());
                if (projPrefix.empty() || IsPropertyClass(propertyClassNameWithoutNamespace, projPrefix)) {
                    objectTypes.insert(classNameWithoutNamespace.substr(projPrefix.size()));
                    objectEnumClassList.push_back(enumClass);
                } else {
                    std::wstring classPrefixStr = !vcc::IsBlank(projPrefix) ? (L"Prefix " + projPrefix + L" or ") : L"";
                    vcc::LogService::LogWarning(logConfig, logId, L"Class " + classPrefixStr + L"Suffix " + propertyClassNameSuffix + L"missing. Not generate object for " + enumClass->GetName());
                }
            
                // ------------------------------------------------------------------------------------------ //
                //                               JAVA Export File                                             //
                // ------------------------------------------------------------------------------------------ //
                std::wstring javaEnumClassName = propertyClassNameWithoutNamespace;
                if (!projPrefix.empty() && !vcc::IsStartWith(javaEnumClassName, projPrefix))
                    javaEnumClassName = projPrefix + javaEnumClassName;

                for (auto const &javaOption : option->GetExports()) {
                    if (vcc::IsBlank(javaOption->GetWorkspace()) || javaOption->GetInterface() != VPGConfigInterfaceType::Java)
                        continue;
                    
                    std::wstring workspace = vcc::IsAbsolutePath(javaOption->GetWorkspace()) ? javaOption->GetWorkspace() : vcc::ConcatPaths({ _Workspace, javaOption->GetWorkspace() });

                    if (!vcc::IsBlank(javaOption->GetTypeDirectory()))
                        VPGJavaGenerationService::GenerateEnum(logConfig, GetConcatPath(workspace, javaOption->GetTypeDirectory(), middlePath, javaEnumClassName + L".java"), middlePath, enumClass.get(), option, javaOption.get());
                    
                    if (IsPropertyClass(propertyClassNameWithoutNamespace, projPrefix)) {
                        std::wstring objectDirectory = javaOption->GetObjectDirectory();
                        if (enumClass->GetType() == VPGEnumClassType::Form && !vcc::IsBlank(javaOption->GetFormDirectory()))
                            objectDirectory = javaOption->GetFormDirectory();
                        if (!vcc::IsBlank(objectDirectory))
                            VPGJavaGenerationService::GenerateObject(logConfig, GetConcatPath(workspace, objectDirectory, middlePath, classNameWithoutNamespace + L".java"), middlePath, enumClass.get(),
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
                
                if (!vcc::IsBlank(objectDirectoryHpp) && !vcc::IsBlank(objectDirectoryCpp)) {
                    objectFileNames.insert(objectFileName + L".hpp");
                    std::wstring fileObjectDirectoryHpp = !vcc::IsBlank(formDirectoryHpp) ? GetConcatPath(projWorkspace, formDirectoryHpp, middlePath, objectFileName + L".hpp") : L"";
                    std::wstring fileObjectDirectoryCpp = !vcc::IsBlank(formDirectoryCpp) ? GetConcatPath(projWorkspace, formDirectoryCpp, middlePath, objectFileName + L".cpp") : L"";
                    std::wstring actionFolderHpp = !vcc::IsBlank(actionDirectoryHpp) ? GetConcatPath(projWorkspace, actionDirectoryHpp, middlePath, L"") : L"";
                    std::wstring actionFolderCpp = !vcc::IsBlank(actionDirectoryCpp) ? GetConcatPath(projWorkspace, actionDirectoryCpp, middlePath, L"") : L"";
                    
                    VPGObjectFileGenerationService::GenerateHpp(logConfig, option, _IncludeFiles, _EnumClasses, GetConcatPath(projWorkspace, objectDirectoryHpp, middlePath, objectFileName + L".hpp"), fileObjectDirectoryHpp, actionFolderHpp, objectEnumClassList);
                    VPGObjectFileGenerationService::GenerateCpp(logConfig, projPrefix, _IncludeFiles, _EnumClasses, GetConcatPath(projWorkspace, objectDirectoryCpp, middlePath, objectFileName + L".cpp"), fileObjectDirectoryCpp, actionFolderCpp, objectEnumClassList);
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
            vcc::LogService::LogInfo(logConfig, logId, L"Parse file completed: " + path);
        }
        // ------------------------------------------------------------------------------------------ //
        //                               Generate Object Type File                                    //
        // ------------------------------------------------------------------------------------------ //
        VPGObjectTypeFileGenerationService::Generate(logConfig, vcc::ConcatPaths({projWorkspace, objectTypeDirectory, objectTypeHppFileName}), objectTypes);

        // ------------------------------------------------------------------------------------------ //
        //                               Generate Object Factory File                                 //
        // ------------------------------------------------------------------------------------------ //
        if (!vcc::IsBlank(objectFactoryDirectoryHpp) && !vcc::IsBlank(objectFactoryDirectoryCpp)) {
            VPGObjectFactoryFileGenerationService::GenerateHpp(logConfig, vcc::ConcatPaths({projWorkspace, objectFactoryDirectoryHpp, objectFactoryFileNameHpp}));
            VPGObjectFactoryFileGenerationService::GenerateCpp(logConfig, projPrefix, objectFileNames, vcc::ConcatPaths({projWorkspace, objectFactoryDirectoryCpp, objectFactoryFileNameCpp}), objectTypes);
        }
        // ------------------------------------------------------------------------------------------ //
        //                               Generate Property Accessor Factory File                      //
        // ------------------------------------------------------------------------------------------ //
        if (!vcc::IsBlank(propertyAccessorFactoryDirectoryHpp) && !vcc::IsBlank(propertyAccessorFactoryDirectoryCpp)) {
            dllOption->SetIsGeneratePropertyAccessor(true);
            VPGPropertyAccessorFactoryFileGenerationService::GenerateHpp(logConfig, vcc::ConcatPaths({projWorkspace, propertyAccessorFactoryDirectoryHpp, propertyAccessorFactoryFileNameHpp}));
            VPGPropertyAccessorFactoryFileGenerationService::GenerateCpp(logConfig, projPrefix, propertyAccessorFileNames, vcc::ConcatPaths({projWorkspace, propertyAccessorFactoryDirectoryCpp, propertyAccessorFactoryFileNameCpp}), objectTypes);
        }

        // ------------------------------------------------------------------------------------------ //
        //                               Generate DLL Inteface File                                   //
        // ------------------------------------------------------------------------------------------ //
        VPGDllFileGenerationService::GenerateHpp(logConfig, vcc::ConcatPaths({projWorkspace, L"DllFunctions.h"}), dllOption.get());
        VPGDllFileGenerationService::GenerateCpp(logConfig, vcc::ConcatPaths({projWorkspace, L"DllFunctions.cpp"}), dllOption.get());

        // ------------------------------------------------------------------------------------------ //
        //                               Generate JAVA bridge                                         //
        // ------------------------------------------------------------------------------------------ //
        VPGJavaGenerationService::GenerateJavaBridge(logConfig, _Workspace, vcc::ConcatPaths({projWorkspace, L"DllFunctions.h"}), option);

        vcc::LogService::LogInfo(logConfig, logId, L"Generate Property Finished.");
    CATCH
}
