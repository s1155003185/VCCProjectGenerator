#pragma once

#include <string>
#include <vector>

enum class ModeType
{
    Add,
    Update
};

enum class InterfaceType
{
    None,
    VCC,
    CPP,
    JAVA,
    SWIFT
};

class Process
{
private:
    std::wstring _Version = L"1.0.0"; // abcde = a.bc.ef
    ModeType _Mode = ModeType::Update;

    // VCC Git Source
    std::wstring _VccLocalSource = L"";

    // workspace
    std::wstring _Workspace = L"";
    
    // dll
    std::wstring _DLLDirectory = L"";
    
    // interface
    InterfaceType _InterfaceType = InterfaceType::None;
    std::wstring _InterfaceDirectory = L"";
    
    // project
    std::wstring _ObjectTypeFilePath = L"";
    std::wstring _ObjectPropertyTypeDirectory = L"";
    std::wstring _ActionTypeDirectory = L"";
    std::wstring _ErrorTypeDirectory = L"";

    // option
    bool _IsSourceForceRefresh = false;
    bool _IsSourceFullHistrory = false;
    bool _IsExcludeExternalUnitTest = false;
    bool _ForceUpdateVCCModel = false;

    std::wstring _assignPath(std::vector<std::wstring> &commands, size_t index);
    void _ParseCommand(std::vector<std::wstring> &commands);

public:
    Process() {};

    void Execute(std::vector<std::wstring> &commands);
};