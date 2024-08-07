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
#include "vpg_dll_file_generation_service.hpp"
#include "vpg_enum_class.hpp"
#include "vpg_enum_class_reader.hpp"
#include "vpg_generation_option.hpp"
#include "vpg_include_path_service.hpp"
#include "vpg_object_file_generation_service.hpp"
#include "vpg_object_factory_file_generation_service.hpp"
#include "vpg_object_type_file_generation_service.hpp"
#include "vpg_property_accessor_generation_service.hpp"
#include "vpg_property_accessor_factory_file_generation_service.hpp"

const std::wstring classMacroFilePath = L"include/External/VCC/Core/Macro/class_macro.hpp";
const std::wstring logId = L"File Generation";

const std::wstring objectTypeHppFileName = L"object_type.hpp";
const std::wstring objectFactoryFileNameHpp = L"object_factory.hpp";
const std::wstring objectFactoryFileNameCpp = L"object_factory.cpp";
const std::wstring propertyAccessorFactoryFileNameHpp = L"property_accessor_factory.hpp";
const std::wstring propertyAccessorFactoryFileNameCpp = L"property_accessor_factory.cpp";
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
        _EnumClasses.clear();
        VPGIncludePathService::GetWorkspaceIncludePath(L"", this->_ClassMacros, _IncludeFiles, _EnumClasses);

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
                    if (IsClassEnumFile(fileName, projectPrefix) && IsClassEnum(enumClassName, projectPrefix)) {
                        std::wstring className = GetClassNameFromEnumClassName(enumClassName);
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

std::wstring VPGFileGenerationManager::GetConcatPath(const std::wstring &projWorkspace, const std::wstring &objWorkspace, const std::wstring &middlePath, const std::wstring &fileName) const
{
    std::vector<std::wstring> objectFilePaths = { projWorkspace, objWorkspace };
    if (!middlePath.empty())
        objectFilePaths.push_back(middlePath);
    objectFilePaths.push_back(fileName);
    return ConcatPaths(objectFilePaths);
}

void VPGFileGenerationManager::GernerateProperty(const LogProperty *logProperty, const VPGGenerationOption *option)
{
    TRY
        std::wstring projPrefix = option->GetProjectPrefix();
        std::wstring projWorkspace = _Workspace;
        std::wstring typeWorkspaceFullPath = ConcatPaths({projWorkspace, option->GetTypeWorkspace()});
        std::wstring propertyAccessorDirectoryHpp = option->GetPropertyAccessorDirectoryHpp();
        std::wstring propertyAccessorDirectoryCpp = option->GetPropertyAccessorDirectoryCpp();

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
        std::set<std::wstring> objectFileNames, propertyAccessorFileNames;
        DECLARE_SPTR(VPGDllFileGenerationServiceOption, dllOption);
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
                objectFileNames.insert(objectFileName + L".hpp");
                VPGObjectFileGenerationService::GenerateHpp(logProperty, projPrefix, _IncludeFiles, GetConcatPath(projWorkspace, option->GetObjectDirectoryHpp(), middlePath, objectFileName + L".hpp"), enumClassList);
                VPGObjectFileGenerationService::GenerateCpp(logProperty, _IncludeFiles, _EnumClasses, GetConcatPath(projWorkspace, option->GetObjectDirectoryCpp(), middlePath, objectFileName + L".cpp"), enumClassList);
                if (!propertyAccessorDirectoryHpp.empty() && !propertyAccessorDirectoryCpp.empty()) {
                    propertyAccessorFileNames.insert(propertyAccessorFileName + L".hpp");
                    VPGPropertyAccessorGenerationService::GenerateHpp(logProperty, GetConcatPath(projWorkspace, propertyAccessorDirectoryHpp, middlePath, propertyAccessorFileName + L".hpp"), enumClassList);
                    VPGPropertyAccessorGenerationService::GenerateCpp(logProperty, _IncludeFiles, GetConcatPath(projWorkspace, propertyAccessorDirectoryCpp, middlePath, propertyAccessorFileName + L".cpp"), enumClassList);
                }
            }
            LogService::LogInfo(logProperty, logId, L"Parse file completed: " + path);
            // ------------------------------------------------------------------------------------------ //
            //                                      Parse File End                                        //
            // ------------------------------------------------------------------------------------------ //
        }
        // ------------------------------------------------------------------------------------------ //
        //                               Generate Object Type File                                    //
        // ------------------------------------------------------------------------------------------ //
        VPGObjectTypeFileGenerationService::Generate(logProperty, ConcatPaths({projWorkspace, option->GetObjectTypeDirectory(), objectTypeHppFileName}), objectTypes);

        // ------------------------------------------------------------------------------------------ //
        //                               Generate Object Factory File                                 //
        // ------------------------------------------------------------------------------------------ //
        if (!option->GetObjectFactoryDirectoryHpp().empty() && !option->GetObjectFactoryDirectoryCpp().empty()) {
            VPGObjectFactoryFileGenerationService::GenerateHpp(logProperty, ConcatPaths({projWorkspace, option->GetObjectFactoryDirectoryHpp(), objectFactoryFileNameHpp}));
            VPGObjectFactoryFileGenerationService::GenerateCpp(logProperty, projPrefix, objectFileNames, ConcatPaths({projWorkspace, option->GetObjectFactoryDirectoryCpp(), objectFactoryFileNameCpp}), objectTypes);
        }
        // ------------------------------------------------------------------------------------------ //
        //                               Generate Property Accessor Factory File                      //
        // ------------------------------------------------------------------------------------------ //
        if (!option->GetPropertyAccessorFactoryDirectoryHpp().empty() && !option->GetPropertyAccessorFactoryDirectoryCpp().empty()) {
            dllOption->SetIsGeneratePropertyAccessor(true);
            VPGPropertyAccessorFactoryFileGenerationService::GenerateHpp(logProperty, ConcatPaths({projWorkspace, option->GetPropertyAccessorFactoryDirectoryHpp(), propertyAccessorFactoryFileNameHpp}));
            VPGPropertyAccessorFactoryFileGenerationService::GenerateCpp(logProperty, projPrefix, propertyAccessorFileNames, ConcatPaths({projWorkspace, option->GetPropertyAccessorFactoryDirectoryCpp(), propertyAccessorFactoryFileNameCpp}), objectTypes);
        }

        // ------------------------------------------------------------------------------------------ //
        //                               Generate DLL Inteface File                                   //
        // ------------------------------------------------------------------------------------------ //
        VPGDllFileGenerationService::GenerateHpp(logProperty, ConcatPaths({projWorkspace, L"DllFunctions.h"}), dllOption.get());
        VPGDllFileGenerationService::GenerateCpp(logProperty, ConcatPaths({projWorkspace, L"DllFunctions.cpp"}), dllOption.get());

        LogService::LogInfo(logProperty, logId, L"Generate Property Finished.");
    CATCH
}
