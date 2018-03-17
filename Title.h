#pragma once
#include "Scene.h"
#include "easing.h"

//タイトルクラス
class Title : public Scene{
protected:
	//ゲーム情報
	DI::Mouse mouse;

	//Alldata構造体
	struct Alldata {
		float x;
		float y;
		int animCnt;				//アニメーション
		ML::Box2D drawBase;
		ML::Box2D src;
		ML::Box2D hitBase;			//当たり判定用
		bool active;				//生存判断
	};

	//Alldata変数
	Alldata title;					//タイトル
	Alldata haikei;					//背景
	Alldata shisyou;				//師匠
	Alldata playbutton;				//ゲーム開始ボタン

public:
	explicit Title() {}
	virtual ~Title() {}

	virtual void InitializeResource();
	virtual bool Initialize();
	virtual void Update();
	virtual void Render();
	virtual void Finalize();
	//-----------------------------------------------------------------------------
	//クラスのメンバ関数
	//-----------------------------------------------------------------------------
	//タイトルの初期化のプロトタイプ宣言
	void title_Initialize(Alldata& a_);
	//タイトルのカウントのプロトタイプ宣言
	void title_UpDate(Alldata& a_);
	//タイトルの表示のプロトタイプ宣言
	void title_Render(Alldata& a_);
	//背景の表示のプロトタイプ宣言
	void haikei_Render(Alldata& a_);
	//師匠の初期化のプロトタイプ宣言
	void shisyou_Initialize(Alldata& a_, float x_, float y_);
	//師匠の表示のプロトタイプ宣言
	void shisyou_Render(Alldata& a_);
	//師匠のアニメーションプロトタイプ宣言
	void shisyou_Anim(Alldata& a_);
	//ゲーム開始ボタンの初期化のプロトタイプ宣言
	void playbutton_Initialize(Alldata& a_, float x_, float y_);
	//ゲーム開始ボタンのカウントのプロトタイプ宣言
	void playbutton_UpDate(Alldata& a_, POINT p_);
	//ゲーム開始ボタンの表示のプロトタイプ宣言
	void playbutton_Render(Alldata& a_);
};