#include "Collider.h"

#include "CollisionManager.h"
#include "Singleton.h"

Collider::Collider(Object* _owner)
	: pManager_(Singleton<CollisionManager>::GetInstance()),
	pOwner_(_owner) {
    pManager_->Add(this);
}

void Collider::Update() {
    if(pOwner_->IsDead()){
        pManager_->Remove(this);
    }
}

void Collider::OnCollision(const Object* pObject) const {
    if (!onCollision_)return;

    onCollision_(pObject);
}

void Collider::OnCollisionExit(const Object* pObject) const {
    if (!onCollisionExit_)return;

    onCollisionExit_(pObject);
}

void Collider::SetEvent(const std::function<void(const Object*)>& callback, const Event event) {
	switch (event){
	case Event::TRIGGER:
		onCollisionTrigger_ = callback;
		break;
	case Event::STAY:
		onCollision_ = callback;
		break;
	case Event::EXIT:
		onCollisionExit_ = callback;
		break;
	}
}

void Collider::OnCollisionTrigger(const Object* pObject) const {
    if(!onCollisionTrigger_)return;

    onCollisionTrigger_(pObject);
}

const Transform& Collider::GetTransform() const {
    return pOwner_->GetTransform();
}

void Collider::SetRadius(const float r) {
    radius_ = r;
}
