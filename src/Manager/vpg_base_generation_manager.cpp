#include "vpg_base_generation_manager.hpp"

#include <assert.h>
#include <filesystem>
#include <iostream>
#include <memory>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "i_document.hpp"
#include "i_document_builder.hpp"
#include "json.hpp"
#include "memory_macro.hpp"
#include "vpg_code_reader.hpp"
#include "vpg_file_generation_service.hpp"
#include "vpg_file_sync_service.hpp"
#include "vpg_project_type.hpp"

using namespace vcc;

std::wstring VPGGenerationOption::SerializeJson(const IDocumentBuilder *builder)
{
    TRY_CATCH(
        DECLARE_UPTR(Json, json);
        json->AddString(L"Version", _Version);
        json->AddBool(L"IsGit", _IsGit);

        json->AddString(L"ProjectPrefix", _ProjectPrefix);

        json->AddString(L"ProjectName", _ProjectName);
        json->AddString(L"ProjectNameDLL", _ProjectNameDLL);
        json->AddString(L"ProjectNameEXE", _ProjectNameEXE);
        json->AddString(L"ProjectNameGtest", _ProjectNameGtest);

        json->AddBool(L"IsExcludeVCCUnitTest", _IsExcludeVCCUnitTest);

        json->AddString(L"ActionTypeDirectory", _ActionTypeDirectory);
        json->AddString(L"ExceptionTypeDirectory", _ExceptionTypeDirectory);
        json->AddString(L"ManagerTypeDirectory", _ManagerTypeDirectory);
        json->AddString(L"ObjectTypeDirectory", _ObjectTypeDirectory);


        json->AddString(L"TypeWorkspace", _TypeWorkspace);
        json->AddString(L"ObjectHppDirectory", _ObjectHppDirectory);
        json->AddString(L"ObjectTypeHppDirectory", _ObjectTypeHppDirectory);
        json->AddString(L"PropertyAccessorHppDirectory", _PropertyAccessorHppDirectory);
        json->AddString(L"PropertyAccessorCppDirectory", _PropertyAccessorCppDirectory);

        DECLARE_SPTR(Json, plugins);
        json->AddArray(L"Plugins", plugins);
        for (auto const &plugin : _Plugins) {
            plugins->AddArrayString(plugin);
        }
        
        return builder->Serialize(json.get());
    )
    return L"";
}

void VPGGenerationOption::DeserializeJson(std::shared_ptr<IDocument> document)
{
    TRY_CATCH(
        std::shared_ptr<Json> json = dynamic_pointer_cast<Json>(document);
        assert(json != nullptr);

        if (json->IsContainKey(L"Version"))
            this->SetVersion(json->GetString(L"Version"));
        if (json->IsContainKey(L"IsGit"))
            this->SetIsGit(json->GetBool(L"IsGit"));
        if (json->IsContainKey(L"ProjectPrefix"))
            this->SetProjectPrefix(json->GetString(L"ProjectPrefix"));


        if (json->IsContainKey(L"ProjectName"))
            this->SetProjectName(json->GetString(L"ProjectName"));
        if (json->IsContainKey(L"ProjectNameDLL"))
            this->SetProjectNameDLL(json->GetString(L"ProjectNameDLL"));
        if (json->IsContainKey(L"ProjectNameEXE"))
            this->SetProjectNameEXE(json->GetString(L"ProjectNameEXE"));
        if (json->IsContainKey(L"ProjectNameGtest"))
            this->SetProjectNameGtest(json->GetString(L"ProjectNameGtest"));

        if (json->IsContainKey(L"IsExcludeVCCUnitTest"))
            this->SetIsExcludeVCCUnitTest(json->GetBool(L"IsExcludeVCCUnitTest"));

        if (json->IsContainKey(L"ActionTypeDirectory"))
            this->SetActionTypeDirectory(json->GetString(L"ActionTypeDirectory"));
        if (json->IsContainKey(L"ExceptionTypeDirectory"))
            this->SetExceptionTypeDirectory(json->GetString(L"ExceptionTypeDirectory"));
        if (json->IsContainKey(L"ManagerTypeDirectory"))
            this->SetManagerTypeDirectory(json->GetString(L"ManagerTypeDirectory"));
        if (json->IsContainKey(L"ObjectTypeDirectory"))
            this->SetObjectTypeDirectory(json->GetString(L"ObjectTypeDirectory"));

        if (json->IsContainKey(L"TypeWorkspace"))
            this->SetTypeWorkspace(json->GetString(L"TypeWorkspace"));
        if (json->IsContainKey(L"ObjectHppDirectory"))
            this->SetObjectHppDirectory(json->GetString(L"ObjectHppDirectory"));
        if (json->IsContainKey(L"ObjectTypeHppDirectory"))
            this->SetObjectTypeHppDirectory(json->GetString(L"ObjectTypeHppDirectory"));
        if (json->IsContainKey(L"PropertyAccessorHppDirectory"))
            this->SetPropertyAccessorHppDirectory(json->GetString(L"PropertyAccessorHppDirectory"));
        if (json->IsContainKey(L"PropertyAccessorCppDirectory"))
            this->SetPropertyAccessorCppDirectory(json->GetString(L"PropertyAccessorCppDirectory"));

        if (json->IsContainKey(L"Plugins")) {
            auto plugins = json->GetArray(L"Plugins");
            for (auto const &plugin : plugins) {
                this->InsertPlugins(plugin->GetValue());
            }
        }
    )
}


