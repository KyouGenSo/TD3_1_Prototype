#include "SMG.h"

#include <GameSystem/SoundManager/SoundManager.h>

void SMG::Initialize()
{
    sound_fire_ = SoundManager::GetInstance()->SearchSoundData("Fire_SMG").handle;
    EnableSound();
}

void SMG::Update()
{
    for (auto& bullet : bullets_)
    {
        bullet->Update();
    }
    DeleteDeadBullet();
}

void SMG::Draw()
{
    for (const auto& bullet : bullets_)
    {
        bullet->Draw();
    }
}

