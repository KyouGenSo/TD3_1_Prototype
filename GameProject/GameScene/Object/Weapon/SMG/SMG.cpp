#include "SMG.h"

void SMG::Initialize() {

}

void SMG::Update() {
    for (auto& bullet : bullets_){
        bullet->Update();
    }
    DeleteDeadBullet();
}

void SMG::Draw() {
    for (const auto& bullet : bullets_){
        bullet->Draw();
    }
}

