#pragma once
#include "MyPG.h"

enum TaskFlag
{
	Task_Non,							//タスクが未選択状態
	Task_Title,							//タイトル
	Task_Game,							//ゲーム本編
	Task_Gameover,						//ゲームオーバー
	Task_Outsidebusinesshours,			//店営業時間外
	Task_Gamesetsumei,					//ゲーム説明
	Task_Gameclear,						//ゲームクリア

	//以下必要に応じて追加
};

extern  void  MyGameMain_Finalize( );
extern  void  MyGameMain_Initalize( );
extern  void  MyGameMain_UpDate( );
extern  void  MyGameMain_Render2D( );
