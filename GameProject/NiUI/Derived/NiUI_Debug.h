#pragma once

#include "../Interface/NiUI_IDebug.h"

class NiUI_Debug : public INiUIDebug
{
public:
    NiUI_Debug() = default;
    void DrawDebugUI();
};