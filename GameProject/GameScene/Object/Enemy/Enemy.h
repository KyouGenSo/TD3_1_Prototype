#pragma once
#include <memory>

#include "Object3d.h"
#include "Camera.h"
#include "GameScene/Object/Object.h"
#include "GameScene/Object/Collision/Collider.h"

class Enemy : public Object
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

    void SetTarget(Object* pTarget) { pTarget_ = pTarget; }



private:
	std::unique_ptr<Object3d> model_;

	Camera* pCamera_ = nullptr;

	std::unique_ptr<Collider> collider_;

	bool isAppearing_ = true;
	float appearCounter_ = 0.0f;

	float targetRotate_ = 2.0f;

    Object* pTarget_ = nullptr;

    float speed_ = 0.2f;

    float appearDuration_ = 60.0f;

    Vector3 defaultScale_ = { 1.0f,1.0f,1.0f };
    Vector3 defaultRotate_ = {};


    //status
    uint16_t hp_ = 5;
};

