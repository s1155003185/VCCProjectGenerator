#include "file_helper.hpp"

#include <filesystem>
#include <fstream>
#include <regex>
#include <string>

#include "exception.hpp"
#include "exception_macro.hpp"
#include "exception_type.hpp"
#include "vector_helper.hpp"

using namespace std;

namespace vcc
{
    std::wstring ConcatPath(std::wstring directory, std::wstring addition)
    {
        if (addition.starts_with(L"/") || addition.starts_with(L"\\"))
            addition.erase(0, 1);

        PATH dir(directory);
        PATH add(addition);
        dir /= add;
        return dir.wstring();
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
            THROW_EXCEPTION_M(ExceptionType::FILE_NOT_FOUND, path + L": File not found.");

        if (!std::filesystem::is_regular_file(path))
            THROW_EXCEPTION_M(ExceptionType::FILE_NOT_FOUND, path + L": Path is not a file.");

        if (std::filesystem::is_block_file(path))
            THROW_EXCEPTION_M(ExceptionType::FILE_IS_BLOCKED, path + L": File is blocked.");
    }

    bool IsFileEqual(const std::wstring &pathA, const std::wstring &pathB)
    {
        try {
            ValidateFile(pathA);
            ValidateFile(pathB);

            std::ifstream f1(pathA, std::ifstream::binary|std::ifstream::ate);
            std::ifstream f2(pathB, std::ifstream::binary|std::ifstream::ate);

            if (f1.fail() || f2.fail())
                return false;
            if (f1.tellg() != f2.tellg())
                return false;

            f1.seekg(0, std::ifstream::beg);
            f2.seekg(0, std::ifstream::beg);

            return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                std::istreambuf_iterator<char>(),
                std::istreambuf_iterator<char>(f2.rdbuf()));
        } catch (std::exception &ex) {
            THROW_EXCEPTION_M(ExceptionType::FILE_IS_BLOCKED, str2wstr(ex.what()));
        }
        return false;
    }

    void CreateDirectory(const std::wstring &path)
    {
        if (path.empty() || IsDirectoryExists(path))
            return;

        try
        {
            PATH currentPath(path);
            CreateDirectory(currentPath.parent_path().wstring());
            filesystem::create_directories(path);
        }
        catch(std::exception& e)
        {
            THROW_EXCEPTION(e);
        }
    }

    std::wstring ReadFileOneLine(std::wstring filePath, int index) 
    {
        try {
            ValidateFile(filePath);

            std::wifstream fileStream(filePath, ios_base::in);
            std::wstring line, result;
            int cnt = 0;
            while (std::getline(fileStream, line))
            {
                if (cnt == index) {
                    result = line;
                    break;
                }
                cnt++;
            }
            fileStream.close();
            return result;
        } catch (std::exception &e) {
            THROW_EXCEPTION(e);
        }
        return L"";
    }

    void AppendFileOneLine(std::wstring filePath, std::wstring line, bool isForce) 
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
                    THROW_EXCEPTION_M(ExceptionType::DIRECTORY_NOT_FOUND, dir.wstring() + L"Directory not found.");
                else if (!std::filesystem::create_directories(dir))
                    THROW_EXCEPTION_M(ExceptionType::DIRECTORY_CANNOT_CREATE, dir.wstring() + L"Directory not found.");
            }
            if (!std::filesystem::exists(_filePath))
            {
                if (!isForce)
                    THROW_EXCEPTION_M(ExceptionType::FILE_NOT_FOUND, _filePath.wstring() + L": File not found.");
            }
            std::wofstream fileStream(_filePath, ios_base::app);
            fileStream << line << NL;
            fileStream.close();
        } catch (std::exception &e) {
            THROW_EXCEPTION(e);
        }
    }
}
