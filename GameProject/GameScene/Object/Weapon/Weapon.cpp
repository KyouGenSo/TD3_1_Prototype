#include "Weapon.h"

#include "QuatFunc.h"
#include "GameScene/Object/Bullets/BulletFactory.h"

void WeaponBase::Fire()
{
    auto coolTime = pChain_->GetCoolTime(pChain_->GetChain().front());
    if (coolTime > 0) return;
    auto bullet = BulletFactory::CreateBullet(pChain_->GetChain().front());
    AddNewBullet(std::move(bullet));
}

void WeaponBase::AddNewBullet(std::unique_ptr<BulletBase> _bullet)
{
    _bullet->SetEmitter(emitter_);
    _bullet->Initialize();
    _bullet->SetPosition(transform_.translate);
    _bullet->SetRotation(transform_.rotate);
    _bullet->SetForward(Quat::RotateVec3({ 0,0,1 }, Quat::MakeRotateAxisAngle({ 0,1,0 }, transform_.rotate.y) * Quat::MakeRotateAxisAngle({ 1,0,0 }, transform_.rotate.x)));
    _bullet->SetIsChainBullet(false);
    _bullet->SetChainManager(pChain_);
    _bullet->Fire();
    bullets_.emplace_back(std::move(_bullet));
}

void WeaponBase::DeleteDeadBullet()
{
    bullets_.remove_if([](const std::unique_ptr<BulletBase>& _bullet) { return _bullet->IsDeadAll(); });
}
