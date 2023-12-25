#pragma once

#include <string>
#include <vector>

class VPGProcessManager
{
    public:
        VPGProcessManager() = default;
        ~VPGProcessManager() {};

        void Add(const std::vector<std::wstring> &cmds);
        void Update(const std::vector<std::wstring> &cmds);
        void Generate(const std::vector<std::wstring> &cmds);
        void Execute(const std::vector<std::wstring> &cmds);
};