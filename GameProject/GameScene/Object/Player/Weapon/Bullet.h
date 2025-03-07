#pragma once
#include <memory>

#include "GameScene/Object/Object.h"
#include "GameScene/Object/Collision/Collider.h"

class Bullet : public Object{
    std::unique_ptr<Collider> collider_;
    Bullet* next_ = nullptr;
};

