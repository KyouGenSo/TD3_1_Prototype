#include "EnemyManager.h"

#include "imgui.h"
#include "GameScene/Object/Player/Player.h"
#include "GameScene/Object/Castle/Castle.h"

void EnemyManager::Initialize(Object* player, Object* castle)
{
    pPlayer_ = player;
    pCastle_ = castle;

    AddEnemy({ 0.0f,0.0f,0.0f });
}

void EnemyManager::Update()
{
	for (auto& enemy : enemies_) {
        if (enemy->IsDead()){
            pMinimap_->Unregister(enemy.get());
            std::erase_if(enemies_, [&](const auto& e){ return e->IsDead(); });
            continue;
        }
        SelectTarget(enemy.get());
		enemy->Update();
	}
}

void EnemyManager::Draw()
{
    for (auto& enemy : enemies_) {
        enemy->Draw();
    }
}

void EnemyManager::Finalize()
{
    for (auto& enemy : enemies_) {
        enemy->Finalize();
    }
    enemies_.clear();
}

void EnemyManager::AddEnemy(const Vector3& position)
{
    auto enemy = std::make_unique<Enemy>();
    enemy->Initialize();
    enemy->SetTranslate(position);
    enemy->SetIsAppearing(true);
    enemy->SetAppearCounter(0.0f);
    //pMinimap_->Register(enemy.get());
    enemies_.push_back(std::move(enemy));
}

void EnemyManager::SelectTarget(Enemy* enemy)
{
    Vector3 enemyPos = enemy->GetTransform().translate;
    Vector3 playerPos = pPlayer_->GetTransform().translate;
    float distanceToPlayer = (enemyPos - playerPos).Length();

    if (distanceToPlayer < leave) { 
        enemy->SetTarget(pPlayer_);
    }
    else {
        enemy->SetTarget(pCastle_);
    }
}

void EnemyManager::ImGui()
{
    ImGui::Begin("EnemyManager");
    if (ImGui::Button("Add Enemy")) {
        for (int i = 0; i < 3; ++i) {
            AddEnemy({ 3.0f*i, 0.0f, 0.0f });
        }
    }
    ImGui::End();
}

void EnemyManager::SetMinimap(Minimap* pMinimap) {
    pMinimap_ = pMinimap;
}
