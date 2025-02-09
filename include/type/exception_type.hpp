// <vcc:vccproj sync="FULL" gen="FULL"/>
#pragma once

// TODO: need review
enum class ExceptionType
{
	NoError = 0
	// ----------------------------------------------------------------------------------------------------
	// Implement - Coding misstake
	// ----------------------------------------------------------------------------------------------------
	// Coding
	, NotSupport
	, NotImplement
	// Validation
	, ArgumentNotValid
	// ----------------------------------------------------------------------------------------------------
	// Exe - Binary cannot be executed
	// ----------------------------------------------------------------------------------------------------
	, DllError
	// ----------------------------------------------------------------------------------------------------
	// Runtime
	// ----------------------------------------------------------------------------------------------------
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
	// System Erorr End
	, SystemError = 50000
	// ----------------------------------------------------------------------------------------------------
	// User Defined
	// ----------------------------------------------------------------------------------------------------
    // <vcc:customExceptionTypes sync="RESERVE" gen="RESERVE">
    // </vcc:customExceptionTypes>
    , CustomError = 99999
};