#include "MyPG.h"
#include "MyGameMain.h"
#include <time.h>

//ゲーム本編
namespace Game
{
	//ゲーム情報
	DI::Mouse mouse;
	DI::VGamePad  in1;
	enum Motion { Non, Normal, Dead, Happy,Angry };

	//寿司の構造体
	struct Sushi {
		float x;
		float y;
		int arr[17][30];
		ML::Box2D chip[24];
		int animCnt;				//寿司アニメーション
		bool active;				//在庫判断
		ML::Box2D hitBase;			//当たり判定用
		bool hitFlag;				//当たり判定
		Motion motion;
		ML::Box2D drawBase;
		ML::Box2D src;
		int number;					//番号
		int moveCnt;
		int Cnt;
	};

	//席の構造体
	struct Seat {
		ML::Vec2 pos;
		bool isEmpty;				//席の判断用
	};

	const static int Maxvalue = 6;			//席、客の数を控える
	int emptySeat = Maxvalue;				//空席状況＝６
	//int newcustomer = 0;

	//席の設定変数
	Seat seiki[Maxvalue]{
		{ ML::Vec2(17,3),true },
		{ ML::Vec2(13,3),true },
		{ ML::Vec2(9,3),true },
		{ ML::Vec2(5,3),true },
		{ ML::Vec2(21,3),true },
		{ ML::Vec2(24, 7),true }
	};

	//客の構造体
	struct Chara {
		float x;
		float y;
		int animCnt;				//客アニメーション
		bool active;				//客が存在か判断
		ML::Box2D hitBase;			//当たり判定
		float speed;				//移動速度
		Motion motion;
		ML::Box2D drawBase;
		ML::Box2D src;
		int number;					//番号
		int typeNum;				//客タイプ
		int kaunto;					//カウント
		bool hitFlag;
	};

	//計算構造体
	struct Keisan {
		int kaisu;		//回数
	};

	//ネタ、寿司変数
	Sushi sushi;				//ネタ
	Sushi allsushi[Maxvalue];	//寿司のまとめ			
	Sushi makisu;				//巻きす
	Sushi kaiten[2];			//回転

	//客変数
	Chara customer[Maxvalue];
	Chara lovefood[Maxvalue];

	//計算変数
	Keisan kuriku;		//クリック計算用
	Keisan syari;		//シャリ計算用
	Keisan tamako;		//玉子計算用
	Keisan samon;		//サーマン計算用
	Keisan makuro;		//まくろ計算用
	Keisan nori;		//のり計算用
	Keisan ikura;		//いくら計算用
	Keisan kyuuri;		//きゅうり計算用


	bool clearFlag;		//クリア
	bool timeOver;		//ゲームオーバー
	int timeCnt;
	int timeLeft;
	int jikan;


	//------------------------------------------------------------------------------
	//関数のプロトタイプ宣言
	//------------------------------------------------------------------------------
	//統計初期化のプロトタイプ宣言
	void toukei_Initialize();

	//客を出現させる関数のプロトタイプ宣言
	void customer_Appear();
	//客の初期化のプロトタイプ宣言
	void customer_Initialize(Chara& c_,Seat& s_);
	//客の表示のプロトタイプ宣言
	void customer_Render(Chara& c_);
	//客の行動のプロトタイプ宣言
	void customer_UpDate(Chara& c_, Seat & set_, Chara& lf_, Sushi & asu_);

	void customer_Respawn(Chara& c_, Sushi& s_, Chara& lf_, Seat& se_);

