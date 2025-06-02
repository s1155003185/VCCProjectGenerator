#pragma once

#include <functional>
#include <string>
#include <vector>

#include "base_object.hpp"
#include "class_macro.hpp"

#define PATH std::filesystem::path

namespace vcc
{
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
		LocalDocuments,
		// ProgramFiles,
		// ProgramFilesCommon,
		// System,
		UserHome
		// Windows
	};

	class CopyDirectoryOption : public BaseObject
	{
		VECTOR(std::wstring, IncludeFileFilters);
		VECTOR(std::wstring, ExcludeFileFilters);
		GETSET(bool, IsRecursive, false);
		GETSET(bool, IsForce, false);

		public:
			CopyDirectoryOption() = default;
			virtual ~CopyDirectoryOption() {}

			virtual std::shared_ptr<IObject> Clone() const override
            {
                return std::make_shared<CopyDirectoryOption>(*this);
            }
	};

	// system
	std::wstring GetCurrentFolderPath();
	void ChangeCurrentFolderPath(const std::wstring &path);
	std::wstring GetSystemFolderPath(const SystemFolderType &fileType);

    // helper
	bool IsAbsolutePath(const std::wstring &filePath);
	std::wstring GetParentPath(const std::wstring &filePath);
	std::wstring GetFileName(const std::wstring &filePath);
    std::wstring ConcatPaths(const std::vector<std::wstring>& paths);
	std::wstring GetRelativePath(const std::wstring &absolutePath, const std::wstring &basePath);
	void GetFileDifferenceBetweenWorkspaces(const std::wstring &sourceWorkspace, const std::wstring &targetWorkspace,
		std::vector<std::wstring> &needToAdd, std::vector<std::wstring> &needToModify, std::vector<std::wstring> &needToDelete);

	// converter
	std::wstring GetCurrentPlatformPath(const std::wstring &path);

	// Regex
	std::wstring GetWindowPath(const std::wstring &path);
	std::wstring GetLinuxPath(const std::wstring &path);
	std::wstring GetRegexFromFileFilter(const std::wstring &fileFilter);
	bool IsPathMatchFileFilter(const std::wstring &filePath, const std::wstring &fileFilter);
	bool IsPathMatchFileFilters(const std::wstring &filePath, const std::vector<std::wstring> &fileFilters);

	// validation
	bool IsDirectoryExists(const std::wstring &path);
	
	bool IsFile(const std::wstring &path);
	bool IsFilePresent(const std::wstring &path);
    void ValidateFile(const std::wstring &path);
	bool IsFileEqual(const std::wstring &pathA, const std::wstring &pathB);

	// action
	void CopyFile(const std::wstring &srcFilePath, const std::wstring &destFilePath, const bool &isForce = false);
	void RemoveFile(const std::wstring &filePath);
	void CreateDirectory(const std::wstring &path);
	void CopyDirectory(const std::wstring &srcDirectory, const std::wstring &destDirectory, const CopyDirectoryOption *option = nullptr);
	void RemoveDirectory(const std::wstring &directory);

	// Read File
	std::wstring ReadFile(const std::wstring &filePath);
	void ReadFilePerLine(const std::wstring &filePath, std::function<void(std::wstring)> action);
	std::wstring ReadFileOneLine(const std::wstring &filePath, int index);

	// Write file
	void WriteFile(const std::wstring &filePath, const std::wstring &content, const bool &isForce = false);
	void AppendFileOneLine(const std::wstring &filePath, const std::wstring &line, const bool &isForce = false);

}
