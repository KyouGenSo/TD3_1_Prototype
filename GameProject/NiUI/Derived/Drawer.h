#pragma once

#include "../Interface/NiUI_IDrawer.h"

#include <Sprite.h>
#include <unordered_map>
#include <memory>

class Drawer : public IDrawer
{
public:
    Drawer() = default;
    ~Drawer() = default;

    void DrawSetting() override;
    void Draw() override;
    void PlayAudio(uint32_t _handle) override;

private:
    using SpriteList = std::list<std::unique_ptr<Sprite>>;
    std::unordered_map<std::string, SpriteList> sprites_; // <テクスチャ名, スプライトリスト>
    std::unordered_map<std::string, uint32_t> buttonSpriteCount_; // <テクスチャ名, スプライト数>
};