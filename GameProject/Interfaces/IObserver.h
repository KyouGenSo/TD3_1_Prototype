#pragma once

#include <string> // std::string

class IObserver
{
public:
    virtual ~IObserver() = default;
    virtual void OnNotify(const std::string& _event) = 0;
};