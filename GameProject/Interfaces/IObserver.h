#pragma once

#include <string> // std::string
#include <GameSystem/GameEventNotifier/GameEventNotifier.h>
#include <GameUI/ColorResolver/ColorResolver.h>

class IObserver
{
public:
    virtual ~IObserver() = default;
    virtual void Initialize() = 0;
    virtual void Finalize() = 0;
    virtual void Update() = 0;
    virtual void OnNotify(const std::string& _name, const std::string& _event) = 0;

protected:
    GameEventNotifier* notifier_ = GameEventNotifier::GetInstance();
    ColorResolver* pColorResolver_ = ColorResolver::GetInstance();
};