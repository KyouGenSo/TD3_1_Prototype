#pragma once

#include <string>
#include <memory>

#pragma comment(lib, "rpcrt4.lib")

#include "Object3d.h"
#include "Object3dBasic.h"
#include "Transform.h"

class Object{
	std::string uuid_;
protected:
    Camera* pCamera_ = nullptr;
    std::unique_ptr<Object3d> model_;
	Transform transform_ {};

public:
	Object();
	virtual ~Object() = default;

    virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;

    virtual void OnCollisionTrigger(const Object* pObject){}
    virtual void OnCollision(const Object* pObject){}
    virtual void OnCollisionExit(const Object* pObject){}
	virtual bool IsDead() = 0;

	Transform& GetTransform(){
	    return transform_;
	}

    const std::string& GetUniqueId(){
        return uuid_;
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

