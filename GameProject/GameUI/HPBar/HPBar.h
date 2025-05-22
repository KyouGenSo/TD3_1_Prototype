#pragma once
#include <memory>
#include <Sprite.h>

#include <GameUI/ColorResolver/ColorResolver.h>
#include <Type/Color.h>

class HPBar
{
public:
    HPBar() = default;
    ~HPBar() = default;

    void Initialize(const Vector2& _position, const Vector2& _size);
    void Update();

private:
    std::unique_ptr<Sprite> sprite_current_;
    std::unique_ptr<Sprite> sprite_max_;

private:
    float valueMax_ = 0.0f;
    float valueCurrent_ = 0.0f;

private:
    void InitializeSprite(std::unique_ptr<Sprite>& _sprite, const RGBA& _color, const Vector2& _position, const Vector2& _size);
};