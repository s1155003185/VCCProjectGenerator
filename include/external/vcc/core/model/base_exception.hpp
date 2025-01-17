#pragma once
#include "i_exception.hpp"

#include <cstdlib>
#include <cstring>
#include <string>

#include "exception_type.hpp"
#include "string_helper.hpp"

namespace vcc
{
    class BaseException : public IException
    {
        private:
            mutable ExceptionType _Type = ExceptionType::CustomError;
            mutable std::wstring _Message = L"";
        protected:
            BaseException() : BaseException(ExceptionType::CustomError) {}
            BaseException(ExceptionType type) : BaseException(type, L"") {}
            BaseException(ExceptionType type, std::wstring message)
            {
                this->_Type = type;
                this->_Message = message;
            }
            virtual ~BaseException() {}
        
        public:
            virtual ExceptionType GetErrorType() const override { return this->_Type; }
            virtual int64_t GetErrorCode() const  override { return static_cast<int64_t>(this->_Type); }
            virtual std::wstring GetErrorMessage() const override { return this->_Message; }
            virtual void SetErrorMessage(const std::wstring &message) const override { this->_Message = std::wstring(message); }

            virtual const char* what() const throw () override
            {
                std::string errMsg = wstr2str(this->_Message);
                char *cstr = new char[strlen(errMsg.c_str()) + 1];
                strcpy(cstr, errMsg.c_str());
                return cstr;
            }

    };
}
