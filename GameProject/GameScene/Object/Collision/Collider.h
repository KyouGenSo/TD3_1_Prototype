#pragma once
#include <functional>
#include <string>

#include "Transform.h"
#include "GameScene/Object/Object.h"

class CollisionManager;

//Component
class Collider{
private:
    //RULES
	static constexpr uint32_t ALLY = 0b1;
    static constexpr uint32_t ENEMY = 0b1 << 1;

public:
    enum class Event{
        TRIGGER,
        STAY,
        EXIT
    };

private:
	CollisionManager* pManager_;

    Object* pOwner_ = nullptr;

	float radius_ = 1.f;

    std::function<void(const Object*)> onCollisionTrigger_;
    std::function<void(const Object*)> onCollision_;
    std::function<void(const Object*)> onCollisionExit_;

    uint32_t attribute_ = 0xffffffff;
    uint32_t ignore_ = 0b0;

public:
	Collider(Object* _owner);
    ~Collider() = default;

    void Update();

    void OnCollisionTrigger(const Object* pObject) const;
	void OnCollision(const Object* pObject) const;
    void OnCollisionExit(const Object* pObject) const;

    void SetEvent(const std::function<void(const Object*)>& callback, Event event = Event::STAY);

    std::string GetUniqueId() const {
        return pOwner_->GetUniqueId();
    }

    void SetOwner(Object* pOwner){
        pOwner_ = pOwner;
    }

    Object* GetOwner() const{
        return pOwner_;
    }

	const Transform& GetTransform() const;

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

