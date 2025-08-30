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

void VPGFileGenerationManager::getClassMacroList(const std::wstring &projWorkspace)
{
    TRY
        std::wstring filePath = vcc::concatPaths({projWorkspace, classMacroFilePath});
        std::wstring prefix = L"#define ";
        size_t prefixLen = wcslen(prefix.c_str());
        vcc::readFilePerLine(filePath, [prefix, prefixLen, this](std::wstring line) {
            vcc::trim(line);
            if (vcc::isStartWith(line, prefix) && vcc::isContain(line, L"(")) {
                std::wstring type = line.substr(prefixLen, vcc::find(line, L"(") - prefixLen);
                vcc::trim(type);
                this->_ClassMacros.insert(type);
            }
        });
        if (this->_ClassMacros.empty())
            THROW_EXCEPTION_MSG(ExceptionType::FileNotFound, classMacroFilePath + L" missing");
    CATCH
}

std::wstring VPGFileGenerationManager::getClassFilenameFromEnumClassFilename(const std::wstring &enumClassFileName)
{
    TRY
        std::wstring tmpFileName = enumClassFileName;
        vcc::replaceAll(tmpFileName, propertyFileSuffix, L".hpp");
        return tmpFileName;
    CATCH
    return enumClassFileName;
}

void VPGFileGenerationManager::getFileList(const VPGEnumClassReader *reader, const std::wstring &directoryFullPath, const std::wstring &projectPrefix, const bool &isSeperateAction)
{
    /****************************************************************************************************
    ****************************** All Generated File should be added here ******************************
    ****************************************************************************************************/
    TRY
        _EnumClasses.clear();
        VPGIncludePathService::getWorkspaceIncludePath(_Workspace, this->_ClassMacros, _IncludeFiles, _EnumClasses);

        std::map<std::wstring, std::wstring> enumClassFiles;
        std::map<std::wstring, std::wstring> classFiles;

        for (auto const &filePath : std::filesystem::recursive_directory_iterator(PATH(directoryFullPath))) {
            TRY
                if (filePath.is_directory())
                    continue;
                std::wstring content = vcc::readFile(filePath.path().wstring());
                std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
                reader->parse(getSimpleCode(content), enumClassList);
                for (auto const &enumClass : enumClassList) {
                    // enum
                    std::wstring enumClassName = enumClass->getName();
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
                        std::wstring className = getClassNameFromPropertyClassName(enumClassName);
                        std::wstring classFileName = getClassFilenameFromEnumClassFilename(fileName);
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
                            for (auto const &property : enumClass->getProperties()) {
                                if (property->getPropertyType() == VPGEnumClassAttributeType::Action) {
                                    std::wstring actionClassName = getActionClassName(enumClass.get(), property.get());
                                    std::wstring actionFileName = getActionFileNameWithoutExtension(actionClassName, projectPrefix) + L".hpp";
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

std::wstring VPGFileGenerationManager::getConcatPath(const std::wstring &projWorkspace, const std::wstring &objWorkspace, const std::wstring &middlePath, const std::wstring &fileName) const
{
    std::vector<std::wstring> objectFilePaths = { projWorkspace, objWorkspace };
    if (!middlePath.empty())
        objectFilePaths.push_back(middlePath);
    objectFilePaths.push_back(fileName);
    return vcc::concatPaths(objectFilePaths);
}

void VPGFileGenerationManager::generateProperty(const vcc::LogConfig *logConfig, const VPGConfig *option)
{
    TRY
        std::wstring projPrefix = option->getProjectPrefix();
        std::wstring projWorkspace = _Workspace;
        std::wstring typeWorkspaceFullPath = vcc::concatPaths({projWorkspace, option->getInputTypeWorkspace()});
        std::wstring propertyAccessorDirectoryHpp = option->getOutputPropertyAccessorDirectoryHpp();
        std::wstring propertyAccessorDirectoryCpp = option->getOutputPropertyAccessorDirectoryCpp();
        std::wstring actionDirectoryHpp = option->getOutputActionDirectoryHpp();
        std::wstring actionDirectoryCpp = option->getOutputActionDirectoryCpp();
        std::wstring formDirectoryHpp = option->getOutputFormDirectoryHpp();
        std::wstring formDirectoryCpp = option->getOutputFormDirectoryCpp();
        std::wstring objectDirectoryHpp = option->getOutputObjectDirectoryHpp();
        std::wstring objectDirectoryCpp = option->getOutputObjectDirectoryCpp();
        std::wstring objectTypeDirectory = option->getOutputObjectTypeDirectory();
        std::wstring objectFactoryDirectoryHpp = option->getOutputObjectFactoryDirectoryHpp();
        std::wstring objectFactoryDirectoryCpp = option->getOutputObjectFactoryDirectoryCpp();
        std::wstring propertyAccessorFactoryDirectoryHpp = option->getOutputPropertyAccessorFactoryDirectoryHpp();
        std::wstring propertyAccessorFactoryDirectoryCpp = option->getOutputPropertyAccessorFactoryDirectoryCpp();
        
        getClassMacroList(projWorkspace);
        VPGEnumClassReader enumClassReader(_ClassMacros);
        getFileList(&enumClassReader, typeWorkspaceFullPath, projPrefix, !vcc::isBlank(actionDirectoryHpp));

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

            std::wstring filePathLinuxPath = vcc::getLinuxPath(filePath.path().wstring());
            std::wstring relativePath = vcc::getRelativePath(filePathLinuxPath.substr(vcc::find(filePathLinuxPath, vcc::getLinuxPath(typeWorkspaceFullPath))), typeWorkspaceFullPath);
            relativePath = PATH(relativePath).parent_path().wstring();
            if (relativePath == L".")
                relativePath = L"";

            for (auto const &enumClassName : includeFileEnumClassMap.find(fileName)->second) {
                if (_EnumClasses.find(enumClassName) == _EnumClasses.end())
                    continue;
                if (_EnumClasses.at(enumClassName)->getType() == VPGEnumClassType::Form)
                    typeWorkspaceClassRelativePathMapForm.insert(std::make_pair(enumClassName, relativePath));
                else
                    typeWorkspaceClassRelativePathMapObject.insert(std::make_pair(enumClassName, relativePath));
            }
        }

        //Generate Object Type, Object Class, PropertyAccessor,
        // 1. get all files from directory
        // 2. get all properties with enum class Prefix + Class + Property
        vcc::LogService::logInfo(logConfig, logId, L"Generate property start.");
        
        // Generate OperationResult
        for (auto const &exportOption : option->getExports()) {
            if (exportOption->getInterface() == VPGConfigInterfaceType::Java) {
                VPGJavaGenerationService::generateOperationResult(logConfig, projPrefix, exportOption.get(),
                    typeWorkspaceClassRelativePathMapObject, typeWorkspaceClassRelativePathMapForm);
            }
        }
        std::wstring filePrefix = projPrefix;
        vcc::toLower(filePrefix);        
        std::set<std::wstring> objectTypes;
        std::set<std::wstring> objectFileNames, propertyAccessorFileNames;
        auto dllOption = std::make_shared<VPGDllFileGenerationServiceOption>();
        for (auto &filePath : std::filesystem::recursive_directory_iterator(PATH(typeWorkspaceFullPath))) {
            if (filePath.is_directory())
                continue;
            std::wstring path = vcc::getLinuxPath(filePath.path().wstring());
            std::wstring fileName = filePath.path().filename().wstring();
            std::wstring middlePath = vcc::getRelativePath(vcc::getLinuxPath(filePath.path().parent_path().wstring()), vcc::getLinuxPath(typeWorkspaceFullPath));
            if (middlePath == L".")
                middlePath = L"";

            if (!vcc::isBlank(projPrefix) && !vcc::isStartWith(fileName, filePrefix))
                vcc::LogService::LogWarning(logConfig, logId, L"Class Prefix " + projPrefix + L" missing. Skip: " + path);

            // ------------------------------------------------------------------------------------------ //
            //                                      Parse File Start                                      //
            // ------------------------------------------------------------------------------------------ //
            vcc::LogService::LogWarning(logConfig, logId, L"Parse file start: " + path);

            std::wstring fileContent = vcc::readFile(path);
            vcc::trim(fileContent);
            if (fileContent.empty())
                continue;

            std::vector<std::shared_ptr<VPGEnumClass>> enumClassList;
            std::vector<std::shared_ptr<VPGEnumClass>> objectEnumClassList;
            enumClassReader.parse(fileContent, enumClassList);
            // ------------------------------------------------------------------------------------------ //
            //                              Override Enum Class based on vcc.json                         //
            // ------------------------------------------------------------------------------------------ //
            if (option->getBehavior()->getActionHistoryType() == VPGConfigActionHistoryType::NoHistory) {
                for (auto enumClass : enumClassList)
                    for (auto property : enumClass->getProperties())
                        property->setIsNoHistory(true);
            }
            // ------------------------------------------------------------------------------------------ //
            //                              Procedure Start                                               //
            // ------------------------------------------------------------------------------------------ //
            for (auto const &enumClass : enumClassList) {
                std::wstring propertyClassNameWithoutNamespace = getTypeOrClassWithoutNamespace(enumClass->getName());
                std::wstring classNameWithoutNamespace = getClassNameFromPropertyClassName(enumClass->getName());
                if (projPrefix.empty() || IsPropertyClass(propertyClassNameWithoutNamespace, projPrefix)) {
                    objectTypes.insert(classNameWithoutNamespace.substr(projPrefix.size()));
                    objectEnumClassList.push_back(enumClass);
                } else {
                    std::wstring classPrefixStr = !vcc::isBlank(projPrefix) ? (L"Prefix " + projPrefix + L" or ") : L"";
                    vcc::LogService::LogWarning(logConfig, logId, L"Class " + classPrefixStr + L"Suffix " + propertyClassNameSuffix + L"missing. Not generate object for " + enumClass->getName());
                }
            
                // ------------------------------------------------------------------------------------------ //
                //                               JAVA Export File                                             //
                // ------------------------------------------------------------------------------------------ //
                std::wstring javaEnumClassName = propertyClassNameWithoutNamespace;
                if (!projPrefix.empty() && !vcc::isStartWith(javaEnumClassName, projPrefix))
                    javaEnumClassName = projPrefix + javaEnumClassName;

                for (auto const &javaOption : option->getExports()) {
                    if (vcc::isBlank(javaOption->getWorkspace()) || javaOption->getInterface() != VPGConfigInterfaceType::Java)
                        continue;
                    
                    std::wstring workspace = vcc::isAbsolutePath(javaOption->getWorkspace()) ? javaOption->getWorkspace() : vcc::concatPaths({ _Workspace, javaOption->getWorkspace() });

                    if (!vcc::isBlank(javaOption->getTypeDirectory()))
                        VPGJavaGenerationService::generateEnum(logConfig, getConcatPath(workspace, javaOption->getTypeDirectory(), middlePath, javaEnumClassName + L".java"), middlePath, enumClass.get(), option, javaOption.get());
                    
                    if (IsPropertyClass(propertyClassNameWithoutNamespace, projPrefix)) {
                        std::wstring objectDirectory = javaOption->getObjectDirectory();
                        if (enumClass->getType() == VPGEnumClassType::Form && !vcc::isBlank(javaOption->getFormDirectory()))
                            objectDirectory = javaOption->getFormDirectory();
                        if (!vcc::isBlank(objectDirectory))
                            VPGJavaGenerationService::generateObject(logConfig, getConcatPath(workspace, objectDirectory, middlePath, classNameWithoutNamespace + L".java"), middlePath, enumClass.get(),
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
                
                if (!vcc::isBlank(objectDirectoryHpp) && !vcc::isBlank(objectDirectoryCpp)) {
                    objectFileNames.insert(objectFileName + L".hpp");
                    std::wstring fileObjectDirectoryHpp = !vcc::isBlank(formDirectoryHpp) ? getConcatPath(projWorkspace, formDirectoryHpp, middlePath, objectFileName + L".hpp") : L"";
                    std::wstring fileObjectDirectoryCpp = !vcc::isBlank(formDirectoryCpp) ? getConcatPath(projWorkspace, formDirectoryCpp, middlePath, objectFileName + L".cpp") : L"";
                    std::wstring actionFolderHpp = !vcc::isBlank(actionDirectoryHpp) ? getConcatPath(projWorkspace, actionDirectoryHpp, middlePath, L"") : L"";
                    std::wstring actionFolderCpp = !vcc::isBlank(actionDirectoryCpp) ? getConcatPath(projWorkspace, actionDirectoryCpp, middlePath, L"") : L"";
                    
                    VPGObjectFileGenerationService::generateHpp(logConfig, option, _IncludeFiles, _EnumClasses, getConcatPath(projWorkspace, objectDirectoryHpp, middlePath, objectFileName + L".hpp"), fileObjectDirectoryHpp, actionFolderHpp, objectEnumClassList);
                    VPGObjectFileGenerationService::generateCpp(logConfig, projPrefix, _IncludeFiles, _EnumClasses, getConcatPath(projWorkspace, objectDirectoryCpp, middlePath, objectFileName + L".cpp"), fileObjectDirectoryCpp, actionFolderCpp, objectEnumClassList);
                }
                if (!propertyAccessorDirectoryHpp.empty() && !propertyAccessorDirectoryCpp.empty()) {
                    propertyAccessorFileNames.insert(propertyAccessorFileName + L".hpp");
                    VPGPropertyAccessorGenerationService::generateHpp(logConfig, projPrefix, getConcatPath(projWorkspace, propertyAccessorDirectoryHpp, middlePath, propertyAccessorFileName + L".hpp"), objectEnumClassList);
                    VPGPropertyAccessorGenerationService::generateCpp(logConfig, projPrefix, _IncludeFiles, getConcatPath(projWorkspace, propertyAccessorDirectoryCpp, middlePath, propertyAccessorFileName + L".cpp"), objectEnumClassList);
                }
            }
            
            // ------------------------------------------------------------------------------------------ //
            //                                      Parse File End                                        //
            // ------------------------------------------------------------------------------------------ //
            vcc::LogService::logInfo(logConfig, logId, L"Parse file completed: " + path);
        }
        // ------------------------------------------------------------------------------------------ //
        //                               Generate Object Type File                                    //
        // ------------------------------------------------------------------------------------------ //
        VPGObjectTypeFileGenerationService::generate(logConfig, vcc::concatPaths({projWorkspace, objectTypeDirectory, objectTypeHppFileName}), objectTypes);

        // ------------------------------------------------------------------------------------------ //
        //                               Generate Object Factory File                                 //
        // ------------------------------------------------------------------------------------------ //
        if (!vcc::isBlank(objectFactoryDirectoryHpp) && !vcc::isBlank(objectFactoryDirectoryCpp)) {
            VPGObjectFactoryFileGenerationService::generateHpp(logConfig, vcc::concatPaths({projWorkspace, objectFactoryDirectoryHpp, objectFactoryFileNameHpp}));
            VPGObjectFactoryFileGenerationService::generateCpp(logConfig, projPrefix, objectFileNames, vcc::concatPaths({projWorkspace, objectFactoryDirectoryCpp, objectFactoryFileNameCpp}), objectTypes);
        }
        // ------------------------------------------------------------------------------------------ //
        //                               Generate Property Accessor Factory File                      //
        // ------------------------------------------------------------------------------------------ //
        if (!vcc::isBlank(propertyAccessorFactoryDirectoryHpp) && !vcc::isBlank(propertyAccessorFactoryDirectoryCpp)) {
            dllOption->setIsGeneratePropertyAccessor(true);
            VPGPropertyAccessorFactoryFileGenerationService::generateHpp(logConfig, vcc::concatPaths({projWorkspace, propertyAccessorFactoryDirectoryHpp, propertyAccessorFactoryFileNameHpp}));
            VPGPropertyAccessorFactoryFileGenerationService::generateCpp(logConfig, projPrefix, propertyAccessorFileNames, vcc::concatPaths({projWorkspace, propertyAccessorFactoryDirectoryCpp, propertyAccessorFactoryFileNameCpp}), objectTypes);
        }

        // ------------------------------------------------------------------------------------------ //
        //                               Generate DLL inteface File                                   //
        // ------------------------------------------------------------------------------------------ //
        VPGDllFileGenerationService::generateHpp(logConfig, vcc::concatPaths({projWorkspace, L"DllFunctions.h"}), dllOption.get());
        VPGDllFileGenerationService::generateCpp(logConfig, vcc::concatPaths({projWorkspace, L"DllFunctions.cpp"}), dllOption.get());

        // ------------------------------------------------------------------------------------------ //
        //                               Generate JAVA bridge                                         //
        // ------------------------------------------------------------------------------------------ //
        VPGJavaGenerationService::generateJavaBridge(logConfig, _Workspace, vcc::concatPaths({projWorkspace, L"DllFunctions.h"}), option);

        vcc::LogService::logInfo(logConfig, logId, L"Generate Property Finished.");
    CATCH
}
