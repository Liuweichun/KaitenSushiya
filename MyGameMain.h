#pragma once
#include "Title.h"
#include "Game.h"
#include "GameRule.h"
#include "OutsideBusinessHours.h"
#include "GameClear.h"
#include "GameOver.h"

//ƒQ[ƒ€î•ñ
extern std::map<std::string, Scene*> scenes;
extern Scene* currentScene;

extern  void  MyGameMain_Finalize( );
extern  void  MyGameMain_Initalize( );
extern  void  MyGameMain_UpDate( );
extern  void  MyGameMain_Render2D( );
