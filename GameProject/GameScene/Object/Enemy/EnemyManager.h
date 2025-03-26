#pragma once
#include <vector>
#include <memory>
#include <random>

#include "Enemy.h"
#include "GameScene/HUD/Minimap.h"

class Player;
class Castle;
class EnemyManager {
public:
    EnemyManager();
    ~EnemyManager();

    void Initialize(Object* player, Object* castle);
    void Update();
    void Draw();
    void Finalize();
	void AddEnemy(const Vector3& position);
    void SelectTarget(Enemy* enemy);
    void ImGui();
    void SpawnEnemy();
    Vector3 RandomSpawnPosition();
    void ChangeWave(std::string key);

    void SetMinimap(Minimap* pMinimap);

    void SetTarget(Object* pTarget) { pTarget_ = pTarget; }

private:
    void InitializeWaveFile(std::string key);

private:
    float deltaTime_ = 1.0f / 60.0f;

    enum Type {
        Normal,
        Fly,
        Bounce
    };
    struct Wave {
        Type type;
        float interval;
        int amount;
        float hpMultiplier;
    };
    Wave wave_;
    std::unordered_map<std::string, Wave>waves_;

    int spawnCount_ = 0;
    float spawnTimer_ = 0.0f;

    std::string key_;
    int keyIndex_ = 0;

    std::vector<std::unique_ptr<Enemy>> enemies_;

    const float leave = 10.0f;

    Minimap* pMinimap_ = nullptr;

    Object* pTarget_ = nullptr;
    Object* pPlayer_ = nullptr;
    Object* pCastle_ = nullptr;


    Vector3 appearancePos_ = { 3.0f,1.0f,0.0f };

    Vector3 minSpawnRange_ = { -50.0f,0.0f,-50.0f };
    Vector3 maxSpawnRange_ = { 50.0f,0.0f,50.0f };

    Vector3 minSpawnPoint_ = { -55.0f,1.0f,-55.0f };
    Vector3 maxSpawnPoint_ = { 55.0f,1.0f,55.0f };


    std::random_device rd_;
    std::mt19937 gen_;

};