#pragma once
#include <vector>
#include "Collider.h"

class CollisionManager{
    std::vector<Collider*> pColliders_; 

public:
	void Add(Collider* pCollider);
	void Remove(Collider* pCollider);

	void Update();

private:
	void CheckAll();
	void Check(const Collider* col, const Collider* other);
};

