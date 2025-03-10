#include "EnemyManager.h"

void EnemyManager::Initialize()
{
    for (int i = 0; i < 3; ++i) {
        auto enemy = std::make_unique<Enemy>();
        enemy->Initialize();
        enemy->SetTranslate({ 3.0f * i,0.0f,0.0f });
        enemies_.push_back(std::move(enemy));
    }
}

void EnemyManager::Update()
{
	for (auto& enemy : enemies_) {
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
