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

    spawnTimer_ = 0.0f;
    spawnCount_ = 0;

    key_ = "0101";

    InitializeWaveFile("0101");
    InitializeWaveFile("0102");
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
#ifdef _DEBUG
    ImGui::Begin("EnemyManager");
    if (ImGui::RadioButton("Key 0101", keyIndex_ == 0)) {
        keyIndex_ = 0;
        key_ = "0101";
    }
    if (ImGui::RadioButton("Key 0102", keyIndex_ == 1)) {
        keyIndex_ = 1;
        key_ = "0102";
    }
    if(ImGui::Button("ChangeWave")) {
        ChangeWave(key_);
    }
    if (ImGui::Button("Add Enemy")) {
        for (int i = 0; i < 3; ++i) {
            AddEnemy({ appearancePos_.x * i, appearancePos_.y, appearancePos_.z });
        }
    }
    ImGui::DragInt("SpawnAmount", &waves_[key_].amount, 1);
    ImGui::DragFloat("SpawnInterval", &waves_[key_].interval, 0.1f);
    ImGui::End();
#endif
}

void EnemyManager::SpawnEnemy()
{
    if (spawnCount_ >= waves_[key_].amount) {
        return;
    }

    spawnTimer_ += deltaTime_;
    if (spawnTimer_ > waves_[key_].interval) {
        AddEnemy(RandomSpawnPosition());
        spawnTimer_ = 0.0f;
        spawnCount_++;
    }
}

void EnemyManager::SetMinimap(Minimap* pMinimap) {
    pMinimap_ = pMinimap;
}

void EnemyManager::InitializeWaveFile(std::string key)
{
    GlobalVariables::GetInstance()->CreateGroup(key);
    GlobalVariables::GetInstance()->AddItem(key, "type", wave_.type);
    GlobalVariables::GetInstance()->AddItem(key, "interval", wave_.interval);
    GlobalVariables::GetInstance()->AddItem(key, "amount", wave_.amount);
    GlobalVariables::GetInstance()->AddItem(key, "hpMultiplier", wave_.hpMultiplier);
    GlobalVariables::GetInstance()->LoadFile(key);
}

Vector3 EnemyManager::RandomSpawnPosition()
{
    Vector3 randomPos = {};

    while ((randomPos.x < maxSpawnRange_.x && randomPos.x > minSpawnRange_.x) && (randomPos.z < maxSpawnRange_.z && randomPos.z > minSpawnRange_.z)) {

        std::uniform_real_distribution<float> disX(minSpawnPoint_.x, maxSpawnPoint_.x);
        std::uniform_real_distribution<float> disY(minSpawnPoint_.y, maxSpawnPoint_.y);
        std::uniform_real_distribution<float> disZ(minSpawnPoint_.z, maxSpawnPoint_.z);

        randomPos = Vector3{ disX(gen_), disY(gen_), disZ(gen_) };
    }

    return randomPos;
}

void EnemyManager::ChangeWave(std::string key)
{
    key_ = key;
    waves_[key_].type = static_cast<Type>(GlobalVariables::GetInstance()->GetValueInt(key_, "type"));
    waves_[key_].interval = GlobalVariables::GetInstance()->GetValueFloat(key_, "interval");
    waves_[key_].amount = GlobalVariables::GetInstance()->GetValueInt(key_, "amount");
    waves_[key_].hpMultiplier = GlobalVariables::GetInstance()->GetValueFloat(key_, "hpMultiplier");
    spawnCount_ = 0;
}
