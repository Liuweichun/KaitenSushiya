#pragma once
#include "Title.h"
#include "GameRule.h"


//�Q�[�����

extern std::map<std::string, Scene*> scenes;
extern Scene* currentScene;

extern  void  MyGameMain_Finalize( );
extern  void  MyGameMain_Initalize( );
extern  void  MyGameMain_UpDate( );
extern  void  MyGameMain_Render2D( );