VPGBaseGenerationManager::VPGBaseGenerationManager(std::shared_ptr<LogProperty> logProperty, std::shared_ptr<VPGGenerationOption> option)
{ 
    assert(logProperty != nullptr);
    assert(option != nullptr);
    this->_LogProperty = logProperty;
    this->_Option = option;
}

void VPGBaseGenerationManager::ValidateOption()
{
    assert(!IsBlank(_Option->GetWorkspaceSource()));
    assert(!IsBlank(_Option->GetWorkspaceDestination()));
    assert(!IsBlank(_Option->GetProjectName()));
    assert(!IsBlank(_Option->GetProjectNameDLL()) || !IsBlank(_Option->GetProjectNameEXE()));
}

std::wstring VPGBaseGenerationManager::GetDLLTestFileContent()
{
    std::wstring result = L"";
    result += L"#include <gtest/gtest.h>\r\n";
    result += L"#ifdef _WIN32\r\n";
    result += L"#include <windows.h>\r\n"; 
    result += L"#else\r\n";
    result += L"#include <dlfcn.h>\r\n";
    result += L"#endif\r\n";
    result += L"\r\n";
    result += L"struct DllHandle\r\n";
    result += L"{\r\n";
    result += L"    #ifdef _WIN32\r\n";
    result += L"    DllHandle(const char * const filename) : h(LoadLibrary(filename)) {}\r\n";
    result += L"    ~DllHandle() { if (h) FreeLibrary(h); }\r\n";
    result += L"    HINSTANCE Get() const { return h; }\r\n";
    result += L"\r\n";
    result += L"    private:\r\n";
    result += L"        HINSTANCE h;\r\n";
    result += L"\r\n";
    result += L"    public:\r\n";
    result += L"        static FARPROC GetProc(HINSTANCE h, const char* procName) { return GetProcAddress(h, procName); };\r\n";
    result += L"    #else\r\n";
    result += L"    DllHandle(const char * const filename) : h(dlopen(filename, RTLD_LAZY)) {}\r\n";
    result += L"    ~DllHandle() { if (h) dlclose(h); }\r\n";
    result += L"    void* Get() const { return h; }\r\n";
    result += L"\r\n";
    result += L"    private:\r\n";
    result += L"        void* h;\r\n";
    result += L"\r\n";
    result += L"    public:\r\n";
    result += L"        static void *GetProc(void *h, const char* procName) { return dlsym(h, procName); };\r\n";
    result += L"    #endif\r\n";
    result += L"};\r\n";
    result += L"\r\n";
    result += L"TEST(DllTest, LoadDll) {\r\n";
    result += L"    #ifdef _WIN32\r\n";
    result += L"    const DllHandle h(\"" + _Option->GetProjectNameDLL() + L".dll\");\r\n";
    result += L"    #else\r\n";
    result += L"    const DllHandle h(\"bin/Debug/" + _Option->GetProjectNameDLL() + L".so\");\r\n";
    result += L"    if (h.Get() == nullptr) {\r\n";
    result += L"        fprintf(stderr, \"%s\\n\", dlerror());\r\n";
    result += L"    }\r\n";
    result += L"    #endif\r\n";
    result += L"    EXPECT_TRUE(h.Get());\r\n";
    result += L"    \r\n";
    result += L"    typedef long long int (*GetVersionFunction)();\r\n";
    result += L"    const GetVersionFunction GetVersion = reinterpret_cast<GetVersionFunction>(DllHandle::GetProc(h.Get(), \"GetVersion\"));\r\n";
    result += L"    EXPECT_TRUE(GetVersion != nullptr);\r\n";
    result += L"    EXPECT_EQ(GetVersion(), 1);\r\n";
    result += L"}\r\n";

    return result;
}

