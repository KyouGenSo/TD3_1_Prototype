#pragma once

#include <BaseScene.h>

class ResultScene : public BaseScene
{
public:
    ResultScene() = default;
    ~ResultScene() override = default;

    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;
    void DrawImGui() override;
};