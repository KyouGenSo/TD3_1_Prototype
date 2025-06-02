#include "WeaponFactory.h"
#include "RocketLauncher/RocketLauncher.h"
#include "AssaultRifle/AssaultRifle.h"
#include "SMG/SMG.h"
#include "ThunderRod/ThunderRod.hpp"

std::unique_ptr<WeaponBase> WeaponFactory::CreateWeapon(WeaponType _type)
{
    std::unique_ptr<WeaponBase> weapon = nullptr;
    switch (_type){
    case WeaponType::None:
        break;
    case WeaponType::RocketLauncher:
        return std::make_unique<RocketLauncher>();
        break;
    case WeaponType::Assault:
        weapon = std::make_unique<AssaultRifle>();
        break;
    case WeaponType::MachineGun:
        weapon = std::make_unique<SMG>();
        break;
    case WeaponType::Thunder:
        weapon = std::make_unique<ThunderRod>();
        break;
    }
    return weapon;
}
