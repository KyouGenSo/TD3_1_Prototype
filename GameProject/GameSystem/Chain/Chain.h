#pragma once

#include <array>
#include <unordered_map>
#include <Timer/Timer.h>

#include <Type/WeaponType.h>

using ChainData = std::array<WeaponType, 4>;

/// <summary>
/// チェーンデータを保持するクラス
/// </summary>
class Chain
{
public:
    const std::unordered_map<WeaponType, float> COOLTIMES = 
    {
        {WeaponType::RocketLauncher, 1.0f},
        {WeaponType::Assault, 0.5f},
        {WeaponType::MachineGun, 0.1f},
        {WeaponType::Lightning, 2.0f},
    };

    const std::array<WeaponType, 5> CHAIN_ARRAY =
    {
        WeaponType::None,
        WeaponType::RocketLauncher,
        WeaponType::Assault,
        WeaponType::MachineGun,
        WeaponType::Lightning,
    };


public:
    void Initialize();
    void Update();
    void OnAttacked(WeaponType _weaponType);
    void ImGui();


public: /// Getter
    const auto& Data() const { return data_; }
    float CoolTime(WeaponType _weaponType) const { return coolTimes_.at(_weaponType); }
    float NextCoolTime(WeaponType _weaponType) const { return coolTimes_.at(data_.at(static_cast<size_t>(_weaponType))); }
    WeaponType NextWeapon(WeaponType _weaponType) const { return data_.at(static_cast<size_t>(_weaponType)); }
    bool IsLastWeapon(WeaponType _weaponType) const;
    WeaponType Front() const { return data_.front(); }


public: /// Setter
    void Data(WeaponType _1, WeaponType _2 = WeaponType::None, WeaponType _3 = WeaponType::None, WeaponType _4 = WeaponType::None);


private:
    std::unordered_map<WeaponType, Timer> coolTimeCounter_;
    std::unordered_map<WeaponType, float> coolTimes_;
    ChainData data_;
};
