#pragma once
#include <string>
#include <vector>
#include "Collider.h"
#include <mutex>

class CollisionManager{
	//uuid , uuid
    using Pair = std::pair<std::string, std::string>;
    using ColliderPair = std::pair<Collider*, Collider*>;

	std::unordered_map<std::string, Collider*> pColliders_;
    std::vector<ColliderPair> pendingOnCollision_;
    std::vector<ColliderPair> pendingOnCollisionTrigger_;
    std::vector<ColliderPair> pendingOnCollisionExit_;
    std::vector<Pair> pairs_;

    std::timed_mutex mtx_;

//#ifdef _DEBUG
    struct Debug{
        uint64_t total;
        uint64_t frame;
        uint64_t filtered;
    };

    Debug debug_{};
//#endif


public:
	void Add(Collider* pCollider);
    void Remove(const std::string& uuid);

	void Update();
    void CallOnCollision();

private:
	void CheckAll();
	void Check(const std::string& col, const std::string& other);

};

