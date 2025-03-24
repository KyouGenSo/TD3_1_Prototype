#include "WeaponFactory.h"
#include "RocketLauncher/RocketLauncher.h"
#include "AssaultRifle/AssaultRifle.h"

std::unique_ptr<WeaponBase> WeaponFactory::CreateWeapon(WeaponType _type)
{
    if (_type == WeaponType::RocketLauncher)
    {
        return std::make_unique<RocketLauncher>();
    }
    else if (_type == WeaponType::Assault)
    {
        return std::make_unique<AssaultRifle>();
    }
    return nullptr;
}
