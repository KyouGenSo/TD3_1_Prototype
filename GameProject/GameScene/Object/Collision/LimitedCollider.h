#pragma once
#include "Collider.h"

class LimitedCollider : public Collider{
    uint32_t lifetime_ = 0;

public:
    explicit LimitedCollider(uint32_t _lifetime) : lifetime_(_lifetime), Collider() {}
    explicit LimitedCollider(Object* _owner, uint32_t _lifetime) : Collider(_owner), lifetime_(_lifetime) {}
    void Update() override;
};

