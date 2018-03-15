#include "Title.h"
#include "MyGameMain.h"


void Title::InitializeResource() {
	DG::Image_Create("TitleImg", "./data/image/Title.png");
	DG::Image_Create("Title_haikeiImg", "./data/image/Title_haikei.png");
	DG::Image_Create("shisyouImg", "./data/image/shisyou.png");						//師匠
	DG::Image_Create("kaishibotanImg", "./data/image/kaishibotan.png");				//ゲーム開始ボタン
}

bool Title::Initialize() {

	if (!Scene::Initialize())
		return false;
																				//タイトルの初期化
	title_Initialize(title);
	//師匠の初期化
	shisyou_Initialize(shisyou, 0, 8);
	//ゲーム開始ボタンの初期化
	playbutton_Initialize(playbutton, 10, 10);


	return true;
}

void Title::Update() {
	//マウスカーソルの位置を得る
	mouse = DI::Mouse_GetState();
	POINT pos;
	pos.x = mouse.cursorPos.x;
	pos.y = mouse.cursorPos.y;

	DI::VGamePad  in1 = DI::GPad_GetState("P1");

	//タイトルのカウント
	title_UpDate(title);

	//ゲーム開始ボタンのカウント
	playbutton_UpDate(playbutton, pos);

	if (mouse.LB.down&&playbutton.active == false) {
		currentScene = scenes["GameRule"];
		currentScene->Initialize();
	}
}

void Title::Render() {
	//背景の表示
	haikei_Render(haikei);

	//タイトルの表示
	title_Render(title);

	//師匠の表示
	shisyou_Render(shisyou);

	//ゲーム開始ボタンの表示
	playbutton_Render(playbutton);
}

void Title::Finalize() {
	DG::Image_Erase("TitleImg");
	DG::Image_Erase("Title_haikeiImg");
	DG::Image_Erase("shisyouImg");
	DG::Image_Erase("kaishibotanImg");
}


void Title::title_Initialize(Alldata& a_)
{
	//タイトルイージンの初期化(個有名、移動の種類、開始値、終了値、移動する時間)
	easing::Set("BACKOUT", easing::EASINGTYPE::BACKOUT, -270, 260, 100);			//BACKOUT
	easing::Start("BACKOUT");
	a_.y = 0.f;
}
//--------------------------------------------------------------------------------------------------------------------------------
//タイトルのカウント
void Title::title_UpDate(Alldata& a_)
{
	//イージングの種類によって移動する
	a_.x = easing::GetPos("BACKOUT");
}
//--------------------------------------------------------------------------------------------------------------------------------
//タイトルの表示
void Title::title_Render(Alldata& a_)
{
	ML::Box2D draw(0, 0, 480, 272);
	draw.Offset(a_.x, a_.y);
	ML::Box2D src(0, 0, 480, 272);
	DG::Image_Draw("TitleImg", draw, src);
}
//--------------------------------------------------------------------------------------------------------------------------------
//背景の表示
void Title::haikei_Render(Alldata& a_)
{
	ML::Box2D draw(0, 0, 960, 544);
	ML::Box2D src(0, 0, 960, 544);
	DG::Image_Draw("Title_haikeiImg", draw, src);
}
//--------------------------------------------------------------------------------------------------------------------------------
//師匠の初期化
void Title::shisyou_Initialize(Alldata& a_, float x_, float y_)
{
	a_.x = x_;
	a_.y = y_;
	a_.animCnt = 0;
}
//--------------------------------------------------------------------------------------------------------------------------------
//師匠の表示
void Title::shisyou_Render(Alldata& a_)
{
	shisyou_Anim(a_);
	ML::Box2D draw = a_.drawBase.OffsetCopy(float(a_.x) * 32, float(a_.y) * 32);
	DG::Image_Draw("shisyouImg", draw, a_.src);
}
//--------------------------------------------------------------------------------------------------------------------------------
//師匠のアニメーション
void Title::shisyou_Anim(Alldata& a_)
{
	a_.animCnt++;
	a_.drawBase = ML::Box2D(0, 0, 288, 288);
	switch ((a_.animCnt / 60) % 2) {
	case 0:a_.src = ML::Box2D(0, 0, 224, 224); break;
	case 1:a_.src = ML::Box2D(224, 0, 224, 224); break;
	}
}
//--------------------------------------------------------------------------------------------------------------------------------
//ゲーム開始ボタンの初期化
void Title::playbutton_Initialize(Alldata& a_, float x_, float y_)
{
	a_.active = true;
	a_.x = x_;
	a_.y = y_;
}
//--------------------------------------------------------------------------------------------------------------------------------
//ゲーム開始ボタンのカウント
void Title::playbutton_UpDate(Alldata& a_, POINT p_)
{
	ML::Box2D me = a_.hitBase.OffsetCopy(float(a_.x)* 32.0f, float(a_.y)* 32.0f);
	if (me.x <= p_.x && p_.x < me.x + me.w&&me.y <= p_.y && p_.y <me.y + me.h) {
		a_.active = false;
	}
	else {
		a_.active = true;
	}
}
//--------------------------------------------------------------------------------------------------------------------------------
//ゲーム開始ボタンの表示
void Title::playbutton_Render(Alldata& a_)
{
	if (a_.active == true) {
		ML::Box2D draw(0, 0, 288, 128);
		a_.hitBase = draw;
		draw.Offset(a_.x * 32, a_.y * 32);
		ML::Box2D src(0, 0, 384, 160);
		DG::Image_Draw("kaishibotanImg", draw, src);
	}
	if (a_.active == false) {
		ML::Box2D draw(0, 0, 288, 128);
		a_.hitBase = draw;
		draw.Offset(a_.x * 32, a_.y * 32);			//
		ML::Box2D src(384, 0, 384, 160);
		DG::Image_Draw("kaishibotanImg", draw, src);
	}
}