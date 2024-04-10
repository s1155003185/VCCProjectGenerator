#pragma once

namespace vcc
{
    class IVPGGenerationManager
    {
    protected:
        IVPGGenerationManager() {}
        ~IVPGGenerationManager() {}

    public:
        virtual void Add() const = 0;
        virtual void Update() const = 0;
        virtual void Generate() const = 0;
    };
}
