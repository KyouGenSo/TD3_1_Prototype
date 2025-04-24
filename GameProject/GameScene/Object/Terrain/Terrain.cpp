#include "Terrain.h"
#include <ModelManager.h>
#include <imgui.h>

#include "Type/ColliderType.h"

void Terrain::Initialize()
{
    //transform_.scale = Vector3(100.0f, 2.0f, 400.0f);
    //transform_.translate = Vector3(0.0f, -transform_.scale.y * 0.5f, 0.0f);
    transform_.scale = Vector3(1.0f, 1.0f, 1.0f);
    transform_.translate = Vector3(0.0f, -2.0f, 0.0f);

    ModelManager::GetInstance()->LoadModel("box.gltf");
    ModelManager::GetInstance()->LoadModel("boxInv.gltf");
    ModelManager::GetInstance()->LoadModel("Terrain.gltf");

    terrainObj_ = std::make_unique<Object3d>();
    terrainObj_->Initialize();
    terrainObj_->SetModel("Terrain.gltf");
    terrainObj_->SetScale(transform_.scale);
    terrainObj_->SetTranslate(transform_.translate);
    terrainObj_->SetMaterialColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    terrainObj_->SetEnableLighting(true);
    terrainObj_->SetEnableHighlight(false);
    terrainObj_->SetRotate(Vector3(0.0f, 0.0f, 0.0f));
    Transform uvTranform;
    uvTranform.scale = Vector3(400.0f, 100.0f, 1.0f);
    uvTranform.translate = Vector3(0.0f, 0.0f, 0.0f);
    uvTranform.rotate = Vector3(0.0f, 0.0f, 0.0f);
    terrainObj_->SetUvTransform(uvTranform);

    Vector3 boxScale = Vector3(transform_.scale.x, 300.0f, transform_.scale.z);
    Vector3 boxTranslate = Vector3(0.0f, boxScale.y * 0.5f - transform_.scale.y * 0.5f, 0.0f);
    boxObj_ = std::make_unique<Object3d>();
    boxObj_->Initialize();
    boxObj_->SetModel("boxInv.gltf");
    boxObj_->SetScale(boxScale);
    boxObj_->SetTranslate(boxTranslate);
    boxObj_->SetMaterialColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    boxObj_->SetEnableLighting(true);
    boxObj_->SetEnableHighlight(false);

    collider_ = std::make_unique<Collision::Collider>();
    collider_->SetEvent(Collision::EventType::Stay, [&](const Collision::Collider* pCol){OnCollision(pCol); })
        ->SetTranslate({transform_.translate.x, transform_.translate.y, transform_.translate.z})
        ->SetSize(Collision::Vec3{100, 0, 400})
        ->SetType(Collision::Type::AABB)
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::STAGE))
        ->SetOwner(this)
        ->Enable();
}

void Terrain::Update()
{
    terrainObj_->Update();
    boxObj_->Update();
}

void Terrain::Draw()
{
    terrainObj_->Draw();
    boxObj_->Draw();
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
            terrainObj_->SetScale(transform_.scale);
        }

        if (ImGui::DragFloat3("Rotate", &transform_.rotate.x, 0.01f))
        {
            terrainObj_->SetRotate(transform_.rotate);
        }

        if (ImGui::DragFloat3("Translate", &transform_.translate.x, 0.01f))
        {
            terrainObj_->SetTranslate(transform_.translate);
        }
    }
    ImGui::End();
}

void Terrain::OnCollision(const Collision::Collider* pCollider) const {
    (void)pCollider;
}

float Terrain::GetFloorHeight() const
{
    return 0.0f;
    //return 0.0f;
}
