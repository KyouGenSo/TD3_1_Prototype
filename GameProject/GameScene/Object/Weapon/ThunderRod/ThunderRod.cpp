#include "ThunderRod.hpp"

#include "Utility/Adaptor.h"

void ThunderRod::Initialize() {
    WeaponBase::Initialize();

    //Sound
}

void ThunderRod::Update() {
    WeaponBase::Update();
    for (auto& bullet : bullets_){
        bullet->Update();
    }
}

void ThunderRod::Draw() {
    for (auto& bullet : bullets_){
        bullet->Draw();
    }
}

void ThunderRod::Fire() {
    Vector3 spawnPosition = Adaptor(hitdata_.hitPoint);
    if (60.f <= (spawnPosition - transform_.translate).Length()){
        spawnPosition = transform_.translate + forward_.Normalize() * 60.f;
    }
    WeaponBase::Fire(spawnPosition);
}
