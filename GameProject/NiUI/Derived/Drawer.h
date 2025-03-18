#pragma once

#include "../Interface/NiUI_IDrawer.h"

#include <Sprite.h>
#include <memory>
#include <NiUI/Type/NiUI_Type_Core.h>

class Drawer : public IDrawer
{
public:
    Drawer() = default;
    ~Drawer() = default;

    void Draw() override;
    void PlayAudio(uint32_t _handle) override;
    void PlayAudio(void* _audioHandler) override {};


private:
    using SpriteList = std::vector<std::unique_ptr<Sprite>>;

    StringMap<SpriteList> sprites_;
    StringMap<uint32_t> textureCount_;
};