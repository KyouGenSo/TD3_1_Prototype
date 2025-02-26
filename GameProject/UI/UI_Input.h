#pragma once

#include "math/NiVec2.h" // NiVec2
#include <minwindef.h>
#include <windef.h>

/// マウスデータ
struct MouseData
{
    NiVec2 pos;
    bool isLeft;
    bool isLeftPre;
    bool isRight;
    bool isRightPre;
    bool isMiddle;
    bool isMiddlePre;
};


/// UI入力クラス
class UI_Input
{
public: /// コンストラクタとデストラクタ
    UI_Input() = default;
    ~UI_Input() = default;


public: /// 一般

    // 初期化
    void Initialize();

    // 入力データ更新
    // データを取得する前に呼び出してください。
    void Update();

    // ウィンドウプロシージャハンドラ
    void WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


public: // 入力データ取得

    // 左クリックプレス
    bool PressLeft() const { return mouseData_.isLeft; }
    // 左クリックトリガー
    bool TriggerLeft() const { return mouseData_.isLeft && !mouseData_.isLeftPre; }

    // 右クリックプレス
    bool PressRight() const { return mouseData_.isRight; }
    // 右クリックトリガー
    bool TriggerRight() const { return mouseData_.isRight && !mouseData_.isRightPre; }

    // 中クリックプレス
    bool PressMiddle() const { return mouseData_.isMiddle; }
    // 中クリックトリガー
    bool TriggerMiddle() const { return mouseData_.isMiddle && !mouseData_.isMiddlePre; }


private:
    // マウスデータ
    MouseData mouseData_;
};