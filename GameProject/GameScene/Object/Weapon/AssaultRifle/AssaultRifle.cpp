#include "AssaultRifle.h"
#include <GameSystem/SoundManager/SoundManager.h>

void AssaultRifle::Initialize()
{
    WeaponBase::Initialize();
    sound_fire_ = SoundManager::GetInstance()->SearchSoundData("Fire_Assault").handle;
    EnableSound();
}

void AssaultRifle::Update()
{
    WeaponBase::Update();
    for (auto& bullet : bullets_)
    {
        bullet->Update();
    }
    DeleteDeadBullet();
}

void AssaultRifle::Draw()
{
    for (auto& bullet : bullets_)
    {
        bullet->Draw();
    }
}
