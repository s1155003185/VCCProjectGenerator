#pragma once

enum class ExceptionType
{
	NO_ERROR = 0,
    // vccobj - begin
	// Directory
	DIRECTORY_NOT_FOUND,
	DIRECTORY_CANNOT_CREATE,
    // File
	FILE_NOT_FOUND,
	FILE_IS_BLOCKED,
	// parser
	READER_ERROR,
    // vccobj - end
    CUSTOM_ERROR = 9999,
};