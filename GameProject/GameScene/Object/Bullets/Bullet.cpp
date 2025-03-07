#include "Bullet.h"

#include <GameScene/Object/Bullets/BulletFactory.h>

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

void Bullet::SetNextBullet(std::unique_ptr<Bullet> _bullet)
{
    _bullet->Initialize();
    _bullet->SetPosition(transform_.translate);
    _bullet->SetIsChainBullet(true);
    _bullet->SetChainManager(pChainManager_);
    pNext_ = std::move(_bullet);
}

void Bullet::CreateNextBullet()
{
    auto bullet = BulletFactory::CreateBullet(pChainManager_->GetChain().at(static_cast<size_t>(type_)));
    SetNextBullet(std::move(bullet));
}

bool Bullet::CheckCoolTime()
{
    float coolTime = pChainManager_->GetNextCoolTime(type_);
    return coolTime <= 0;
}
