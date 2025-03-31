#include "CollisionManager.h"

#include <array>
#include <iostream>
#include <ranges>

#include "Collider.h"
#include "imgui.h"


void CollisionManager::Add(Collider* pCollider) {
    if (!pCollider){
        std::cout << "Null Exception : Collider is not valid [" << pCollider->GetUniqueId() << " " << pCollider->GetOwner()->GetUniqueId() << "]" << '\n'; 
        return;
    }
    try{
        std::lock_guard lock(mutex_);
        pColliders_[pCollider->GetUniqueId()] = pCollider;
    } catch (const std::exception& e){
        std::cout << "Exception : " << e.what() << '\n';
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

    CleanUp();

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
    std::vector<std::tuple<Collider*, Collider*, Collider::Event>> eventsToFire;

    // データアクセス部分のみロック - できるだけ短く！
    {
        std::lock_guard<std::mutex> lock(mutex_);

        // キャパシティを事前に確保（これだけでも結構速くなる！）
        eventsToFire.reserve(state_.size() * 1.2); // 少し余裕を持たせる

        for (auto itr = state_.begin(); itr != state_.end(); ){
            const std::string& id = itr->first;
            bool isColliding = itr->second;

            // ID分解を一度だけ行う
            size_t delimiter = id.find('_');
            if (delimiter == std::string::npos){
                itr = state_.erase(itr);
                continue;
            }

            // 文字列は参照で保持（コピー回避）
            const std::string_view id1(id.data(), delimiter);
            const std::string_view id2(id.data() + delimiter + 1, id.size() - delimiter - 1);

            // コライダー検索も一度だけ
            auto pCollider1 = pColliders_.find(std::string(id1));
            auto pCollider2 = pColliders_.find(std::string(id2));

            if (pCollider1 == pColliders_.end() || pCollider2 == pColliders_.end()){
                itr = state_.erase(itr);
                continue;
            }

            Collider* c1 = pCollider1->second;
            Collider* c2 = pCollider2->second;

            if (isColliding){
                if (!pairs_.contains(id)){
                    // トリガーイベント
                    eventsToFire.emplace_back(c1, c2, Collider::Event::TRIGGER);
                    pairs_[id] = true;
                } else{
                    // 継続衝突
                    eventsToFire.emplace_back(c1, c2, Collider::Event::STAY);
                }
                ++itr;
            } else{
                // 衝突終了
                eventsToFire.emplace_back(c1, c2, Collider::Event::EXIT);
                pairs_.erase(id);
                itr = state_.erase(itr);
            }
        }
    }

    // ロック解除後にイベント発火（こっちが一番時間かかるはず）
    for (const auto& [c1, c2, eventType] : eventsToFire){
        try{
            switch (eventType){
                case Collider::Event::TRIGGER:
                    c1->OnCollisionTrigger(c2);
                    c2->OnCollisionTrigger(c1);
                    break;

            case Collider::Event::STAY:
                    c1->OnCollision(c2);
                    c2->OnCollision(c1);
                    break;

            case Collider::Event::EXIT:
                    c1->OnCollisionExit(c2);
                    c2->OnCollisionExit(c1);
                    break;
            }
        } catch (const std::exception& e){
            std::cout << "Exception in collision event: " << e.what() << '\n';
        }
    }
}

void CollisionManager::ImText() const {
    #ifdef _DEBUG
    ImGui::Begin("CollisionManager");
    ImGui::Text("Total: %llu", debug_.total);
    ImGui::Text("Hit: %llu", debug_.collision);
    ImGui::Text("Filtered: %llu", debug_.filtered);
    ImGui::End();
    #endif
}

std::string CollisionManager::CreatePair(const std::string& col, const std::string& other) {
    if (col < other){
        return col + "_" + other;
    }
    return other + "_" + col;
}

void CollisionManager::CleanUp() {
    // 存在しないコライダーに関連するエントリを削除
    std::vector<std::string> toRemoveFromPairs;
    for (const auto& key : pairs_ | std::views::keys){
        size_t delimiter = key.find('_');
        if (delimiter == std::string::npos){
            toRemoveFromPairs.push_back(key);
            continue;
        }

        std::string id1 = key.substr(0, delimiter);
        std::string id2 = key.substr(delimiter + 1);

        if (!pColliders_.contains(id1) || !pColliders_.contains(id2)){
            toRemoveFromPairs.push_back(key);
        }
    }

    for (const auto& key : toRemoveFromPairs){
        pairs_.erase(key);
    }
}

