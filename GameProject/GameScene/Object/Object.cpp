#include "Object.h"

#include <Utility/Adaptor.h>
#include <Draw2D.h>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

void Object::ImGui(std::function<void()> _guiWidgetsFunc)
{
#ifdef _DEBUG
    bool isOpen = true;

    if (name_.empty())
    {
        isOpen = ImGui::Begin(uuid_.c_str());
    }
    else
    {
        isOpen = ImGui::Begin(name_.c_str());
    }

    if (isOpen)
    {
        ImGui::Text("UUID: %s", uuid_.c_str());
        
        if (ImGui::TreeNodeEx("Transform"))
        {
            ImGui::DragFloat3("Scale", &transform_.scale.x, 0.01f);
            ImGui::DragFloat3("Rotation", &transform_.rotate.x, 0.01f);
            ImGui::DragFloat3("Position", &transform_.translate.x, 0.01f);

            ImGui::TreePop();
        }
        if (ImGui::TreeNodeEx("Physics"))
        {
            ImGui::DragFloat3("Velocity", &velocity_.x, 0.01f);
            ImGui::DragFloat3("Acceleration", &acceleration_.x, 0.01f);
            ImGui::DragFloat("Gravity", &gravity_, 0.01f);
            ImGui::DragFloat("Mass", &mass_, 0.01f);

            ImGui::TreePop();
        }
        if (ImGui::TreeNodeEx("Features"))
        {
            ImGui::Checkbox("Status update", &enableUpdateStatus_);

            ImGui::TreePop();
        }
        if (_guiWidgetsFunc && ImGui::TreeNodeEx("Custom settings"))
        {
            _guiWidgetsFunc();

            ImGui::TreePop();
        }
    }

    ImGui::End();
#endif // _DEBUG
}

void Object::ImGui()
{
    this->ImGui({});
}

void Object::UpdateCollider() const {
    if (pCollider_ == nullptr) return;
    pCollider_->SetTranslate(Adaptor(transform_.translate));
}

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

void Object::StatusUpdateOnCollision(const Collision::Collider* _other)
{
    if (enableUpdateStatus_ == false) return;

    Object* object = static_cast<Object*>(_other->GetOwner());
    statusCurrent_.OnCollision(object->getStatusCurrent());
    statusCurrent_.Update();
}
