#pragma once
#include "Camera.h"

class FollowCamera{
	Camera* pCamera_ = nullptr;
	const Transform* pTarget_ = nullptr;

    // Parameters
    Vector3 targetPositionPre_ = {};
    Vector3 shiftDirection_ = { 0.04f, 0.121f, -0.992f };
    Vector3 targetPositionOffset_ = { 0.0f, 0.0f, 0.0f };
	float offset_ = 20.0f;
    float factorLerp_ = 0.2f;
    float rotationX_ = 0.1f;

public:
	void Initialize();
	void Update();
	void Finalize();

    void SetTarget(const Transform* pTarget) {pTarget_ = pTarget;}

    void SetTranslate(const Vector3& translate) { pCamera_->SetTranslate(translate); }

	void ImGui();
};

