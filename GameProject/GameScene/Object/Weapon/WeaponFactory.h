#pragma once
#include <memory>
#include "Weapon.h"

class WeaponFactory
{
public:
    static std::unique_ptr<WeaponBase> CreateWeapon(WeaponType _type);
};