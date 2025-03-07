#pragma once
#include <memory>

#include "Object.h"
#include "Camera.h"
#include "Chain.h"
#include "Collider.h"
#include <Weapon.h>

class Player : public Object {
    std::unique_ptr<Collider> collider_;
    std::unique_ptr<ChainManager> chainManager_;
    std::unique_ptr<Weapon> weapon_;

    Vector3 move_{};

    bool isGround_ = true;

    const float FLOOR = 0.f;
    const float GRAVITY = -.16f;
public:
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Finalize();
    void ImGui();
    void OnCollision(const Object* pObject) override;

private:
    void UpdateInputCommands();
    void UpdateMovement();
};

