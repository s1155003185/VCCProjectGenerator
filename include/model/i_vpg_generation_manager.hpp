#pragma once

class IVPGGenerationManager
{
protected:
    IVPGGenerationManager() {}
    ~IVPGGenerationManager() {}

public:
    virtual void add() const = 0;
    virtual void update() const = 0;
    virtual void generate() const = 0;
};