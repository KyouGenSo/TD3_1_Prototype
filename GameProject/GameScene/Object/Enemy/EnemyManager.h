#pragma once
#include <vector>
#include <memory>
#include <random>

#include "Enemy.h"
#include "FryEnemy.h"
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
    void ChangeWave(std::string key, bool resetSpawnCount = true);
    void SetMinimap(Minimap* pMinimap);

    void SetTarget(Object* pTarget) { pTarget_ = pTarget; }

private:
    void SpawnEnemy();
    Vector3 RandomSpawnPosition();
    void CreateWaveFile(std::string key);
    void InitializeWaveFile(std::string key);
    void TurnControl();

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
        int turn;
    };
    Wave wave_;
    std::unordered_map<std::string, Wave>waves_;

    std::unordered_map<std::string, int> spawnCount_;
    std::unordered_map<std::string, float> spawnTimer_;

    std::vector<std::string> keys_;
    int keyIndex_ = 0;

    std::vector<std::unique_ptr<Enemy>> enemies_;
    std::vector<std::unique_ptr<FryEnemy>> fryEnemies_;

    const float leave = 10.0f;

    int turnProgress = 0;

    //bool resetSpawnCount = true;

    Minimap* pMinimap_ = nullptr;

    Object* pTarget_ = nullptr;
    Object* pPlayer_ = nullptr;
    Object* pCastle_ = nullptr;


    Vector3 appearancePos_ = { 3.0f,1.0f,0.0f };

    Vector3 minSpawnRange_ = { -50.0f,0.0f,-50.0f };
    Vector3 maxSpawnRange_ = { 50.0f,0.0f,50.0f };

    Vector3 minSpawnPoint_ = { -55.0f,1.0f,-55.0f };
    Vector3 maxSpawnPoint_ = { 55.0f,1.0f,55.0f };


    Vector3 fryAppearancePos_ = { 3.0f,3.0f,0.0f };

    Vector3 fryMinSpawnRange_ = { -50.0f,3.0f,-50.0f };
    Vector3 fryMaxSpawnRange_ = { 50.0f,3.0f,50.0f };

    Vector3 fryMinSpawnPoint_ = { -55.0f,3.0f,-55.0f };
    Vector3 fryMaxSpawnPoint_ = { 55.0f,3.0f,55.0f };


    std::random_device rd_;
    std::mt19937 gen_;

};