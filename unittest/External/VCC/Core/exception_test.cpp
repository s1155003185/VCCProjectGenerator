#include <gtest/gtest.h>
#include <exception>

#include "base_exception.hpp"
#include "i_exception.hpp"
#include "exception.hpp"
#include "exception_helper.hpp"
#include "exception_macro.hpp"
#include "exception_type.hpp"
#include "file_helper.hpp"
#include "log_property.hpp"
#include "string_helper.hpp"

using namespace std;
using namespace vcc;

TEST(ExceptionTest, General)
{
    bool throwException = false;
    try 
    {
        throw Exception(ExceptionType::FILE_NOT_FOUND, L"FILE_NOT_FOUND");
    } 
    catch (exception &ex) 
    {
        throwException = true;

        IException *iex = dynamic_cast<IException *>(&ex);
        EXPECT_TRUE(iex != nullptr);
        EXPECT_EQ(iex->GetErrorMessage(), L"FILE_NOT_FOUND");
        string whatStr(iex->what());
        EXPECT_EQ(whatStr, "FILE_NOT_FOUND");
    }
    EXPECT_TRUE(throwException);
}

TEST(ExceptionTest, Full)
{
    PATH filePath = std::filesystem::current_path() / "bin/Debug/AppLogs/ExceptionTest.log";
    remove(filePath);
    
    LogProperty logProperty;
    logProperty.SetIsConsoleLog(false);
    logProperty.SetFilePath(filePath);

    bool throwException = false;
    try
    {
        THROW_EXCEPTION(ExceptionType::FILE_NOT_FOUND, L"FILE_NOT_FOUND");
    }
    catch(exception &ex)
    {
        throwException = true;
        HandleException(logProperty, &ex);
    }
    EXPECT_TRUE(throwException);
}
