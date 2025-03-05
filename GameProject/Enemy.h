#pragma once
#include <memory>

#include "Object.h"
#include "Object3d.h"
#include "Camera.h"
#include "Collider.h"

class Enemy : public Object
{
public:
	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	bool IsDead() override;

	void OnCollisionTrigger(const Object* eObject) override;

private:
	std::unique_ptr<Object3d> model_;

	Transform transform_{};

	Camera* eCamera_ = nullptr;

	std::unique_ptr<Collider> collider_;
};

