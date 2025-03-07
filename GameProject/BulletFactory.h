#pragma once

#include <memory>
#include <Chain.h>

class Bullet;

class BulletFactory
{
public:
    static std::unique_ptr<Bullet> CreateBullet(WeaponType _type);
};