#include "RocketLauncher.h"

#include <GameScene/Object/Bullets/BulletFactory.h>

void RocketLauncher::Initialize() {

}


void RocketLauncher::Update() 
{
    for (auto& bullet : bullets_)
    {
        bullet->Update();
    }

    WeaponBase::DeleteDeadBullet();
}

void RocketLauncher::Draw() 
{
    for (auto& bullet : bullets_)
    {
        bullet->Draw();
    }
}

void RocketLauncher::Fire()
{
    auto coolTime = pChain_->GetCoolTime(WeaponType::RocketLauncher);
    if (coolTime > 0) return;

    auto bullet = BulletFactory::CreateBullet(WeaponType::RocketLauncher);
    AddNewBullet(std::move(bullet));
}
