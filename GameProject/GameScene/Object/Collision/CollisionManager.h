#pragma once
#include <string>
#include <vector>
#include "Collider.h"

class CollisionManager{
	//uuid , uuid
    using Pair = std::pair<std::string, std::string>;

	std::unordered_map<std::string, Collider*> pColliders_; 
    std::vector<Pair> pairs_;

public:
	void Add(Collider* pCollider);
	void Remove(const Collider* pCollider);

	void Update();

private:
	void CheckAll();
	void Check(const std::string& col, const std::string& other);
};

