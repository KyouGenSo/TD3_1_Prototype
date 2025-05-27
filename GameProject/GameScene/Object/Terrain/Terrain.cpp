#include "Terrain.h"
#include <ModelManager.h>
#include <imgui.h>

#include "Type/ColliderType.h"
#include <Draw2D.h>

void Terrain::Initialize()
{
    //transform_.scale = Vector3(100.0f, 2.0f, 400.0f);
    //transform_.translate = Vector3(0.0f, -transform_.scale.y * 0.5f, 0.0f);
    transform_.scale = Vector3(1.0f, 1.0f, 1.0f);
    transform_.translate = Vector3(0.0f, -2.0f, 0.0f);

    model_ = std::make_unique<Object3d>();
    model_->Initialize();
    model_->SetModel("Terrain.gltf");
    model_->SetScale(transform_.scale);
    model_->SetTranslate(transform_.translate);
    model_->SetMaterialColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    model_->SetEnableLighting(true);
    model_->SetEnableHighlight(false);
    model_->SetRotate(Vector3(0.0f, 0.0f, 0.0f));
  
    Transform uvTranform;
    uvTranform.scale = Vector3(200.0f, 100.0f, 1.0f);
    uvTranform.translate = Vector3(0.0f, 0.0f, 0.0f);
    uvTranform.rotate = Vector3(0.0f, 0.0f, 0.0f);
    model_->SetUvTransform(uvTranform);

    collider_ = std::make_unique<Collision::Collider>();
    collider_->SetEvent(Collision::EventType::Stay, [&](const Collision::Collider* pCol){OnCollision(pCol); })
        ->SetTranslate({transform_.translate.x, transform_.translate.y, transform_.translate.z})
        ->SetSize(Collision::Vec3{200.0f, 4.1f, 800.0f})
        ->SetType(Collision::Type::AABB)
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::STAGE))
        ->SetOwner(this)
        ->Enable();
}

void Terrain::Update()
{
    model_->Update();
}

void Terrain::Draw()
{
    // AABB Debug Draw
    Vector3 size = Vector3(200.0f, 4.0f, 800.0f);
    AABB aabb = {};
    aabb.min = size * -0.5f + transform_.translate;
    aabb.max = size * 0.5f + transform_.translate;

    Draw2D::GetInstance()->DrawAABB(aabb, {0.0f, 1.0f, 0.0f, 1.0f});
    

    model_->Draw();
    //boxObj_->Draw();
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
            model_->SetScale(transform_.scale);
        }

        if (ImGui::DragFloat3("Rotate", &transform_.rotate.x, 0.01f))
        {
            model_->SetRotate(transform_.rotate);
        }

        if (ImGui::DragFloat3("Translate", &transform_.translate.x, 0.01f))
        {
            model_->SetTranslate(transform_.translate);
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
