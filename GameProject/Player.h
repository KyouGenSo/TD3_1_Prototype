#pragma once
#include <memory>

#include "Object3d.h"
#include "Camera.h"

class Player{
    std::unique_ptr<Object3d> model_;

    Transform transform_ {};

    Camera* pCamera_ = nullptr;
public:
	void Initialize();
    void Update();
    void Draw();
    void Finalize();
    void ImGui();

    Transform& GetTransform() {return transform_;}
};