void VPGBaseGenerationManager::CreateWorkspaceDirectory()
{
    TRY_CATCH(
        ValidateOption();
        // All type has same project structure
        std::vector<std::wstring> checkList;
        // bin
        checkList.push_back(L"bin");
        checkList.push_back(L"bin/Debug");
        checkList.push_back(L"bin/Release");

        // include
        checkList.push_back(L"include");

        // lib
        checkList.push_back(L"lib");

        // src
        checkList.push_back(L"src");

        // unittest
        if (!IsBlank(_Option->GetProjectNameGtest()))
            checkList.push_back(_Option->GetProjectNameGtest());
        
        std::wstring workspace = _Option->GetWorkspaceDestination();
        for (auto path : checkList) {
            std::wstring absPath = ConcatPaths({workspace, path});
            if (!IsDirectoryExists(absPath)) {
                CreateDirectory(absPath);
                LogService::LogInfo(this->GetLogProperty().get(), L"", L"Create Directory: " + path);
            }        
        }
    )
}

void VPGBaseGenerationManager::CreateBasicProject()
{
    TRY_CATCH(
        ValidateOption();
        this->CreateWorkspaceDirectory();

        std::wstring src = _Option->GetWorkspaceSource();
        std::wstring dest = _Option->GetWorkspaceDestination();
        CopyFile(ConcatPaths({src, MakeFileName}), ConcatPaths({dest, MakeFileName}));

        if (_Option->GetIsGit()) {
            CopyFile(ConcatPaths({src, L".gitignore"}), ConcatPaths({dest, L".gitignore"}));
        }
        if (!IsBlank(_Option->GetProjectNameEXE())) {
            CopyFile(ConcatPaths({src, L"main.cpp"}), ConcatPaths({dest, L"main.cpp"}));
        }
        if (!IsBlank(_Option->GetProjectNameDLL())) {
            CopyFile(ConcatPaths({src, L"DllEntryPoint.cpp"}), ConcatPaths({dest, L"DllEntryPoint.cpp"}));
            CopyFile(ConcatPaths({src, L"DllFunctions.cpp"}), ConcatPaths({dest, L"DllFunctions.cpp"}));
            CopyFile(ConcatPaths({src, L"DllFunctions.h"}), ConcatPaths({dest, L"DllFunctions.h"}));
        }
        if (!IsBlank(_Option->GetProjectNameGtest())) {
            CopyFile(ConcatPaths({src, L"unittest", L"gtest_main.cpp"}), ConcatPaths({dest, _Option->GetProjectNameGtest(), L"gtest_main.cpp"}), true);

            if (!IsBlank(_Option->GetProjectNameDLL())) {
                AppendFileOneLine(ConcatPaths({dest, _Option->GetProjectNameGtest(), L"dllTest/dynamic_library_test.cpp"}), GetDLLTestFileContent(), true);
            }
        }
        // Cannot Copy
        AppendFileOneLine(ConcatPaths({dest, L"LICENSE"}), L"", true);
        AppendFileOneLine(ConcatPaths({dest, L"README.md"}), L"", true);

        if (IsFileExists(ConcatPaths({src, MakeFileName}))) {
            // Makefile
            std::wstring makefilePath = ConcatPaths({dest, MakeFileName});
            std::wstring makefileContent = ReadFile(makefilePath);
            WriteFile(makefilePath, this->AdjustMakefile(makefileContent), true);
        }
    )
}

