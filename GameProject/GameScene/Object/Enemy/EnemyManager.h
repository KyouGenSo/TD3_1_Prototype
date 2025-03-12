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

private:
    std::vector<std::unique_ptr<Enemy>> enemies_;
};