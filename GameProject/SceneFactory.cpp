#include "SceneFactory.h"
#include "TitleScene.h"


BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	BaseScene* newScene = nullptr;

	if (sceneName == "title") {
		newScene = new TitleScene();
	} 

	return newScene;
}
