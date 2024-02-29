#include "file_helper.hpp"

#include <assert.h>
#include <filesystem>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>

#include "exception.hpp"
#include "exception_macro.hpp"
#include "exception_type.hpp"
#include "vector_helper.hpp"

#ifdef __WIN32
#include "file_helper_win.hpp"
#else
#include "file_helper_linux.hpp"
#endif

using namespace std;

namespace vcc
{
	std::wstring GetSystemFolderPath(SystemFolderType fileType)
    {
        #ifdef __WIN32
        return GetSystemFolderPathWindow(fileType);
        #else
        return GetSystemFolderPathLinux(fileType);
        #endif
    }

    std::wstring GetFileName(const std::wstring &filePath)
    {
        std::wstring fileName = L"";
        TRY_CATCH(
            fileName = PATH(filePath).filename().wstring();
        )
        return fileName;
    }

    std::wstring ConcatPath(std::wstring directory, std::wstring addition)
    {
        if (addition.starts_with(L"/") || addition.starts_with(L"\\"))
            addition.erase(0, 1);

        PATH dir(directory);
        PATH add(addition);
        dir /= add;
        return dir.wstring();
    }

    std::wstring GetRelativePath(const std::wstring &absolutePath, const std::wstring &basePath)
    {
        if (IsBlank(basePath))
            return absolutePath;
        if (!HasPrefix(absolutePath, basePath))
            THROW_EXCEPTION_MSG(ExceptionType::CustomError, L"Absolute Path " + absolutePath + L" is not start with " + basePath);
        std::wstring path = absolutePath.substr(basePath.length());
        return path.starts_with(L"/") || path.starts_with(L"\\") ? path.substr(1) : path;
    }

    void GetFileDifferenceBetweenWorkspaces(std::wstring sourceWorkspace, std::wstring targetWorkspace, 
        std::vector<std::wstring> &needToAdd, std::vector<std::wstring> &needToModify, std::vector<std::wstring> &needToDelete)
    {
        std::vector<std::wstring> srcFileList, tarFileList;
        for (auto &filePath : std::filesystem::recursive_directory_iterator(PATH(sourceWorkspace)))
            srcFileList.push_back(std::wstring(filePath.path().wstring().substr(sourceWorkspace.length())));
        for (auto &filePath : std::filesystem::recursive_directory_iterator(PATH(targetWorkspace)))
            tarFileList.push_back(std::wstring(filePath.path().wstring().substr(targetWorkspace.length())));

        std::sort(srcFileList.begin(), srcFileList.end());
        std::sort(tarFileList.begin(), tarFileList.end());

        std::vector<std::wstring> equalFiles;
        std::set_intersection(srcFileList.begin(),srcFileList.end(), tarFileList.begin(), tarFileList.end(), back_inserter(equalFiles));

        RemoveVectorIfContainElements(srcFileList, equalFiles);
        RemoveVectorIfContainElements(tarFileList, equalFiles);

        needToAdd.assign(srcFileList.begin(), srcFileList.end());
        needToDelete.assign(tarFileList.begin(), tarFileList.end());

        for (auto &str : equalFiles) {
            std::wstring srcFile = ConcatPath(sourceWorkspace, str);
            std::wstring tarFile = ConcatPath(targetWorkspace, str);
            if (IsFile(srcFile) && IsFile(tarFile)) {
                if (!IsFileEqual(srcFile, tarFile))
                    needToModify.push_back(std::wstring(str));
            }
        }
    }

    std::wstring GetRegexFromFileFilter(const std::wstring &fileFilter)
    {
        std::wstring patten = GetEscapeString(EscapeStringType::Regex, fileFilter);

        // Excape *
        ReplaceAll(patten, L"\\*", L".*");

        // Excape ?
        ReplaceAll(patten, L"\\?", L".");
        return patten;
    }

	bool IsPathMatchFileFilter(const std::wstring &filePath, const std::wstring &fileFilter)
    {
        return std::regex_match(filePath, std::wregex(GetRegexFromFileFilter(fileFilter)));
    }

    bool IsPathMatchFileFilters(const std::wstring &filePath, const std::vector<std::wstring> &fileFilters)
    {
        std::wstring regexFilter = L"";
        for (auto const &filter : fileFilters) {
            if (!regexFilter.empty())
                regexFilter += L"|";
            regexFilter += GetRegexFromFileFilter(filter);
        }
        return std::regex_match(filePath, std::wregex(regexFilter));
    }

