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

namespace vcc
{
    std::wstring getCurrentFolderPath()
    {
        return std::filesystem::current_path().wstring();
    }

	std::wstring getSystemFolderPath(const SystemFolderType &fileType)
    {
        #ifdef __WIN32
        return getSystemFolderPathWindow(fileType);
        #else
        return getSystemFolderPathLinux(fileType);
        #endif
    }
    
	bool isAbsolutePath(const std::wstring &filePath)
    {
        TRY
            return PATH(filePath).is_absolute();
        CATCH
        return false;
    }

    std::wstring getParentPath(const std::wstring &filePath)
    {
        std::wstring parentPath = L"";
        TRY
            parentPath = PATH(filePath).parent_path().wstring();
        CATCH
        return parentPath;
    }

    std::wstring getFileName(const std::wstring &filePath)
    {
        std::wstring fileName = L"";
        TRY
            fileName = PATH(filePath).filename().wstring();
        CATCH
        return fileName;
    }

    std::wstring concatPaths(const std::vector<std::wstring>& paths)
    {
        if (paths.empty())
            return L"";

        PATH result(L"");
        for (auto const &path : paths) {
            if (result.wstring().empty())
                result = PATH(path);
            else {
                result /= PATH(path);
            }
        }
        #ifdef __WIN32
        return getWindowPath(result.wstring());
        #else
        return getLinuxPath(result.wstring());
        #endif
    }

    std::wstring getRelativePath(const std::wstring &absolutePath, const std::wstring &basePath)
    {
        TRY
            if (isBlank(basePath))
                return absolutePath;
            return PATH(absolutePath).lexically_relative(PATH(basePath)).wstring();
        CATCH
        return L"";
    }

    void getFileDifferenceBetweenWorkspaces(const std::wstring &sourceWorkspace, const std::wstring &targetWorkspace, 
        std::vector<std::wstring> &needToAdd, std::vector<std::wstring> &needToModify, std::vector<std::wstring> &needToDelete)
    {
        TRY
            std::vector<std::wstring> srcFileList, tarFileList;
            TRY
                for (auto &filePath : std::filesystem::recursive_directory_iterator(PATH(!sourceWorkspace.empty() ? sourceWorkspace : L".")))
                    srcFileList.push_back(getRelativePath(filePath.path().wstring(), sourceWorkspace));
            CATCH
            TRY
                for (auto &filePath : std::filesystem::recursive_directory_iterator(PATH(!targetWorkspace.empty() ? targetWorkspace : L".")))
                    tarFileList.push_back(getRelativePath(filePath.path().wstring(),targetWorkspace));
            CATCH

            std::sort(srcFileList.begin(), srcFileList.end());
            std::sort(tarFileList.begin(), tarFileList.end());

            std::vector<std::wstring> equalFiles;
            std::set_intersection(srcFileList.begin(),srcFileList.end(), tarFileList.begin(), tarFileList.end(), back_inserter(equalFiles));

            remove(srcFileList, equalFiles);
            remove(tarFileList, equalFiles);

            needToAdd.assign(srcFileList.begin(), srcFileList.end());
            needToDelete.assign(tarFileList.begin(), tarFileList.end());

            for (auto &str : equalFiles) {
                std::wstring srcFile = concatPaths({sourceWorkspace, str});
                std::wstring tarFile = concatPaths({targetWorkspace, str});
                if (isFile(srcFile) && isFile(tarFile)) {
                    if (!isFileEqual(srcFile, tarFile))
                        needToModify.push_back(std::wstring(str));
                }
            }
        CATCH
    }

    std::wstring getWindowPath(const std::wstring &path)
    {
        std::wstring result = path;
        replaceAll(result, L"/", L"\\");
        return result;
    }

    std::wstring getLinuxPath(const std::wstring &path)
    {
        std::wstring result = path;
        replaceAll(result, L"\\", L"/");
        return result;
    }

    std::wstring getCurrentPlatformPath(const std::wstring &path)
    {
        #ifdef __WIN32
        getWindowPath(path);
        #else
        getLinuxPath(path);
        #endif
        return path;
    }

    std::wstring getRegexFromFileFilter(const std::wstring &fileFilter)
    {
        std::wstring patten = getEscapeString(EscapeStringType::Regex, fileFilter);

        // Excape *
        replaceAll(patten, L"\\*", L".*");

        // Excape ?
        replaceAll(patten, L"\\?", L".");
        return patten;
    }

