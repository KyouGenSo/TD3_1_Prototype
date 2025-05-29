#include "Minimap.h"

#include <algorithm>

#include "TextureManager.h"
#include "Utility/Equals.h"

void Minimap::Initialize() {
	TextureManager::GetInstance()->LoadTexture("white.png");
	back_ = std::make_unique<Sprite>();
	back_->Initialize("white.png");
    back_->SetSize(Vector2 {MINIMAP_SIZE, MINIMAP_SIZE});
    back_->SetPos(MINIMAP_LEFTTOP);
	back_->SetColor({1,1,1,0.7f});
}

void Minimap::Update() {size_t i = 0;
    for (auto& object : pObjects_){
        if (!object){
            auto& o = *(objects_.begin() + i);
            pObjects_.erase(std::ranges::remove(pObjects_, object).begin(), pObjects_.end());
            objects_.erase(std::ranges::remove(objects_,o).begin(), objects_.end());
            assert(pObjects_.size() == objects_.size());
        } else{
            Vector2 pos = {pObjects_[i]->GetTransform().translate.x, pObjects_[i]->GetTransform().translate.z};

            pos.x = std::clamp(pos.x, stageRange_.min.x + OBJECT_SIZE / 2.f, stageRange_.max.x - OBJECT_SIZE / 2.f);
            pos.y = std::clamp(pos.y, stageRange_.min.z + OBJECT_SIZE / 2.f, stageRange_.max.z - OBJECT_SIZE / 2.f);

            pos.x = (pos.x - stageRange_.min.x) / (stageRange_.max.x - stageRange_.min.x) * MINIMAP_SIZE + MINIMAP_LEFTTOP.x;
            pos.y = (pos.y - stageRange_.min.z) / (stageRange_.max.z - stageRange_.min.z) * MINIMAP_SIZE + MINIMAP_LEFTTOP.y;

            objects_[i]->SetPos(pos);
            objects_[i]->Update();
            ++i;
        }
    }

	back_->Update();
}

void Minimap::Draw() {
	back_->Draw();

    for (auto& object : objects_){
        object->Draw();
    }
}

void Minimap::Register(Object* object) {
	pObjects_.push_back(object);

    if (object->GetName().empty())return;

	std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
	sprite->Initialize("white.png");
    sprite->SetSize({5, 5});
    sprite->SetAnchorPoint({0.5f, 0.5f});

	if (Utility::EqualsIgnoreCase(object->GetName(), std::string("Player")))sprite->SetColor({0, 1, 1, 1});
    else if (Utility::EqualsIgnoreCase(object->GetName(), std::string("enemy"))) sprite->SetColor({1, 0, 0, 1});
    else if (Utility::EqualsIgnoreCase(object->GetName(), std::string("boss"))) sprite->SetColor({1, 0, 0, 1});
    else{
        __debugbreak();
        return;
    }

	objects_.push_back(std::move(sprite));

	assert(pObjects_.size() == objects_.size());
}

void Minimap::Unregister(Object* object) {
    auto it = std::ranges::find(pObjects_, object);
    if (it == pObjects_.end()) return;
    auto index = std::distance(pObjects_.begin(), it);
    pObjects_.erase(it);
    objects_.erase(objects_.begin() + index);
    assert(pObjects_.size() == objects_.size());
}

void Minimap::SetSize(const Vector3 min, const Vector3 max) {
	stageRange_.min = min;
	stageRange_.max = max;
}
