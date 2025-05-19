#include "Object.h"

#include <GameScene/Object/Collision/Collider.h>
#include <Utility/Adaptor.h>
#include <Draw2D.h>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

void Object::DrawCollider(const Collision::Collider* _collider)
{
    if (_collider == nullptr) return;

    Collision::Vec3 pos = _collider->GetTranslate();

    if (_collider->GetType() == Collision::Type::AABB)
    {
        AABB aabb = {};
        Collision::Vec3 size = std::get<Collision::Vec3>(_collider->GetSize());
        aabb.min = Adaptor(size * -0.5f + pos);
        aabb.max = Adaptor(size * 0.5f + pos);

        Draw2D::GetInstance()->DrawAABB(aabb, { 0.0f, 1.0f, 0.0f, 1.0f });
    }
    else if (_collider->GetType() == Collision::Type::Sphere)
    {
        float size = std::get<float>(_collider->GetSize());

        Draw2D::GetInstance()->DrawSphere(Adaptor(pos), size, { 0.0f, 1.0f, 0.0f, 1.0f });
    }
}

void Object::DebugObject()
{
    #ifdef _DEBUG
    ImGui::PushID("DebugObject");
    ImGui::SeparatorText("Transform");
    ImGui::DragFloat3("Scale", &transform_.scale.x, 0.01f);
    ImGui::DragFloat3("Rotation", &transform_.rotate.x, 0.01f);
    ImGui::DragFloat3("Position", &transform_.translate.x, 0.01f);

    ImGui::SeparatorText("Physics");
    ImGui::DragFloat3("Velocity", &velocity_.x, 0.01f);
    ImGui::DragFloat3("Acceleration", &acceleration_.x, 0.01f);
    ImGui::DragFloat("Gravity", &gravity_, 0.01f);
    ImGui::DragFloat("Mass", &mass_, 0.01f);
    ImGui::PopID();
    #endif
}

void Object::StatusUpdateOnCollision(const Collision::Collider* pObject)
{
    Object* object = static_cast<Object*>(pObject->GetOwner());
    statusCurrent_.OnCollision(object->getStatusCurrent());
}
