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

private:
    std::unique_ptr<Object3d> object_;
    Transform transform_;

    std::unique_ptr<Collider> collider_;
};
