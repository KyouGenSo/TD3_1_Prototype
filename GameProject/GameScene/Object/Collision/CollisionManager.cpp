#include "CollisionManager.h"

#include <array>
#include <iostream>
#include <ranges>

#include "Collider.h"


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
                (pOther->GetAttribute() & pCollider->GetIgnore()))
            {
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

void CollisionManager::Check(const std::string& col, const std::string& other)
{
    const auto* const pCollider = pColliders_[col];
    const auto* pOther = pColliders_[other];

    std::string id = CreatePair(col, other);

    bool isHit = false;

    if (std::holds_alternative<float>(pCollider->GetSize()) && std::holds_alternative<float>(pOther->GetSize()))
    {
        isHit = (pCollider->GetPosition() - pOther->GetPosition()).Length() < std::get<float>(pCollider->GetSize()) + std::get<float>(pOther->GetSize());
    }
    else if (std::holds_alternative<Vector3>(pCollider->GetSize()) && std::holds_alternative<Vector3>(pOther->GetSize()))
    {
        auto size1 = std::get<Vector3>(pCollider->GetSize());
        auto size2 = std::get<Vector3>(pOther->GetSize());
        isHit = (std::abs(pCollider->GetPosition().x - pOther->GetPosition().x) < size1.x + size2.x) &&
            (std::abs(pCollider->GetPosition().y - pOther->GetPosition().y) < size1.y + size2.y) &&
            (std::abs(pCollider->GetPosition().z - pOther->GetPosition().z) < size1.z + size2.z);
    }
    else if (std::holds_alternative<float>(pCollider->GetSize()) && std::holds_alternative<Vector3>(pOther->GetSize()))
    {
        auto size = std::get<Vector3>(pOther->GetSize());
        isHit = (std::abs(pCollider->GetPosition().x - pOther->GetPosition().x) < std::get<float>(pCollider->GetSize()) + size.x) &&
            (std::abs(pCollider->GetPosition().y - pOther->GetPosition().y) < std::get<float>(pCollider->GetSize()) + size.y) &&
            (std::abs(pCollider->GetPosition().z - pOther->GetPosition().z) < std::get<float>(pCollider->GetSize()) + size.z);
    }
    else if (std::holds_alternative<Vector3>(pCollider->GetSize()) && std::holds_alternative<float>(pOther->GetSize()))
    {
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
    // 最大サイズ事前計算（メモリ再割り当て防止）
    size_t maxEvents = 0;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        maxEvents = state_.size();
    }

    std::vector<std::tuple<Collider*, Collider*, Collider::Event>> eventsToFire;
    eventsToFire.reserve(maxEvents * 1.2);

    // データコピー部分（ロック必要）
    {
        std::lock_guard<std::mutex> lock(mutex_);

        for (auto itr = state_.begin(); itr != state_.end(); ){
            const std::string& id = itr->first;
            bool isColliding = itr->second;

            // キャッシュ用の文字列ビュー
            size_t delimiter = id.find('_');
            if (delimiter == std::string::npos){
                itr = state_.erase(itr);
                continue;
            }

            // string_viewだけ作って一時文字列は避ける！
            std::string_view id1(id.data(), delimiter);
            std::string_view id2(id.data() + delimiter + 1);

            // 文字列検索
            auto pCollider1 = pColliders_.find(std::string(id1));
            auto pCollider2 = pColliders_.find(std::string(id2));

            if (pCollider1 == pColliders_.end() || pCollider2 == pColliders_.end()){
                itr = state_.erase(itr);
                continue;
            }

            // イベント登録
            if (isColliding){
                if (!pairs_.contains(id)){
                    eventsToFire.emplace_back(pCollider1->second, pCollider2->second, Collider::Event::TRIGGER);
                    pairs_[id] = true;
                } else{
                    eventsToFire.emplace_back(pCollider1->second, pCollider2->second, Collider::Event::STAY);
                }
                ++itr;
            } else{
                eventsToFire.emplace_back(pCollider1->second, pCollider2->second, Collider::Event::EXIT);
                pairs_.erase(id);
                itr = state_.erase(itr);
            }
        }
    }

    // イベント発火（ロック不要）
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
    // ロックはUpdate内で既にかかってるはず
    std::vector<std::string> toRemove;

    // pairs_のイテレートは避ける
    for (auto itr = state_.begin(); itr != state_.end(); ){
        const std::string& key = itr->first;
        size_t delimiter = key.find('_');

        if (delimiter == std::string::npos){
            pairs_.erase(key);
            itr = state_.erase(itr);
            continue;
        }

        std::string_view id1(key.data(), delimiter);
        std::string_view id2(key.data() + delimiter + 1);

        if (!pColliders_.contains(std::string(id1)) || !pColliders_.contains(std::string(id2))){
            pairs_.erase(key);
            itr = state_.erase(itr);
        } else{
            ++itr;
        }
    }
}

