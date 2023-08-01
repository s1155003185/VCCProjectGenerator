#pragma once
#include <filesystem>
#include <fstream>
#include <string>

#include "exception.hpp"
#include "exception_macro.hpp"
#include "exception_type.hpp"
#include "file_constant.hpp"

namespace vcc
{
	inline bool AppendFileSingleLline(PATH filePath, std::wstring line, bool isForce) 
	{
		// 1. Check directory exists
		// 2. Check file exists
		// 3. Check file can read write
		// 4. Write file
		
		PATH dir = filePath.parent_path();
		if (!std::filesystem::is_directory(dir))
		{
			if (!isForce)
				THROW_EXCEPTION(ExceptionType::DIRECTORY_NOT_FOUND, dir.wstring() + L"Directory not found.");
			else if (!std::filesystem::create_directories(dir))
				THROW_EXCEPTION(ExceptionType::DIRECTORY_CANNOT_CREATE, dir.wstring() + L"Directory not found.");
		}
		if (!std::filesystem::exists(filePath))
		{
			if (!isForce)
				THROW_EXCEPTION(ExceptionType::FILE_NOT_FOUND, filePath.wstring() + L": File not found.");
		}
		std::wofstream fileStream(filePath, ios_base::app);
		fileStream << line << NL;
		fileStream.close();
		return true;
	}

	inline std::wstring ReadFileSingleLline(PATH filePath, int index) 
	{
		// 1. Check directory exists
		// 2. Check file exists
		// 3. Check file can read
		// 4. Read file
		
		PATH dir = filePath.parent_path();
		if (!std::filesystem::is_directory(dir))
			THROW_EXCEPTION(ExceptionType::DIRECTORY_NOT_FOUND, dir.wstring() + L": Directory not found.");
		if (!std::filesystem::exists(filePath))
			THROW_EXCEPTION(ExceptionType::FILE_NOT_FOUND, filePath.wstring() + L": File not found.");
		
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
	}

	inline std::wstring PathConcat(std::wstring directory, std::wstring addition)
	{
		PATH dir(directory);
		PATH add(addition);
		dir /= add;
		return dir.wstring();
	}
}
