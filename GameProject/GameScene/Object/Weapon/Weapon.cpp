#include "Weapon.h"


void Weapon::AddNewBullet(std::unique_ptr<BulletBase> _bullet)
{
    _bullet->Initialize();
    _bullet->SetPosition(transform_.translate);
    _bullet->SetRotation(transform_.rotate);
    _bullet->SetIsChainBullet(false);
    _bullet->SetChainManager(pChainManager_);
    _bullet->Fire();
    bullets_.emplace_back(std::move(_bullet));
}

void Weapon::DeleteDeadBullet()
{
    bullets_.remove_if([](const std::unique_ptr<BulletBase>& _bullet) { return _bullet->IsDead(); });
}
