#pragma once
#include <string>
#include <mutex>
#include "Collider.h"

class CollisionManager{
    using CollisionPair = std::pair<Collider*, Collider*>;

    std::mutex mutex_;

    // UUID, Collider
	std::unordered_map<std::string, Collider*> pColliders_;

    std::unordered_map<std::string, bool> state_;
    std::unordered_map<std::string, bool> pairs_;

//#ifdef _DEBUG
    //per frame
    struct Debug{
        //frame total
        uint64_t total;
        //filtered count
        uint64_t filtered;
        //detect hit count
        uint64_t collision;
    };

    Debug debug_{};
    //#endif


public:
    CollisionManager() = default;
    ~CollisionManager() = default;

	void Add(Collider* pCollider);
    void Remove(const std::string& uuid);

	void Update();
    void ProcessEvents();

    void ImText() const;
private:
    static std::string CreatePair(const std::string& col, const std::string& other);

    void CheckAll();
	void Check(const std::string& col, const std::string& other);
    void CleanUp();
};

