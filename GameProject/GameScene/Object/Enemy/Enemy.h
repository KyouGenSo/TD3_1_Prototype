#pragma once
#include <memory>

#include "Object3d.h"
#include "Camera.h"
#include "GameScene/Object/Object.h"
#include "GameScene/Object/Collision/Collider.h"

class Enemy : public Object
{
public:
	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	void OnCollision(const Object* pObject) override;

	void SetTranslate(Vector3 translate) { transform_.translate = translate; }

	void SetTarget(Object* pTarget) { pTarget_ = pTarget; }

private:
	std::unique_ptr<Object3d> model_;

	Camera* eCamera_ = nullptr;

	std::unique_ptr<Collider> collider_;

	Vector3 prePos;

	Object* pTarget_ = nullptr;
};

