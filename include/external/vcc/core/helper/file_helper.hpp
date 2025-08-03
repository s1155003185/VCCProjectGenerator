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

			virtual std::shared_ptr<IObject> clone() const override
            {
                return std::make_shared<CopyDirectoryOption>(*this);
            }
	};

	// system
	std::wstring getCurrentFolderPath();
	void changeCurrentFolderPath(const std::wstring &path);
	std::wstring getSystemFolderPath(const SystemFolderType &fileType);

    // helper
	bool isAbsolutePath(const std::wstring &filePath);
	std::wstring getParentPath(const std::wstring &filePath);
	std::wstring getFileName(const std::wstring &filePath);
    std::wstring concatPaths(const std::vector<std::wstring>& paths);
	std::wstring getRelativePath(const std::wstring &absolutePath, const std::wstring &basePath);
	void getFileDifferenceBetweenWorkspaces(const std::wstring &sourceWorkspace, const std::wstring &targetWorkspace,
		std::vector<std::wstring> &needToAdd, std::vector<std::wstring> &needToModify, std::vector<std::wstring> &needToDelete);

	// converter
	std::wstring getCurrentPlatformPath(const std::wstring &path);

	// Regex
	std::wstring getWindowPath(const std::wstring &path);
	std::wstring getLinuxPath(const std::wstring &path);
	std::wstring getRegexFromFileFilter(const std::wstring &fileFilter);
	bool isPathMatchFileFilter(const std::wstring &filePath, const std::wstring &fileFilter);
	bool isPathMatchFileFilters(const std::wstring &filePath, const std::vector<std::wstring> &fileFilters);

	// validation
	bool isDirectoryExists(const std::wstring &path);
	
	bool isFile(const std::wstring &path);
	bool isFilePresent(const std::wstring &path);
    void validateFile(const std::wstring &path);
	bool isFileEqual(const std::wstring &pathA, const std::wstring &pathB);

	// action
	void copyFile(const std::wstring &srcFilePath, const std::wstring &destFilePath, const bool &isForce = false);
	void removeFile(const std::wstring &filePath);
	void createDirectory(const std::wstring &path);
	void copyDirectory(const std::wstring &srcDirectory, const std::wstring &destDirectory, const CopyDirectoryOption *option = nullptr);
	void removeDirectory(const std::wstring &directory);

	// Read File
	std::wstring readFile(const std::wstring &filePath);
	void readFilePerLine(const std::wstring &filePath, std::function<void(std::wstring)> action);
	std::wstring readFileOneLine(const std::wstring &filePath, int index);

	// Write file
	void writeFile(const std::wstring &filePath, const std::wstring &content, const bool &isForce = false);
	void appendFileOneLine(const std::wstring &filePath, const std::wstring &line, const bool &isForce = false);

}
