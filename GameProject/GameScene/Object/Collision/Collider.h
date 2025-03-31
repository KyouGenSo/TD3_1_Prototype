#pragma once
#include <functional>
#include <string>
#include <variant>

#include "GameScene/Object/Object.h"

class CollisionManager;

//Component
class Collider{
public:
    enum class Type{
        ALLY,
        P_BULLET,
        ENEMY,
        STAGE
    };
    enum class Event{
        TRIGGER,
        STAY,
        EXIT
    };

protected:
	CollisionManager* pManager_;

    Object* pOwner_ = nullptr;

    Vector3 position_{};
	std::variant<float, Vector3> size_;

    std::function<void(const Collider*)> onCollisionTrigger_;
    std::function<void(const Collider*)> onCollision_;
    std::function<void(const Collider*)> onCollisionExit_;

    Type type_ = Type::ALLY;

    uint32_t attribute_ = 0b0;
    uint32_t ignore_ = 0b0;

    bool disable_ = false;

public:
    Collider();
	Collider(Object* _owner);
    virtual ~Collider();

    virtual void Update();

    void OnCollisionTrigger(const Collider* pCollider) const;
	void OnCollision(const Collider* pCollider) const;
    void OnCollisionExit(const Collider* pCollider) const;


    bool IsDisable() const {
        return disable_;
    }

    std::string GetUniqueId() const;

    Collider* SetOwner(Object* pOwner){
        pOwner_ = pOwner;
        return this;
    }

    Object* GetOwner() const{
        return pOwner_;
    }

    Vector3 GetPosition() const;

    std::variant<float, Vector3> GetSize() const;

    uint32_t GetAttribute() const;

    uint32_t GetIgnore() const;

    Type GetType() const;

    Collider* SetEvent(const std::function<void(const Collider*)>& callback, Event event = Event::STAY);
    void Disable();
    Collider* SetPosition(const Vector3 _pos);
    Collider* SetSize(std::variant<float, Vector3> size);
    Collider* SetType(Type type);
    Collider* SetIgnore(Type type);
};

