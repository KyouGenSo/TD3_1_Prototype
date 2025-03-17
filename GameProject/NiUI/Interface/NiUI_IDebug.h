#pragma once

#include "../Type/NiUI_Type_Core.h"

class INiUIDebug
{
public:
    INiUIDebug(const NiUIIO& _io, const NiUICoreState& _state) : io_(_io) , state_(_state){}
    virtual void DrawDebugUI() = 0;

protected:
    const NiUIIO& io_;
    const NiUICoreState& state_;
};