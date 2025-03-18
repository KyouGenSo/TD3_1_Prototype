#include "ProcHandler.h"

#include <NiGui.h>

void ProcHandler::OnWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    NiGui::NiGui_WndProcHandler(hWnd, msg, wParam, lParam);
}
