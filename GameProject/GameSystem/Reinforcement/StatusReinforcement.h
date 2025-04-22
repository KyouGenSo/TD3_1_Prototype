#pragma once

#include "IReinforcement.h"
#include <string>
#include <GameScene/Status/Status.h>

class StatusReinforcement : public IReinforcement
{
public:
    StatusReinforcement() {};
    ~StatusReinforcement() override = default;

    void Initialize(const std::string& _cardName);

    void Apply() override;
    void Remove() override;
    void IncrementLevel() override;
    void DecrementLevel() override;

    void SetStatus(Status* _status) { status_ = _status; }

private:
    void ParseFromJson(const std::string& _cardName);
    void ApplicationByAddition();
    void ApplicationByMultiplication();

private:
    Status* status_;
};