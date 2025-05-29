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
    collider_->SetEvent(Collision::EventType::Stay, [&](const Collision::Collider* pCol) {OnCollision(pCol); })
        ->SetTranslate({ transform_.translate.x, transform_.translate.y, transform_.translate.z })
        ->SetSize(Collision::Vec3{ 200.0f, 4.1f, 800.0f })
        ->SetType(Collision::Type::AABB)
        ->AddAttribute(static_cast<uint32_t>(Collider::Type::STAGE))
        ->SetOwner(this)
        ->Enable();

    //---------------------------------- Long Wall Model1 ----------------------------//
    longWallTransform1_.scale = Vector3(1.0f, 1.0f, 1.0f);
    longWallTransform1_.translate = Vector3(98.0f, 0.0f, 0.0f);
    longWallTransform1_.rotate = Vector3(0.0f, 0.0f, DirectX::XMConvertToRadians(90.f));

    longWallModel1_ = std::make_unique<Object3d>();
    longWallModel1_->Initialize();
    longWallModel1_->SetModel("long_wall.gltf");
    longWallModel1_->SetScale(longWallTransform1_.scale);
    longWallModel1_->SetTranslate(longWallTransform1_.translate);
    longWallModel1_->SetRotate(longWallTransform1_.rotate);
    longWallModel1_->SetMaterialColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    longWallModel1_->SetEnableLighting(true);
    longWallModel1_->SetEnableHighlight(false);

    //------------------------------- Long Wall Model2 ----------------------------//
    longWallTransform2_.scale = Vector3(1.0f, 1.0f, 1.0f);
    longWallTransform2_.translate = Vector3(-98.0f, 0.0f, 0.0f);
    longWallTransform2_.rotate = Vector3(0.0f, 0.0f, DirectX::XMConvertToRadians(90.f));

    longWallModel2_ = std::make_unique<Object3d>();
    longWallModel2_->Initialize();
    longWallModel2_->SetModel("long_wall.gltf");
    longWallModel2_->SetScale(longWallTransform2_.scale);
    longWallModel2_->SetTranslate(longWallTransform2_.translate);
    longWallModel2_->SetRotate(longWallTransform2_.rotate);
    longWallModel2_->SetMaterialColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    longWallModel2_->SetEnableLighting(true);
    longWallModel2_->SetEnableHighlight(false);

    //------------------------------- Short Wall Model1 ----------------------------//
    shortWallTransform1_.scale = Vector3(1.0f, 1.0f, 1.0f);
    shortWallTransform1_.translate = Vector3(0.0f, 0.0f, 398.0f);
    shortWallTransform1_.rotate = Vector3(-DirectX::XMConvertToRadians(90.f), 0.0f, -DirectX::XMConvertToRadians(90.f));

    shortWallModel1_ = std::make_unique<Object3d>();
    shortWallModel1_->Initialize();
    shortWallModel1_->SetModel("long_wall.gltf");
    shortWallModel1_->SetScale(shortWallTransform1_.scale);
    shortWallModel1_->SetTranslate(shortWallTransform1_.translate);
    shortWallModel1_->SetRotate(shortWallTransform1_.rotate);
    shortWallModel1_->SetMaterialColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    shortWallModel1_->SetEnableLighting(true);
    shortWallModel1_->SetEnableHighlight(false);

    //------------------------------- Short Wall Model2 ----------------------------//
    shortWallTransform2_.scale = Vector3(1.0f, 1.0f, 1.0f);
    shortWallTransform2_.translate = Vector3(0.0f, 0.0f, -398.0f);
    shortWallTransform2_.rotate = Vector3(-DirectX::XMConvertToRadians(90.f), 0.0f, -DirectX::XMConvertToRadians(90.f));

    shortWallModel2_ = std::make_unique<Object3d>();
    shortWallModel2_->Initialize();
    shortWallModel2_->SetModel("long_wall.gltf");
    shortWallModel2_->SetScale(shortWallTransform2_.scale);
    shortWallModel2_->SetTranslate(shortWallTransform2_.translate);
    shortWallModel2_->SetRotate(shortWallTransform2_.rotate);
    shortWallModel2_->SetMaterialColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    shortWallModel2_->SetEnableLighting(true);
    shortWallModel2_->SetEnableHighlight(false);

    //------------------------------- Wall UV Transform ----------------------------//
    wallUvTransform.scale = Vector3(8.0f, 4.0f, 1.0f);
    wallUvTransform.translate = Vector3(0.0f, 0.0f, 0.0f);
    wallUvTransform.rotate = Vector3(0.0f, 0.0f, 0.0f);
    longWallModel1_->SetUvTransform(wallUvTransform);
    longWallModel2_->SetUvTransform(wallUvTransform);
    shortWallModel1_->SetUvTransform(wallUvTransform);
    shortWallModel2_->SetUvTransform(wallUvTransform);

}

void Terrain::Update()
{
    model_->Update();
    longWallModel1_->Update();
    longWallModel2_->Update();
    shortWallModel1_->Update();
    shortWallModel2_->Update();
}

void Terrain::Draw()
{
    // AABB Debug Draw
    Vector3 size = Vector3(200.0f, 4.0f, 800.0f);
    AABB aabb = {};
    aabb.min = size * -0.5f + transform_.translate;
    aabb.max = size * 0.5f + transform_.translate;

    Draw2D::GetInstance()->DrawAABB(aabb, { 0.0f, 1.0f, 0.0f, 1.0f });


    model_->Draw();
    longWallModel1_->Draw();
    longWallModel2_->Draw();
    shortWallModel1_->Draw();
    shortWallModel2_->Draw();
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

        if (ImGui::DragFloat3("Wall Scale", &longWallTransform1_.scale.x, 0.01f))
        {
            longWallModel1_->SetScale(longWallTransform1_.scale);
        }
        if (ImGui::DragFloat3("Wall Rotate", &longWallTransform1_.rotate.x, 0.01f))
        {
            longWallModel1_->SetRotate(longWallTransform1_.rotate);
        }
        if (ImGui::DragFloat3("Wall Translate", &longWallTransform1_.translate.x, 0.01f))
        {
            longWallModel1_->SetTranslate(longWallTransform1_.translate);
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
}
