#pragma once
#include <vector>
#include <memory>
#include "Enemy.h"
#include "GameScene/HUD/Minimap.h"

class EnemyManager {
public:
    void Initialize();
    void Update();
    void Draw();
    void Finalize();
	void AddEnemy(const Vector3& position);
    void ImGui();

    void SetMinimap(Minimap* pMinimap);

private:
    std::vector<std::unique_ptr<Enemy>> enemies_;
    Minimap* pMinimap_ = nullptr;
};
