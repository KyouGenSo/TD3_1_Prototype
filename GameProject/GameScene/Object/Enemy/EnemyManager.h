#pragma once
#include <vector>
#include <memory>
#include "Enemy.h"
#include "GameScene/HUD/Minimap.h"

class Player;
class Castle;
class EnemyManager {
public:
    void Initialize(Object* player, Object* castle);
    void Update();
    void Draw();
    void Finalize();
	void AddEnemy(const Vector3& position);
    void SelectTarget(Enemy* enemy);
    void ImGui();
    void SpawnEnemy();
    Vector3 RandomSpawnPosition();

    void SetMinimap(Minimap* pMinimap);

    void SetTarget(Object* pTarget) { pTarget_ = pTarget; }

private:
    std::vector<std::unique_ptr<Enemy>> enemies_;
    Minimap* pMinimap_ = nullptr;

    Object* pTarget_ = nullptr;
    Object* pPlayer_ = nullptr;
    Object* pCastle_ = nullptr;

    const float leave = 10.0f;

    Vector3 appearancePos_ = { 3.0f,0.0f,0.0f };

    float spawnTimer_;
    float spawnInterval_ = 1.0f;

    Vector3 minSpawnRange_ = { -50.0f,0.0f,-50.0f };
    Vector3 maxSpawnRange_ = { 50.0f,0.0f,50.0f };

    Vector3 minSpawnPoint_ = { -55.0f,0.0f,-55.0f };
    Vector3 maxSpawnPoint_ = { 55.0f,0.0f,55.0f };

    float deltaTime_ = 1.0f / 60.0f;

};