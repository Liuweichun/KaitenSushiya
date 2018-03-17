#pragma once
#include "Scene.h"

//ゲームクリアクラス
class GameOver :public Scene
{
protected:
	//ゲーム情報
	DI::Mouse mouse;

	//構造体型定義
	struct Object {
	};
	Object overstage;
	Object haikei;

public:
	explicit GameOver() {}
	virtual ~GameOver() {}

	virtual void InitializeResource();
	virtual bool Initialize();
	virtual void Update();
	virtual void Render();
	virtual void Finalize();
	//-----------------------------------------------------------------------------
	//クラスのメンバ関数
	//-----------------------------------------------------------------------------
	//ゲームオーバーの表示のプロトタイプ宣言
	void overstage_Render(Object& ob_);
	//背景の表示のプロトタイプ宣言
	void haikei_Render(Object& ob_);
};