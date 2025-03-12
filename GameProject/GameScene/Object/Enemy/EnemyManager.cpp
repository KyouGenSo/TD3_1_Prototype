#include "EnemyManager.h"

#include "imgui.h"

void EnemyManager::Initialize()
{
    for (int i = 0; i < 3; ++i) {
		AddEnemy({ 3.0f*i, 0.0f, 0.0f });
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

void EnemyManager::AddEnemy(const Vector3& position)
{
    auto enemy = std::make_unique<Enemy>();
    enemy->Initialize();
    enemy->SetTranslate(position);
    enemy->SetIsAppearing(true);
    enemy->SetAppearCounter(0.0f);
    enemies_.push_back(std::move(enemy));
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
