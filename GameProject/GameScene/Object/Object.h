#pragma once

#include <string>
#include <memory>

#pragma comment(lib, "rpcrt4.lib")

#include "Object3d.h"
#include "Object3dBasic.h"
#include "Transform.h"
#include <Input.h>
#include <array>

class Collider;

class Object{
	std::string uuid_;
protected:
    Camera* pCamera_ = nullptr;
    std::unique_ptr<Object3d> model_;
	Transform transform_ = {};
    Vector3 velocity_ = {};
    Vector3 acceleration_ = {};
    bool isDead_ = false;

protected:
    Input* pInput_ = nullptr;

public:
	Object();
	virtual ~Object() = default;

    virtual void Initialize();
    virtual void Update() = 0;
    virtual void Draw() = 0;

    virtual void OnCollisionTrigger(const Collider* pObject){}
    virtual void OnCollision(const Collider* pObject){}
    virtual void OnCollisionExit(const Collider* pObject){}


public: /// Setter
    void SetTransform(const Transform& _transform) {
        transform_ = _transform;
    }
    void SetPosition(const Vector3& _position) {
        transform_.translate = _position;
    }
    void SetRotation(const Vector3& _rotation) {
        transform_.rotate = _rotation;
    }
    void SetScale(const Vector3& _scale) {
        transform_.scale = _scale;
    }


public: /// Getter
	const Transform& GetTransform() const {
	    return transform_;
    }

    const std::string& GetUniqueId() {
        return uuid_;
    }

    bool IsDead() const
    {
        return isDead_;
    }
};

inline Object::Object() {
    UUID uuid;
    UuidCreate(&uuid);
    RPC_CSTR szUuid = nullptr;
    UuidToStringA(&uuid, &szUuid);
    struct UUIDCleaner{
        RPC_CSTR& ptr;
        ~UUIDCleaner() {
            if (ptr)RpcStringFreeA(&ptr);
        }
    } cleaner {szUuid};
    uuid_ = reinterpret_cast<char*>(szUuid);

    pCamera_ = Object3dBasic::GetInstance()->GetCamera();
}

inline void Object::Initialize()
{
    pInput_ = Input::GetInstance();
}

