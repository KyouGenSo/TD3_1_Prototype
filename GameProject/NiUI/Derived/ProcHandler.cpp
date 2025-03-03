#include "ProcHandler.h"

#include "../NiUI.h"

void ProcHandler::OnWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    NiUI::NiUI_WndProcHandler(hWnd, msg, wParam, lParam);
}
