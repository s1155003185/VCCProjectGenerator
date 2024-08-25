#pragma once

// TODO: need review
enum class ExceptionType
{
	NoError = 0,
	// Coding error
	NotSupport,
	NotImplement,
	// System
	DllError,

	// Directory
	DirectoryNotFound,
	DirectoryCannotCreate,
    // File
	FileNotFound,
	FileBlocked,
	FileAlreadyExist,
	FileCannotOpen,
	// parser
	ParserError,
	// key
	KeyInvalid,
	KeyNotFound,
	KeyDuplicated,

	SystemError = 50000,
	// <vcc:exceptionType action:"RESERVE">
	// </vcc:exceptionType>
    CustomError = 99999
};