#include "GameEventNotifier.h"
#include <algorithm>

uint32_t GameEventNotifier::RegisterCallback(const std::string& _eventName, std::function<void(std::any)> _func)
{
    FunctionData fd;
    fd.func= _func;
    fd.id = m_idCounter++;

    m_callbacks[_eventName].emplace_back(fd);

    return fd.id;
}

void GameEventNotifier::UnregisterCallback(const std::string& _eventName, uint32_t _id)
{
    auto it = m_callbacks.find(_eventName);
    if (it != m_callbacks.end())
    {
        auto& callbacks = it->second;
        callbacks.erase(std::remove_if(callbacks.begin(), callbacks.end(), [_id](const FunctionData& fd) { return fd.id == _id; }), callbacks.end());
    }
}

void GameEventNotifier::Notify(const std::string& _eventName, std::any _data)
{
    auto it = m_callbacks.find(_eventName);
    if (it != m_callbacks.end())
    {
        for (const auto& fd : it->second)
        {
            fd.func(_data);
        }
    }
}
