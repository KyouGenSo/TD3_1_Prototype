#include "Chain.h"

void Chain::SetTypes(Type _1, Type _2, Type _3, Type _4)
{
    types_[0] = _1;
    types_[1] = _2;
    types_[2] = _3;
    types_[3] = _4;
}

void Chain::Initialize() 
{
    for (auto& type : types_)
    {
        coolTimeCounter_[type].Reset();
        coolTimeCounter_[type].Start();
    }
}

void Chain::Update()
{
    /// クールタイムの更新
    for (const auto& type : types_)
    {
        auto now = coolTimeCounter_[type].GetNow();
        float cooltime = COOLTIMES.at(type) - now;

        /// クールタイムがあがっていれば0にする
        if (cooltime <= 0) cooltime = 0;

        coolTimes_[type] = cooltime;
    }
}
