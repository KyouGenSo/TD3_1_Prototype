#pragma once

#include <GameScene/Object/Object.h>
#include <Object3d.h>
#include <memory>

#include <Collision/Collider.h>

#include "AABB.h"

class Terrain : public Object
{
public:
    Terrain() = default;
    ~Terrain() = default;
    void Initialize();
    void Update();
    void Draw();
    void Finalize();

    void ImGui();

    void OnCollision(const Collision::Collider* pCollider) const;

public:
    float GetFloorHeight() const;

private:
    std::unique_ptr<Collision::Collider> collider_;
    std::unique_ptr<Collision::Collider> longWall1Collider_;
    std::unique_ptr<Collision::Collider> longWall2Collider_;
    std::unique_ptr<Collision::Collider> shortWall1Collider_;
    std::unique_ptr<Collision::Collider> shortWall2Collider_;

    std::unique_ptr<Object3d> longWallModel1_;
    Transform longWallTransform1_;

    std::unique_ptr<Object3d> longWallModel2_;
    Transform longWallTransform2_;

    std::unique_ptr<Object3d> shortWallModel1_;
    Transform shortWallTransform1_;

    std::unique_ptr<Object3d> shortWallModel2_;
    Transform shortWallTransform2_;

    Transform wallUvTransform;
};
