#pragma once

#include <IWndProcHandler.h>

class ProcHandler : public IWndProcHandler
{
public:
    ProcHandler() = default;
    ~ProcHandler() = default;
    void OnWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
};