#pragma once
#include <string>
#include <rpc.h>

#pragma comment(lib, "rpcrt4.lib")

#include "Transform.h"

class Object{
	std::string uuid_;
protected:
	Transform transform_ {};

public:
	Object();
	virtual ~Object() = default;
    virtual void OnCollision(const Object* pObject){}
    virtual void OnCollisionTrigger(const Object* pObject){}
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
}

