
#ifdef _WIN32
#include <windows.h> 
#include <tchar.h>
#include <stdio.h> 
#include <strsafe.h>

#include <stdexcept>

#include "string_helper.hpp"

namespace vcc
{
    #define BUFSIZE 4096 
 
    //HANDLE pStdInRead = NULL;
    //HANDLE pStdInWrite = NULL;
    HANDLE pStdOutRead = NULL;
    HANDLE pStdOutWrite = NULL;
    HANDLE pStdErrorRead = NULL;
    HANDLE pStdErrorWrite = NULL;

    // HANDLE hFile = NULL;
    
    void CreateChildProcess(const std::wstring &command); 
    //void WriteToPipe(void); 
    std::wstring ReadStdOut(void);
    std::wstring ReadStdError(void);
    
    std::wstring ProcessServiceWin(const std::wstring &command)
    {
        // ref to https://learn.microsoft.com/en-us/windows/win32/procthread/creating-a-child-process-with-redirected-input-and-output
        SECURITY_ATTRIBUTES saAttr;
        saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
        saAttr.bInheritHandle = true; 
        saAttr.lpSecurityDescriptor = nullptr; 

        // create pipe for std-out
        if (!CreatePipe(&pStdOutRead, &pStdOutWrite, &saAttr, 0))
            throw std::runtime_error("CreatePipe: " + std::to_string(GetLastError()));
        if (!SetHandleInformation(pStdOutRead, HANDLE_FLAG_INHERIT, 0))
            throw std::runtime_error("SetHandleInformation: " + std::to_string(GetLastError()));
        // create pipe for std-error
        if (!CreatePipe(&pStdErrorRead, &pStdErrorWrite, &saAttr, 0))
            throw std::runtime_error("CreatePipe: " + std::to_string(GetLastError()));
        if (!SetHandleInformation(pStdErrorRead, HANDLE_FLAG_INHERIT, 0))
            throw std::runtime_error("SetHandleInformation: " + std::to_string(GetLastError()));

        // TODO: std-in
        // if (!CreatePipe(&pStdInRead, &pStdInWrite, &saAttr, 0)) 
        //     throw std::runtime_error("CreatePipe: " + std::to_string(GetLastError()));

        // if (!SetHandleInformation(pStdInWrite, HANDLE_FLAG_INHERIT, 0))
        //     throw std::runtime_error("SetHandleInformation: " + std::to_string(GetLastError()));
        
        // Child Process
        CreateChildProcess(command);

        // TODO: std-in
        // hFile = CreateFileW(
        //     process, 
        //     GENERIC_READ, 
        //     0, 
        //     NULL, 
        //     OPEN_EXISTING, 
        //     FILE_ATTRIBUTE_READONLY, 
        //     NULL); 

        // if ( hFile == INVALID_HANDLE_VALUE ) 
        //     throw std::runtime_error("CreateFile: " + std::to_string(GetLastError()));
        
        //WriteToPipe();
        std::wstring errMsg = ReadStdError();
        if (!errMsg.empty())
            throw std::runtime_error(vcc::wstr2str(errMsg));
        return ReadStdOut();
    } 
    
    void CreateChildProcess(const std::wstring &command)
    { 
        PROCESS_INFORMATION pi; 
        STARTUPINFOW si;
        
        // procInfo
        ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
        
        // startup info
        ZeroMemory(&si, sizeof(STARTUPINFOW));
        si.cb = sizeof(STARTUPINFOW); 
        si.hStdError = pStdErrorWrite;
        si.hStdOutput = pStdOutWrite;
        // si.hStdInput = pStdInRead;
        si.dwFlags |= STARTF_USESTDHANDLES;
        
        // Create child process. 
        wchar_t *commandStr = (wchar_t *)command.c_str();
        if (!CreateProcessW(nullptr,
            commandStr,     // command line 
            NULL,           // process security attributes 
            NULL,           // primary thread security attributes 
            TRUE,           // handles are inherited 
            0,              // creation flags 
            NULL,           // use parent's environment 
            NULL,           // use parent's current directory 
            &si,            // STARTUPINFOW pointer 
            &pi))           // receives PROCESS_INFORMATION
            throw std::runtime_error("Create Process: " + std::to_string(GetLastError()));
        else 
        {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            
            CloseHandle(pStdOutWrite);
            CloseHandle(pStdErrorWrite);
            // CloseHandle(pStdInRead);
        }
    }
    
    // TODO: std-in
    // void WriteToPipe(void)
    // { 
    //     DWORD dwRead, dwWritten; 
    //     CHAR chBuf[BUFSIZE];
    //     BOOL bSuccess = FALSE;
        
    //     while (true)
    //     { 
    //         bSuccess = ReadFile(hFile, chBuf, BUFSIZE, &dwRead, NULL);
    //         if (!bSuccess || dwRead == 0 ) break; 
            
    //         bSuccess = WriteFile(pStdInWrite, chBuf, dwRead, &dwWritten, NULL);
    //         if (!bSuccess ) break; 
    //     }
    //     if (!CloseHandle(pStdInWrite))
    //         throw std::runtime_error("Close Handle: " + std::to_string(GetLastError()));
    // } 
    
    std::wstring ReadStdOut(void)
    { 
        DWORD dwRead, dwWritten; 
        CHAR buffer[BUFSIZE]; 
        BOOL bSuccess = FALSE;
        HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

        std::wstring result = L"";
        while (true)
        { 
            bSuccess = ReadFile(pStdOutRead, buffer, BUFSIZE, &dwRead, NULL);
            if(!bSuccess || dwRead == 0) break;
            result += vcc::str2wstr(buffer);

            bSuccess = WriteFile(hParentStdOut, buffer, dwRead, &dwWritten, NULL);
            if (!bSuccess) break;
        }
        return result;
    }
    
    std::wstring ReadStdError(void)
    { 
        DWORD dwRead, dwWritten; 
        CHAR buffer[BUFSIZE]; 
        BOOL bSuccess = FALSE;
        HANDLE hParentStdError = GetStdHandle(STD_ERROR_HANDLE);

        std::wstring result = L"";
        while (true)
        { 
            bSuccess = ReadFile(pStdErrorRead, buffer, BUFSIZE, &dwRead, NULL);
            if(!bSuccess || dwRead == 0) break; 
            result += vcc::str2wstr(buffer);

            bSuccess = WriteFile(hParentStdError, buffer, dwRead, &dwWritten, NULL);
            if (!bSuccess) break; 
        } 
        return result;
    }
}

#endif