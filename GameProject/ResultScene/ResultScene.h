#pragma once

#include <BaseScene.h>
#include <memory>

#include "Sprite.h"

class ResultScene : public BaseScene
{
    std::unique_ptr<Sprite> press_;
public:
    ResultScene() = default;
    ~ResultScene() override = default;

    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;
    void DrawImGui() override;
};
