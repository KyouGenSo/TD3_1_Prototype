#pragma once
#include <memory>

#include "Object3d.h"
#include "Camera.h"
#include "GameScene/Object/Collision/Collider.h"
#include "EnemyBase.h"

class Enemy : public EnemyBase
{
public:
	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Finalize();

	void OnCollision(const Collider* pCollider) override;

    void Move();

    void AppearanceProduction();

	void SetTranslate(Vector3 translate) { transform_.translate = translate; }

	void SetIsAppearing(bool isAppearing) { isAppearing_ = true; }

	void SetAppearCounter(float appearCounter) { appearCounter_ = appearCounter; }

	float GetAmoRotate() { return amoRotate; }

    Vector3 GetDirection() { return direction; }


private:

	std::unique_ptr<Collider> collider_;

	Vector3 prePos;

	bool isAppearing_;
	float appearCounter_ = 0.0f;

	float amoRotate = 0.1f;
	float targetRotate = 2.0f;
	float rotateSpeed = 0.1f;
	float preRotateSpeed = 0.1f;

    Vector3 direction{ 0.0f,0.0f,0.0f };

    float speed = 0.2f;

    float appearDuration = 60.0f;

    Vector3 defaultScale = { 1.0f,1.0f,1.0f };
    Vector3 defaultRotate = { 0.0f,0.0f,0.0f };


    //status
    uint16_t hp_ = 5;
};

