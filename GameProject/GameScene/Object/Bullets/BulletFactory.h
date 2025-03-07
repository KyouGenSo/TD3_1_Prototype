#pragma once

#include <memory>
#include <GameScene/System/ChainManager.h>

class Bullet;

class BulletFactory
{
public:
    static std::unique_ptr<Bullet> CreateBullet(WeaponType _type);
};