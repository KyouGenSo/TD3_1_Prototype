#include "AssaultRifle.h"

#include "GameScene/Object/Bullets/BulletFactory.h"

void AssaultRifle::Initialize() {
}

void AssaultRifle::Update() {
    for (auto& bullet : bullets_){
        bullet->Update();
    }
    DeleteDeadBullet();
}

void AssaultRifle::Draw() {
    for (auto& bullet : bullets_){
        bullet->Draw();
    }
}

void AssaultRifle::Fire() {
    auto coolTime = pChainManager_->GetCoolTime(WeaponType::Assault);
    if (coolTime > 0) return;
    auto bullet = BulletFactory::CreateBullet(WeaponType::Assault);
    AddNewBullet(std::move(bullet));
}
