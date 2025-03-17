#include "Collider.h"

#include <sstream>

#include "CollisionManager.h"

#include <Type/Singleton.h>

Collider::Collider(): pManager_(Singleton<CollisionManager>::GetInstance()) {
    pManager_->Add(this);
}

Collider::Collider(Object* _owner)
	: pManager_(Singleton<CollisionManager>::GetInstance()),
	pOwner_(_owner) {
    pManager_->Add(this);
}

void Collider::Update() {
    if (disable_)return;

    if(pOwner_){
        position_ = pOwner_->GetTransform().translate;
        disable_ = pOwner_->IsDead();
    }
}

void Collider::OnCollision(const Collider* pCollider) const {
    if (!onCollision_)return;

    onCollision_(pCollider);
}

void Collider::OnCollisionExit(const Collider* pCollider) const {
    if (!onCollisionExit_)return;

    onCollisionExit_(pCollider);
}

void Collider::SetEvent(const std::function<void(const Collider*)>& callback, const Event event) {
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

std::string Collider::GetUniqueId() const {
    if (pOwner_)return pOwner_->GetUniqueId();

    std::stringstream ss;
    ss << this;
    return ss.str();
}

void Collider::OnCollisionTrigger(const Collider* pCollider) const {
    if(!onCollisionTrigger_)return;

    onCollisionTrigger_(pCollider);
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
