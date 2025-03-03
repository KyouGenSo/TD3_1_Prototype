#pragma once

#include "../Interface/NiUI_IDebug.h"

class NiUI_Debug : public INiUIDebug
{
public:
    NiUI_Debug(const NiUIIO& _io, const NiUICoreState& _state) : INiUIDebug(_io, _state) {}
    void DrawDebugUI();
};