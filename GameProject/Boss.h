#pragma once
#include <memory>

#include "Object.h"
#include "Object3d.h"
#include "Camera.h"
#include "Collider.h"

class Boss : public Object
{
public:
	void Initialize();

	void Update();

	void Draw();

	void Finalize();

	bool IsDead() override;

	void OnCollision(const Object* pObject) override;

private:
	std::unique_ptr<Object3d> model_;

	Camera* bCamera_ = nullptr;

	std::unique_ptr<Collider> collider_;

	Vector3 prePos;
};

