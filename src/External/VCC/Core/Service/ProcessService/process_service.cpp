#include "process_service.hpp"

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
        std::wstring ProcessService::_ExecuteWindow(std::wstring command)
        {
            return process_service_win(command);
        }
        #else
        std::wstring ProcessService::_ExecuteLinux(std::string command)
        {
            std::wstring result = L"";
            // convert to token
            vector<char *> tokens;
            char *token = strtok((char *)command.c_str(), " ");
            while (token != nullptr) {
                tokens.push_back(token);
                token = strtok(nullptr, " ");
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

        std::wstring ProcessService::_Execute(std::wstring command)
        {
            #ifdef _WIN32
            return ProcessService::_ExecuteWindow(command);
            #else
            return ProcessService::_ExecuteLinux(wstr2str(command));
            #endif
        }

        std::wstring ProcessService::Execute(std::wstring command)
        {
            LogProperty defaultProperty;
            return ProcessService::Execute(defaultProperty, L"PROCESS", L"", command);
        }

        std::wstring ProcessService::Execute(LogProperty &logProperty, std::wstring id, std::wstring userId, std::wstring command)
        {
            LogService::LogProcess(logProperty, id, userId, command);

            std::wstring result = L"";
            try {
                result = ProcessService::_Execute(command);
            } catch (exception &e) {
                THROW_EXCEPTION(ExceptionType::CUSTOM_ERROR, str2wstr(std::string(e.what())));
            }
            LogService::LogProcessResult(logProperty, id, userId, result);
            return result;
        }
}