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

    void Move();

    void AppearanceProduction();

	void SetTranslate(Vector3 translate) { transform_.translate = translate; }

	void SetIsAppearing(bool isAppearing) { isAppearing_ = true; }

	void SetAppearCounter(float appearCounter) { appearCounter_ = appearCounter; }

	float GetAmoRotate() { return amoRotate; }


private:
	std::unique_ptr<Object3d> model_;

	Camera* pCamera_ = nullptr;

	std::unique_ptr<Collider> collider_;

	Vector3 prePos;

	bool isAppearing_;
	float appearCounter_ = 0.0f;

	float rotation = 0.0f;

	float amoRotate = 0.1f;
	float targetRotate = 2.0f;
	float rotateSpeed = 0.1f;
	float preRotateSpeed = 0.1f;
};

