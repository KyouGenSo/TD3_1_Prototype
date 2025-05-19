#pragma once

#include <Utility/RandomGenerator/RandomGenerator.h>
#include <string>


class Status
{
public:
    Status() = default;
    ~Status() = default;

    void Initalize();
    void Update();
    void ImGui(const std::string& _name);
    void LoadFromFile(const std::string& _filename);

    // Getter
    float getHp() const                     { return hp_; }
    float getMaxHp() const                  { return maxHp_; }
    float getAttack() const                 { return attack_; }
    float getDefence() const                { return defence_; }
    float getSpeed() const                  { return speed_; }
    float getExperiencePoints() const;
    float getGainedXP() const;

    // Setter
    Status& setHp(float _hp)                { hp_ = _hp; return *this; }
    Status& setMaxHp(float _maxHp)          { maxHp_ = _maxHp; return *this; }
    Status& setAttack(float _attack)        { attack_ = _attack; return *this; }
    Status& setDefence(float _defence)      { defence_ = _defence; return *this; }
    Status& setSpeed(float _speed)          { speed_ = _speed; return *this; }
    Status& setXpAmount(float _xpAmount)    { xpAmount_ = _xpAmount; return *this; }

    // Adder
    void AddHp(float _hp)                   { hp_ += _hp; }
    void AddAttack(float _attack)           { attack_ += _attack; }
    void AddDefence(float _defence)         { defence_ += _defence; }
    void AddSpeed(float _speed)             { speed_ += _speed; }
    void AddXpAmount(float _xpAmount)       { xpAmount_ += _xpAmount; }

public:
    void OnCollision(const Status& _status);

private:
    float               hp_             = 0;
    float               maxHp_          = 0;
    float               attack_         = 0;
    float               defence_        = 0;
    float               speed_          = 0;
    float               xpAmount_       = 0;
};