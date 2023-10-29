#pragma once

#include <string>

namespace vcc
{
    #define PATH std::filesystem::path

    // helper
	std::wstring ConcatPath(std::wstring directory, std::wstring addition);
	void GetFileDifferenceBetweenWorkspaces(std::wstring sourceWorkspace, std::wstring targetWorkspace,
		std::vector<std::wstring> &needToAdd, std::vector<std::wstring> &needToModify, std::vector<std::wstring> &needToDelete);

	// validation
	bool IsDirectoryExists(const std::wstring &path);
	
	bool IsFile(const std::wstring &path);
	bool IsFileExists(const std::wstring &path);
    void ValidateFile(const std::wstring &path);
	bool IsFileEqual(const std::wstring &pathA, const std::wstring &pathB);

	// action
	void CreateDirectory(const std::wstring &path);	
	std::wstring ReadFile(const std::wstring &filePath);
	void ReadFilePerLine(const std::wstring &filePath, std::function<void(std::wstring)> action);
	std::wstring ReadFileOneLine(const std::wstring &filePath, int index);
	void AppendFileOneLine(const std::wstring &filePath, const std::wstring &line, bool isForce);
}
