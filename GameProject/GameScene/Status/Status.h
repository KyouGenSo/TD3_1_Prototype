#pragma once

class Status
{
public:
    void Initalize();
    void Update();

    // Getter
    int getHp() const       { return hp_; }
    int getMaxHp() const    { return maxHp_; }
    int getAttack() const   { return attack_; }
    int getDefence() const  { return defence_; }
    int getSpeed() const    { return speed_; }
    int getLevel() const    { return level_; }
    int getExp() const      { return exp_; }
    int getMaxExp() const   { return maxExp_; }

    // Setter
    Status& setHp(int hp)              { hp_ = hp; return *this; }
    Status& setMaxHp(int maxHp)        { maxHp_ = maxHp; return *this; }
    Status& setAttack(int attack)      { attack_ = attack; return *this; }
    Status& setDefence(int defence)    { defence_ = defence; return *this; }
    Status& setSpeed(int speed)        { speed_ = speed; return *this; }
    Status& setLevel(int level)        { level_ = level; return *this; }
    Status& setExp(int exp)            { exp_ = exp; return *this; }
    Status& setMaxExp(int maxExp)      { maxExp_ = maxExp; return *this; }

    // Adder
    void AddHp(int hp)              { hp_ += hp; }
    void AddAttack(int attack)      { attack_ += attack; }
    void AddDefence(int defence)    { defence_ += defence; }
    void AddSpeed(int speed)        { speed_ += speed; }
    void AddLevel(int level)        { level_ += level; }
    void AddExp(int exp)            { exp_ += exp; }


private:
    int hp_;
    int maxHp_;
    int attack_;
    int defence_;
    int speed_;
    int level_;
    int exp_;
    int maxExp_;
};