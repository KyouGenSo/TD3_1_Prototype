#pragma once

#include <TextureManager.h>
#include <Sprite.h>
#include <memory>
#include <GlobalVariables.h>
#include <Vector2.h>
#include <Input.h>

#include "StatusBar.h"

class StatusHUD
{
public:
    struct Transform2d
    {
        Vector2 scale;
        float rotation;
        Vector2 translate;
    };

public:
    StatusHUD() = default;
    ~StatusHUD() = default;

    void Initialize();
    void Reload();
    void Update();
    void Draw2D();
    void ImGui();
    void OnResized(Vector2 _size);

    StatusBar* GetHpBar() { return hpBar_.get(); }
    StatusBar* GetXPBar() { return xpBar_.get(); }

private:
    GlobalVariables* globalVariables_ = nullptr;
    TextureManager* textureManager_ = nullptr;
    Input* input_ = nullptr;

    NiVec2 standardDisplaySize_ = {};

    std::unique_ptr<StatusBar> hpBar_ = nullptr;
    std::unique_ptr<StatusBar> bulletBar_ = nullptr;
    std::unique_ptr<StatusBar> xpBar_ = nullptr;
    NiVec2 retio_ = { 1.0f, 1.0f };
    NiVec2 hpBarPos_ = { 0.0f, 0.0f };
    NiVec2 bulletBarPos_ = { 0.0f, 0.0f };
    NiVec2 xpBarPos_ = { 0.0f, 0.0f };

    bool isReloaded_ = false;

private:
    void UpdateHotReload();
};