#pragma once
#include <cstdint>
#include <string>

class IReinforcement
{
public:
    virtual ~IReinforcement() = default;

    virtual void Apply() = 0;
    virtual void Remove() = 0;
    virtual void IncrementLevel() = 0;
    virtual void DecrementLevel() = 0;
    virtual void OnHit() = 0;

public:
    void Disable() { isDisabled_ = true; }
    void Enable() { isDisabled_ = false; }
    void ToggleDisable() { isDisabled_ = !isDisabled_; }

public:
    std::string GetName() const { return name_; }
    std::string GetEvent() const { return event_; }
    std::string GetImagePath() const { return imagePath_; }

protected:
    bool isDisabled_ = false;
    uint32_t level_ = 0u;

    std::string     name_           = "unnamed";
    std::string     description_    = "no description";
    std::string     statusType      = "none";
    std::string     operatorType_   = "none";
    std::string     imagePath_      = "none";
    float           value_          = 0.0f;
    int             rarity_         = 0;
    std::string     event_          = "none";
};