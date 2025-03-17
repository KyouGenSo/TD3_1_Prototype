#pragma once
#include <memory>

#include "Object3d.h"
#include "Camera.h"
#include "GameScene/Object/Object.h"
#include "GameScene/Object/Collision/Collider.h"

class Boss : public Object
{
public:
	void Initialize();

	void Update();

	void Draw();

	void Finalize();

    void ImGui();

	void OnCollision(const Object* pObject) override;

    bool GetIsValid() const { return isValid_; }

    void SetIsValid(bool isValid) { isValid_ = isValid; }

private:
	std::unique_ptr<Object3d> model_;

	Camera* bCamera_ = nullptr;

	std::unique_ptr<Collider> collider_;

	Vector3 prePos;

    bool isValid_; // ボスの有効無効
};

