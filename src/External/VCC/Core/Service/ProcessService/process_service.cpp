#include "process_service.hpp"

#include <filesystem>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#ifdef _WIN32
// win process is implemented in process_service_win.hpp
#include "process_service_win.hpp"
#else
#include <sys/wait.h>
#include <unistd.h>
#endif

#include "exception_macro.hpp"
#include "exception_type.hpp"
#include "log_property.hpp"
#include "log_service.hpp"
#include "string_helper.hpp"

namespace vcc
{

        #ifdef _WIN32
        std::wstring ProcessService::_ExecuteWindow(const std::wstring &command)
        {
            return ProcessServiceWin(command);
        }
        #else
        std::wstring ProcessService::_ExecuteLinux(const std::string &command)
        {
            std::wstring result = L"";
            // convert to token
            std::vector<std::string> cmdTokens = ProcessService::ParseCMDLinux(command);
            vector<char *> tokens;
            for (size_t i = 0; i < cmdTokens.size(); i++) {
                tokens.push_back((char *)(cmdTokens[i].c_str()));
            }
            tokens.push_back(nullptr);

            // process
            int pipefd_stdout[2];
            int pipefd_stderr[2];
            int status;

            // pipe
            if (pipe(pipefd_stdout) < 0)
                perror("pipe: ");
            if (pipe(pipefd_stderr) < 0)
                perror("pipe: ");

            pid_t pid = fork();
            if (pid < 0) {
                perror("fork: ");
            } else if (pid == 0) {
                // child process
                // duplicate writing end to stdout
                dup2(pipefd_stdout[1], STDOUT_FILENO);
                dup2(pipefd_stderr[1], STDERR_FILENO);
                close(pipefd_stdout[0]);
                close(pipefd_stdout[1]);
                close(pipefd_stderr[0]);
                close(pipefd_stderr[1]);
                if (execvp(tokens[0], tokens.data())) {
                    perror("execvp: ");
                    exit(-1);
                }
            }
            // parent process
            // close writing end of pipe
            close(pipefd_stdout[1]);
            close(pipefd_stderr[1]);

            FILE *fStdout = fdopen(pipefd_stdout[0], "r");
            FILE *fStderr = fdopen(pipefd_stderr[0], "r");

            char ch;
            std::string tmpResult;
            while (!feof(fStdout)) {
                ch = fgetc(fStdout);
                tmpResult += ch;
            }
            result = str2wstr(tmpResult);

            std::string error;
            while (!feof(fStderr)) {
                ch = fgetc(fStderr);
                error += ch;
            }
            wait(&status);
            if (status != 0)
                throw runtime_error(error);
            return result;
        }
        #endif

        std::wstring ProcessService::_Execute(const std::wstring &command)
        {
            #ifdef _WIN32
            return ProcessService::_ExecuteWindow(command);
            #else
            return ProcessService::_ExecuteLinux(wstr2str(command));
            #endif
        }

        std::vector<std::string> ProcessService::ParseCMDLinux(const std::string &cmd)
        {
            // need to remove double quote and escape "\"
            std::vector<std::string> results;
            std::string token = "";
            char strStartToken = '\0';
            bool isEscape = false;
            for (char c : cmd) {
                if (isEscape) {
                    token += c;
                    isEscape = false;
                    continue;
                }
                if (strStartToken == '\0') {
                    if (c == L' ') {
                        if (!token.empty()) {
                            results.push_back(token);
                        }
                        token = "";
                    } else if (c == '"' || c == '\'') {
                        strStartToken = c;
                    } else {
                        token += c;
                    }
                } else {
                    if (c == '\\') {
                        isEscape = true;
                    } else if (c == strStartToken) {
                        strStartToken = '\0';
                    } else {
                        token += c;
                    }
                }
            }
            if (!token.empty())
                results.push_back(token);
            return results;
        }

        std::wstring ProcessService::Execute(const LogProperty *logProperty, const std::wstring &id, const std::wstring &command)
        {
            LogService::LogProcess(logProperty, id, command);

            std::wstring result = L"";
            try {
                result = ProcessService::_Execute(command);
            } catch (exception &e) {
                THROW_EXCEPTION(e);
            }
            LogService::LogProcessResult(logProperty, id, result);
            Trim(result);
            return result;
        }

        std::wstring ProcessService::Execute(const LogProperty *logProperty, const std::wstring &id, const std::wstring &workspace, const std::wstring &command)
        {
            std::wstring currentDirectory = L"";
            std::wstring result = L"";
            try {
                currentDirectory = std::filesystem::current_path().wstring();
                if (!IsBlank(workspace))
                    std::filesystem::current_path(workspace);
            } catch (exception &e) {
                if (!IsBlank(workspace))
                    std::filesystem::current_path(currentDirectory);
                THROW_EXCEPTION(e);
                return result;
            }
            try {
                result = ProcessService::Execute(logProperty, id, command);
            } catch (Exception &e) {
                if (!IsBlank(workspace))
                    std::filesystem::current_path(currentDirectory);
                THROW_EXCEPTION(e);
            }
            try {
                if (!IsBlank(workspace))
                    std::filesystem::current_path(currentDirectory);
            } catch (exception &e) {
                THROW_EXCEPTION(e);
                return result;
            }            
            return result;
        }
}