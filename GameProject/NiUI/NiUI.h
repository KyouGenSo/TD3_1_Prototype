#pragma once

#include "Type/NiUI_ComponentData.h" // ComponentData
#include "math/NiVec2.h" // NiVec2
#include "NiUI_Input.h" // NiUI_Input
#include "Interface/NiUI_IDrawer.h"

#include <unordered_map> // unordered_map
#include <string> // string
#define WIN32_LEAN_AND_MEAN
#include <Windows.h> // HWND, UINT, WPARAM, LPARAM
#include "Type/NiUI_Enum.h" // enums


/// UIクラス
class NiUI
{
public: /// コンストラクタとデストラクタ
    NiUI() = default;
    ~NiUI() = default;


public: /// 一般

    // ルートウィンドウの初期化
    // クライアント領域の左上座標とサイズを指定してください。
    static void Initialize(const NiVec2& _size, const NiVec2& _leftTop = {0, 0});


    // フレームごとの初期化処理
    // UIコンポーネントを追加する前に呼び出してください。
    static void BeginFrame();


    // UIの更新及び描画
    // この関数を呼び出したら、BeginFrame()を呼び出すまでUIコンポーネントの追加は反映されません。
    static void DrawUI();


    // ウィンドウプロシージャハンドラ
    // 注意：ImGuiのハンドラより先に呼び出してください。
    static void NiUI_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


public: /// UIコンポーネントの追加
    // ボタンの追加
    // ボタンのテクスチャ名、左上座標、サイズを指定してください。
    static NiUI_ButtonState Button(
        const std::string& _id,
        const std::string& _textureName,
        const NiVec2& _position,
        const NiVec2& _size,
        NiUI_StandardPoint _anchor = NiUI_StandardPoint::LeftTop,
        NiUI_StandardPoint _pivot = NiUI_StandardPoint::LeftTop
    );

    
public: /// セッター
    static void SetDrawer(IDrawer* _drawer) { drawer_ = _drawer; }
    static void SetWindowInfo(const NiVec2& _size, const NiVec2& _leftTop) { size_ = _size; leftTop_ = _leftTop; }
    static void SetHoverSound(uint32_t _hoverSE) { hoverSE_ = _hoverSE; }
    static void SetConfirmSound(uint32_t _confirmSE) { confirmSE_ = _confirmSE; }


private: /// メンバ変数

    // 確認用フラグ
    static bool isInitialized_;
    static bool isBeginFrame_;

    // 入力データ
    static NiUI_Input input_;

    // ウィンドウのサイズ
    static NiVec2 leftTop_;
    static NiVec2 size_;


    // アクティブコンポーネントID
    static std::string activeComponentID_;
    static std::string hoverComponentID_;

    // SEのハンドル
    static int32_t hoverSE_;
    static int32_t confirmSE_;


    // コンポーネントのリスト
    static std::unordered_map<std::string, ButtonImageData> buttonImages_;


    // 描画クラス
    static IDrawer* drawer_;


private:
    static bool ButtonBehavior(const std::string& _id, bool _isHover, bool _isTrigger, bool _isRelease, bool& _out_held);


private:
    static void ButtonDataEnqueue();

private:
    static void CheckValid_BeginFrame();
    static void CheckValid_DrawUI();
    static void JudgeClickRect(const NiVec2& _leftTop, const NiVec2& _size, bool& _isHover, bool& _isTrigger, bool& _isRelease);
    static NiVec2 ComputeStandardPoint(NiUI_StandardPoint _stdpoint);
    static void ClearData();
};