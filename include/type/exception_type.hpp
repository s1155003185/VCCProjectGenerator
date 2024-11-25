// <vcc:vccproj sync="FULL" gen="FULL"/>
#pragma once

// TODO: need review
enum class ExceptionType
{
	NoError = 0
	// Coding error
	, NotSupport
	, NotImplement
	// System
	, DllError

	// Directory
	, DirectoryNotFound
	, DirectoryCannotCreate
    // File
	, FileNotFound
	, FileBlocked
	, FileAlreadyExist
	, FileCannotOpen
	// parser
	, ParserError
	// key
	, KeyInvalid
	, KeyNotFound
	, KeyDuplicated

	, SystemError = 50000
    // <vcc:customExceptionTypes sync="RESERVE" gen="RESERVE">
    // </vcc:customExceptionTypes>
    , CustomError = 99999
};