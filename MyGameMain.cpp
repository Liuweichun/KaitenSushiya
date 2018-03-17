#include "MyGameMain.h"

std::map<std::string, Scene*> scenes
{
	{ "Title", new Title() },
	{ "GameRule", new GameRule() },
	{ "Game",new Game() },
	{ "OutsideBusinessHours",new OutsideBusinessHours() },
	{ "GameClear", new GameClear() },
	{ "GameOver",new GameOver() }
};
Scene* currentScene = scenes["Title"];
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

	for (auto& s : scenes)
		if (!s.second->Initialize()) {
			OutputDebugString("Initialize Failed!");
			ge->quitRequest = true;
		}
}
//-----------------------------------------------------------------------------
//解放処理
//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
//-----------------------------------------------------------------------------
void  MyGameMain_Finalize()
{
	DI::GPad_Erase("P1");	//明示的に解放したい場合は
	for (auto& s : scenes)
		s.second->Finalize();
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
	currentScene->Update();
}
//-----------------------------------------------------------------------------
//描画処理
//機能概要：ゲームの１フレームに当たる表示処理
//-----------------------------------------------------------------------------
void  MyGameMain_Render2D()
{
	currentScene->Render();
}
