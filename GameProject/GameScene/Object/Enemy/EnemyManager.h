#pragma once
#include <vector>
#include <memory>
#include <random>

#include "EnemyBase.h"
#include "Enemy.h"
#include "FlyEnemy.h"
#include "BounceEnemy.h"
#include "GameScene/HUD/Minimap.h"
#include "Timer/Timer.h"

class Player;
class Castle;
class EnemyManager
{
private:
    enum Type
    {
        Normal,
        Fly,
        Bounce
    };
    struct Wave
    {
        Type type;
        float interval;
        int amount;
        float hpMultiplier;
        int turn;
        float time;
    };

public:
    EnemyManager();
    ~EnemyManager();

    void Initialize(Object* player, Object* castle);
    void Update();
    void Draw();
    void Finalize();
    void AddEnemy(const Vector3& position, Type type);
    void SelectTarget(EnemyBase* enemy);
    void ImGui();
    void ChangeWave(std::string key, bool resetSpawnCount = true);
    void SetMinimap(Minimap* pMinimap);

    void SetTarget(Object* pTarget) { pTarget_ = pTarget; }



private:
    void SpawnEnemy();
    Vector3 RandomSpawnPosition(Type type);
    //void CreateWaveFile(std::string key);
    void InitializeWaveFile(std::string key);
    void TurnControl();

private:
    float deltaTime_ = 1.0f / 60.0f;

    Wave wave_;
    std::unordered_map<std::string, Wave>waves_;

    std::unordered_map<std::string, int> spawnCount_;
    std::unordered_map<std::string, float> spawnTimer_;

    std::vector<std::string> keys_;
    int keyIndex_ = 0;

    std::vector<std::unique_ptr<Enemy>> enemies_;
    std::vector<std::unique_ptr<FlyEnemy>> flyEnemies_;
    std::vector<std::unique_ptr<BounceEnemy>> bounceEnemies_;

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


    Vector3 flyAppearancePos_ = { 3.0f,5.0f,0.0f };

    Vector3 flyMinSpawnRange_ = { -50.0f,5.0f,-50.0f };
    Vector3 flyMaxSpawnRange_ = { 50.0f,5.0f,50.0f };

    Vector3 flyMinSpawnPoint_ = { -55.0f,5.0f,-55.0f };
    Vector3 flyMaxSpawnPoint_ = { 55.0f,5.0f,55.0f };


    Vector3 bounceAppearancePos_ = { 3.0f,1.0f,0.0f };

    Vector3 bounceMinSpawnRange_ = { -50.0f,0.0f,-50.0f };
    Vector3 bounceMaxSpawnRange_ = { 50.0f,0.0f,50.0f };

    Vector3 bounceMinSpawnPoint_ = { -55.0f,1.0f,-55.0f };
    Vector3 bounceMaxSpawnPoint_ = { 55.0f,1.0f,55.0f };


    std::random_device rd_;
    std::mt19937 gen_;

};