#pragma once
#include <string>
#include <memory>

#pragma comment(lib, "rpcrt4.lib")

#include "Object3d.h"
#include "Object3dBasic.h"
#include "Transform.h"
#include <Input.h>
#include <Collision/Collider.h>
#include <array>
#include <GameScene/Status/Status.h>

#include "EmitterManager.h"

class Object{
	std::string uuid_;
protected:
    Camera* pCamera_ = nullptr;
    std::unique_ptr<Object3d> model_;

    std::unique_ptr<Collision::Collider> pCollider_;

	Transform transform_ = {};
    Vector3 velocity_ = {};
    Vector3 acceleration_ = {};
    float gravity_ = 9.8f;
    float mass_ = 30.0f;

    float deltaTime_ = 0.0f;
    bool isDead_ = false;
    Status statusInit_ = {};
    Status statusCurrent_ = {};

    EmitterManager* emitter_ = nullptr;

protected:
    Input* pInput_ = nullptr;

public:
    Object();
    virtual ~Object() = default;

    virtual void Initialize();
    virtual void Update();
    virtual void Draw() = 0;

    virtual void OnCollisionTrigger(const Collision::Collider* pObject){}
    virtual void OnCollision(const Collision::Collider* pObject){}
    virtual void OnCollisionExit(const Collision::Collider* pObject){}


public: /// Setter
    void SetTransform(const Transform& _transform)
    {
        transform_ = _transform;
    }
    void SetPosition(const Vector3& _position)
    {
        transform_.translate = _position;
    }
    void SetRotation(const Vector3& _rotation)
    {
        transform_.rotate = _rotation;
    }
    void SetScale(const Vector3& _scale)
    {
        transform_.scale = _scale;
    }

    void SetEmitter(EmitterManager* _emitter)
    {
        emitter_ = _emitter;
    }

public: /// Getter
    const Transform& GetTransform() const
    {
        return transform_;
    }

    const std::string& GetUniqueId()
    {
        return uuid_;
    }

    bool IsDead() const
    {
        return isDead_;
    }

    Status& getStatusCurrent()
    {
        return statusCurrent_;
    }

    Status& getStatusInit()
    {
        return statusInit_;
    }

protected:
    void DebugObject();
    void ApplyForce(const Vector3& _force)
    {
        acceleration_ += _force / mass_;
    }
    void ApplyFriction(const float _frictionCoef)
    {
        Vector3 frictionForce = velocity_ * -_frictionCoef;
        velocity_ += frictionForce * deltaTime_;
    }

    static Collision::Vec3 Adaptor(Vector3 _vec)
    {
        return Collision::Vec3(_vec.x, _vec.y, _vec.z);
    }

    void StatusUpdateOnCollision(const Collision::Collider* pObject);
};

inline Object::Object()
{
    UUID uuid;
    UuidCreate(&uuid);
    RPC_CSTR szUuid = nullptr;
    UuidToStringA(&uuid, &szUuid);
    struct UUIDCleaner
    {
        RPC_CSTR& ptr;
        ~UUIDCleaner()
        {
            if (ptr)RpcStringFreeA(&ptr);
        }
    } cleaner{ szUuid };
    uuid_ = reinterpret_cast<char*>(szUuid);

    pCamera_ = Object3dBasic::GetInstance()->GetCamera();
}

inline void Object::Initialize()
{
    pInput_ = Input::GetInstance();
    statusCurrent_ = statusInit_;
}

inline void Object::Update()
{
    if (isDead_) return;

    if (statusCurrent_.getHp() <= 0.0f)
    {
        isDead_ = true;
    }
}

