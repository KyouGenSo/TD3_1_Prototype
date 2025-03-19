#include "CollisionManager.h"

#include <algorithm>
#include <array>
#include <set>

#include "Collider.h"

void CollisionManager::Add(Collider* pCollider) {
    pColliders_[pCollider->GetUniqueId()] = pCollider;
}

void CollisionManager::Remove(const Collider* pCollider) {
    pColliders_.erase(pCollider->GetUniqueId());
    pCollider = nullptr;
}

void CollisionManager::Update() {
    for (auto itr = pColliders_.begin(); itr != pColliders_.end();){
        if (itr->second->IsDisable()){
            itr = pColliders_.erase(itr);
            continue;
        }
        itr->second->Update();
        ++itr;
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
            if ((pCollider->GetAttribute() & pOther->GetIgnore()) ||
                (pOther->GetAttribute() & pCollider->GetIgnore())){
                continue;
            }

            Check(key, kOther);
        }
    }
}

void CollisionManager::Check(const std::string& col, const std::string& other) {
    auto pCollider = pColliders_[col];
    auto pOther = pColliders_[other];

    bool isHit = false;

    if (std::holds_alternative<float>(pCollider->GetSize()) && std::holds_alternative<float>(pOther->GetSize())){
        isHit = (pCollider->GetPosition() - pOther->GetPosition()).Length() < std::get<float>(pCollider->GetSize()) + std::get<float>(pOther->GetSize());
    } else if (std::holds_alternative<Vector3>(pCollider->GetSize()) && std::holds_alternative<Vector3>(pOther->GetSize())){
        auto size1 = std::get<Vector3>(pCollider->GetSize());
        auto size2 = std::get<Vector3>(pOther->GetSize());
        isHit = (std::abs(pCollider->GetPosition().x - pOther->GetPosition().x) < size1.x + size2.x) &&
            (std::abs(pCollider->GetPosition().y - pOther->GetPosition().y) < size1.y + size2.y) &&
            (std::abs(pCollider->GetPosition().z - pOther->GetPosition().z) < size1.z + size2.z);
    } else if (std::holds_alternative<float>(pCollider->GetSize()) && std::holds_alternative<Vector3>(pOther->GetSize())){
        auto size = std::get<Vector3>(pOther->GetSize());
        isHit = (std::abs(pCollider->GetPosition().x - pOther->GetPosition().x) < std::get<float>(pCollider->GetSize()) + size.x) &&
            (std::abs(pCollider->GetPosition().y - pOther->GetPosition().y) < std::get<float>(pCollider->GetSize()) + size.y) &&
            (std::abs(pCollider->GetPosition().z - pOther->GetPosition().z) < std::get<float>(pCollider->GetSize()) + size.z);
    } else if (std::holds_alternative<Vector3>(pCollider->GetSize()) && std::holds_alternative<float>(pOther->GetSize())){
        auto size = std::get<Vector3>(pCollider->GetSize());
        isHit = (std::abs(pCollider->GetPosition().x - pOther->GetPosition().x) < size.x + std::get<float>(pOther->GetSize())) &&
            (std::abs(pCollider->GetPosition().y - pOther->GetPosition().y) < size.y + std::get<float>(pOther->GetSize())) &&
            (std::abs(pCollider->GetPosition().z - pOther->GetPosition().z) < size.z + std::get<float>(pOther->GetSize()));
    }

    if (isHit){
        Pair p {col, other};

        if (p.first > p.second){
            std::swap(p.first, p.second);
        }

        if (std::ranges::find_if(pairs_, [&p](const Pair& pair){return pair == p;}) == pairs_.end()){
            pairs_.push_back(p);

            pCollider->OnCollisionTrigger(pOther);
            pOther->OnCollisionTrigger(pCollider);
        }

        pCollider->OnCollision(pOther);
        pOther->OnCollision(pCollider);
        return;
    }

    if (pairs_.end() != std::ranges::find(pairs_, Pair {col, other})){
        pairs_.erase(std::ranges::find(pairs_, Pair {col, other}));
        pCollider->OnCollisionExit(pOther);
        pOther->OnCollisionExit(pCollider);
    }
}
