#include "Collider.h"
#include "CollisionManager.h"

#include <Type/Singleton.h>

Collider::Collider(Object* _owner)
	: pManager_(Singleton<CollisionManager>::GetInstance()),
	pOwner_(_owner) {
    pManager_->Add(this);
}

void Collider::Update() {
    if (disable_)return;

    if(pOwner_){
        position_ = pOwner_->GetTransform().translate;
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

Vector3 Collider::GetPosition() const {
    return position_;
}

std::variant<float, Vector3> Collider::GetSize() const {
    return size_;
}

void Collider::SetSize(const std::variant<float, Vector3> size) {
    size_ = size;
}
