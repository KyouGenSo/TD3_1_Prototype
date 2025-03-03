#pragma once
#include <memory>

#include "Object.h"
#include "Object3d.h"
#include "Camera.h"
#include "Collider.h"

class Player : public Object{
    std::unique_ptr<Object3d> model_;

    Camera* pCamera_ = nullptr;

    std::unique_ptr<Collider> collider_;

    Vector3 move_ {};

    bool isGround_ = true;

    const float FLOOR = 0.f;
    const float GRAVITY = -.16f;
public:
	void Initialize();
    void Update();
    void Draw();
    void Finalize();
    void ImGui();
    bool IsDead() override;
    void OnCollision(const Object* pObject) override;
};