void VPGBaseGenerationManager::SyncWorkspace(const LogProperty *logProperty, const std::wstring &sourceWorkspace, const std::wstring &targetWorkspace,
            const std::vector<std::wstring> &includeFileFilters, const std::vector<std::wstring> &excludeFileFilters)
{
    try {
        std::vector<std::wstring> needToAdd, needToModify, needToDelete;
        GetFileDifferenceBetweenWorkspaces(sourceWorkspace, targetWorkspace, needToAdd, needToModify, needToDelete);

        // Delete
        for (auto path : needToDelete) {
            std::wstring targetPath = ConcatPaths({targetWorkspace, path});
            if (!std::filesystem::exists(targetPath))
                continue;
            if (std::filesystem::is_directory(targetPath)) {
                if (!path.ends_with(L"/")) {
                    path += L"/";
                    targetPath += L"/";
                }                
            }

            if (!includeFileFilters.empty() && !IsPathMatchFileFilters(path, includeFileFilters))
                continue;
            if (!excludeFileFilters.empty() && IsPathMatchFileFilters(path, excludeFileFilters))
                continue;


            if (IsFile(targetPath)) {
                remove(PATH(targetPath));
                LogService::LogInfo(logProperty, L"", L"Removed Directory: " + targetPath);
            } else {
                std::filesystem::remove_all(PATH(targetPath));
                LogService::LogInfo(logProperty, L"", L"Removed File: " + targetPath);
            }
        }

        // Add
        for (auto path : needToAdd) {
            std::wstring sourcePath = ConcatPaths({sourceWorkspace, path});
            std::wstring targetPath = ConcatPaths({targetWorkspace, path});
            if (!std::filesystem::exists(sourcePath))
                continue;
            if (std::filesystem::exists(targetPath))
                continue;
            if (std::filesystem::is_directory(sourcePath)) {
                if (!path.ends_with(L"/")) {
                    path += L"/";
                    sourcePath += L"/";
                    targetPath += L"/";
                }                
            }

            if (!includeFileFilters.empty() && !IsPathMatchFileFilters(path, includeFileFilters))
                continue;
            if (!excludeFileFilters.empty() && IsPathMatchFileFilters(path, excludeFileFilters))
                continue;

            if (IsFile(sourcePath)) {
                VPGFileSyncService::CopyFile(logProperty, sourcePath, targetPath);
            } else {
                CreateDirectory(targetPath);
                LogService::LogInfo(logProperty, L"", L"Added Directory: " + targetPath);
            }
        }

        // Modify
        for (auto path : needToModify) {
            if (!includeFileFilters.empty() && !IsPathMatchFileFilters(path, includeFileFilters))
                continue;
            if (!excludeFileFilters.empty() && IsPathMatchFileFilters(path, excludeFileFilters))
                continue;

            std::wstring sourcePath = ConcatPaths({sourceWorkspace, path});
            std::wstring targetPath = ConcatPaths({targetWorkspace, path});
            if (!IsFile(sourcePath) || !IsFile(targetPath))
                continue;
            
            // modify file
            VPGFileSyncService::CopyFile(logProperty, sourcePath, targetPath);
        }
    } catch (const std::exception &e) {
        THROW_EXCEPTION(e);
    }
}

std::wstring VPGBaseGenerationManager::AdjustMakefile(const std::wstring &fileContent)
{
    ValidateOption();
    std::wstring result = L"";
    TRY_CATCH(
        DECLARE_SPTR(XMLElement, elements);
        VPGCodeReader reader(L"#");
        reader.Parse(fileContent, elements);
        for (std::shared_ptr<vcc::XMLElement> element : elements->GetChildren()) {
            if (element->GetNamespace() == L"vcc" && element->GetName() == L"name") {
                std::wstring projName = !IsBlank(_Option->GetProjectName()) ? (L" " + _Option->GetProjectName()) : L"";
                std::wstring dllName = !IsBlank(_Option->GetProjectNameDLL()) ? (L" " + _Option->GetProjectNameDLL()) : L"";
                std::wstring exeName = !IsBlank(_Option->GetProjectNameEXE()) ? (L" " + _Option->GetProjectNameEXE()) : L"";
                std::wstring gtestName = !IsBlank(_Option->GetProjectNameGtest()) ? (L" " + _Option->GetProjectNameGtest()) : L"";
        
                result += L"# <vcc:name sync=\"ALERT\">\r\n";
                result += L"#----------------------------------#\r\n";
                result += L"#---------- Project Name ----------#\r\n";
                result += L"#----------------------------------#\r\n";
                result += L"PROJ_NAME :=" + projName + L"\r\n";
                result += L"PROJ_NAME_DLL :=" + dllName + L"\r\n";
                result += L"PROJ_NAME_EXE :=" + exeName + L"\r\n";
                result += L"PROJ_NAME_GTEST :=" + gtestName + L"\r\n";
                result += L"# </vcc:name>";
            } else
                result += element->GetFullText();
        }
    )
    return result;
}

void VPGBaseGenerationManager::Add()
{
    THROW_EXCEPTION_MSG(ExceptionType::NotSupport, L"Update mode only support VCCModule.");
}

void VPGBaseGenerationManager::Update()
{
    THROW_EXCEPTION_MSG(ExceptionType::NotSupport, L"Update mode only support VCCModule.");
}

void VPGBaseGenerationManager::Generate()
{
    THROW_EXCEPTION_MSG(ExceptionType::NotSupport, L"Generate mode only support VCCModule.");
}