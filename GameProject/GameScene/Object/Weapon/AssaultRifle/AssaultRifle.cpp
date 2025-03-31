#include "AssaultRifle.h"

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
