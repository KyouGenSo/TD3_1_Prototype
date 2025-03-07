#include "SceneFactory.h"

#include <TitleScene/TitleScene.h>
#include <GameScene/GameScene.h>

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	BaseScene* newScene = nullptr;

	if (sceneName == "title") {
		newScene = new TitleScene();
	}
    if (sceneName == "play"){
        newScene = new GameScene();
    }

	return newScene;
}
