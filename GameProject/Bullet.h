#pragma once
#include <memory>

#include "Collider.h"
#include "Object.h"
class Bullet : public Object{
    std::unique_ptr<Collider> collider_;
    Bullet* next_ = nullptr;
};

