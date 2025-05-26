#include "RocketLauncher.h"

#include <GameScene/Object/Bullets/BulletFactory.h>
#include <GameSystem/SoundManager/SoundManager.h>

void RocketLauncher::Initialize()
{
    WeaponBase::Initialize();
    sound_fire_ = SoundManager::GetInstance()->SearchSoundData("Fire_Rocket").handle;
    EnableSound();
}


void RocketLauncher::Update()
{
    WeaponBase::Update();
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