	//客A初期化のプロトタイプ宣言
	void customerA_Initialize(Chara& c_, float x_, float y_);
	//客Aの表示のプロトタイプ宣言
	void customerA_Render(Chara& c_);
	//客Aアニメーションのプロトタイプ宣言
	void customerA_Anim(Chara&  c_);
	//客Aの動作プロトタイプ宣言
	void customerA_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi & asu_);

	//客B初期化のプロトタイプ宣言
	void customerB_Initialize(Chara& c_, float x_, float y_);
	//客Bの表示のプロトタイプ宣言
	void customerB_Render(Chara& c_);
	//客Bアニメーションのプロトタイプ宣言
	void customerB_Anim(Chara&  c_);
	//客Bの動作プロトタイプ宣言
	void customerB_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi & asu_);


	//客C初期化のプロトタイプ宣言
	void customerC_Initialize(Chara& c_, float x_, float y_);
	//客Cの表示のプロトタイプ宣言
	void customerC_Render(Chara& c_);
	//客Cアニメーションのプロトタイプ宣言
	void customerC_Anim(Chara&  c_);
	//客Cの動作プロトタイプ宣言
	void customerC_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi& asu_);

	//客D初期化のプロトタイプ宣言
	void customerD_Initialize(Chara& c_, float x_, float y_);
	//客Dの表示のプロトタイプ宣言
	void customerD_Render(Chara& c_);
	//客Dアニメーションのプロトタイプ宣言
	void customerD_Anim(Chara&  c_);
	//客Dの動作プロトタイプ宣言
	void customerD_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi & asu_);




	//客の好み寿司の初期化のプロトタイプ宣言
	void customerlovefood_Initialize(Chara& lf_, float x_, float y_);
	//客の好み寿司の表示のプロトタイプ宣言
	void customerlovefood_Render(Chara& lf_);
	//客の好み寿司チェックプロトタイプ宣言
	void customerlovefood_Anim(Chara& lf_);




	//寿司まとめチェックのプロトタイプ宣言
	void allsushi_Anim(Sushi& asu_);
	//寿司まとめの初期化のプロトタイプ宣言
	void allsushi_Initialize(float x_, float y_);
	//寿司まとめの表示のプロトタイプ宣言
	void allsushi_Render(Sushi& asu_);
	//寿司まとめの行動のプロトタイプ宣言
	void allsushi_UpDate(Sushi& asu_/*,Chara& c_,Chara& lv_*/);




	//回転のレーン初期化のプロトタイプ宣言
	void kaiten_Initialize(Sushi& k_, float x_, float y_);
	//回転のレーンの行動プロトタイプ宣言
	void kaiten_move(Sushi& k_);
	//回転のレーン表示のプロトタイプ宣言
	void kaiten_Render(Sushi& k_);


	//巻きす初期化のプロトタイプ宣言
	void makisu_Initialize(Sushi& c_, float x_, float y_);
	//巻きす表示のプロトタイプ宣言
	void makisu_Render(Sushi& c_);
	//巻きす行動のプロトタイプ宣言
	void makisu_UpDate(Sushi& c_, POINT p_, Sushi& md_);


	//ネタ単位の移動チェックプロトタイプ宣言
	void Sushi_Check(Sushi& su_, POINT p_, Keisan& ku_);
	//寿司情報を読み込むプロトタイプ宣言
	bool Sushi_Load(Sushi& su_, int n_);
	//寿司の表示プロトタイプ宣言
	void Sushi_Render(Sushi&su_);
	//寿司初期化のプロトタイプ宣言
	void Sushi_Initialize(Sushi&su_);


	//寿司の番号のチェックのプロトタイプ宣言
	int CheckDisabledSushi();


	//-----------------------------------------------------------------------------
	//初期化処理
	//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
	//-----------------------------------------------------------------------------
	void Initialize()
	{
		DG::Image_Create("sushisozaiImg", "./data/image/sushisozai01.png");				//寿司、ネター
		DG::Image_Create("De_sushisozaiImg", "./data/image/De_sushisozai.png");			//腐った寿司
		DG::Image_Create("makurosushiImg", "./data/image/sushi_makurosushi.png");		//マクロ寿司
		DG::Image_Create("tamakosushiImg", "./data/image/sushi_tamakosushi.png");		//たまこ寿司
		DG::Image_Create("samonsushiImg", "./data/image/sushi_samonsushi.png");			//サーモン寿司
		DG::Image_Create("kyuuriImg", "./data/image/sushi_kyuuri.png");					//きゅうり
		DG::Image_Create("makisuImg", "./data/image/sushi_makisu.png");					//巻きす
		DG::Image_Create("makisu01Img", "./data/image/sushi_makisu01.png");				//巻きす
		DG::Image_Create("kaitenImg", "./data/image/kaiten.png");						//回転
		DG::Image_Create("customerImg", "./data/image/customer.png");					//客
		DG::Image_Create("ClearImg", "./data/image/Clear.bmp");			//クリア
		DG::Image_Create("GameoverImg", "./data/image/Gameover.bmp");			//クリア
		DG::Font_Create("FontA", "MS ゴシック", 12, 24);					//文字表示用のフォントを作成

		//PG実行時の時刻を利用して毎回違う乱数表を選択する
		srand((unsigned int)time(NULL));

		//回転の初期化
		for (int k = 0; k < 2; k++) {
			kaiten_Initialize(kaiten[k],-4, 6);
		}

		//巻きす初期化
		makisu_Initialize(makisu, 7, 13);

		//統計の初期化
		toukei_Initialize();

		//寿司のまとめ初期化
		for (int i = 0; i < Maxvalue; ++i) {
			allsushi[i].active = false;
		}

		//客を出現させる
		customer_Appear();

		//客の好み寿司の初期化
		for (int i = 0; i < Maxvalue; ++i) {
			customerlovefood_Initialize(lovefood[i], customer[i].x +1, customer[i].y-1);
		}

		//クリアの初期化
		clearFlag = false;
		//ゲームオーバーの初期化
		bool timeOver = false;
		int timeCnt = 0;
		timeLeft = 60;
		jikan = 0;

		//寿司配列情報の初期化
		Sushi_Initialize(sushi);
		//寿司情報を読み込む
		Sushi_Load(sushi, 1);
	}
	//-----------------------------------------------------------------------------
	//解放処理
	//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
	//-----------------------------------------------------------------------------
	void Finalize()
	{
		DG::Image_Erase("MapChipImg");
		DG::Font_Erase("FontA");
		DG::Image_Erase("ClearImg");
		DG::Image_Erase("GameoverImg");
		DG::Image_Erase("makurosushiImg");
		DG::Image_Erase("makisu01ImgImg");
		DG::Image_Erase("makisuImgImg");

	}
	//-----------------------------------------------------------------------------
	//実行処理
	//機能概要：ゲームの１フレームに当たる処理
	//-----------------------------------------------------------------------------
	TaskFlag UpDate()
	{
		//マウスカーソルの位置を得る
		mouse = DI::Mouse_GetState();
		POINT pos;
		pos.x = mouse.cursorPos.x / 32;
		pos.y = mouse.cursorPos.y / 32;


		if (mouse.LB.down) {
			//範囲内でクリックしたらクリック回数が増えていく
			if (pos.x > 1 && pos.x < 5 && pos.y>11 && pos.y < 15){
				//マップチェック
				Sushi_Check(sushi, pos, kuriku);
				kuriku.kaisu++;				//クリック回数
			}
			//巻きす行動
			makisu_UpDate(makisu, pos, sushi);
		}



		//寿司まとめ行動
		for (int i = 0; i < Maxvalue; ++i) {
			allsushi_UpDate(allsushi[i]);
		}

		//客の行動
		for (int i = 0; i < Maxvalue; ++i) {
			customer_UpDate(customer[i], seiki[i],lovefood[i],allsushi[i]);
		}

		//回転レーンの行動
		for (int k = 0; k < 2; k++) {
			kaiten_move(kaiten[k]);
		}


		//クリアしてないときのみ、プレイヤを動かす
		//if (clearFlag == false&& (timeLeft - time) > 0)
		//{
		//	//playerの行動
		//	player_UpDate(player);
		//}

		//制限時間カウント
		timeCnt++;
		if (timeCnt % 60 == 0 && clearFlag == false && timeOver == false) {
			jikan++;
		}
		if ((timeLeft - jikan) <= 0) {
			timeOver = true;
		}

		TaskFlag rtv = Task_Game;//取りあえず現在のタスクを指定
		if (true == in1.ST.down) {
			rtv = Task_Title;	//次のタスクをタイトルへ
		}
		return rtv;
	}
	//-----------------------------------------------------------------------------
	//描画処理
	//機能概要：ゲームの１フレームに当たる表示処理
	//-----------------------------------------------------------------------------
	void Render()
	{
		//巻きす表示
		makisu_Render(makisu);

		//ネタの表示
		Sushi_Render(sushi);


		//回転の表示
		for (int k = 0; k < 2; k++) {
			kaiten_Render(kaiten[k]);
		}


		//客の表示
		for (int i = 0; i < Maxvalue; ++i) {
			customer_Render(customer[i]);
		}

		//客の好み寿司の表示
		for (int i = 0; i < Maxvalue; ++i) {
			customerlovefood_Render(lovefood[i]);
		}

		//寿司のまとめ表示
		for (int i = 0; i < Maxvalue; ++i) {
			allsushi_Render(allsushi[i]);
		}

		//クリア表示
		if (clearFlag == true) {
			ML::Box2D draw(224, 190, 512, 160);
			ML::Box2D src(0, 0, 250, 80);
			DG::Image_Draw("ClearImg", draw, src);
		}

		if (true == timeOver) {
			ML::Box2D draw(224, 190, 512, 160);
			ML::Box2D src(0, 0, 250, 80);
			DG::Image_Draw("GameoverImg", draw, src);
		}

		//制限時間
		ML::Box2D textBox(0, 0, 480, 32);
		string text = "制限時間：" + to_string(timeLeft - jikan) + "秒";
		DG::Font_Draw("FontA", textBox, text, ML::Color(1, 1, 1, 1));
	}
	//----------------------------------------------------------------
	//関数
	//----------------------------------------------------------------
	//客の初期化
	void customer_Initialize(Chara& c_,Seat& s_)
	{	//客の席の処理
		if (s_.isEmpty==true) {
				s_.isEmpty = false;
				c_.typeNum = rand() % 4;					//客のタイプを乱数で決まる
				switch (c_.typeNum) {
				case 0:
					customerA_Initialize(c_, s_.pos.x, s_.pos.y); break;
				case 1:
					customerB_Initialize(c_, s_.pos.x, s_.pos.y); break;
				case 2:
					customerC_Initialize(c_, s_.pos.x, s_.pos.y); break;
				case 3:
					customerD_Initialize(c_, s_.pos.x, s_.pos.y); break;
				}
				s_.isEmpty = true;
				/*emptySeat -= 1;*/
		}
		//方法B
		//if (emptySeat > 0) {
		//	int newcustomer = 0;
		//	do {
		//		newcustomer = rand() % Maxvalue;
		//	} while (!seiki[newcustomer].isEmpty);
		//	seiki[newcustomer].isEmpty = false;
		//		c_.typeNum = rand() % 3;					//客のタイプを乱数で決まる
		//	switch (c_.typeNum) {
		//	case 0:
		//		customerA_Initialize(c_, seiki[newcustomer].pos.x, seiki[newcustomer].pos.y); break;
		//	case 1:
		//		customerB_Initialize(c_, seiki[newcustomer].pos.x, seiki[newcustomer].pos.y); break;
		//	case 2:
		//		customerC_Initialize(c_, seiki[newcustomer].pos.x, seiki[newcustomer].pos.y); break;
		//	}
		//	emptySeat -= 1;
		//}
	}
	//----------------------------------------------------------------
	//客を出現させる
	void customer_Appear()
	{
		for (int i = 0; i < Maxvalue; ++i) {
			customer_Initialize(customer[i],seiki[i]);
		}
	}
	//----------------------------------------------------------------
	//客の表示
	void customer_Render(Chara& c_)
	{
		switch (c_.typeNum) {
		case 0:
			customerA_Render(c_);
			break;
		case 1:
			customerB_Render(c_);
			break;
		case 2:
			customerC_Render(c_);
			break;
		case 3:
			customerD_Render(c_);
			break;
		}
	}
	//----------------------------------------------------------------
	//客の行動
	void customer_UpDate(Chara& c_, Seat & set_, Chara& lf_, Sushi& asu_)
	{
		switch (c_.typeNum) {
		case 0:
			customerA_UpDate(c_, set_,lf_, asu_);
			break;
		case 1:
			customerB_UpDate(c_, set_,lf_, asu_);
			break;
		case 2:
			customerC_UpDate(c_, set_,lf_, asu_);
			break;
		case 3:
			customerD_UpDate(c_, set_,lf_, asu_);
			break;
		}
		//客アニメーションの行動
		c_.animCnt++;
	}
	//----------------------------------------------------------------
	//客の入店、お帰り処理
	void customer_Respawn(Chara & c_, Sushi & asu_, Chara& lf_, Seat & set_)
	{
		set_.isEmpty = false;
		c_.active = false;
		asu_.active = false;				//寿司が消える
		lf_.active = false;			//客の好み寿司が消える

		//また客を出現させる
		c_.typeNum = rand() % 4;				//客のタイプを乱数で決まる
		switch (c_.typeNum) {
		case 0:
			customerA_Initialize(c_, set_.pos.x, set_.pos.y); break;
		case 1:
			customerB_Initialize(c_, set_.pos.x, set_.pos.y); break;
		case 2:
			customerC_Initialize(c_, set_.pos.x, set_.pos.y); break;
		case 3:
			customerD_Initialize(c_, set_.pos.x, set_.pos.y); break;
		}
		//また客の好み寿司を出現させる
		customerlovefood_Initialize(lf_, c_.x+1,c_.y-1);

		//c_.active = false;			//客がお会計
		//s_.active = false;				//寿司が消える
		//lf_.active = false;			//客の好み寿司が消える
		//se_.isEmpty = true;
	
		//emptySeat++;
		////また客を出現させる
		//c_.typeNum = rand() % 4;					//客のタイプを乱数で決まる
		//switch (c_.typeNum) {
		//case 0:
		//	customerA_Initialize(c_, se_.pos.x, se_.pos.y); break;
		//case 1:									
		//	customerB_Initialize(c_, se_.pos.x, se_.pos.y); break;
		//case 2:									
		//	customerC_Initialize(c_, se_.pos.x, se_.pos.y); break;
		//case 3:									
		//	customerD_Initialize(c_, se_.pos.x, se_.pos.y); break;
		//}
		////また客の好み寿司を出現させる
		//customerlovefood_Initialize(lf_, c_.x + 3, c_.y);
		//ループ回しても、新しいループになるので、席の状態判断できなくなる？？？今の状態だと、第一回の客の当たりになったら次の客が出たら客が重ねる
		//どうやって当たり判断だけの席の状態はtrueになって、emptySeat+1になれるのか＿？？？
	}
	//----------------------------------------------------------------
	//客Aの初期化
	void customerA_Initialize(Chara& c_, float x_, float y_)
	{
		c_.active = false;
		c_.motion = Normal;
		c_.x = x_;
		c_.y = y_;
		c_.animCnt = 0;
		c_.kaunto = 0;
		c_.hitBase = ML::Box2D(-5, 0, 96, 96);
	}
	//----------------------------------------------------------------
	//客Aの表示
	void customerA_Render(Chara& c_)
	{
		if (c_.active == true) {
			if (c_.motion == Non) { return; }
			customerA_Anim(c_);
			ML::Box2D draw = c_.drawBase.OffsetCopy(float(c_.x) * 32, float(c_.y) * 32);
			DG::Image_Draw("customerImg", draw, c_.src);
		}
	}
	//----------------------------------------------------------------
	//客Aアニメーション
	void customerA_Anim(Chara&  c_)
	{
		switch (c_.motion) {
		case Normal:
			c_.drawBase = ML::Box2D(0, -10, 96, 96);
			switch ((c_.animCnt / 10) % 10)
			{
			case 0:c_.src = ML::Box2D(0, 0, 96, 96); break;
			case 1:c_.src = ML::Box2D(96, 0, 96, 96); break;
			case 2:c_.src = ML::Box2D(0, 0, 96, 96); break;
			case 3:c_.src = ML::Box2D(192, 0, 96, 96); break;
			}
			break;
		case Dead:
			//仮
			//c_.drawBase = ML::Box2D(-32, -8, 64, 32);
			//c_.src = ML::Box2D(192, 96, 64, 32);
			break;
		case Happy:
			//仮
			//c_.drawBase = ML::Box2D(-16, -40, 32, 64);
			//switch ((c_.animCnt / 16) % 2)
			//{
			//case 0:c_.src = ML::Box2D(0, 128, 32, 64); break;
			//case 1:c_.src = ML::Box2D(32, 128, 32, 64); break;
			//}
			break;
		case Angry:
			switch ((c_.animCnt / 10) % 10)
			{
			case 0:c_.src = ML::Box2D(192, 288, 96, 96); break;
			case 1:c_.src = ML::Box2D(0, 384, 96, 96); break;
			case 2:c_.src = ML::Box2D(192, 288, 96, 96); break;
			}
			break;
		}
	}
	//----------------------------------------------------------------
	//客Aの動作
	void customerA_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi& asu_)
	{
		//客カウント
		c_.kaunto++;
		if (c_.kaunto == 200){
			c_.active = true;
			if (c_.active == false){
				c_.kaunto = 0;
				c_.kaunto++;
			}
		}
		if (c_.kaunto>1000){
			c_.motion = Angry;
		}
		//if (c_.kaunto > 1100) {
		//	for (int c = 0; c < Maxvalue; ++c) {
		//		if (seiki[c].isEmpty == true) {
		//			//客の入店、お帰り処理
		//			customer_Respawn(c_, allsushi[c], lf_, set_);
		//		}
		//		seiki[c].isEmpty = true;
		//		break;
		//	}
		//}
	}
	//----------------------------------------------------------------
	//客Bの動作
	void customerB_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi& asu_)
	{
		if (c_.kaunto == 300){
			c_.active = true;
			if (c_.active == false){
				c_.kaunto = 0;
				c_.kaunto++;
			}
		}
		if (c_.kaunto>1000){
			c_.motion = Angry;
		}
		//if (c_.kaunto > 1100) {
		//	for (int c = 0; c < Maxvalue; ++c) {
		//		if (seiki[c].isEmpty == true) {
		//			//客の入店、お帰り処理
		//			customer_Respawn(c_, allsushi[c], lf_, set_);
		//		}
		//		seiki[c].isEmpty = true;
		//		break;
		//	}
		//}
		//客カウント
		c_.kaunto++;
	}
	//----------------------------------------------------------------
	//客Cの動作
	void customerC_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi& asu_)
	{
		//客カウント
		c_.kaunto++;
		if (c_.kaunto == 400){
			c_.active = true;
		}
		if (c_.kaunto > 1000){
			c_.motion = Angry;
		}
		//if (c_.kaunto > 1100) {
		//	for (int c = 0; c < Maxvalue; ++c) {
		//		if (seiki[c].isEmpty == true) {
		//			//客の入店、お帰り処理
		//			customer_Respawn(c_, allsushi[c], lf_, set_);
		//		}
		//		seiki[c].isEmpty = true;
		//		break;
		//	}
		//}
	}
	//----------------------------------------------------------------
	//客Dの動作
	void customerD_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi& asu_)
	{
		//客カウント
		c_.kaunto++;
		if (c_.kaunto == 100){
			c_.active = true;
		}
		//客が待つ時間は遅過ぎると怒る
		if (c_.kaunto>1000){
			c_.motion = Angry;
		}
		if (c_.kaunto > 1500) {
			//for (int c = 0; c < Maxvalue; ++c) {
				if (set_.isEmpty == true) {
					//客の入店、お帰り処理
					customer_Respawn(c_, asu_, lf_, set_);
				}
				set_.isEmpty = true;
				//break;
			//}
		}
	}
	//----------------------------------------------------------------
	//客B初期化
	void customerB_Initialize(Chara& c_, float x_, float y_)
	{
		c_.active = false;
		c_.motion = Normal;
		c_.x = x_;
		c_.y = y_;
		c_.animCnt = 0;
		c_.kaunto = 0;
		c_.hitBase = ML::Box2D(-5, 0, 96, 96);
	}
	//----------------------------------------------------------------
	//客Bの表示
	void customerB_Render(Chara& c_)
	{
		if (c_.active == true) {
			if (c_.motion == Non) { return; }
			customerB_Anim(c_);
			ML::Box2D draw = c_.drawBase.OffsetCopy(float(c_.x) * 32, float(c_.y) * 32);
			DG::Image_Draw("customerImg", draw, c_.src);
		}
	}
	//----------------------------------------------------------------
	//客Bアニメーション
	void customerB_Anim(Chara&  c_)
	{
		switch (c_.motion) {
		case Normal:
			c_.drawBase = ML::Box2D(0, -10, 96, 96);
			switch ((c_.animCnt / 10) % 10){
			case 0:c_.src = ML::Box2D(0, 96, 96, 96); break;
			case 1:c_.src = ML::Box2D(96, 96, 96, 96); break;
			case 2:c_.src = ML::Box2D(0, 96, 96, 96); break;
			case 3:c_.src = ML::Box2D(192, 96, 96, 96); break;
			}
			break;
		case Dead:
			//仮
			//c_.drawBase = ML::Box2D(-32, -8, 64, 32);
			//c_.src = ML::Box2D(192, 96, 64, 32);
			break;
		case Happy:
			//仮
			//c_.drawBase = ML::Box2D(-16, -40, 32, 64);
			//switch ((c_.animCnt / 16) % 2)
			//{
			//case 0:c_.src = ML::Box2D(0, 128, 32, 64); break;
			//case 1:c_.src = ML::Box2D(32, 128, 32, 64); break;
			//}
			break;
		case Angry:
			switch ((c_.animCnt / 10) % 10){
			case 0:c_.src = ML::Box2D(96, 384, 96, 96); break;
			case 1:c_.src = ML::Box2D(192, 384, 96, 96); break;
			case 2:c_.src = ML::Box2D(96, 384, 96, 96); break;
			}
			break;
		}
	}
	//----------------------------------------------------------------
	//客C初期化
	void customerC_Initialize(Chara& c_, float x_, float y_)
	{
		c_.active = false;
		c_.motion = Normal;
		c_.x = x_;
		c_.y = y_;
		c_.animCnt = 0;
		c_.kaunto = 0;
		c_.hitBase = ML::Box2D(-5, 0, 96, 96);
	}
	//----------------------------------------------------------------
	//客Cの表示
	void customerC_Render(Chara& c_)
	{
		if (c_.active == true) {
			if (c_.motion == Non) { return; }
			customerC_Anim(c_);
			ML::Box2D draw = c_.drawBase.OffsetCopy(float(c_.x) * 32, float(c_.y) * 32);
			DG::Image_Draw("customerImg", draw, c_.src);
		}
	}
	//----------------------------------------------------------------
	//客Cアニメーション
	void customerC_Anim(Chara&  c_)
	{
		switch (c_.motion) {
		case Normal:
			c_.drawBase = ML::Box2D(0, -10, 96, 96);
			switch ((c_.animCnt / 10) % 10)
			{
			case 0:c_.src = ML::Box2D(0, 192, 96, 96); break;
			case 1:c_.src = ML::Box2D(96, 192, 96, 96); break;
			case 2:c_.src = ML::Box2D(0, 192, 96, 96); break;
			case 3:c_.src = ML::Box2D(192, 192, 96, 96); break;
			}
			break;
		case Dead:
			//仮
			//c_.drawBase = ML::Box2D(-32, -8, 64, 32);
			//c_.src = ML::Box2D(192, 96, 64, 32);
			break;
		case Happy:
			//仮
			//c_.drawBase = ML::Box2D(-16, -40, 32, 64);
			//switch ((c_.animCnt / 16) % 2)
			//{
			//case 0:c_.src = ML::Box2D(0, 128, 32, 64); break;
			//case 1:c_.src = ML::Box2D(32, 128, 32, 64); break;
			//}
			break;
		case Angry:
			switch ((c_.animCnt / 10) % 10){
			case 0:c_.src = ML::Box2D(0, 288, 96, 96); break;
			case 1:c_.src = ML::Box2D(96, 288, 96, 96); break;
			case 2:c_.src = ML::Box2D(0, 288, 96, 96); break;
			}
			break;
		}
	}
	//----------------------------------------------------------------
	//客Dの初期化
	void customerD_Initialize(Chara& c_, float x_, float y_)
	{
		c_.active = false;
		c_.motion = Normal;
		c_.x = x_;
		c_.y = y_;
		c_.animCnt = 0;
		c_.kaunto = 0;
		c_.hitBase = ML::Box2D(-5,0, 96, 96);
	}
	//----------------------------------------------------------------
	//客Dの表示
	void customerD_Render(Chara& c_)
	{
		if (c_.active == true) {
			if (c_.motion == Non) { return; }
			customerD_Anim(c_);
			ML::Box2D draw = c_.drawBase.OffsetCopy(float(c_.x) * 32, float(c_.y) * 32);
			DG::Image_Draw("customerImg", draw, c_.src);
		}
	}
	//----------------------------------------------------------------
	//客Dアニメーション
	void customerD_Anim(Chara&  c_)
	{
		switch (c_.motion) {
		case Normal:
			c_.drawBase = ML::Box2D(0, -10, 96, 96);
			switch ((c_.animCnt / 10) % 10){
			case 0:c_.src = ML::Box2D(0, 480, 96, 96); break;
			case 1:c_.src = ML::Box2D(96, 480, 96, 96); break;
			}
			break;
		case Dead:c_.src = ML::Box2D(192, 480, 96, 96); break;
			break;
		case Happy:
			switch ((c_.animCnt / 5) % 5) {
			case 0:c_.src = c_.src = ML::Box2D(0, 576, 96, 96); break;
			case 1:c_.src = c_.src = ML::Box2D(96, 576, 96, 96); break;
			}
			break;
		case Angry:
			switch ((c_.animCnt / 8) % 8){
			case 0:c_.src = ML::Box2D(192, 576, 96, 96); break;
			case 1:c_.src = ML::Box2D(0, 672, 96, 96); break;
			}
			break;
		}
	}
	//----------------------------------------------------------------
	//客の好み寿司の初期化
	void customerlovefood_Initialize(Chara& lf_, float x_, float y_)
	{
		lf_.x = x_;
		lf_.y = y_;
		lf_.active = true;
		lf_.number = 9 + rand() % 5;
	}
	//----------------------------------------------------------------
	//客の好み寿司の表示
	void customerlovefood_Render(Chara& lf_)
	{
		if (lf_.active == true) {
			customerlovefood_Anim(lf_);
			ML::Box2D draw = lf_.drawBase.OffsetCopy(float(lf_.x) * 32, float(lf_.y) * 32);
			DG::Image_Draw("sushisozaiImg", draw, lf_.src);
		}
	}
	//----------------------------------------------------------------
	//客の好み寿司チェック
	void customerlovefood_Anim(Chara& lf_)
	{
		lf_.drawBase = ML::Box2D(0, -12, 32, 32);
		switch (lf_.number) {
		case 9:lf_.src = ML::Box2D(32, 64, 32, 32);  break;			//マクロ寿司
		case 10:lf_.src = ML::Box2D(64, 64, 32, 32); break;			//サーモン寿司
		case 11:lf_.src = ML::Box2D(96, 64, 32, 32); break;			//玉子寿司
		case 12:lf_.src = ML::Box2D(0, 96, 32, 32); break;			//いくら寿司
		case 13:lf_.src = ML::Box2D(32, 96, 32, 32); break;			//きゅうり寿司
		//case 13:lf_.src = ML::Box2D(32, 32, 32, 32); break;
		//case 14:lf_.src = ML::Box2D(160, 0, 32, 32); break;
		}
	}
	//----------------------------------------------------------------
	//寿司のまとめ初期化
	void allsushi_Initialize(float x_, float y_)
	{
		for (int i = 0; i < Maxvalue; ++i) {
			if (allsushi[i].active == false) {
				//寿司を生成する
				allsushi[i].active = true;
				allsushi[i].x = x_;
				allsushi[i].y = y_;
				allsushi[i].motion = Normal;
				allsushi[i].moveCnt = 0;
				allsushi[i].hitBase = ML::Box2D(0, -12, 32, 32);
				allsushi[i].animCnt = 0;
				allsushi[i].Cnt = 0;
				break;						//寿司がループを抜ける
			}
		}
	}
	//----------------------------------------------------------------
	//寿司まとめの表示
	void allsushi_Render(Sushi& asu_)
	{
		if (asu_.active == true) {
			if (asu_.motion == Non) { return; }
			if (asu_.motion == Normal) {
				allsushi_Anim(asu_);
				ML::Box2D draw = asu_.drawBase.OffsetCopy(float(asu_.x) * 32, float(asu_.y) * 32);
				DG::Image_Draw("sushisozaiImg", draw, asu_.src);
			}
			if (asu_.motion == Dead) {
				allsushi_Anim(asu_);
				ML::Box2D draw = asu_.drawBase.OffsetCopy(float(asu_.x) * 32, float(asu_.y) * 32);
				DG::Image_Draw("De_sushisozaiImg", draw, asu_.src);
			}
		}
	}
	//----------------------------------------------------------------
	//寿司まとめチェック
	void allsushi_Anim(Sushi& asu_)
	{
		//enum {
		//	makurosushi = 9, samonsushi = 10,tamakosushi = 11,
		//};
		switch (asu_.motion) {
		case Normal:
			asu_.drawBase = ML::Box2D(0, -12, 32, 32);
			switch (asu_.number){
			case 9:asu_.src = ML::Box2D(32, 64, 32, 32); break;			//マクロ寿司
			case 10:asu_.src = ML::Box2D(64, 64, 32, 32); break;		//サーモン寿司
			case 11:asu_.src = ML::Box2D(96, 64, 32, 32); break;		//玉子寿司
			case 12:asu_.src = ML::Box2D(0, 96, 32, 32); break;			//いくら寿司
			case 13:asu_.src = ML::Box2D(32, 96, 32, 32); break;		//きゅうり寿司
			case 14:asu_.src = ML::Box2D(160, 0, 32, 32); break;
			case 15:asu_.src = ML::Box2D(160, 0, 32, 32); break;
			}
			break;

		case Dead:
			asu_.drawBase = ML::Box2D(0, -12, 32, 32);
			switch (asu_.number) {
			case 9:														//マクロ寿司
				switch ((asu_.animCnt / 5) % 5){
				case 0:asu_.src = ML::Box2D(0,0, 32, 32); break;
				case 1:asu_.src = ML::Box2D(32, 0, 32, 32); break;
				case 2:asu_.src = ML::Box2D(64, 0, 32, 32); break;
				case 3:asu_.src = ML::Box2D(96, 0, 32, 32); break;
				}
			}
			break;
		}
	}
	//----------------------------------------------------------------
	//寿司まとめ行動
	void allsushi_UpDate(Sushi& asu_)
	{
		asu_.moveCnt++;
		if (asu_.active == true) {
			float speedCnt;					//移動速度
			speedCnt = 0.08f;
			if (asu_.moveCnt<250) {
				asu_.x += speedCnt;
			}
			if (asu_.moveCnt>250&& asu_.moveCnt<300) {
				asu_.y += speedCnt;
			}
			if (asu_.moveCnt>300 && asu_.moveCnt<550) {
				asu_.x -= speedCnt;
			}
			if (asu_.moveCnt>550 && asu_.moveCnt<600) {
				asu_.y -= speedCnt;
			}
			if (asu_.moveCnt>600) {
				asu_.motion = Dead;
				asu_.moveCnt = 0;
			}
		}
		//寿司と客の当たり判定
		if (asu_.active == true) {
			ML::Box2D me = asu_.hitBase.OffsetCopy(asu_.x * 32, asu_.y * 32);
			//寿司は客と当たり判定をさせる
			for (int c = 0; c < Maxvalue; ++c) {
				if (customer[c].active == true) {											//客いる場合
					//寿司自身の当たり判定の写真を用意（現在位置）
					ML::Box2D you = customer[c].hitBase.OffsetCopy(customer[c].x * 32, customer[c].y * 32);
					//接触判定

					if (true == you.Hit(me) && lovefood[c].number == asu_.number){			//自分と接触してるか相手に判断してもらう
						switch (asu_.motion)
						{
						case Normal:
							customer[c].motion = Happy;
							if (customer[c].motion == Happy) {
								asu_.Cnt++;
							}
							break;
						case Dead:
							customer[c].motion = Dead;
							if (customer[c].motion == Dead) {
								asu_.Cnt++;
							}
							break;
						}
						//客が新鮮な寿司を食べたら、笑う
						if (customer[c].motion ==Happy&&asu_.Cnt>35) {
							//客の入店、お帰り処理
							customer_Respawn(customer[c], asu_, lovefood[c], seiki[c]);
							asu_.Cnt=0;
						}
						//客が新鮮じゃない寿司を食べたら、泣く
						if(customer[c].motion == Dead && asu_.Cnt>35){
							//客の入店、お帰り処理
							customer_Respawn(customer[c], asu_, lovefood[c], seiki[c]);
							asu_.Cnt = 0;
						}
						//客の入店、お帰り処理
						//customer_Respawn(customer[c], asu_, lovefood[c], seiki[c]);
						seiki[c].isEmpty = true;
						break;
					}
				}
			}
		}
		//寿司アニメーションの行動
		asu_.animCnt++;
	}
	//----------------------------------------------------------------
	//巻きす初期化
	void makisu_Initialize(Sushi& c_, float x_, float y_)
	{
		c_.active = true;
		c_.x = x_;
		c_.y = y_;
	}
	//----------------------------------------------------------------
	//巻きす行動
	void makisu_UpDate(Sushi& c_, POINT p_, Sushi& md_)
	{
		int bangou;
		//範囲内チェック
		if (p_.x >= 7 && p_.x < 10 && p_.y >= 12 && p_.y <= 15) {
			for (int i = 12; i <= 14; ++i) {
				for (int j = 7; j <= 9; ++j) {
					bangou = md_.arr[i][j];
					switch (bangou) {
					case 0:					//シャリ
						syari.kaisu++;
						break;
					case 1:					//たまこ
						tamako.kaisu++;
						break;
					case 2:
						samon.kaisu++;		//サーモン
						break;
					case 3:
						nori.kaisu++;		//のり
						break;
					case 4:
						makuro.kaisu++;		//まくろ
						break;
					case 5:
						ikura.kaisu++;		//いくら
						break;
					case 6:
						kyuuri.kaisu++;		//きゅうり
						break;
					}
					md_.arr[i][j] = 24;			//クリックしたら、空欄になる
				}
			}
			//シャリ=2、たまこ=1、玉子寿司（11番）
			if (syari.kaisu == 2 && tamako.kaisu == 1) {
				allsushi[CheckDisabledSushi()].number = 11;
				allsushi_Initialize(3,6);
			}
			//シャリ=2、サーモン=1、サーモン寿司（10番）
			if (syari.kaisu == 2 && samon.kaisu == 1) {
				allsushi[CheckDisabledSushi()].number = 10;
				allsushi_Initialize(3, 6);
			}
			//シャリ=2、まくろ=1なら、マクロ寿司（9番）
			if (syari.kaisu == 2 && makuro.kaisu == 1) {
				allsushi[CheckDisabledSushi()].number = 9;
				allsushi_Initialize(3, 6);
			}
			//シャリ=2、いくら=1、のり=1なら、いくら寿司（12番）
			if (syari.kaisu == 2 && ikura.kaisu == 1 && nori.kaisu == 1&&kyuuri.kaisu==1) {
				allsushi[CheckDisabledSushi()].number = 12;
				allsushi_Initialize(3, 6);
			}
			//シャリ=3、のり=1なら、のり=1なら、きゅうり寿司（13番）
			if (syari.kaisu == 3&& nori.kaisu == 1 && kyuuri.kaisu == 1) {
				allsushi[CheckDisabledSushi()].number = 13;
				allsushi_Initialize(3, 6);
			}

			//寿司を作ったら、各ネーター計算0になる
			syari.kaisu = 0, samon.kaisu = 0, tamako.kaisu = 0;
			makuro.kaisu = 0, ikura.kaisu = 0, nori.kaisu=0;
			kyuuri.kaisu = 0;
		}
	}
	//----------------------------------------------------------------
	//巻きす表示
	void makisu_Render(Sushi& c_)
	{
		if (c_.active == true) {
			ML::Box2D draw(-14, -46, 128, 128);
			c_.hitBase = draw;
			draw.Offset(c_.x * 32, c_.y * 32);
			ML::Box2D src(0, 0, 718, 759);
			DG::Image_Draw("makisuImg", draw, src);
		}
		else {
			ML::Box2D draw(-14, -46, 128, 128);
			c_.hitBase = draw;
			draw.Offset(c_.x * 32, c_.y * 32);
			ML::Box2D src(0, 0, 718, 759);
			DG::Image_Draw("makisu01Img", draw, src);
		}
	}
	//----------------------------------------------------------------
	//寿司単位の移動チェック
	void Sushi_Check(Sushi& md_, POINT p_, Keisan& ku_)
	{
		//マップ配列外のデータをクリックしすることを防止
		if (p_.x < 0) { return; }
		if (p_.y < 0) { return; }
		if (p_.x >= 30) { return; }
		if (p_.y >= 17) { return; }

		//マウスに対する処理
		int chip = md_.arr[p_.y][p_.x];
		//シャリ=0、たまこ=1、サーモン=2、のり=3
		switch (chip) {
			//シャリをクリックしたら、コピーする
		case 0:
			if (ku_.kaisu == 0) { md_.arr[p_.y][p_.x + 5] = 0; }
			if (ku_.kaisu == 1) { md_.arr[p_.y][p_.x + 6] = 0; }
			if (ku_.kaisu == 2) { md_.arr[p_.y][p_.x + 7] = 0; }
			if (ku_.kaisu == 3) { md_.arr[p_.y + 1][p_.x + 5] = 0; }
			if (ku_.kaisu == 4) { md_.arr[p_.y + 1][p_.x + 6] = 0; }
			if (ku_.kaisu == 5) { md_.arr[p_.y + 1][p_.x + 7] = 0; }
			if (ku_.kaisu == 6) { md_.arr[p_.y + 2][p_.x + 5] = 0; }
			if (ku_.kaisu == 7) { md_.arr[p_.y + 2][p_.x + 6] = 0; }
			if (ku_.kaisu == 8) { md_.arr[p_.y + 2][p_.x + 7] = 0; }
			if (ku_.kaisu > 8) { ku_.kaisu = -1; }
			break;
			//玉子をクリックしたら、コピーする
		case 1:
			if (ku_.kaisu == 0) { md_.arr[p_.y][p_.x + 4] = 1; }
			if (ku_.kaisu == 1) { md_.arr[p_.y][p_.x + 5] = 1; }
			if (ku_.kaisu == 2) { md_.arr[p_.y][p_.x + 6] = 1; }
			if (ku_.kaisu == 3) { md_.arr[p_.y + 1][p_.x + 4] = 1; }
			if (ku_.kaisu == 4) { md_.arr[p_.y + 1][p_.x + 5] = 1; }
			if (ku_.kaisu == 5) { md_.arr[p_.y + 1][p_.x + 6] = 1; }
			if (ku_.kaisu == 6) { md_.arr[p_.y + 2][p_.x + 4] = 1; }
			if (ku_.kaisu == 7) { md_.arr[p_.y + 2][p_.x + 5] = 1; }
			if (ku_.kaisu == 8) { md_.arr[p_.y + 2][p_.x + 6] = 1; }
			if (ku_.kaisu > 8) {
				ku_.kaisu = -1;
			}
			break;
			//サーモンをクリックしたら、コピーする
		case 2:
			if (ku_.kaisu == 0) { md_.arr[p_.y][p_.x + 3] = 2; }
			if (ku_.kaisu == 1) { md_.arr[p_.y][p_.x + 4] = 2; }
			if (ku_.kaisu == 2) { md_.arr[p_.y][p_.x + 5] = 2; }
			if (ku_.kaisu == 3) { md_.arr[p_.y + 1][p_.x + 3] = 2; }
			if (ku_.kaisu == 4) { md_.arr[p_.y + 1][p_.x + 4] = 2; }
			if (ku_.kaisu == 5) { md_.arr[p_.y + 1][p_.x + 5] = 2; }
			if (ku_.kaisu == 6) { md_.arr[p_.y + 2][p_.x + 3] = 2; }
			if (ku_.kaisu == 7) { md_.arr[p_.y + 2][p_.x + 4] = 2; }
			if (ku_.kaisu == 8) { md_.arr[p_.y + 2][p_.x + 5] = 2; }
			if (ku_.kaisu > 8)
			{
				ku_.kaisu = -1;
			}
			break;
			//海苔をクリックしたら、コピーする
		case 3:
			if (ku_.kaisu == 0) { md_.arr[p_.y - 1][p_.x + 5] = 3; }
			if (ku_.kaisu == 1) { md_.arr[p_.y - 1][p_.x + 6] = 3; }
			if (ku_.kaisu == 2) { md_.arr[p_.y - 1][p_.x + 7] = 3; }
			if (ku_.kaisu == 3) { md_.arr[p_.y][p_.x + 5] = 3; }
			if (ku_.kaisu == 4) { md_.arr[p_.y][p_.x + 6] = 3; }
			if (ku_.kaisu == 5) { md_.arr[p_.y][p_.x + 7] = 3; }
			if (ku_.kaisu == 6) { md_.arr[p_.y + 1][p_.x + 5] = 3; }
			if (ku_.kaisu == 7) { md_.arr[p_.y + 1][p_.x + 6] = 3; }
			if (ku_.kaisu == 8) { md_.arr[p_.y + 1][p_.x + 7] = 3; }
			if (ku_.kaisu > 8)
			{
				ku_.kaisu = -1;
			}
			break;
			//まくろをクリックしたら、コピーする
		case 4:
			if (ku_.kaisu == 0) { md_.arr[p_.y - 1][p_.x + 4] = 4; }
			if (ku_.kaisu == 1) { md_.arr[p_.y - 1][p_.x + 5] = 4; }
			if (ku_.kaisu == 2) { md_.arr[p_.y - 1][p_.x + 6] = 4; }
			if (ku_.kaisu == 3) { md_.arr[p_.y][p_.x + 4] = 4; }
			if (ku_.kaisu == 4) { md_.arr[p_.y][p_.x + 5] = 4; }
			if (ku_.kaisu == 5) { md_.arr[p_.y][p_.x + 6] = 4; }
			if (ku_.kaisu == 6) { md_.arr[p_.y + 1][p_.x + 4] = 4; }
			if (ku_.kaisu == 7) { md_.arr[p_.y + 1][p_.x + 5] = 4; }
			if (ku_.kaisu == 8) { md_.arr[p_.y + 1][p_.x + 6] = 4; }
			if (ku_.kaisu > 8)
			{
				ku_.kaisu = -1;
			}
			break;
			//いくらをクリックしたら、コピーする
		case 5:
			if (ku_.kaisu == 0) { md_.arr[p_.y - 1][p_.x + 3] = 5; }
			if (ku_.kaisu == 1) { md_.arr[p_.y - 1][p_.x + 4] = 5; }
			if (ku_.kaisu == 2) { md_.arr[p_.y - 1][p_.x + 5] = 5; }
			if (ku_.kaisu == 3) { md_.arr[p_.y][p_.x + 3] = 5; }
			if (ku_.kaisu == 4) { md_.arr[p_.y][p_.x + 4] = 5; }
			if (ku_.kaisu == 5) { md_.arr[p_.y][p_.x + 5] = 5; }
			if (ku_.kaisu == 6) { md_.arr[p_.y + 1][p_.x + 3] = 5; }
			if (ku_.kaisu == 7) { md_.arr[p_.y + 1][p_.x + 4] = 5; }
			if (ku_.kaisu == 8) { md_.arr[p_.y + 1][p_.x + 5] = 5; }
			if (ku_.kaisu > 8)
			{
				ku_.kaisu = -1;
			}
			break;
			//きゅうりをクリックしたら、コピーする
		case 6:
			if (ku_.kaisu == 0) { md_.arr[p_.y-2][p_.x + 5] = 6; }
			if (ku_.kaisu == 1) { md_.arr[p_.y-2][p_.x + 6] = 6; }
			if (ku_.kaisu == 2) { md_.arr[p_.y-2][p_.x + 7] = 6; }
			if (ku_.kaisu == 3) { md_.arr[p_.y-1][p_.x + 5] = 6; }
			if (ku_.kaisu == 4) { md_.arr[p_.y-1][p_.x + 6] = 6; }
			if (ku_.kaisu == 5) { md_.arr[p_.y-1][p_.x + 7] = 6; }
			if (ku_.kaisu == 6) { md_.arr[p_.y][p_.x + 5] = 6; }
			if (ku_.kaisu == 7) { md_.arr[p_.y][p_.x + 6] = 6; }
			if (ku_.kaisu == 8) { md_.arr[p_.y][p_.x + 7] = 6; }
			if (ku_.kaisu > 8)
			{
				ku_.kaisu = -1;
			}
			break;
		}
	}
	//----------------------------------------------------------------
	//寿司情報を読み込む
	bool Sushi_Load(Sushi& md_, int n_)
	{
		//ファイルパスを作る
		string filePath = "./data/map/Map" + to_string(n_) + ".txt";

		//ファイルを開く（読み込み）
		ifstream fin(filePath);
		if (!fin) { return false; }		//読み込み失敗

		//配列にデータを取り込む
		for (int y = 0; y < 17; ++y) {
			for (int x = 0; x < 30; ++x) {
				fin >> md_.arr[y][x];
			}
		}
		//ファイルを閉じる
		fin.close();
		return true;
	}
	//----------------------------------------------------------------
	//寿司表示
	void Sushi_Render(Sushi&md_)
	{
		for (int y = 0; y < 17; ++y){
			for (int x = 0; x < 30; ++x) {
				ML::Box2D  draw(0, 0, 32, 32);
				draw.Offset(x * 32, y * 32);		//表示位置を調整
				DG::Image_Draw("sushisozaiImg", draw, md_.chip[md_.arr[y][x]]);
			}
		}
	}
	//----------------------------------------------------------------
	//寿司初期化
	void Sushi_Initialize(Sushi&md_)
	{
		//データのコピー
		for (int y = 0; y < 17; ++y){
			for (int x = 0; x < 30; ++x){
				md_.arr[y][x];
			}
		}
		//寿司チップ情報の初期化
		for (int c = 0; c < 24; ++c){
			int x = (c % 4);
			int y = (c / 4);
			md_.chip[c] = ML::Box2D(x * 32, y * 32, 32, 32);
			//ML::Box2D draw = md_.chip[c].OffsetCopy(md_.x, md_.y);
		}
	}
	//----------------------------------------------------------------
	//回転レーンの初期化
	void kaiten_Initialize(Sushi& k_, float x_, float y_)
	{
		k_.x = x_;
		k_.y = y_;
		k_.moveCnt = 0;
	}
	//----------------------------------------------------------------
	//回転レーンの行動
	void kaiten_move(Sushi& k_)
	{
		k_.moveCnt++;
		if (k_.moveCnt == 100) {
			k_.moveCnt = 0;
		}
	}
	//----------------------------------------------------------------
	//回転レーンの表示
	void kaiten_Render(Sushi& k_)
	{
		ML::Box2D draw(0, -12, 1100, 32);
		//c_.hitBase = draw;
		draw.x = draw.x +k_.moveCnt;
		draw.Offset(k_.x * 32, k_.y * 32);
		ML::Box2D src(0, 0, 1100, 32);
		DG::Image_Draw("kaitenImg", draw, src);
	}
	//----------------------------------------------------------------
	//統計初期化
	void toukei_Initialize()
	{
		//マウスでクリック回数
		kuriku.kaisu = 0;
		//シャリ計算用
		syari.kaisu = 0;
		//玉子計算用
		tamako.kaisu = 0;
		//サーマン計算用
		samon.kaisu = 0;
		//まくろ計算用
		makuro.kaisu = 0;
		//のり計算用
		nori.kaisu = 0;
	}
	//----------------------------------------------------------------
	//寿司の番号のチェック
	int CheckDisabledSushi()
	{
		//*@brief		現在非使用中のすしの番号を調べます
		//	*	@return		使える寿司の番号
		for (int i = 0; i < _countof(allsushi); ++i)
			if (!allsushi[i].active)
				return i;
		return -1;
	}
}
