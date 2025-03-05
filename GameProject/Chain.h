#pragma once

#include <array>
#include <unordered_map>
#include <Timer/Timer.h>

class Chain
{
public:
    enum class Type{
        None,
        RocketLauncher,
        MachineGun,
        Lightning,
    };

    const std::unordered_map<Type, float> COOLTIMES = 
    {
        {Type::RocketLauncher, 1.0f},
        {Type::MachineGun, 0.1f},
        {Type::Lightning, 2.0f},
    };

public:
    void Initialize();
    void Update();


public: /// Getter
    const auto& GetTypes() const { return types_; }
    float GetCoolTime(Type _type) const;


public: /// Setter
    void SetTypes(Type _1, Type _2, Type _3, Type _4);


private:
    std::unordered_map<Type, Timer> coolTimeCounter_;
    std::unordered_map<Type, float> coolTimes_;
    std::array<Type, 4> types_;
};

