#pragma once

#include <memory>

#include <Type/WeaponType.h>

class BulletBase;

class BulletFactory
{
public:
    static std::unique_ptr<BulletBase> CreateBullet(WeaponType _type);
};
