#pragma once
#include "Object.h"
#include "Camera.h"
#include "Collider.h"

#include "Chain.h"

class Player : public Object{
    std::unique_ptr<Collider> collider_;

    std::unique_ptr<Chain> chain_;

    Vector3 move_ {};

    bool isGround_ = true;

    const float FLOOR = 0.f;
    const float GRAVITY = -.16f;
public:
	void Initialize() override;
    void Update() override;
    void Draw() override;
    void Finalize();
    void ImGui();
    bool IsDead() override;
    void OnCollision(const Object* pObject) override;
};

