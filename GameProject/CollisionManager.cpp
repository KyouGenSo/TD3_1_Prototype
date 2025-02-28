#include "CollisionManager.h"

#include <ranges>

#include "Collider.h"

void CollisionManager::Add(Collider* pCollider) {
    pColliders_[pCollider->GetUniqueId()] = pCollider;
}

void CollisionManager::Remove(const Collider* pCollider) {
    pColliders_.erase(pCollider->GetUniqueId());
}

void CollisionManager::Update() {
	for (auto& collider : pColliders_ | std::views::values){
        collider->Update();
	}

    CheckAll();
}

void CollisionManager::CheckAll() {
	for (auto& [key, pCollider] : pColliders_){
        for (auto& [kOther,  pOther ] : pColliders_){
            if (pCollider == pOther){
                continue;
            }
            //filter
            if (pCollider->GetAttribute() & pOther->GetIgnore() || 
                pOther->GetAttribute() & pCollider->GetIgnore()){
                continue;
            }

            Check(key, kOther);
        }
    }
}

void CollisionManager::Check(const std::string& col, const std::string& other) {
    auto pCollider = pColliders_[col];
    auto pOther = pColliders_[other];
	if ((pCollider->GetTransform().translate - pOther->GetTransform().translate).Length() < pCollider->GetRadius() + pOther->GetRadius()){
        if (pairs_.end() != std::ranges::find(pairs_, Pair {col, other})){
            pairs_.emplace_back(col, other);

            pCollider->OnCollisionTrigger(pOther->GetOwner());
            pOther->OnCollisionTrigger(pCollider->GetOwner());
        }

        pCollider->OnCollision(pOther->GetOwner());
        pOther->OnCollision(pCollider->GetOwner());
    }
}
