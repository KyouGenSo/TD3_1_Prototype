#pragma once

#include "ComponentData.h" // ComponentData
#include "math/NiVec2.h" // NiVec2
#include <unordered_map> // unordered_map
#include <string> // string

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


private:

    // 確認用フラグ
    static bool isInitialized_;
    static bool isBeginFrame_;


    // ウィンドウのサイズ
    static NiVec2 leftTop_;
    static NiVec2 size_;


    // コンポーネントのリスト
    static std::unordered_map<std::string, ButtonImageData> buttonImages_;


private:
    static void CheckValid_BeginFrame();
    static void CheckValid_DrawUI();
};