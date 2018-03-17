#pragma once
#include "Scene.h"

//ゲームクリアクラス
class GameClear:public Scene
{
protected:
	//ゲーム情報
	DI::Mouse mouse;

	//構造体型定義
	struct Object {
	};
	Object gameclear;
	Object haikei;

public:
	explicit GameClear() {}
	virtual ~GameClear() {}

	virtual void InitializeResource();
	virtual bool Initialize();
	virtual void Update();
	virtual void Render();
	virtual void Finalize();
	//-----------------------------------------------------------------------------
	//クラスのメンバ関数
	//-----------------------------------------------------------------------------
	//ゲームクリアの表示のプロトタイプ宣言
	void gameclear_Render(Object& ob_);
	//背景の表示のプロトタイプ宣言
	void haikei_Render(Object& ob_);
};

