#pragma once

// TODO: need review
enum class ExceptionType
{
	NoError = 0,
	// Coding error
	NotSupport,
	NotImplement,


	SystemError = 50000,
	// <vcc:exceptionType action:"ALERT">
	// Directory
	DirectoryNotFound,
	DirectoryCannotCreate,
    // File
	FileNotFound,
	FileIsBlocked,
	FileCannotOpen,
	// parser
	ReaderError,
	// </vcc:exceptionType>

    CustomError = 99999
};