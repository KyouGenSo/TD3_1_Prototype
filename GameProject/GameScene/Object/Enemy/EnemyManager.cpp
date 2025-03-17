#include "EnemyManager.h"

#include "imgui.h"
#include "GameScene/Object/Player/Player.h"
#include "GameScene/Object/Castle/Castle.h"

#include <random>

void EnemyManager::Initialize(Object* player, Object* castle)
{
    pPlayer_ = player;
    pCastle_ = castle;

    AddEnemy(appearancePos_);
    spawnTimer_ = 0.0f;
}

void EnemyManager::Update()
{
    SpawnEnemy();
    for (auto enemy = enemies_.begin(); enemy != enemies_.end(); ) {
        if ((*enemy)->IsDead()){
            //pMinimap_->Unregister(enemy.get());
            enemy = enemies_.erase(enemy);
            continue;
        }
        SelectTarget(enemy->get());
        (*enemy)->Update();
        ++enemy;
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
            AddEnemy({ appearancePos_.x * i, appearancePos_.y, appearancePos_.z });
        }
    }
    ImGui::End();
}

void EnemyManager::SpawnEnemy()
{
    spawnTimer_ += deltaTime_;
    if (spawnTimer_ > spawnInterval_) {
        AddEnemy(RandomSpawnPosition());
        spawnTimer_ = 0.0f;
    }
}

void EnemyManager::SetMinimap(Minimap* pMinimap) {
    pMinimap_ = pMinimap;
}

Vector3 EnemyManager::RandomSpawnPosition()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    Vector3 randomPos;

    while ((randomPos.x < maxSpawnRange_.x && randomPos.x > minSpawnRange_.x) && (randomPos.z < maxSpawnRange_.z && randomPos.z > minSpawnRange_.z)) {

        std::uniform_real_distribution<float> disX(minSpawnPoint_.x, maxSpawnPoint_.x);
        std::uniform_real_distribution<float> disY(minSpawnPoint_.y, maxSpawnPoint_.y);
        std::uniform_real_distribution<float> disZ(minSpawnPoint_.z, maxSpawnPoint_.z);

        randomPos = Vector3{ disX(gen), disY(gen), disZ(gen) };
    }

    return randomPos;
}