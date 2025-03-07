#pragma once

#include <memory>

#include <Type/WeaponType.h>

class Bullet;

class BulletFactory
{
public:
    static std::unique_ptr<Bullet> CreateBullet(WeaponType _type);
};
