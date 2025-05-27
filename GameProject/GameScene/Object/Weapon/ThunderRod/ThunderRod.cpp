#include "ThunderRod.hpp"

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
