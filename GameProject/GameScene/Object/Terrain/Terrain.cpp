#include "Terrain.h"
#include <ModelManager.h>
#include <imgui.h>

void Terrain::Initialize()
{
    transform_.scale = Vector3(100.0f, 2.0f, 400.0f);
    transform_.translate = Vector3(0.0f, -transform_.scale.y * 0.5f, 0.0f);

    ModelManager::GetInstance()->LoadModel("box.gltf");

    object_ = std::make_unique<Object3d>();
    object_->Initialize();
    object_->SetModel("box.gltf");
    object_->SetScale(transform_.scale);
    object_->SetTranslate(transform_.translate);
    object_->SetMaterialColor(Vector4(0.1f, 0.1f, 0.1f, 1.0f));
    object_->SetEnableLighting(true);
    object_->SetEnableHighlight(true);
}

void Terrain::Update()
{
    object_->Update();
}

void Terrain::Draw()
{
    object_->Draw();
}

void Terrain::Finalize()
{
}

void Terrain::ImGui()
{
    if (ImGui::Begin("Terrain"))
    {
        if (ImGui::DragFloat3("Scale", &transform_.scale.x, 0.01f))
        {
            object_->SetScale(transform_.scale);
        }
        
        if (ImGui::DragFloat3("Rotate", &transform_.rotate.x, 0.01f))
        {
            object_->SetRotate(transform_.rotate);
        }

        if (ImGui::DragFloat3("Translate", &transform_.translate.x, 0.01f))
        {
            object_->SetTranslate(transform_.translate);
        }
    }
    ImGui::End();
}
