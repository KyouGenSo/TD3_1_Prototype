#include "UI_Input.h"

#include <WinUser.h> // WM_LBUTTONDOWN, WM_LBUTTONUP, WM_RBUTTONDOWN, WM_RBUTTONUP, WM_MBUTTONDOWN, WM_MBUTTONUP

void UI_Input::Initialize()
{
    mouseData_ = {};
}

void UI_Input::Update()
{
    /// 前フレームの状態を保存
    mouseData_.isLeftPre = mouseData_.isLeft;
    mouseData_.isRightPre = mouseData_.isRight;
    mouseData_.isMiddlePre = mouseData_.isMiddle;

    /// マウスの座標を取得
    POINT point;
    GetCursorPos(&point); // マウスの座標を取得
    mouseData_.pos = NiVec2(static_cast<float>(point.x), static_cast<float>(point.y));
}

void UI_Input::WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_LBUTTONDOWN:
        mouseData_.isLeft = true;
        break;
    case WM_LBUTTONUP:
        mouseData_.isLeft = false;
        break;
    case WM_RBUTTONDOWN:
        mouseData_.isRight = true;
        break;
    case WM_RBUTTONUP:
        mouseData_.isRight = false;
        break;
    case WM_MBUTTONDOWN:
        mouseData_.isMiddle = true;
        break;
    case WM_MBUTTONUP:
        mouseData_.isMiddle = false;
        break;
    }

    return;
}
