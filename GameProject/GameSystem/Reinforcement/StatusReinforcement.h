#pragma once

#include "IReinforcement.h"
#include <string>
#include <GameScene/Status/Status.h>
#include <variant>

class StatusReinforcement : public IReinforcement
{
public:
    StatusReinforcement() {};
    ~StatusReinforcement() override = default;

    void Initialize(const std::string& _cardName);
    void Update();
   
    void Apply() override;
    void Remove() override;
    void IncrementLevel() override;
    void DecrementLevel() override;
    void OnHit() override;

    void SetStatus(Status* _status) { status_ = _status; }
    void SetBehaviorLimitter(BehaviorLimitter* _behaviorLimitter) { behavior_limitter_ = _behaviorLimitter; }

private:
    void ParseFromJson(const std::string& _cardName);
    void ApplicationByAddition();
    void ApplicationByMultiplication();

private:
    Status* status_ = nullptr;
    BehaviorLimitter* behavior_limitter_ = nullptr;

};