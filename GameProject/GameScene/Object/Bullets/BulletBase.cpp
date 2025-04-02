#include "BulletBase.h"

#include <assert.h>
#include <mutex>
#include <GameScene/Object/Bullets/BulletFactory.h>

void BulletBase::Initialize()
{
    pLifeTimer_ = std::make_unique<Timer>();
    pLifeTimer_->Start();

    pNextBulletTimer_ = std::make_unique<Timer>();
}

void BulletBase::Update() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (rdy_ && pNext_){
        // ���̒e�̔���
        pNext_->Fire();
        rdy_ = false;
    }
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

    pChainManager_->OnAttacked(type_); // �`�F�C���}�l�[�W���[�ɍU�����ꂽ���Ƃ�ʒm
    pNextBulletTimer_->Start();
}

bool BulletBase::IsDeadAll()
{
    if (!isDead_) return false;
    else if (pNext_ == nullptr) return true;
    else return pNext_->IsDeadAll();
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

void BulletBase::Next()
{
    if (pNextBulletTimer_->GetIsStart())
    {
        pNextBulletTimer_->Reset();

        if (pChainManager_->IsLastWeapon(type_)) return;

        // �N�[���^�C���̊m�F
        if (!BulletBase::CheckCoolTime()) return;

        // ���̒e�̐���
        std::thread([this](){
            BulletBase::CreateNextBullet();
            rdy_ = true;
        }).detach();
    }
}

void BulletBase::CreateNextBullet()
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto bullet = BulletFactory::CreateBullet(pChainManager_->GetNextWeapon(type_));
    assert(bullet);
    SetNextBullet(std::move(bullet));
}

bool BulletBase::CheckCoolTime()
{
    float coolTime = pChainManager_->GetNextCoolTime(type_);
    return coolTime <= 0;
}

bool BulletBase::CheckLifeTime() const
{
    if (isDead_)return true;

    return pLifeTimer_->GetNow<float>() >= lifeTime_;
}
