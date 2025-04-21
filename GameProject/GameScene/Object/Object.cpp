#include "Object.h"

#include <GameScene/Object/Collision/Collider.h>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

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
    status_.OnCollision(object->getStatus());
}
