#pragma once
#include "Transform.h"

class Object{
protected:
	Transform transform_ {};

public:
	virtual ~Object() = default;

    virtual void OnCollision(const Object* pObject){}

	Transform& GetTransform(){
	    return transform_;
	}

	virtual bool IsDead() = 0;
};

