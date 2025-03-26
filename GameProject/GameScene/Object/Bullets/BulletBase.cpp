#include "BulletBase.h"

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
        InitializeChain();
    }
    else
    {
        InitializeNormal();
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

void BulletBase::Next() {
    if (pNextBulletTimer_->GetIsStart()){
        pNextBulletTimer_->Reset();

        if (pChainManager_->IsLastWeapon(type_)) return;

        // クールタイムの確認
        if (BulletBase::CheckCoolTime() == false) return;

        // 次の弾の生成
        BulletBase::CreateNextBullet();

        // 次の弾の発射
        pNext_->Fire();
    }
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

bool BulletBase::CheckLifeTime() const {
    if (isDead_)return true;

    return pLifeTimer_->GetNow<float>() >= lifeTime_;
}
