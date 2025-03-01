#include "Drawer.h"

#include <TextureManager.h>
#include <Audio.h>

void Drawer::DrawSetting()
{
    IDrawer::DrawSetting();

    buttonSpriteCount_.clear();

    /// 同じテクスチャのボタンの数をカウント
    for (auto& buttonDrawData : buttonDrawDataQueue_)
    {
        buttonSpriteCount_[buttonDrawData->textureName]++;
    }

    /// スプライトの数が足りない場合は追加
    for (auto& buttonSpriteCount : buttonSpriteCount_)
    {
        auto& textureName = buttonSpriteCount.first;
        auto& spriteList = sprites_[textureName];
        /// スプライトリストが足りない場合は追加 & 初期化
        while (spriteList.size() < buttonSpriteCount.second)
        {
            spriteList.push_back(std::make_unique<Sprite>());
            // テクスチャの読み込み
            TextureManager::GetInstance()->LoadTexture(textureName);
            // スプライトの初期化
            spriteList.back()->Initialize(textureName);
        }
    }

    /// イテレータの生成
    std::unordered_map<std::string, SpriteList::iterator> itr_sprites;
    for (auto& spriteList : sprites_)
    {
        itr_sprites[spriteList.first] = spriteList.second.begin();
    }

    /// ボタンの描画データをスプライトに変換
    for(auto& data : buttonDrawDataQueue_)
    {
        auto sprite = itr_sprites[data->textureName]->get();
        
        sprite->SetPos(Vector2(data->leftTop.x, data->leftTop.y));
        sprite->SetSize(Vector2(data->size.x, data->size.y));

        Vector4 color = {};

        if(data->isHeld)
        {
            color = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
            sprite->SetColor(color);
        }
        else if(data->isHover)
        {
            color = Vector4(0.7f, 0.7f, 0.7f, 1.0f);
            sprite->SetColor(color);
        }
        else
        {
            sprite->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
        }

        /// イテレータを進める
        ++itr_sprites[data->textureName];
    }

    return;
}

void Drawer::Draw()
{
    /// 実際に描画するスプライトの数 = ボタンの描画データの数
    for (auto& pathCountPair : buttonSpriteCount_)
    {
        auto& textureName = pathCountPair.first;
        auto spriteListItr = sprites_[textureName].begin();
        
        for (uint32_t i = 0; i < pathCountPair.second; ++i)
        {
            spriteListItr->get()->Update();
            spriteListItr->get()->Draw();
            ++spriteListItr;
        }
    }

    /// 描画データのクリア
    buttonDrawDataQueue_.clear();

    return;
}

void Drawer::PlayAudio(uint32_t _handle)
{
    Audio::GetInstance()->Play(_handle);
    return;
}
