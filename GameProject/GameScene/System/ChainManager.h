#pragma once

#include <array>
#include <unordered_map>
#include <Timer/Timer.h>

#include <Type/WeaponType.h>

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

    const std::array<WeaponType, 4> CHAIN_ARRAY =
    {
        WeaponType::None,
        WeaponType::RocketLauncher,
        WeaponType::MachineGun,
        WeaponType::Lightning,
    };


public:
    void Initialize();
    void Update();
    void OnAttacked(WeaponType _weaponType);
    void ImGui();


public: /// Getter
    const auto& GetChain() const { return chain_; }
    float GetCoolTime(WeaponType _weaponType) const { return coolTimes_.at(_weaponType); }
    float GetNextCoolTime(WeaponType _weaponType) const { return coolTimes_.at(chain_.at(static_cast<size_t>(_weaponType))); }
    WeaponType GetNextWeapon(WeaponType _weaponType) const { return chain_.at(static_cast<size_t>(_weaponType)); }
    bool IsLastWeapon(WeaponType _weaponType) const;


public: /// Setter
    void SetChain(WeaponType _1, WeaponType _2 = WeaponType::None, WeaponType _3 = WeaponType::None, WeaponType _4 = WeaponType::None);


private:
    std::unordered_map<WeaponType, Timer> coolTimeCounter_;
    std::unordered_map<WeaponType, float> coolTimes_;
    Chain chain_;
};
