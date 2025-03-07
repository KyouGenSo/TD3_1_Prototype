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
}

void RocketLauncher::Draw() 
{
}

void RocketLauncher::Fire()
{
    auto coolTime = pChainManager_->GetCoolTime(WeaponType::RocketLauncher);
    if (coolTime > 0) return;

    auto bullet = BulletFactory::CreateBullet(WeaponType::RocketLauncher);
    AddNewBullet(std::move(bullet));
}
