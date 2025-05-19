#pragma once

#include <GameScene/Object/Object.h>
#include <Object3d.h>
#include <memory>

#include <Collision/Collider.h>

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
};
