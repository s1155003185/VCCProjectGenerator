#pragma once

#include <string>

namespace vcc
{
    #define PATH std::filesystem::path

    // helper
	std::wstring ConcatPath(std::wstring directory, std::wstring addition);
	void GetFileDifferenceBetweenWorkspaces(std::wstring sourceWorkspace, std::wstring targetWorkspace,
		std::vector<std::wstring> &needToAdd, std::vector<std::wstring> &needToDelete, std::vector<std::wstring> &needToModify);

	// validation
	bool IsDirectoryExists(const std::wstring &path);
	
	bool IsFile(const std::wstring &path);
    void ValidateFile(const std::wstring &path);
	bool IsFileEqual(const std::wstring &pathA, const std::wstring &pathB);

	// action
	void CreateDirectory(const std::wstring &path);
	std::wstring ReadFileSingleLine(std::wstring filePath, int index);
	void AppendFileSingleLine(std::wstring filePath, std::wstring line, bool isForce);
}
