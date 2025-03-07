#include "Drawer.h"

#include <TextureManager.h>
#include <Audio.h>
#include <NiUI/NiUI.h>

// スプライトリストは前から順に
// - Button
// - Div
// に整列する(もしくはされている)

void Drawer::DrawSetting()
{
    IDrawer::DrawSetting();

    buttonSpriteCount_.clear();
    divSpriteCount_.clear();
    spritesZOrdered_.clear();


    /// 同じテクスチャの数をカウント
    for (auto& buttonDrawData : buttonDrawDataList_)
    {
        buttonSpriteCount_[buttonDrawData->textureName]++;
    }
    for (auto& divDrawData : divDrawDataList_)
    {
        std::string path = divDrawData->textureName;
        if (path == "") path = "white.png";
        divSpriteCount_[path]++;
    }

    /// スプライトの数が足りない場合は追加
    for (auto& buttonSpriteCount : buttonSpriteCount_)
    {
        CheckCountAndCreateSprite(buttonSpriteCount.first, spritesButton_[buttonSpriteCount.first], buttonSpriteCount.second);
    }
    for (auto& divSpriteCount : divSpriteCount_)
    {
        std::string path = divSpriteCount.first;
        if (divSpriteCount.first == "") path = "white.png";
        CheckCountAndCreateSprite(path, spritesDiv_[path], divSpriteCount.second);
    }

    /// Zオーダーのスプライトリストのリサイズ
    size_t spriteCount = 0;
    spriteCount += buttonDrawDataList_.size();
    spriteCount += divDrawDataList_.size();
    spritesZOrdered_.resize(spriteCount);

    SpriteSettingByButtonData();
    SpriteSettingByDivData();

    return;
}

void Drawer::Draw()
{
    //SpriteDraw(buttonSpriteCount_, spritesButton_);
    //SpriteDraw(divSpriteCount_, spritesDiv_);

    for (auto& sprite : spritesZOrdered_)
    {
        sprite->Update();
        sprite->Draw();
    }

    /// 描画データのクリア
    buttonDrawDataList_.clear();
    divDrawDataList_.clear();

    return;
}

void Drawer::PlayAudio(uint32_t _handle)
{
    Audio::GetInstance()->Play(_handle);
    return;
}

void Drawer::SpriteSettingByButtonData()
{
    /// UIクラスからデータを取得
    auto hoveredComponentID = NiUI::GetHoverComponentID();
    auto activeComponentID = NiUI::GetActiveComponentID();

    /// イテレータの初期化
    std::unordered_map<std::string, SpriteList::iterator> itr_sprites;
    for (auto& spriteList : spritesButton_)
    {
        itr_sprites[spriteList.first] = spriteList.second.begin();
    }

    /// ボタンの描画データをスプライトに変換
    for(auto& data : buttonDrawDataList_)
    {
        auto sprite = itr_sprites[data->textureName]->get();

        sprite->SetPos(Vector2(data->leftTop.x, data->leftTop.y));
        sprite->SetSize(Vector2(data->size.x, data->size.y));

        Vector4 color = {};

        if(data->id == activeComponentID)
        {
            color = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
            sprite->SetColor(color);
        }
        else if(data->id == hoveredComponentID)
        {
            color = Vector4(0.7f, 0.7f, 0.7f, 1.0f);
            sprite->SetColor(color);
        }
        else
        {
            sprite->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
        }

        spritesZOrdered_[data->zOrder] = sprite;

        /// イテレータを進める
        ++itr_sprites[data->textureName];
    }
}

void Drawer::SpriteSettingByDivData()
{
    /// イテレータの初期化
    std::unordered_map<std::string, SpriteList::iterator> itr_sprites;
    for (auto& spriteList : spritesDiv_)
    {
        itr_sprites[spriteList.first] = spriteList.second.begin();
    }

    /// ボタンの描画データをスプライトに変換
    for (auto& data : divDrawDataList_)
    {
        std::string path = data->textureName;
        if (data->textureName == "")
        {
            path = "white.png";
        }

        auto sprite = itr_sprites[path]->get();

        sprite->SetPos(Vector2(data->leftTop.x, data->leftTop.y));
        sprite->SetSize(Vector2(data->size.x, data->size.y));

        auto color = NiUI::GetStyle().color.backGround;
        sprite->SetColor({ color.x, color.y, color.z, color.w });

        spritesZOrdered_[data->zOrder] = sprite;

        /// イテレータを進める
        ++itr_sprites[path];
    }
}

void Drawer::SpriteDraw(const std::unordered_map<std::string, uint32_t>& _spriteCount, const std::unordered_map<std::string, SpriteList>& _spriteMap)
{
    /// 実際に描画するスプライトの数 = ボタンの描画データの数
    for (auto& pathCountPair : _spriteCount)
    {
        auto& textureName = pathCountPair.first;
        auto spriteItr = _spriteMap.find(textureName)->second.begin();

        for (uint32_t i = 0; i < pathCountPair.second; ++i)
        {
            spriteItr->get()->Update();
            spriteItr->get()->Draw();
            ++spriteItr;
        }
    }
}

void Drawer::CheckCountAndCreateSprite(const std::string& _textureName, Drawer::SpriteList& _spriteList, uint32_t _countComponent)
{
    /// スプライトリストが足りない場合は追加 & 初期化
    while (_spriteList.size() < _countComponent)
    {
        _spriteList.push_back(std::make_unique<Sprite>());
        // テクスチャの読み込み
        TextureManager::GetInstance()->LoadTexture(_textureName);
        // スプライトの初期化
        _spriteList.back()->Initialize(_textureName);
    }
}
