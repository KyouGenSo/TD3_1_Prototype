#include "EnemyFactory.h"

#include "BounceEnemy.h"
#include "Enemy.h"
#include "FlyEnemy.h"

std::unique_ptr<EnemyBase> EnemyFactory::CreateEnemy(EnemyBase::Type _type)
{
    switch (_type)
    {
    case EnemyBase::Normal:
        return std::make_unique<Enemy>();
    case EnemyBase::Fly:
        return std::make_unique<FlyEnemy>();
    case EnemyBase::Bounce:
        return std::make_unique<BounceEnemy>();
    default:
        return nullptr;
    }
}
