#pragma once
#include <vector>
#include <memory>
#include "Enemy.h"

class EnemyManager {
public:
    void Initialize();
    void Update();
    void Draw();
    void Finalize();
	void AddEnemy(const Vector3& position);
    void ImGui();

    void SetTarget(Object* pTarget) { pTarget_ = pTarget; }

private:
    std::vector<std::unique_ptr<Enemy>> enemies_;

    Object* pTarget_ = nullptr;
};