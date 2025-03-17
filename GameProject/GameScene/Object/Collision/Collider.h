#pragma once
#include <functional>
#include <string>
#include <variant>

#include "GameScene/Object/Object.h"

class CollisionManager;

//Component
class Collider{
private:
    //RULES
	static constexpr uint32_t ALLY = 0b1;
    static constexpr uint32_t ENEMY = 0b1 << 1;
    static constexpr uint32_t STAGE = 0b1 << 2;

public:
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

    uint32_t attribute_ = 0xffffffff;
    uint32_t ignore_ = 0b0;

    bool disable_ = false;

public:
    Collider();
	Collider(Object* _owner);
    virtual ~Collider() = default;

    virtual void Update();

    void OnCollisionTrigger(const Collider* pCollider) const;
	void OnCollision(const Collider* pCollider) const;
    void OnCollisionExit(const Collider* pCollider) const;

    void SetEvent(const std::function<void(const Collider*)>& callback, Event event = Event::STAY);

    bool IsDisable() const {
        return disable_;
    }

    std::string GetUniqueId() const;

    void SetOwner(Object* pOwner){
        pOwner_ = pOwner;
    }

    Object* GetOwner() const{
        return pOwner_;
    }

    Vector3 GetPosition() const;

    std::variant<float, Vector3> GetSize() const;

    void SetSize(std::variant<float, Vector3> size);


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

    void Disable(){
        disable_ = true;
    }

    void SetPosition(const Vector3 _pos){
        position_ = _pos;
    }
};

