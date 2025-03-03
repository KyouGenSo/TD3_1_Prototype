#include "Collider.h"

#include "CollisionManager.h"
#include "Singleton.h"

Collider::Collider() {
    pManager_ = Singleton<CollisionManager>::GetInstance();
    pManager_->Add(this);
}

void Collider::Update() {
    if(pOwner_->IsDead()){
        pManager_->Remove(this);
    }
}

void Collider::OnCollision(Object* pObject) const {
    if (!onCollision_)return;

    onCollision_(pObject);
}

void Collider::OnCollisionExit(Object* pObject) const {
    if (!onCollisionExit_)return;

    onCollisionExit_(pObject);
}

void Collider::OnCollisionTrigger(Object* pObject) const {
    if(!onCollisionTrigger_)return;

    onCollisionTrigger_(pObject);
}

Transform& Collider::GetTransform() const {
    return pOwner_->GetTransform();
}

void Collider::SetRadius(const float r) {
    radius_ = r;
}
