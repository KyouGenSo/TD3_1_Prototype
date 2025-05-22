#include "./HPBar.h"

void HPBar::Initialize(const Vector2& _position, const Vector2& _size)
{
    auto pColorResolver = ColorResolver::GetInstance();
    auto maxColor = pColorResolver->Resolve(ColorName::)

    // Initialize the background sprite
    InitializeSprite(sprite_max_, , _position, _size);
}

void HPBar::InitializeSprite(std::unique_ptr<Sprite>& _sprite, const RGBA& _color, const Vector2& _position, const Vector2& _size)
{
    _sprite = std::make_unique<Sprite>();
    _sprite->SetColor(_color.toVector4());
    _sprite->SetPos(_position);
    _sprite->SetSize(_size);
}
