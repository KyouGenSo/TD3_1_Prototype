#include "EnemyManager.h"

#include "imgui.h"
#include "GameScene/Object/Player/Player.h"

#include <random>

#include "GlobalVariables.h"
#include "EnemyBase.h"
#include "EnemyFactory.h"
#include <list>

EnemyManager::EnemyManager()
    : wave_()
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Initialize(Object* player, Object* castle)
{
    RandomGenerator::Initialize();

    pPlayer_ = player;
    pCastle_ = castle;

    spawnTimer_.clear();
    spawnCount_.clear();

    keys_.clear();



    InitializeWaveFile("00n");
    InitializeWaveFile("01n");
    InitializeWaveFile("02n");
    InitializeWaveFile("03f");
    InitializeWaveFile("04n");
    InitializeWaveFile("04f");
    InitializeWaveFile("05n");
    InitializeWaveFile("05f");
    InitializeWaveFile("06n");
    InitializeWaveFile("06b");
    InitializeWaveFile("07f");
    InitializeWaveFile("07b");
    InitializeWaveFile("08f");
    InitializeWaveFile("08b");
    InitializeWaveFile("09n");
    InitializeWaveFile("10n");
    InitializeWaveFile("10f");
    InitializeWaveFile("10b");


    TurnControl();
    turnProgress++;
}

