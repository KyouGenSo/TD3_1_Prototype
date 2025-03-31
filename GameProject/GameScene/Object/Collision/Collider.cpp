#include "Collider.h"

#include <map>
#include <sstream>

#include "CollisionManager.h"

#include <Type/Singleton.h>

namespace  {
    std::map<Collider::Type, uint32_t> filterTable = {
        {Collider::Type::ALLY, 0b1},
        {Collider::Type::ENEMY, 0b1<<1},
        {Collider::Type::STAGE, 0b1<<2}
    };
}


Collider::Collider(): pManager_(Singleton<CollisionManager>::GetInstance()) {
    pManager_->Add(this);
}

Collider::Collider(Object* _owner)
	: pManager_(Singleton<CollisionManager>::GetInstance()),
	pOwner_(_owner) {
    pManager_->Add(this);
}

Collider::~Collider() {
    pManager_->Remove(GetUniqueId());
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

Collider* Collider::SetEvent(const std::function<void(const Collider*)>& callback, const Event event) {
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

    return this;
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

uint32_t Collider::GetAttribute() const {
    return attribute_;
}

Collider* Collider::SetSize(const std::variant<float, Vector3> size) {
    size_ = size;
    return this;
}

uint32_t Collider::GetIgnore() const {
    return ignore_;
}

Collider::Type Collider::GetType() const {
    return type_;
}

void Collider::Disable() {
    disable_ = true;
}

Collider* Collider::SetPosition(const Vector3 _pos) {
    position_ = _pos;
    return this;
}

Collider* Collider::SetType(const Type type) {
    type_ = type;
    attribute_ = filterTable[type];

    return this;
}

Collider* Collider::SetIgnore(Type type) {
    ignore_ |= filterTable[type];

    return this;
}
