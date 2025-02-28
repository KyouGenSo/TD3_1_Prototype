#include "CollisionManager.h"

#include "Collider.h"

void CollisionManager::Add(Collider* pCollider) {
    pColliders_.push_back(pCollider);
}

void CollisionManager::Remove(Collider* pCollider) {
    std::erase(pColliders_, pCollider);
}

void CollisionManager::Update() {
	for (auto collider : pColliders_){
        collider->Update();
	}

    CheckAll();
}

void CollisionManager::CheckAll() {
	for (auto& pCollider : pColliders_){
        for (auto& pOther : pColliders_){
            if (pCollider == pOther){
                continue;
            }
            //filter
            if (pCollider->GetAttribute() & pOther->GetIgnore() || 
                pOther->GetAttribute() & pCollider->GetIgnore()){
                continue;
            }

            Check(pCollider, pOther);
        }
    }
}

void CollisionManager::Check(const Collider* col, const Collider* other) {
    if ((col->GetTransform().translate - other->GetTransform().translate).Length() < col->GetRadius() + other->GetRadius()){
        col->OnCollision();
        other->OnCollision();
    }
}