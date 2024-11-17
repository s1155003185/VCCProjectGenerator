#include "vpg_include_path_service.hpp"

#include <filesystem>
#include <fstream>
#include <map>
#include <set>
#include <vector>

#include "exception_macro.hpp"
#include "file_helper.hpp"
#include "memory_macro.hpp"
#include "platform_type.hpp"
#include "string_helper.hpp"
#include "vector_helper.hpp"

#include "vpg_cpp_helper.hpp"
#include "vpg_enum_class.hpp"
#include "vpg_enum_class_reader.hpp"
#include "vpg_include_path_reader.hpp"

void VPGIncludePathService::GetWorkspaceIncludePath(const std::wstring &workspace, const std::set<std::wstring> &classMacroList, std::map<std::wstring, std::wstring> &classPathMapping, std::map<std::wstring, std::shared_ptr<VPGEnumClass>> &enumClassMapping)
{
    TRY
        DECLARE_UPTR(VPGEnumClassReader, reader, classMacroList);
        for (auto const &filePath : std::filesystem::recursive_directory_iterator(PATH(!workspace.empty() ? workspace : L"."))) {
            if (filePath.is_directory() || !filePath.path().wstring().ends_with(L".hpp"))
                continue;
            std::wstring fileContent = ReadFile(filePath.path().wstring());//GetSimpleCode(ReadFile(filePath.path().wstring()));
            std::vector<std::shared_ptr<VPGEnumClass>> curremtnEnumClasses;
            reader->Parse(fileContent, curremtnEnumClasses);
            for (auto enumClass : curremtnEnumClasses)
                enumClassMapping.insert(std::make_pair((!enumClass->GetNamespace().empty()? (enumClass->GetNamespace() + L"::") : L"") + enumClass->GetName(), enumClass));
            std::set<std::wstring> classNames;
            VPGIncludePathReader reader;
            reader.Parse(PlatformType::NA, fileContent, classNames);
            for (auto const &className : classNames) {
                classPathMapping.insert(std::make_pair(className, GetFileName(filePath.path().wstring())));
            }
        }
    CATCH
}

// void VPGIncludePathService::GetSystemIncludePath(const PlatformType &platformType, const std::wstring &workspace, std::map<std::wstring, std::wstring> &classPathMapping)
// {
//     TRY
//         for (auto const &filePath : std::filesystem::recursive_directory_iterator(PATH(!workspace.empty() ? workspace : L"."))) {
//             std::wstring fileName = GetFileName(filePath.path().wstring());
//             if (filePath.is_directory() || fileName.starts_with(L"_"))
//                 continue;
//             // check all parents and file has prefix _
//             // if yes, then skip
//             std::wstring linuxPath = GetLinuxPath(filePath.path().wstring());
//             if (Find(linuxPath, L"/_") != std::wstring::npos)
//                 continue;

//             std::wstring fileContent = ReadFile(filePath.path().wstring());
//             std::set<std::wstring> classNames;
//             VPGIncludePathReader reader;
//             reader.Parse(platformType, fileContent, classNames);
//             for (auto const &className : classNames) {
//                 classPathMapping.insert(std::make_pair(className, GetFileName(filePath.path().wstring())));
//             }
//         }
//     }
// }
