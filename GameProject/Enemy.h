#pragma once
#include <memory>

#include "Object3d.h"
#include "Camera.h"

class Enemy
{
public:
	void Initialize();

	void Update();

	void Draw();

	void Finalize();

private:
	std::unique_ptr<Object3d> model_;

	Transform transform_{};

	Camera* eCamera_ = nullptr;
};

