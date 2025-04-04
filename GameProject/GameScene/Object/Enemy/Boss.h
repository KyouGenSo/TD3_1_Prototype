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
	std::unique_ptr<Object3d> model_;

	Camera* bCamera_ = nullptr;

	Vector3 prePos;

    bool isValid_; // ボスの有効無効
};