    bool IsDirectoryExists(const std::wstring &path)
    {
        return std::filesystem::exists(path) && std::filesystem::is_directory(path);
    }

    bool IsFile(const std::wstring &path)
    {
        return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
    }

	bool IsFileExists(const std::wstring &path)
    {
        return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
    }

    void ValidateFile(const std::wstring &path)
    {
        if (!std::filesystem::exists(path))
            THROW_EXCEPTION_MSG(ExceptionType::FileNotFound, path + L": File not found.");

        if (!std::filesystem::is_regular_file(path))
            THROW_EXCEPTION_MSG(ExceptionType::FileNotFound, path + L": Path is not a file.");

        if (std::filesystem::is_block_file(path))
            THROW_EXCEPTION_MSG(ExceptionType::FileBlocked, path + L": File is blocked.");
    }

    void ValidateDirectory(const std::wstring &path)
    {
        if (!std::filesystem::exists(path))
            THROW_EXCEPTION_MSG(ExceptionType::FileNotFound, path + L": Directory not found.");

        if (!std::filesystem::is_directory(path))
            THROW_EXCEPTION_MSG(ExceptionType::FileNotFound, path + L": Path is not a directory.");
    }

    bool IsFileEqual(const std::wstring &pathA, const std::wstring &pathB)
    {
        try {
            ValidateFile(pathA);
            ValidateFile(pathB);

            std::ifstream f1(pathA.c_str(), std::ifstream::binary|std::ifstream::ate);
            std::ifstream f2(pathB.c_str(), std::ifstream::binary|std::ifstream::ate);

            if (f1.fail() || f2.fail())
                return false;
            if (f1.tellg() != f2.tellg())
                return false;

            f1.seekg(0, std::ifstream::beg);
            f2.seekg(0, std::ifstream::beg);

            return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                std::istreambuf_iterator<char>(),
                std::istreambuf_iterator<char>(f2.rdbuf()));
        } catch (const std::exception &e) {
            THROW_EXCEPTION_MSG(ExceptionType::FileBlocked, str2wstr(e.what()));
        }
        return false;
    }

    void CopyFile(const std::wstring &srcFilePath, const std::wstring &destFilePath, bool isForce)
    {
        TRY_CATCH(
            ValidateFile(srcFilePath);
            if (isForce && IsFileExists(destFilePath))
                filesystem::remove(destFilePath);
            filesystem::copy(PATH(srcFilePath), PATH(destFilePath));
        )
    }

    void CopyDirectory(const std::wstring &srcDirectory, const std::wstring &destDirectory, const CopyDirectoryOption *option)
    {
        assert(!IsBlank(srcDirectory));
        assert(!IsBlank(destDirectory));
        TRY_CATCH(
            bool isForce = option != nullptr && option->GetIsForce();
            std::vector<std::wstring> srcFileList;
            for (auto &filePath : std::filesystem::recursive_directory_iterator(PATH(srcDirectory))) {
                if (option != nullptr && !option->GetIsRecursive()) {
                    if (filePath.path().parent_path().wstring() != srcDirectory)
                        continue;
                }
                std::wstring relativePath =  GetRelativePath(filePath.path().wstring(), srcDirectory);
                std::wstring destAbsolutePath = ConcatPath(destDirectory, relativePath);

                if (option != nullptr) {
                    if (!option->GetIncludeFileFilters().empty() && !IsPathMatchFileFilters(relativePath, option->GetIncludeFileFilters()))
                        continue;
                    if (!option->GetExcludeFileFilters().empty() && IsPathMatchFileFilters(relativePath, option->GetExcludeFileFilters()))
                        continue;
                }
                if (filePath.is_directory()) {
                    if (!IsDirectoryExists(destAbsolutePath))
                        CreateDirectory(destAbsolutePath);
                } else {
                    if (IsFileExists(destAbsolutePath)) {
                        if (isForce)
                            filesystem::remove(destAbsolutePath);
                        else
                            THROW_EXCEPTION_MSG(ExceptionType::FileAlreadyExist, L"File " + destAbsolutePath + L" already exists.");
                    }
                    std::wstring parentPath = PATH(destAbsolutePath).parent_path().wstring();
                    if (!IsDirectoryExists(parentPath))
                        CreateDirectory(parentPath);
                    CopyFile(filePath.path().wstring(), destAbsolutePath);
                }
            }
        )
    }

    void CreateDirectory(const std::wstring &path)
    {
        if (IsBlank(path) || IsDirectoryExists(path))
            return;

        try
        {
            PATH currentPath(path);
            CreateDirectory(currentPath.parent_path().wstring());
            filesystem::create_directories(path);
        }
        catch(const std::exception& e)
        {
            THROW_EXCEPTION(e);
        }
    }

	void RemoveFile(const std::wstring &filePath)
    {
        if (filePath.empty())
            return;
        try
        {
            if (!IsFileExists(filePath))
                return;
                
            PATH currentPath(filePath);
            filesystem::remove(filePath);
        }
        catch(const std::exception& e)
        {
            THROW_EXCEPTION(e);
        }
    }

    std::wstring ReadFile(const std::wstring &filePath)
    {
        std::wstring result = L"";
        try {
            ValidateFile(filePath);

            std::wifstream fileStream(filePath.c_str(), ios_base::binary);
            if (!fileStream)
                THROW_EXCEPTION_MSG(ExceptionType::FileCannotOpen, L"Cannot Open File " + filePath);
            
            std::wstringstream buffer;
            buffer << fileStream.rdbuf();
            result = buffer.str();
            fileStream.close();
        } catch (const std::exception &e) {
            THROW_EXCEPTION(e);
        }
        return result;
    }

	void ReadFilePerLine(const std::wstring &filePath, std::function<void(std::wstring)> action)
    {
        try {
            ValidateFile(filePath);

            std::wifstream fileStream(filePath.c_str(), ios_base::in);
            std::wstring line;
            while (std::getline(fileStream, line)) {
                action(line);
            }
            fileStream.close();
        } catch (const std::exception &e) {
            THROW_EXCEPTION(e);
        }
    }

    std::wstring ReadFileOneLine(const std::wstring &filePath, int index) 
    {
        try {
            ValidateFile(filePath);

            std::wifstream fileStream(filePath.c_str(), ios_base::in);
            std::wstring line, result;
            int cnt = 0;
            while (std::getline(fileStream, line))
            {
                if (cnt == index) {
                    result = line;
                    if (!result.empty() && result.at(result.length() - 1) == '\r')
                        result.pop_back();
                    break;
                }
                cnt++;
            }
            fileStream.close();
            return result;
        } catch (const std::exception &e) {
            THROW_EXCEPTION(e);
        }
        return L"";
    }

	void WriteFile(const std::wstring &filePath, const std::wstring &content, bool isForce)
    {
        try {
            PATH _filePath(filePath);		
            PATH dir = PATH(_filePath).parent_path();
            if (!IsDirectoryExists(dir.wstring()))
            {
                if (!isForce)
                    THROW_EXCEPTION_MSG(ExceptionType::DirectoryNotFound, dir.wstring() + L"Directory not found.");
                else if (!std::filesystem::create_directories(dir))
                    THROW_EXCEPTION_MSG(ExceptionType::DirectoryCannotCreate, dir.wstring() + L"Directory not found.");
            }

            std::wofstream file(filePath.c_str(), std::ios::out | std::ios::binary);
            if (file.is_open()) {
                file << content;
                file.close();
            } else {
                THROW_EXCEPTION_MSG(ExceptionType::FileBlocked, L"Cannot open file: " + filePath);
            }
        } catch (const std::exception &e) {
            THROW_EXCEPTION(e);
        }
    }

    void AppendFileOneLine(const std::wstring &filePath, const std::wstring &line, bool isForce) 
    {
        try {
            // 1. Check directory exists
            // 2. Check file exists
            // 3. Check file can read write
            // 4. Write file
            PATH _filePath(filePath);		
            PATH dir = PATH(_filePath).parent_path();
            if (!IsDirectoryExists(dir.wstring()))
            {
                if (!isForce)
                    THROW_EXCEPTION_MSG(ExceptionType::DirectoryNotFound, dir.wstring() + L"Directory not found.");
                else if (!std::filesystem::create_directories(dir))
                    THROW_EXCEPTION_MSG(ExceptionType::DirectoryCannotCreate, dir.wstring() + L"Directory not found.");
            }
            std::wofstream fileStream(_filePath, ios_base::app);
            fileStream << line << NL;
            fileStream.close();
        } catch (const std::exception &e) {
            THROW_EXCEPTION(e);
        }
    }
}
