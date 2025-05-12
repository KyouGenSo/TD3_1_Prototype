#pragma once
#include "BaseScene.h"
#include"Sprite.h"
#include"Object3d.h"
#include <vector>
#include <memory>
#include "AABB.h"

class TitleScene : public BaseScene
{
public: // メンバ関数

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize() override;

    /// <summary>
    /// 終了処理
    /// </summary>
    void Finalize() override;

    /// <summary>
    /// 更新
    /// </summary>
    void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
    void DrawWithoutEffect() override;

    /// <summary>
    /// ImGuiの描画
    /// </summary>
    void DrawImGui() override;

private: // メンバ変数

    bool isDebug_ = false;

    const float AnimationFrames = 120;
    float frameCount_ = 0;
    Vector2 pos{};

    Vector2 start {1500, 300};
    Vector2 end {300, 300};

    std::unique_ptr<Sprite> button_;

    std::unique_ptr<Sprite> press_;
    std::unique_ptr<Sprite> guide_;

    uint32_t bgmPlayHandle_ = 0;
};
