#pragma once

#include <array>
#include <unordered_map>
#include <Timer/Timer.h>

enum class WeaponType : int{
    None,
    RocketLauncher,
    MachineGun,
    Lightning,
};

using Chain = std::array<WeaponType, 4>;

class ChainManager
{
public:
    const std::unordered_map<WeaponType, float> COOLTIMES = 
    {
        {WeaponType::RocketLauncher, 1.0f},
        {WeaponType::MachineGun, 0.1f},
        {WeaponType::Lightning, 2.0f},
    };

public:
    void Initialize();
    void Update();
    void OnAttacked(WeaponType _weaponType);


public: /// Getter
    const auto& GetChain() const { return chain_; }
    float GetCoolTime(WeaponType _weaponType) const { return coolTimes_.at(_weaponType); }
    float GetNextCoolTime(WeaponType _weaponType) const { return coolTimes_.at(chain_.at(static_cast<size_t>(_weaponType))); }
    WeaponType GetNextWeapon(WeaponType _weaponType) const { return chain_.at(static_cast<size_t>(_weaponType)); }
    bool IsLastWeapon(WeaponType _weaponType) const { return chain_.back() == _weaponType; }


public: /// Setter
    void SetChain(WeaponType _1, WeaponType _2, WeaponType _3, WeaponType _4);


private:
    std::unordered_map<WeaponType, Timer> coolTimeCounter_;
    std::unordered_map<WeaponType, float> coolTimes_;
    Chain chain_;
};
