#include "Status.h"

void Status::Initalize()
{
    hp_ = 0;
    maxHp_ = 0;
    attack_ = 0;
    defence_ = 0;
    speed_ = 0;
    level_ = 0;
    exp_ = 0;
    maxExp_ = 0;
}

void Status::Update()
{
    // 整合性チェック
    if (hp_ > maxHp_)
    {
        hp_ = maxHp_;
    }
    if (exp_ > maxExp_)
    {
        exp_ = maxExp_;
    }
    if (level_ < 1)
    {
        level_ = 1;
    }

    // レベルアップ
    if (exp_ >= maxExp_)
    {
        level_++;
        exp_ -= maxExp_;
        maxExp_ = level_ * 100;
    }
}
