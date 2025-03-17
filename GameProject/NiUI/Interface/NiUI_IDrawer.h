#pragma once

#include "../Type/NiUI_Type_Component.h"
#include <list>

class IDrawer
{
public:
    IDrawer() = default;
    virtual ~IDrawer() = default;

    virtual void EnqueueDrawInfo(const ButtonData* _data) { buttonDrawDataList_.push_back(_data); }
    virtual void EnqueueDrawInfo(const DivData* _data) { divDrawDataList_.push_back(_data); }
    virtual void EnqueueDrawInfo(const DragItemAreaData* _data) { dragItemAreaDrawDataList_.push_back(_data); }
    virtual void EnqueueDrawInfo(const DragItemData* _data) { dragItemDrawDataList_.push_back(_data); }
    virtual void DrawSetting();
    virtual void Draw() = 0;
    virtual void PlayAudio(uint32_t _handle) = 0;
    virtual void PlayAudio(void* _audioHandler) = 0;

protected:
    std::list<const ButtonData*> buttonDrawDataList_;
    std::list<const DivData*> divDrawDataList_;
    std::list<const DragItemAreaData*> dragItemAreaDrawDataList_;
    std::list<const DragItemData*> dragItemDrawDataList_;
};