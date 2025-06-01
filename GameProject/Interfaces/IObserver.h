#pragma once

#include <string> // std::string
#include <GameSystem/GameEventNotifier/GameEventNotifier.h>

class IObserver
{
public:
    virtual ~IObserver() = default;
    virtual void OnNotify(const std::string& _event) = 0;

protected:
    GameEventNotifier* notifier_ = GameEventNotifier::GetInstance();
};