	bool isPathMatchFileFilter(const std::wstring &filePath, const std::wstring &fileFilter)
    {
        TRY
            return std::regex_match(getLinuxPath(filePath), std::wregex(getRegexFromFileFilter(getLinuxPath(fileFilter))));
        CATCH
        return false;
    }

    bool isPathMatchFileFilters(const std::wstring &filePath, const std::vector<std::wstring> &fileFilters)
    {
        if (fileFilters.empty())
            return false;
        TRY
            std::vector<std::wstring> regexFilters;
            for (auto const &str: fileFilters) {
                regexFilters.push_back(getRegexFromFileFilter(getLinuxPath(str)));
            }
            return std::regex_match(getLinuxPath(filePath), std::wregex(concat(regexFilters, L"|")));
        CATCH
        return false;
    }

    bool isDirectoryExists(const std::wstring &path)
    {
        return std::filesystem::exists(path) && std::filesystem::is_directory(path);
    }

    bool isFile(const std::wstring &path)
    {
        return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
    }

	bool isFilePresent(const std::wstring &path)
    {
        return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
    }

    void validateFile(const std::wstring &path)
    {
        if (!std::filesystem::exists(path))
            THROW_EXCEPTION_MSG(ExceptionType::FileNotFound, path + L": File not found.");

        if (!std::filesystem::is_regular_file(path))
            THROW_EXCEPTION_MSG(ExceptionType::FileNotFound, path + L": Path is not a file.");

        if (std::filesystem::is_block_file(path))
            THROW_EXCEPTION_MSG(ExceptionType::FileBlocked, path + L": File is blocked.");
    }

    void validateDirectory(const std::wstring &path)
    {
        if (!std::filesystem::exists(path))
            THROW_EXCEPTION_MSG(ExceptionType::FileNotFound, path + L": Directory not found.");

        if (!std::filesystem::is_directory(path))
            THROW_EXCEPTION_MSG(ExceptionType::FileNotFound, path + L": Path is not a directory.");
    }

