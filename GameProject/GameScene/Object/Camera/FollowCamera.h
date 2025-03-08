#pragma once
#include "Camera.h"

class FollowCamera{
	Camera* pCamera_ = nullptr;

	const Transform* pTarget_ = nullptr;

    Vector3 targetPositionPre_ = {};

    Vector3 shiftDirection_ = { 0.0f, 0.0f, -1.0f };

	float offset_ = 20.0f;

	float yaw_ = 0.f;

    float factorLerp_ = 0.2f;

public:
	void Initialize();
	void Update();
	void Finalize();

    void SetTarget(const Transform* pTarget) {pTarget_ = pTarget;}

    void SetTranslate(const Vector3& translate) { pCamera_->SetTranslate(translate); }

	void ImGui();
};

