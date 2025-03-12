#include "BulletFactory.h"

#include "Bullet.h"

#include "Rocket/RocketBullet.h"
#include "Assault/AssaultBullet.h"
//#include "MachineGunBullet.h"
//#include "LightningBullet.h"

std::unique_ptr<BulletBase> BulletFactory::CreateBullet(WeaponType _type)
{
    switch (_type)
    {
    case WeaponType::RocketLauncher:
        return std::make_unique<RocketBullet>();
    case WeaponType::Assault:
        return std::make_unique<AssaultBullet>();
    //case WeaponType::MachineGun:
    //    return std::make_unique<MachineGunBullet>();
    //case WeaponType::Lightning:
    //    return std::make_unique<LightningBullet>();
    default:
        return nullptr;
    }
}
