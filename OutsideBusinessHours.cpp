#include"MyGameMain.h"
#include"OutsideBusinessHours.h"

//-----------------------------------------------------------------------------
//リソース処理
//-----------------------------------------------------------------------------
void OutsideBusinessHours::InitializeResource()
{
	DG::Image_Create("eigyoujikanngaiImg", "./data/image/eigyoujikanngai.png");		//営業時間外
	DG::Image_Create("haikei_blackImg", "./data/image/haikei_black.png");			//背景
}
//-----------------------------------------------------------------------------
//初期化処理
//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
//-----------------------------------------------------------------------------
bool OutsideBusinessHours::Initialize()
{
	if (!Scene::Initialize()) {
		return false;
	}
	return true;
}
//-----------------------------------------------------------------------------
//更新処理
//機能概要：ゲームの１フレームに当たる処理
//-----------------------------------------------------------------------------
void OutsideBusinessHours::Update()
{
	//マウスカーソルの位置を得る
	mouse = DI::Mouse_GetState();

	if (mouse.LB.down) {
		currentScene = scenes["Game"];		//次のタスクをゲーム本編へ
		currentScene->Initialize();
	}
}
//-----------------------------------------------------------------------------
//描画処理
//機能概要：ゲームの１フレームに当たる表示処理
//-----------------------------------------------------------------------------
void OutsideBusinessHours::Render()
{
	//背景の表示
	haikei_Render(haikei);
	//営業時間外の表示
	eigyougaistage_Render(eigyougaistage);
}
//-----------------------------------------------------------------------------
//解放処理
//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
//-----------------------------------------------------------------------------
void OutsideBusinessHours::Finalize()
{
	DG::Image_Erase("eigyoujikanngaiImg");
	DG::Image_Erase("haikei_blackImg");
}
//--------------------------------------------------------------------------------------------------------------------------------
//関数
//--------------------------------------------------------------------------------------------------------------------------------
//営業時間外の表示
void OutsideBusinessHours::eigyougaistage_Render(Object& ob_)
{
	ML::Box2D draw(0, 0, 960, 544);
	ML::Box2D src(0, 0, 960, 544);
	DG::Image_Draw("eigyoujikanngaiImg", draw, src);
}
//--------------------------------------------------------------------------------------------------------------------------------
//背景の表示
void OutsideBusinessHours::haikei_Render(Object& ob_)
{
	ML::Box2D draw(0, 0, 960, 544);
	ML::Box2D src(0, 0, 960, 544);
	DG::Image_Draw("haikei_blackImg", draw, src);
}