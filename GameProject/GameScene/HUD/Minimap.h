#pragma once
#include <memory>
#include <vector>

#include "Sprite.h"
#include "GameScene/Object/Object.h"

class Minimap{
	struct StageRange{
        Vector3 min;
        Vector3 max;
	};

	std::vector<Object*> pObjects_;

    std::unique_ptr<Sprite> back_;

    Transform transform_{};

    // first data is player
    std::vector<std::unique_ptr<Sprite>> objects_;

    StageRange stageRange_{};

    const float MINIMAP_SIZE = 200.f;
    const Vector2 MINIMAP_LEFTTOP = {50, 50};

    const float OBJECT_SIZE = 5.f;
public:
	void Initialize();
    void Update();
    void Draw();

    void Register(Object* object);

	void SetSize(const Vector3 min, const Vector3 max);
};

