#include "EnemyManager.h"

#include "imgui.h"
#include "GameScene/Object/Player/Player.h"

#include <random>

#include "GlobalVariables.h"

EnemyManager::EnemyManager()
    : gen_(rd_())
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Initialize(Object* player, Object* castle)
{
    pPlayer_ = player;
    pCastle_ = castle;

    spawnTimer_.clear();
    spawnCount_.clear();

    keys_.clear();


    InitializeWaveFile("0101");
    InitializeWaveFile("0102");
    InitializeWaveFile("0103");

}

void EnemyManager::Update()
{
    if (!keys_.empty()) {
        SpawnEnemy();
    }
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
    for (auto enemy = flyEnemies_.begin(); enemy != flyEnemies_.end(); ) {
        if ((*enemy)->IsDead()) {
            enemy = flyEnemies_.erase(enemy);
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
    for (auto& enemy : flyEnemies_) {
        enemy->Draw();
    }
}

void EnemyManager::Finalize()
{
    for (auto& enemy : enemies_) {
        enemy->Finalize();
    }
    enemies_.clear();
    for (auto& enemy : flyEnemies_) {
        enemy->Finalize();
    }
    flyEnemies_.clear();
}

void EnemyManager::AddEnemy(const Vector3& position, Type type)
{
    if (type == Type::Normal) {
        auto enemy = std::make_unique<Enemy>();
        enemy->Initialize();
        enemy->SetTranslate(position);
        enemy->SetIsAppearing(true);
        enemy->SetAppearCounter(0.0f);
        //pMinimap_->Register(enemy.get());
        enemies_.push_back(std::move(enemy));
    }
    else if (type == Type::Fly) {
        auto flyEnemy = std::make_unique<FlyEnemy>();
        flyEnemy->Initialize();
        flyEnemy->SetTranslate(position);
        flyEnemy->SetIsAppearing(true);
        flyEnemy->SetAppearCounter(0.0f);
        flyEnemies_.push_back(std::move(flyEnemy));
    }
}

void EnemyManager::SelectTarget(Object* enemy)
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
#ifdef _DEBUG
    ImGui::Begin("EnemyManager");
    if(ImGui::Button("TurnControl")) {
        TurnControl();
        turnProgress++;
    }
    ImGui::DragInt("turnProgress", &turnProgress, 1);
    ImGui::End();
#endif
}

void EnemyManager::SpawnEnemy()
{
    if (keys_.empty()) {
        return;
    }
    for (const auto& key : keys_) {
        if (spawnCount_[key] >= waves_[key].amount) {
            continue;
        }

        spawnTimer_[key] += deltaTime_;
        if (spawnTimer_[key] > waves_[key].interval) {
            AddEnemy(RandomSpawnPosition(waves_[key].type),waves_[key].type);
            spawnTimer_[key] = 0.0f;
            spawnCount_[key]++;
        }
    }
}

void EnemyManager::SetMinimap(Minimap* pMinimap) {
    pMinimap_ = pMinimap;
}

void EnemyManager::InitializeWaveFile(std::string key)
{
    GlobalVariables::GetInstance()->CreateGroup(key);
    GlobalVariables::GetInstance()->LoadFile(key);
    spawnCount_[key] = 0;

    Wave wave;
    wave.type = static_cast<Type>(GlobalVariables::GetInstance()->GetValueInt(key, "type"));
    wave.interval = GlobalVariables::GetInstance()->GetValueFloat(key, "interval");
    wave.amount = GlobalVariables::GetInstance()->GetValueInt(key, "amount");
    wave.hpMultiplier = GlobalVariables::GetInstance()->GetValueFloat(key, "hpMultiplier");
    wave.turn = GlobalVariables::GetInstance()->GetValueInt(key, "turn");
    waves_[key] = wave;
}

void EnemyManager::TurnControl()
{
    keys_.clear();
    for (const auto& wave : waves_) {
        if (turnProgress == wave.second.turn) {
            keys_.push_back(wave.first);
        }
    }
    for (const auto& key : keys_) {
        ChangeWave(key, true);
    }
}

Vector3 EnemyManager::RandomSpawnPosition(Type type)
{
    Vector3 randomPos = {};

    if (type == Type::Normal) {
        while ((randomPos.x < maxSpawnRange_.x && randomPos.x > minSpawnRange_.x) && (randomPos.z < maxSpawnRange_.z && randomPos.z > minSpawnRange_.z)) {

            std::uniform_real_distribution<float> disX(minSpawnPoint_.x, maxSpawnPoint_.x);
            std::uniform_real_distribution<float> disY(minSpawnPoint_.y, maxSpawnPoint_.y);
            std::uniform_real_distribution<float> disZ(minSpawnPoint_.z, maxSpawnPoint_.z);
            randomPos = Vector3{ disX(gen_), disY(gen_), disZ(gen_) };
        }
    }
    else if (type == Type::Fly) {
        while ((randomPos.x < flyMaxSpawnRange_.x && randomPos.x > flyMinSpawnRange_.x) && (randomPos.z < flyMaxSpawnRange_.z && randomPos.z > flyMinSpawnRange_.z)) {
            std::uniform_real_distribution<float> disX(flyMinSpawnPoint_.x, flyMaxSpawnPoint_.x);
            std::uniform_real_distribution<float> disY(flyMinSpawnPoint_.y, flyMaxSpawnPoint_.y);
            std::uniform_real_distribution<float> disZ(flyMinSpawnPoint_.z, flyMaxSpawnPoint_.z);
            randomPos = Vector3{ disX(gen_), disY(gen_), disZ(gen_) };
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
}

void EnemyManager::ChangeWave(std::string key, bool resetSpawnCount)
{
    waves_[key].type = static_cast<Type>(GlobalVariables::GetInstance()->GetValueInt(key, "type"));
    waves_[key].interval = GlobalVariables::GetInstance()->GetValueFloat(key, "interval");
    waves_[key].amount = GlobalVariables::GetInstance()->GetValueInt(key, "amount");
    waves_[key].hpMultiplier = GlobalVariables::GetInstance()->GetValueFloat(key, "hpMultiplier");
    waves_[key].turn = GlobalVariables::GetInstance()->GetValueInt(key, "turn");
    if (resetSpawnCount) {
        spawnCount_[key] = 0;
    }
}
