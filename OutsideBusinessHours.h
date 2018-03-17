#pragma once
#include "Scene.h"

//営業時間外クラス
class OutsideBusinessHours :public Scene
{
protected:
	//ゲーム情報
	DI::Mouse mouse;

	//構造体型定義
	struct Object {
	};
	Object eigyougaistage;
	Object haikei;

public:
	explicit OutsideBusinessHours() {}
	virtual ~OutsideBusinessHours() {}

	virtual void InitializeResource();
	virtual bool Initialize();
	virtual void Update();
	virtual void Render();
	virtual void Finalize();
	//-----------------------------------------------------------------------------
	//クラスのメンバ関数
	//-----------------------------------------------------------------------------
	//営業時間外の表示のプロトタイプ宣言
	void eigyougaistage_Render(Object& ob_);
	//背景の表示のプロトタイプ宣言
	void haikei_Render(Object& ob_);
};
