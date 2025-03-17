#pragma once

#include <Object3d.h>
#include <memory>

#include "GameScene/Object/Collision/Collider.h"

class Terrain
{
public:
    Terrain() = default;
    ~Terrain() = default;
    void Initialize();
    void Update();
    void Draw();
    void Finalize();

    void ImGui();

    void OnCollision(const Collider* pCollider) const;

public:
    float GetFloorHeight() const;

private:
    std::unique_ptr<Object3d> terrainObj_;
    std::unique_ptr<Object3d> boxObj_;
    Transform transform_;

    std::unique_ptr<Collider> collider_;
};
