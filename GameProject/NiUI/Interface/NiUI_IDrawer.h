#pragma once

#include "../Type/NiUI_ComponentData.h"
#include <list>

class IDrawer
{
public:
    IDrawer() = default;
    virtual ~IDrawer() = default;

    virtual void EnqueueDrawInfo(const ButtonData* _data) { buttonDrawDataQueue_.push_back(_data); }
    virtual void DrawSetting();
    virtual void Draw() = 0;
    virtual void PlayAudio(uint32_t _handle) = 0;

protected:
    std::list<const ButtonData*> buttonDrawDataQueue_;
};