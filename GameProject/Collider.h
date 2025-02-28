#pragma once
#include <functional>
#include "Object.h"
#include "Transform.h"

class CollisionManager;

//Component
class Collider{
private:
    //RULES
	static constexpr uint32_t ALLY = 0b1;
    static constexpr uint32_t ENEMY = 0b1 << 1;

private:
	CollisionManager* pManager_;

    Object* pOwner_ = nullptr;

	float radius_ = 1.f;

    std::function<void()> onCollision_;


    uint32_t attribute_ = 0xffffffff;
    uint32_t ignore_ = 0b0;

public:
	Collider();
    ~Collider() = default;

    void Update();

	void OnCollision() const;

    void SetOwner(Object* pOwner){
        pOwner_ = pOwner;
    }

	Transform& GetTransform() const;

    float GetRadius() const{
        return radius_;
    }

    void SetRadius(float r);

    uint32_t GetAttribute() const{
        return attribute_;
    }

    void SetAttribute(const uint32_t attr){
        attribute_ = attr;
    }

    uint32_t GetIgnore() const {
        return ignore_;
    }

    void SetIgnore(const uint32_t ignore) {
        ignore_ = ignore;
    }
};

