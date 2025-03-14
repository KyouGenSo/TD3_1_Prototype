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

    void SetMinimap(Minimap* pMinimap);

    void SetTarget(Object* pTarget) { pTarget_ = pTarget; }

private:
    std::vector<std::unique_ptr<Enemy>> enemies_;
    Minimap* pMinimap_ = nullptr;

    Object* pTarget_ = nullptr;
    Object* pPlayer_ = nullptr;
    Object* pCastle_ = nullptr;

    const float leave = 10.0f;
};