void EnemyManager::Update()
{
    bool spawnLimitReached = (enemies_.size() >= maxSpawnCount_);

    std::vector<std::string> keysToRemove;
    for (auto key = keys_.begin(); key != keys_.end(); )
    {
        if (waves_[*key].time > 0.0f)
        {
            if (!spawnLimitReached) {
                waves_[*key].time -= deltaTime_;
            }
            key++;
        }
        else
        {
            keysToRemove.push_back(*key);
            key = keys_.erase(key);
        }
    }

    if (!keysToRemove.empty())
    {
        TurnControl();
        turnProgress++;
    }

    if (!spawnLimitReached) {
        SpawnEnemy();
    }

    for (auto enemy = enemies_.begin(); enemy != enemies_.end(); ) {
        if ((*enemy)->IsDead()) {
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
    for (auto& enemy : enemies_)
    {
        enemy->Draw();
    }
}

void EnemyManager::Draw2d()
{
    std::list<EnemyBase*> allEnemies_ = {};
    for (auto& e : enemies_) e->Draw2d();
}

void EnemyManager::Finalize()
{
    for (auto& enemy : enemies_)
    {
        enemy->Finalize();
    }
    enemies_.clear();
}

void EnemyManager::AddEnemy(const Vector3& position, EnemyBase::Type type)
{
    auto enemy = EnemyFactory::CreateEnemy(type);
    enemy->Initialize();
    enemy->SetTranslate(position);
    enemy->SetIsAppearing(true);
    enemy->SetAppearCounter(0.0f);
    enemy->SetEmitter(pEmitter_);
    pMinimap_->Register(enemy.get());
    enemies_.push_back(std::move(enemy));
}

void EnemyManager::SelectTarget(EnemyBase* enemy)
{
    Vector3 enemyPos = enemy->GetTransform().translate;
    Vector3 playerPos = pPlayer_->GetTransform().translate;
    float distanceToPlayer = (enemyPos - playerPos).Length();

    if (distanceToPlayer < leave)
    {
        enemy->SetTarget(pPlayer_);
    }
    else
    {
        enemy->SetTarget(pCastle_);
    }
}

void EnemyManager::ImGui()
{
    #ifdef _DEBUG
    ImGui::Begin("EnemyManager");
    if (ImGui::Button("Normal"))
    {
        AddEnemy({ 0.0f,1.0f,0.0f }, EnemyBase::Type::Normal);
    }
    if (ImGui::Button("Fly"))
    {
        AddEnemy({ 0.0f,5.0f,0.0f }, EnemyBase::Type::Fly);
    }
    if (ImGui::Button("Bounce"))
    {
        AddEnemy({ 0.0f,1.0f,0.0f }, EnemyBase::Type::Bounce);
    }
    if (ImGui::Button("TurnControl"))
    {
        TurnControl();
        turnProgress++;
    }
    ImGui::DragInt("turnProgress", &turnProgress, 1);
    ImGui::End();
    #endif
}


void EnemyManager::SetMinimap(Minimap* pMinimap)
{
    pMinimap_ = pMinimap;
}

void EnemyManager::InitializeWaveFile(std::string key)
{
    GlobalVariables::GetInstance()->CreateGroup(key);
    GlobalVariables::GetInstance()->LoadFile(key);
    spawnCount_[key] = 0;

    Wave wave;
    wave.type = static_cast<EnemyBase::Type>(GlobalVariables::GetInstance()->GetValueInt(key, "type"));
    wave.interval = GlobalVariables::GetInstance()->GetValueFloat(key, "interval");
    wave.amount = GlobalVariables::GetInstance()->GetValueInt(key, "amount");
    wave.hpMultiplier = GlobalVariables::GetInstance()->GetValueFloat(key, "hpMultiplier");
    wave.turn = GlobalVariables::GetInstance()->GetValueInt(key, "turn");
    waves_[key] = wave;
}

void EnemyManager::TurnControl()
{
    std::vector<std::string> newKeys;
    for (const auto& wave : waves_)
    {
        if (turnProgress == wave.second.turn)
        {
            newKeys.push_back(wave.first);
        }
    }
    keys_.clear();
    for (const auto& key : newKeys)
    {
        keys_.push_back(key);
        ChangeWave(key, true);
    }
}

void EnemyManager::SetEmitter(EmitterManager* pEmitter) {
    pEmitter_ = pEmitter;
}

void EnemyManager::SpawnEnemy()
{
    /// スポーンしたい敵の数をカウントアップする
    if (!keys_.empty())
    {
        for (const auto& key : keys_)
        {
            const auto& wave = waves_[key];

            spawnTimer_[key] += deltaTime_;
            if (spawnTimer_[key] > wave.interval)
            {
                pendingSpawnCount_[wave.type] += wave.amount;
                spawnTimer_[key] = 0.0f;
            }
        }
    }


    // 敵のスポーンを実行する (毎フレーム １タイずつ)
    for (auto& [key, count] : pendingSpawnCount_)
    {
        if (count <= 0) continue;

        AddEnemy(RandomSpawnPosition(key), key);
        --count;
    }
}

Vector3 EnemyManager::RandomSpawnPosition(EnemyBase::Type type)
{
    Vector3 randomPos = {};

    if (type == EnemyBase::Type::Normal)
    {
        while ((randomPos.x < maxSpawnRange_.x && randomPos.x > minSpawnRange_.x) && (randomPos.z < maxSpawnRange_.z && randomPos.z > minSpawnRange_.z))
        {
            randomPos = Vector3{ RandomGenerator::Generate(minSpawnPoint_.x, maxSpawnPoint_.x), RandomGenerator::Generate(minSpawnPoint_.y, maxSpawnPoint_.y), RandomGenerator::Generate(minSpawnPoint_.z, maxSpawnPoint_.z)};
        }
    }
    else if (type == EnemyBase::Type::Fly)
    {
        while ((randomPos.x < flyMaxSpawnRange_.x && randomPos.x > flyMinSpawnRange_.x) && (randomPos.z < flyMaxSpawnRange_.z && randomPos.z > flyMinSpawnRange_.z))
        {
            randomPos = Vector3{ RandomGenerator::Generate(flyMinSpawnPoint_.x, flyMaxSpawnPoint_.x), RandomGenerator::Generate(flyMinSpawnPoint_.y, flyMaxSpawnPoint_.y), RandomGenerator::Generate(flyMinSpawnPoint_.z, flyMaxSpawnPoint_.z), };
        }
    }
    else if (type == EnemyBase::Type::Bounce) {
        while ((randomPos.x < bounceMaxSpawnRange_.x && randomPos.x > bounceMinSpawnRange_.x) && (randomPos.z < bounceMaxSpawnRange_.z && randomPos.z > bounceMinSpawnRange_.z)) 
        {
            randomPos = Vector3{ RandomGenerator::Generate(bounceMinSpawnPoint_.x, bounceMaxSpawnPoint_.x), RandomGenerator::Generate(bounceMinSpawnPoint_.y, bounceMaxSpawnPoint_.y), RandomGenerator::Generate(bounceMinSpawnPoint_.z, bounceMaxSpawnPoint_.z), };
        }
    }

    return randomPos;
}


void EnemyManager::CreateWaveFile(std::string key)
{
    GlobalVariables::GetInstance()->CreateGroup(key);
    GlobalVariables::GetInstance()->AddItem(key, "type", wave_.type);
    GlobalVariables::GetInstance()->AddItem(key, "interval", wave_.interval);
    GlobalVariables::GetInstance()->AddItem(key, "amount", wave_.amount);
    GlobalVariables::GetInstance()->AddItem(key, "hpMultiplier", wave_.hpMultiplier);
    GlobalVariables::GetInstance()->AddItem(key, "turn", wave_.turn);
    GlobalVariables::GetInstance()->AddItem(key, "time", wave_.time);
}


void EnemyManager::ChangeWave(std::string key, bool resetSpawnCount)
{
    waves_[key].type = static_cast<EnemyBase::Type>(GlobalVariables::GetInstance()->GetValueInt(key, "type"));
    waves_[key].interval = GlobalVariables::GetInstance()->GetValueFloat(key, "interval");
    waves_[key].amount = GlobalVariables::GetInstance()->GetValueInt(key, "amount");
    waves_[key].hpMultiplier = GlobalVariables::GetInstance()->GetValueFloat(key, "hpMultiplier");
    waves_[key].turn = GlobalVariables::GetInstance()->GetValueInt(key, "turn");
    waves_[key].time = GlobalVariables::GetInstance()->GetValueFloat(key, "time");
    if (resetSpawnCount) {
        spawnCount_[key] = 0;
    }
}
