#include "Minimap.h"

#include <algorithm>

#include "TextureManager.h"

void Minimap::Initialize() {
	TextureManager::GetInstance()->LoadTexture("white.png");
	back_ = std::make_unique<Sprite>();
	back_->Initialize("white.png");
    back_->SetSize(Vector2 {MINIMAP_SIZE, MINIMAP_SIZE});
    back_->SetPos(MINIMAP_LEFTTOP);
	back_->SetColor({1,1,1,0.7f});
}

void Minimap::Update() {
    for (size_t i = 0; i < pObjects_.size(); i++){
        Vector2 pos = {pObjects_[i]->GetTransform().translate.x, pObjects_[i]->GetTransform().translate.z};

		pos.x = std::clamp(pos.x, stageRange_.min.x + OBJECT_SIZE/2.f, stageRange_.max.x - OBJECT_SIZE/2.f);
		pos.y = std::clamp(pos.y, stageRange_.min.z + OBJECT_SIZE/2.f, stageRange_.max.z - OBJECT_SIZE/2.f);

        pos.x = (pos.x - stageRange_.min.x) / (stageRange_.max.x - stageRange_.min.x) * MINIMAP_SIZE + MINIMAP_LEFTTOP.x;
        pos.y = (pos.y - stageRange_.min.z) / (stageRange_.max.z - stageRange_.min.z) * MINIMAP_SIZE + MINIMAP_LEFTTOP.y;

    	objects_[i]->SetPos(pos);
		objects_[i]->Update();
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

	std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
	sprite->Initialize("white.png");
    sprite->SetSize({5, 5});
    sprite->SetAnchorPoint({0.5f, 0.5f});

	if (objects_.empty())sprite->SetColor({0, 1, 1, 1});
	else sprite->SetColor({1, 0, 0, 1});

	objects_.push_back(std::move(sprite));

	assert(pObjects_.size() == objects_.size());
}

void Minimap::SetSize(const Vector3 min, const Vector3 max) {
	stageRange_.min = min;
	stageRange_.max = max;
}
