#include <gtest/gtest.h>
#include <exception>
#include <filesystem>

#include "base_exception.hpp"
#include "i_exception.hpp"
#include "exception.hpp"
#include "exception_macro.hpp"
#include "exception_type.hpp"
#include "file_helper.hpp"
#include "log_config.hpp"
#include "string_helper.hpp"

TEST(ExceptionMacroTest, General)
{
    bool throwException = false;
    try 
    {
        throw vcc::Exception(ExceptionType::FileNotFound, L"FILE_NOT_FOUND");
    } 
    catch (std::exception &ex) 
    {
        throwException = true;

        vcc::IException *iex = dynamic_cast<vcc::IException *>(&ex);
        EXPECT_TRUE(iex != nullptr);
        EXPECT_EQ(iex->GetErrorMessage(), L"FILE_NOT_FOUND");
        std::string whatStr(iex->what());
        EXPECT_EQ(whatStr, "FILE_NOT_FOUND");
    }
    EXPECT_TRUE(throwException);
}

TEST(ExceptionMacroTest, Full)
{
    bool throwException = false;
    try
    {
        THROW_EXCEPTION_MSG(ExceptionType::FileNotFound, L"FILE_NOT_FOUND");
    }
    catch(std::exception &ex)
    {
        throwException = true;
    }
    EXPECT_TRUE(throwException);
}
