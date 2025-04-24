#pragma once

#include "../Object.h"

class EnemyBase : public Object
{
public:
    void SetTarget(Object* pTarget) { pTarget_ = pTarget; }

protected:
    void HitEffect();

protected:
    Object* pTarget_ = nullptr;
};
