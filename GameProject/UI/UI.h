#pragma once

#include "ComponentData.h" // ComponentData
#include "math/NiVec2.h" // NiVec2
#include "UI_Input.h" // UI_Input
#include <unordered_map> // unordered_map
#include <string> // string
#define WIN32_LEAN_AND_MEAN
#include <Windows.h> // HWND, UINT, WPARAM, LPARAM


/// UIクラス
class UI
{
public: /// コンストラクタとデストラクタ
    UI() = default;
    ~UI() = default;


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
    void NiUI_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


public: /// UIコンポーネントの追加
    // ボタンの追加
    // ボタンのテクスチャ名、左上座標、サイズを指定してください。
    static bool Button(const std::string& _id, const std::string& _textureName, const NiVec2& _leftTop, const NiVec2& _size);


private: /// メンバ変数

    // 確認用フラグ
    static bool isInitialized_;
    static bool isBeginFrame_;

    // 入力データ
    static UI_Input input_;

    // ウィンドウのサイズ
    static NiVec2 leftTop_;
    static NiVec2 size_;


    // コンポーネントのリスト
    static std::unordered_map<std::string, ButtonImageData> buttonImages_;


private:
    static void CheckValid_BeginFrame();
    static void CheckValid_DrawUI();
    static void JudgeClickRect(const NiVec2& _leftTop, const NiVec2& _size, bool& _isHover, bool& _isPress);
};