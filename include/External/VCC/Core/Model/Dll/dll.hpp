#pragma once

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <string>

#include "exception_macro.hpp"
#include "string_helper.hpp"

namespace vcc
{
    class Dll
    {
        private:
            void* h; // win version: HINSTANCE h

        public:
            Dll(const std::wstring &filename) : h(
                    #ifdef _WIN32
                    LoadLibrary(wstr2str(filename).c_str())
                    #else
                    dlopen(wstr2str(filename).c_str(), RTLD_LAZY)
                    #endif
                ) {}
            ~Dll() 
            {
                if (h) {
                    #ifdef _WIN32
                    FreeLibrary((HINSTANCE)h);
                    #else
                    dlclose(h);
                    #endif
                }
            }

            void* GetH() const 
            {
                if (h == nullptr) {
                    #ifdef _WIN32
                    THROW_EXCEPTION_MSG(ExceptionType::DllError, str2wstr(std::to_string(GetLastError())));
                    #else
                    THROW_EXCEPTION_MSG(ExceptionType::DllError, str2wstr(std::to_string(dlerror())));
                    #endif
                    return nullptr;
                }
                return (void *)h;
            }

            //win version: FARPROC GetProc(HINSTANCE h, const char* procName)
            void *GetProcedure(const std::wstring &procName) const
            {
                #ifdef _WIN32
                return (void *)GetProcAddress((HINSTANCE)h, wstr2str(procName).c_str());
                #else
                return dlsym(h, wstr2str(procName).c_str());
                #endif
            };
    };
}
