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
    void PlayAudio(void* _audioHandler) override {};


private:
    using SpriteList = std::list<std::unique_ptr<Sprite>>;
    std::unordered_map<std::string, SpriteList> spritesButton_; // <テクスチャ名, スプライトリスト>
    std::unordered_map<std::string, SpriteList> spritesDiv_; // <テクスチャ名, スプライトリスト>
    std::unordered_map<std::string, SpriteList> spritesDragItemArea_; // <テクスチャ名, スプライトリスト>
    std::unordered_map<std::string, SpriteList> spritesDragItem_; // <テクスチャ名, スプライトリスト>

    std::unordered_map<std::string, uint32_t> buttonSpriteCount_; // <テクスチャ名, スプライト数>
    std::unordered_map<std::string, uint32_t> divSpriteCount_; // <テクスチャ名, スプライト数>
    std::unordered_map<std::string, uint32_t> dragItemAreaSpriteCount_; // <テクスチャ名, スプライト数>
    std::unordered_map<std::string, uint32_t> dragItemSpriteCount_; // <テクスチャ名, スプライト数>
    std::vector<Sprite*> spritesZOrdered_;

private:
    void SpriteSettingByButtonData();
    void SpriteSettingByDivData();
    void SpriteSettingByDragItemAreaData();
    void SpriteSettingByDragItemData();
    void SpriteDraw(const std::unordered_map<std::string, uint32_t>& _spriteCount, const std::unordered_map<std::string, SpriteList>& _spriteMap);

private:
    void CheckCountAndCreateSprite(const std::string& _textureName, Drawer::SpriteList& _spriteList, uint32_t _countComponent);
};