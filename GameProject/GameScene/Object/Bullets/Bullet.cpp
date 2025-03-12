#include "Bullet.h"

#include <GameScene/Object/Bullets/BulletFactory.h>

void BulletBase::Initialize()
{
    pLifeTimer_ = std::make_unique<Timer>();
    pLifeTimer_->Start();

    pNextBulletTimer_ = std::make_unique<Timer>();
}

void BulletBase::Fire()
{
    if (isChainBullet_)
    {
        AttackChainInitialize();
    }
    else
    {
        AttackNormalInitialize();
    }

    pNextBulletTimer_->Start();
}

bool BulletBase::IsDead()
{
    if (!isDead_) return false;
    else if (pNext_ == nullptr) return true;
    else return pNext_->IsDead();
}

void BulletBase::SetNextBullet(std::unique_ptr<BulletBase> _bullet)
{
    _bullet->Initialize();
    _bullet->SetPosition(transform_.translate);
    _bullet->SetIsChainBullet(true);
    _bullet->SetChainManager(pChainManager_);
    _bullet->SetForward(forward_);
    pNext_ = std::move(_bullet);
}

void BulletBase::CreateNextBullet()
{
    auto bullet = BulletFactory::CreateBullet(pChainManager_->GetChain().at(static_cast<size_t>(type_)));
    SetNextBullet(std::move(bullet));
}

bool BulletBase::CheckCoolTime()
{
    float coolTime = pChainManager_->GetNextCoolTime(type_);
    return coolTime <= 0;
}
