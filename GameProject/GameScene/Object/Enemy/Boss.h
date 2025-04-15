#pragma once
#include <memory>

#include "Object3d.h"
#include "Camera.h"
#include "GameScene/Object/Object.h"
#include "Collision/Collider.h"

class Boss : public Object
{
public:
    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Finalize();

    void ImGui();

	void OnCollision(const Collision::Collider* pObject) override;

    bool GetIsValid() const { return isValid_; }

    void SetIsValid(bool isValid) { isValid_ = isValid; }

private:

    std::unique_ptr<Collision::Collider> collider_;

    Vector3 prePos_;

    bool isValid_ = false; // ボスの有効無効
};

