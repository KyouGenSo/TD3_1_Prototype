#include "Drawer.h"

#include <TextureManager.h>
#include <Audio.h>

void Drawer::Draw()
{
    textureCount_.clear();

    for (auto& data : drawDataZOrdered_)
    {
        const auto& textureName = data->textureName;
        auto& currentIndex = textureCount_[textureName];
        auto& spriteList = sprites_[textureName];
        Sprite* sprite = nullptr;

        // 足りなくなったら追加
        if (spriteList.size() == textureCount_[textureName])
        {
            TextureManager::GetInstance()->LoadTexture(textureName);
            spriteList.push_back(std::make_unique<Sprite>());
            sprite = spriteList.back().get();
            sprite->Initialize(textureName);
        }
        else
        {
            sprite = sprites_[data->textureName][currentIndex].get();
        }

        sprite->SetPos({ data->leftTop.x, data->leftTop.y });
        sprite->SetSize({ data->size.x, data->size.y });
        sprite->SetColor({ data->color.x, data->color.y, data->color.z, data->color.w });
        
        sprite->Update();
        sprite->Draw();

        currentIndex++;
    }

    return;
}

void Drawer::PlayAudio(uint32_t _handle)
{
    Audio::GetInstance()->Play(_handle);
    return;
}