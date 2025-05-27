#pragma once
#include <memory>

#include "EnemyBase.h"

class EnemyFactory
{
public:
    static std::unique_ptr<EnemyBase> CreateEnemy(EnemyBase::Type _type);
};

