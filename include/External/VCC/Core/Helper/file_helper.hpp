#pragma once

#include <functional>
#include <string>
#include <vector>

#include "base_object.hpp"
#include "class_macro.hpp"

namespace vcc
{
    #define PATH std::filesystem::path

	// TODO: Support folder type
	enum class SystemFolderType
	{
		// AdminTools,
		// AppData,
		// CommonAdminTools,
		// CommonAppdata,
		// CommonDocuments,
		// Cookies,
		// FlagCreate,
		// FlagDontVerify,
		// History,
		// InternetCache,
		// LocalAppData,
		// LocalPictures,
		LocalDocuments
		// ProgramFiles,
		// ProgramFilesCommon,
		// System,
		// Windows
	};

	class CopyDirectoryOption : public BaseObject<CopyDirectoryOption>
	{
		VECTOR(std::wstring, IncludeFileFilters);
		VECTOR(std::wstring, ExcludeFileFilters);
		GETSET(bool, IsRecursive, false);
		GETSET(bool, IsForce, false);

		public:
			CopyDirectoryOption() = default;
			virtual ~CopyDirectoryOption() {}
	};

	// system
	std::wstring GetSystemFolderPath(SystemFolderType fileType);

    // helper
	std::wstring GetFileName(const std::wstring &filePath);
	std::wstring ConcatPath(std::wstring directory, std::wstring addition);
	std::wstring GetRelativePath(const std::wstring &absolutePath, const std::wstring &basePath);
	void GetFileDifferenceBetweenWorkspaces(std::wstring sourceWorkspace, std::wstring targetWorkspace,
		std::vector<std::wstring> &needToAdd, std::vector<std::wstring> &needToModify, std::vector<std::wstring> &needToDelete);

	// Regex
	std::wstring GetRegexFromFileFilter(const std::wstring &fileFilter);
	bool IsPathMatchFileFilter(const std::wstring &filePath, const std::wstring &fileFilter);
	bool IsPathMatchFileFilters(const std::wstring &filePath, const std::vector<std::wstring> &fileFilters);

	// validation
	bool IsDirectoryExists(const std::wstring &path);
	
	bool IsFile(const std::wstring &path);
	bool IsFileExists(const std::wstring &path);
    void ValidateFile(const std::wstring &path);
	bool IsFileEqual(const std::wstring &pathA, const std::wstring &pathB);

	// action
	void CreateDirectory(const std::wstring &path);
	void CopyFile(const std::wstring &srcFilePath, const std::wstring &destFilePath, bool isForce = false);
	void CopyDirectory(const std::wstring &srcDirectory, const std::wstring &destDirectory, const CopyDirectoryOption *option = nullptr);
	void RemoveFile(const std::wstring &filePath);

	// Read File
	std::wstring ReadFile(const std::wstring &filePath);
	void ReadFilePerLine(const std::wstring &filePath, std::function<void(std::wstring)> action);
	std::wstring ReadFileOneLine(const std::wstring &filePath, int index);

	// Write file
	void WriteFile(const std::wstring &filePath, const std::wstring &content, bool isForce = false);
	void AppendFileOneLine(const std::wstring &filePath, const std::wstring &line, bool isForce = false);

}
