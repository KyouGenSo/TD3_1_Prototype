#pragma once
#include "Object.h"

class Weapon : public Object{
protected:
	struct Status{
		
	};


	bool isRoot_ = false;
	Status status_;

	Object* pOwner_ = nullptr;

public:
	void Fire();
	void Execute();

	void Update() override = 0;
    void Draw() override = 0;
	bool IsDead() override;
protected:
	void Initialize() override = 0;
};

