#include "Bullet.h"

void Bullet::Fire()
{
    if (isChainBullet_)
    {
        AttackChain();
    }
    else
    {
        AttackNormal();
    }
}