    bool isFileEqual(const std::wstring &pathA, const std::wstring &pathB)
    {
        try {
            validateFile(pathA);
            validateFile(pathB);

            std::ifstream f1(PATH(pathA), std::ifstream::binary|std::ifstream::ate);
            std::ifstream f2(PATH(pathB), std::ifstream::binary|std::ifstream::ate);

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

    void copyFile(const std::wstring &srcFilePath, const std::wstring &destFilePath, const bool &isForce)
    {
        TRY
            validateFile(srcFilePath);
            if (isForce)
                createDirectory(PATH(destFilePath).parent_path().wstring());
            if (isForce && isFilePresent(destFilePath))
                std::filesystem::remove(destFilePath);
            std::filesystem::copy(PATH(srcFilePath), PATH(destFilePath));
        CATCH
    }

	void removeFile(const std::wstring &filePath)
    {
        if (filePath.empty())
            return;
        TRY
            if (!isFilePresent(filePath))
                return;
                
            PATH currentPath(filePath);
            std::filesystem::remove(filePath);
        CATCH
    }

    void createDirectory(const std::wstring &path)
    {
        if (isBlank(path) || isDirectoryExists(path))
            return;

        TRY
            PATH currentPath(path);
            createDirectory(currentPath.parent_path().wstring());
            std::filesystem::create_directories(path);
        CATCH
    }

    void copyDirectory(const std::wstring &srcDirectory, const std::wstring &destDirectory, const CopyDirectoryOption *option)
    {
        assert(!isBlank(srcDirectory));
        assert(!isBlank(destDirectory));
        TRY
            bool isForce = option != nullptr && option->getIsForce();
            std::vector<std::wstring> srcFileList;
            for (auto &filePath : std::filesystem::recursive_directory_iterator(PATH(srcDirectory))) {
                if (option != nullptr && !option->getIsRecursive()) {
                    if (filePath.path().parent_path().wstring() != srcDirectory)
                        continue;
                }
                std::wstring relativePath =  getRelativePath(filePath.path().wstring(), srcDirectory);
                if (filePath.is_directory() && !(relativePath.ends_with(L"/") || relativePath.ends_with(L"\\")))
                    relativePath += L"/";
                std::wstring destAbsolutePath = concatPaths({destDirectory, relativePath});
                if (filePath.is_directory() && !(destAbsolutePath.ends_with(L"/") || destAbsolutePath.ends_with(L"\\")))
                    destAbsolutePath += L"/";
                if (option != nullptr) {
                    if (!option->getIncludeFileFilters().empty() && !isPathMatchFileFilters(relativePath, option->getIncludeFileFilters()))
                        continue;
                    if (!option->getExcludeFileFilters().empty() && isPathMatchFileFilters(relativePath, option->getExcludeFileFilters()))
                        continue;
                }
                if (filePath.is_directory()) {
                    if (!isDirectoryExists(destAbsolutePath))
                        createDirectory(destAbsolutePath);
                } else {
                    if (isFilePresent(destAbsolutePath)) {
                        if (isForce)
                            std::filesystem::remove(destAbsolutePath);
                        else
                            THROW_EXCEPTION_MSG(ExceptionType::FileAlreadyExist, L"File " + destAbsolutePath + L" already exists.");
                    }
                    std::wstring parentPath = PATH(destAbsolutePath).parent_path().wstring();
                    if (!isDirectoryExists(parentPath))
                        createDirectory(parentPath);
                    copyFile(filePath.path().wstring(), destAbsolutePath);
                }
            }
        CATCH
    }

	void removeDirectory(const std::wstring &directory)
    {
        assert(!isBlank(directory));
        TRY
            std::filesystem::remove_all(PATH(directory));
        CATCH
    }

    std::wstring readFile(const std::wstring &filePath)
    {
        std::wstring result = L"";
        TRY
            validateFile(filePath);

            std::wifstream fileStream(PATH(filePath), std::ios_base::binary);
            if (!fileStream)
                THROW_EXCEPTION_MSG(ExceptionType::FileCannotOpen, L"Cannot Open File " + filePath);
            
            std::wstringstream buffer;
            buffer << fileStream.rdbuf();
            result = buffer.str();
            fileStream.close();
        CATCH
        return result;
    }

	void readFilePerLine(const std::wstring &filePath, std::function<void(std::wstring)> action)
    {
        TRY
            validateFile(filePath);

            std::wifstream fileStream(PATH(filePath), std::ios_base::in);
            std::wstring line;
            while (std::getline(fileStream, line)) {
                action(line);
            }
            fileStream.close();
        CATCH
    }

    std::wstring readFileOneLine(const std::wstring &filePath, int index) 
    {
        TRY
            validateFile(filePath);

            std::wifstream fileStream(PATH(filePath), std::ios_base::in);
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
        CATCH
        return L"";
    }

	void writeFile(const std::wstring &filePath, const std::wstring &content, const bool &isForce)
    {
        TRY
            PATH _filePath(filePath);		
            PATH dir = _filePath.parent_path();
            if (!dir.wstring().empty() && !isDirectoryExists(dir.wstring()))
            {
                if (!isForce)
                    THROW_EXCEPTION_MSG(ExceptionType::DirectoryNotFound, dir.wstring() + L"Directory not found.");
                else if (!std::filesystem::create_directories(dir))
                    THROW_EXCEPTION_MSG(ExceptionType::DirectoryCannotCreate, dir.wstring() + L"Directory not found.");
            }

            std::wofstream file(PATH(filePath), std::ios::out | std::ios::binary);
            if (file.is_open()) {
                file << content;
                file.close();
            } else {
                THROW_EXCEPTION_MSG(ExceptionType::FileBlocked, L"Cannot open file: " + filePath);
            }
        CATCH
    }

    void appendFileOneLine(const std::wstring &filePath, const std::wstring &line, const bool &isForce) 
    {
        TRY
            // 1. Check directory exists
            // 2. Check file exists
            // 3. Check file can read write
            // 4. Write file
            PATH _filePath(filePath);		
            PATH dir = PATH(_filePath).parent_path();
            if (!isDirectoryExists(dir.wstring()))
            {
                if (!isForce)
                    THROW_EXCEPTION_MSG(ExceptionType::DirectoryNotFound, dir.wstring() + L"Directory not found.");
                else if (!std::filesystem::create_directories(dir))
                    THROW_EXCEPTION_MSG(ExceptionType::DirectoryCannotCreate, dir.wstring() + L"Directory not found.");
            }
            std::wofstream fileStream(_filePath, std::ios_base::app);
            #ifdef __WIN32
            fileStream << line + L"\n";
            #else
            fileStream << line + L"\r\n";
            #endif
            fileStream.close();
        CATCH
    }
}
