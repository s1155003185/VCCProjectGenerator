#pragma once

enum class ExceptionType
{
	NoError = 0,
    // vccobj - begin
	// Directory
	DirectoryNotFound,
	DirectoryCannotCreate,
    // File
	FileNotFound,
	FileIsBlocked,
	FileCannotOpen,
	// parser
	ReaderError,
    // vccobj - end
    CustomError = 9999,
};