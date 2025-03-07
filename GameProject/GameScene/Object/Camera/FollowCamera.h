#pragma once
#include "Camera.h"

class FollowCamera{
	Camera* pCamera_ = nullptr;

	const Transform* pTarget_ = nullptr;

    Vector3 offset_ = {0, 3.f, -18.f};

	float yaw = 0.f;

public:
	void Initialize();
	void Update();
	void Finalize();

    void SetTarget(const Transform* pTarget) {pTarget_ = pTarget;}

	void ImGui();
};

