#include "MyPG.h"
#include "MyGameMain.h"
#include "easing.h"

//ゲーム情報
TaskFlag			actTask, nextTask; 

//-----------------------------------------------------------------------------
//初期化処理
//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
//-----------------------------------------------------------------------------
void  MyGameMain_Initalize()
{
	//キーボードの入力を受け取るオブジェクトを生成する
	DI::AnalogAxisKB	ls = { DIK_LEFT, DIK_RIGHT, DIK_UP, DIK_DOWN };
	DI::AnalogAxisKB	rs = { DIK_NUMPAD4, DIK_NUMPAD6, DIK_NUMPAD8, DIK_NUMPAD2 };
	DI::KeyDatas_KB	 key = {
		{ DIK_Z, DI::But::B1 },{ DIK_X, DI::But::B2 },{ DIK_C, DI::But::B3 },
		{ DIK_V, DI::But::B4 },{ DIK_A, DI::But::SE },{ DIK_S, DI::But::ST },
		{ DIK_Q, DI::But::L1 },{ DIK_W, DI::But::R1 },{ DIK_E, DI::But::L2 },
		{ DIK_R, DI::But::R2 },{ DIK_D, DI::But::L3 },{ DIK_NUMPAD5, DI::But::R3 }, 
	};
	//ゲームパッドパラメータ
	DI::KeyDatas_GP	but = {
		{ DI::GPB::B01, DI::But::B1 },{ DI::GPB::B02, DI::But::B2 },
		{ DI::GPB::B03, DI::But::B3 },{ DI::GPB::B04, DI::But::B4 },
		{ DI::GPB::B05, DI::But::L1 },{ DI::GPB::B06, DI::But::R1 },
		{ DI::GPB::B07, DI::But::L2 },{ DI::GPB::B08, DI::But::R2 },
		{ DI::GPB::B09, DI::But::SE },{ DI::GPB::B10, DI::But::ST },
		{ DI::GPB::B11, DI::But::L3 },{ DI::GPB::B12, DI::But::R3 },
	};
	DI::GPad_CreateKBGP("P1", ls, rs, key, 0, but);

	actTask  = Task_Non;	//初期状態は実行中のタスクを無効にしておく
	nextTask = Task_Title;	//次のタスクをタイトルにすることで最初にタイトルが実行される
}
//-----------------------------------------------------------------------------
//解放処理
//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
//-----------------------------------------------------------------------------
void  MyGameMain_Finalize()
{
	DI::GPad_Erase("P1");	//明示的に解放したい場合は
}


//-----------------------------------------------------------------------------
//外部ファイルより
namespace  Title {//タイトルタスク
	extern  void  Initialize();
	extern  TaskFlag  UpDate();
	extern  void  Render();
	extern  void  Finalize();
}
namespace  Game {//ゲーム本編タスク
	extern  void  Initialize();
	extern  TaskFlag  UpDate();
	extern  void  Render();
	extern  void  Finalize();
}
namespace  Gameover {//ゲームオーバータスク
	extern  void  Initialize();
	extern  TaskFlag  UpDate();
	extern  void  Render();
	extern  void  Finalize();
}
namespace  Outsidebusinesshours {//店営業外タスク
	extern  void  Initialize();
	extern  TaskFlag  UpDate();
	extern  void  Render();
	extern  void  Finalize();
}
namespace  Gamesetsumei {//ゲーム説明タスク
	extern  void  Initialize();
	extern  TaskFlag  UpDate();
	extern  void  Render();
	extern  void  Finalize();
}
namespace  Gameclear {//ゲームクリアタスク
	extern  void  Initialize();
	extern  TaskFlag  UpDate();
	extern  void  Render();
	extern  void  Finalize();
}
//以下必要に応じて追加
//-----------------------------------------------------------------------------
//更新処理
//機能概要：ゲームの１フレームに当たる処理
//-----------------------------------------------------------------------------
void  MyGameMain_UpDate()
{
	//1フレームに一回
	easing::UpDate();

	//次に実行するタスクが、今のタスクと一致しない
	if (actTask != nextTask) {
		//今実行中のタスクの終了処理を呼びだす
		switch (actTask) {
		case Task_Title:					Title::Finalize();	break;
		case Task_Game:						Game::Finalize();	break;
		case Task_Gameover:					Gameover::Finalize();	break;
		case Task_Outsidebusinesshours:		Outsidebusinesshours::Finalize();	break;
		case Task_Gamesetsumei:				Gamesetsumei::Finalize();	break;
		case Task_Gameclear:				Gameclear::Finalize();	break;
			//以下必要に応じて追加
		}
		//次のタスクに移る
		actTask = nextTask;
		//次のタスクの初期化処理を呼びだす
		switch (actTask) {
		case Task_Title:					Title::Initialize();	break;
		case Task_Game:						Game::Initialize();		break;
		case Task_Gameover:					Gameover::Initialize();		break;
		case Task_Outsidebusinesshours:		Outsidebusinesshours::Initialize();		break;
		case Task_Gamesetsumei:				Gamesetsumei::Initialize();		break;
		case Task_Gameclear:				Gameclear::Initialize();		break;
			//以下必要に応じて追加
		}
	}
	//現在のタスクの実行・更新処理を呼びだす
	switch (actTask) {
	case Task_Title:					nextTask = Title::UpDate();		break;
	case Task_Game:						nextTask = Game::UpDate();		break;
	case Task_Gameover:					nextTask = Gameover::UpDate();		break;
	case Task_Outsidebusinesshours:		nextTask = Outsidebusinesshours::UpDate();		break;
	case Task_Gamesetsumei:				nextTask = Gamesetsumei::UpDate();		break;
	case Task_Gameclear:				nextTask = Gameclear::UpDate();		break;
		//以下必要に応じて追加
	}
}
//-----------------------------------------------------------------------------
//描画処理
//機能概要：ゲームの１フレームに当たる表示処理
//-----------------------------------------------------------------------------
void  MyGameMain_Render2D()
{
	switch (actTask){
		case Task_Title:					Title::Render();	break;
		case Task_Game:						Game::Render();		break;
		case Task_Gameover:					Gameover::Render();		break;
		case Task_Outsidebusinesshours:		Outsidebusinesshours::Render();		break;
		case Task_Gamesetsumei:				Gamesetsumei::Render();		break;
		case Task_Gameclear:				Gameclear::Render();		break;
		//以下必要に応じて追加
	}
}
