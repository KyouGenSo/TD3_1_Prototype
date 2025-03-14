#pragma once

#include <Object3d.h>
#include <memory>

class Terrain
{
public:
    Terrain() = default;
    ~Terrain() = default;
    void Initialize();
    void Update();
    void Draw();
    void Finalize();

    void ImGui();

private:
    std::unique_ptr<Object3d> terrainObj_;
    std::unique_ptr<Object3d> boxObj_;
    Transform transform_;
};