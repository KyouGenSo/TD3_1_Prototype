#include "CollisionManager.h"

#include <algorithm>
#include <array>
#include <iostream>

#include "Collider.h"
#include "imgui.h"

#include <EventTimer/EventTimer.h>

void CollisionManager::Add(Collider* pCollider) {
    try{
        std::lock_guard lock(mutex_);
        pColliders_[pCollider->GetUniqueId()] = pCollider;
    } catch (const std::exception& e){
        std::cout << e.what() << '\n';
    }
}

void CollisionManager::Remove(const std::string& uuid) {
    std::lock_guard lock(mutex_);

    auto itr = state_.begin();
    while (itr != state_.end()){
        if (itr->first.find(uuid) != std::string::npos){
            pairs_.erase(itr->first);
            itr = state_.erase(itr);
            continue;
        }
        ++itr;
    }

    pColliders_.erase(uuid);
}

void CollisionManager::Update() {
    std::lock_guard lock(mutex_);
    debug_.total = 0;
    debug_.filtered = 0;
    debug_.collision = 0;

    for (auto itr = pColliders_.begin(); itr != pColliders_.end();){
        if (!itr->second || itr->second->IsDisable()){
            auto uuid = itr->first;
            auto stateItr = state_.begin();
            while (stateItr != state_.end()){
                if (stateItr->first.find(uuid) != std::string::npos){
                    pairs_.erase(stateItr->first);
                    stateItr = state_.erase(stateItr);
                    continue;
                }
                ++stateItr;
            }

            itr = pColliders_.erase(itr);
            continue;
        }
        itr->second->Update();
        ++itr;
    }

    CheckAll();
    ProcessEvents();

#ifdef _DEBUG
    ImGui::Begin("CollisionManager");
    ImGui::Text("Total: %llu", debug_.total);
    ImGui::Text("Collision: %llu", debug_.collision);
    ImGui::Text("Filtered: %llu", debug_.filtered);
    //ImGui::TableHeader();
    ImGui::End();
#endif
}

void CollisionManager::CheckAll() {
    std::unordered_map<std::string, bool> tmp;
	for (auto& [key, pCollider] : pColliders_){
        ++debug_.total;
        if (!pCollider || pCollider->IsDisable() || (pCollider->GetOwner() && pCollider->GetOwner()->IsDead())){
            ++debug_.filtered;
            continue;
        }

        for (auto& [kOther,  pOther ] : pColliders_){
            if (pOther->IsDisable() || pOther->GetOwner() && pOther->GetOwner()->IsDead()){
                ++debug_.filtered;
                continue;
            }

            if (pCollider == pOther){
                ++debug_.filtered;
                continue;
            }

            //filter
            if ((pCollider->GetAttribute() & pOther->GetIgnore()) ||
                (pOther->GetAttribute() & pCollider->GetIgnore())){
                ++debug_.filtered;
                continue;
            }

            std::string pair = CreatePair(key, kOther);

            if (tmp.contains(pair)){
                ++debug_.filtered;
                continue;
            }

            Check(key, kOther);

            tmp[pair] = true;
        }
    }
}

void CollisionManager::Check(const std::string& col, const std::string& other) {
    auto pCollider = pColliders_[col];
    auto pOther = pColliders_[other];

    std::string id = CreatePair(col, other);

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

    auto preState = state_.find(id);
    bool wasCollision = (preState != state_.end() && preState->second);

    if (isHit){
        ++debug_.collision;
        state_[id] = true;
        return;
    }
    if (wasCollision){
        state_[id] = false;
    }
}

void CollisionManager::ProcessEvents() {
    for (auto itr = state_.begin(); itr != state_.end(); ){
        const std::string& id = itr->first;
        bool isColliding = itr->second;

        size_t delimiter = id.find('_');
        if (delimiter == std::string::npos){
            itr = state_.erase(itr);
            continue;
        }

        std::string id1 = id.substr(0, delimiter);
        std::string id2 = id.substr(delimiter + 1);

        auto pCollider1 = pColliders_.find(id1);
        auto pCollider2 = pColliders_.find(id2);

        if (pCollider1 == pColliders_.end() || pCollider2 == pColliders_.end()){
            itr = state_.erase(itr);
            continue;
        }

        if (isColliding){
            if (!pairs_.contains(id)){
                pCollider1->second->OnCollisionTrigger(pCollider2->second);
                pCollider2->second->OnCollisionTrigger(pCollider1->second);
                pairs_[id] = true;
                continue;
            }

            pCollider1->second->OnCollision(pCollider2->second);
            pCollider2->second->OnCollision(pCollider1->second);

            ++itr;
        }else{
            pCollider1->second->OnCollisionExit(pCollider2->second);
            pCollider2->second->OnCollisionExit(pCollider1->second);

            pairs_.erase(id);
            itr = state_.erase(itr);
        }
    }
}

std::string CollisionManager::CreatePair(const std::string& col, const std::string& other) {
    if (col < other){
        return col + "_" + other;
    }
    return other + "_" + col;
}
