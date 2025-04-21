#include "SceneFactory.h"

#include <TitleScene/TitleScene.h>
#include <GameScene/GameScene.h>
#include <ResultScene/ResultScene.h>

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	BaseScene* newScene = nullptr;

	if (sceneName == "title") {
		newScene = new TitleScene();
	}
    else if (sceneName == "play"){
        newScene = new GameScene();
    }
    else if (sceneName == "result")
    {
        newScene = new ResultScene();
    }

	return newScene;
}
