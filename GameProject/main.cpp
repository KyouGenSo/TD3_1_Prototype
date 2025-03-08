#include"TakoFramework.h"
#include <MyGame/MyGame.h>

//Windowsプログラムのエントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	TakoFramework* game = new MyGame();

	game->Run();

	delete game;

	return 0;
}