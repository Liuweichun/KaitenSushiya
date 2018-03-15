#include "GameRule.h"
#include "MyGameMain.h"

void GameRule::InitializeResource()
{
	DG::Image_Create("GamesetsumeiImg", "./data/image/Gamesetsumei.png");			//”wŒi
}

bool GameRule::Initialize()
{
	if (!Scene::Initialize())
		return false;
	return true;
}

void GameRule::Update()
{
	mouse = DI::Mouse_GetState();


	if (mouse.LB.down) {
		currentScene = scenes["Title"];
		currentScene->Initialize();
	}
}

void GameRule::Render()
{
	ML::Box2D draw(0, 0, 960, 544);
	ML::Box2D src(0, 0, 960, 544);
	DG::Image_Draw("GamesetsumeiImg", draw, src);
}

void GameRule::Finalize()
{
	DG::Image_Erase("GamesetsumeiImg");
}
