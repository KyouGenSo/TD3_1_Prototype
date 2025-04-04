#pragma once
#include <memory>

#include "Object3d.h"
#include "Camera.h"
#include "GameScene/Object/Object.h"
#include "Collision/Collider.h"

class Enemy : public Object
{
public:
	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize();

	void OnCollision(const Collision::Collider* pCollider) override;

    void Move();

    void AppearanceProduction();

	void SetTranslate(Vector3 translate) { transform_.translate = translate; }

	void SetIsAppearing(bool isAppearing) { isAppearing_ = true; }

	void SetAppearCounter(float appearCounter) { appearCounter_ = appearCounter; }

	float GetAmoRotate() { return amoRotate; }

    void SetTarget(Object* pTarget) { pTarget_ = pTarget; }

    Vector3 GetDirection() { return direction; }


private:
	std::unique_ptr<Object3d> model_;

	Camera* pCamera_ = nullptr;

	Vector3 prePos;

	bool isAppearing_;
	float appearCounter_ = 0.0f;

	float amoRotate = 0.1f;
	float targetRotate = 2.0f;
	float rotateSpeed = 0.1f;
	float preRotateSpeed = 0.1f;

    Object* pTarget_ = nullptr;

    Vector3 direction{ 0.0f,0.0f,0.0f };

    float speed = 0.2f;

    float appearDuration = 60.0f;

    Vector3 defaultScale = { 1.0f,1.0f,1.0f };
    Vector3 defaultRotate = { 0.0f,0.0f,0.0f };


    //status
    uint16_t hp_ = 5;
};

