#pragma once

#include <Interface/NiGui_IDrawer.h>

#include <Sprite.h>
#include <memory>
#include <Type/NiGui_Type_Core.h>

class NiGuiDrawer : public INiGuiDrawer
{
public:
    NiGuiDrawer() = default;
    ~NiGuiDrawer();

    void Draw() override;
    void PlayAudio(uint32_t _handle) override;
    void PlayAudio(void* _audioHandler) override {};


private:
    using SpriteList = std::vector<std::unique_ptr<Sprite>>;

    StringMap<SpriteList> sprites_;
    StringMap<uint32_t> textureCount_;
    uint32_t playhandle_ = 0;
};