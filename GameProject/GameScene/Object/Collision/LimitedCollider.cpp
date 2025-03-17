#include "LimitedCollider.h"
#include "CollisionManager.h"

void LimitedCollider::Update() {
    Collider::Update();
    if(0 < lifetime_){
        lifetime_--;
    } else{
        disable_ = true;
    }
}
