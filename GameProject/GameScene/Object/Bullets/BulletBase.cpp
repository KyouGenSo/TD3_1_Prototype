#include "BulletBase.h"

#include <cassert>
#include <mutex>
#include <GameScene/Object/Bullets/BulletFactory.h>
#include <Type/ColliderType.h>
#include <GameSystem/Reinforcement/Manager/ReinforcementManager.h>
#include <GameSystem/DeltaTimeManager/DeltaTimeManager.h>

void BulletBase::Initialize()
{
	pLifeTimer_ = std::make_unique<Timer>();
	pLifeTimer_->Start();
}

void BulletBase::Update() {
	std::lock_guard<std::mutex> lock(mutex_);
	if (rdy_ && pNext_){
		pNext_->Fire();
		rdy_ = false;
	}
    deltaTime_ = DeltaTimeManager::GetInstance()->GetDeltaTime(1);
}

void BulletBase::Fire()
{
    if (forward_.x != 0.0f || forward_.z != 0.0f) {
        transform_.rotate.y = std::atan2(forward_.x, forward_.z);
    }
    if (isChainBullet_)
    {
        InitializeChain();
    }
    else
    {
        InitializeNormal();
    }

	pChainManager_->OnAttacked(type_);
}

bool BulletBase::IsDeadAll()
{
	if (!isDead_) return false;
	else if (pNext_ == nullptr) return true;
	else return pNext_->IsDeadAll();
}

void BulletBase::SetNextBullet(std::unique_ptr<BulletBase> _bullet)
{
	_bullet->SetIsChainBullet(true);
	_bullet->SetChainManager(pChainManager_);
	_bullet->SetPosition(transform_.translate);
	_bullet->SetForward(forward_);
	_bullet->SetEmitter(emitter_);
	_bullet->Initialize();
	pNext_ = std::move(_bullet);
}

void BulletBase::Next()
{
	if (pChainManager_->IsLastWeapon(type_)) return;

	if (!BulletBase::CheckCoolTime()) return;

	std::thread([this](){
		BulletBase::CreateNextBullet();
		rdy_ = true;
	}).detach();
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

void BulletBase::NotifyReinforcementManager(const Collision::Collider* _other)
{
	if (_other->GetAttribute() & static_cast<uint32_t>(Collider::Type::ENEMY))
	{
		//敵に当たった場合
		ReinforcementManager::GetInstance()->Notify("onHit");
	}
}
