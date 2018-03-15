#include "MyPG.h"
#include "MyGameMain.h"
#include <time.h>
#include "easing.h"

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
		bool active;				//生存判断
		ML::Box2D hitBase;			//当たり判定用
		bool hitFlag;				//当たり判定
		Motion motion;
		ML::Box2D drawBase;
		ML::Box2D src;
		int number;					//番号
		int moveCnt;
		int Cnt;
	};

	//席、設備の構造体
	struct Seat {
		ML::Vec2 pos;
		bool isEmpty;				//席の判断用
		float x;
		float y;
		bool active;
		ML::Box2D hitBase;			//当たり判定用
		int moveCnt;
		ML::Box2D drawBase;
		ML::Box2D src;
		int animCnt;				//アニメーション
		int kaunto;					//カウント
	};

	const static int Maxvalue = 6;			//席、客の数を控える,空席状況＝６
	int emptySeat = Maxvalue;				//空席状況＝６
	//int newcustomer = 0;

	//席、店設備の設定変数
	Seat seiki[Maxvalue]{
		{ ML::Vec2(21,3),true },
		{ ML::Vec2(16,3),true },
		{ ML::Vec2(11,3),true },
		{ ML::Vec2(6,3),true },
		{ ML::Vec2(2.3f,6),true },
		{ ML::Vec2(25, 6),true }
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
		int nokori;		//残り個数
		int money;		//値段
	};
	enum {
		//ネタ
		SYARI,			//シャリ
		TAMAKO,			//玉子
		SAMON,			//サーマン
		NORI,			//のり
		MAKURO,			//まくろ
		IKURA,			//いくら
		KYUURI,			//きゅうり
		AMAEBI,			//甘海老
		EBI,			//海老
		//計算
		SUSHI,
		MAKI,
		KURIKU,			//クリック
		NETA_MAXNUM,	//最大数
	};
	Keisan neta[NETA_MAXNUM];		//ネタの最大数

	//プレイヤーの構造体
	struct Player {
		bool clearFlag;			//クリア
		bool gameoverFlag;		//gameover
		bool timeOver;			//営業終了時間
		int timeCnt;
		int timeStart;			//営業開始時間
		int jikan;
		int shikin;				//プレイヤー資金
		int ninki;				//プレイヤー人気
		ML::Box2D hitBase;
		ML::Box2D drawBase;
		ML::Box2D src;
		int animCnt;			//アニメーション
		bool active;
		float x;
		float y;
		int Cnt;
	};
	//プレイヤー変数
	Player player;
	Player shisyou;				//師匠
	Player haikei;				//背景
	Player eigyoukaishi;		//営業開始

	//席、店設備の設定変数
	Seat menu;					//メニュー
	Seat phone;					//電話
	Seat kaitenren;				//回転レーン
	Seat toragu;				//運送の車

	//ネタ、寿司変数
	Sushi sushi;				//ネタ
	Sushi allsushi[Maxvalue];	//寿司のまとめ			
	Sushi makisu;				//巻きす

	//客、客の好み寿司変数
	Chara customer[Maxvalue];
	Chara lovefood[Maxvalue];
	Chara fukidashi[Maxvalue];


	//------------------------------------------------------------------------------
	//関数のプロトタイプ宣言
	//------------------------------------------------------------------------------
	//プレイヤーの初期化のプロトタイプ宣言
	void player_Initialize(Player& pl_);
	//プレイヤーのカウントのプロトタイプ宣言
	void player_UpDate(Player& pl_);
	//プレイヤーの表示のプロトタイプ宣言
	void player_Render(Player& pl_);
	//背景の初期化のプロトタイプ宣言
	void haikei_Initialize(Player& h_, float x_, float y_);
	//背景の表示のプロトタイプ宣言
	void haikei_Render(Player& h_);
	//師匠の初期化のプロトタイプ宣言
	void shisyou_Initialize(Player& siy_, float x_, float y_);
	//師匠の表示のプロトタイプ宣言
	void shisyou_Render(Player& siy_);
	//師匠のアニメーションプロトタイプ宣言
	void shisyou_Anim(Player& siy_);
	//営業開始の初期化のプロトタイプ宣言
	void eigyoukaishi_Initialize(Player& c_);
	//営業開始のカウントのプロトタイプ宣言
	void eigyoukaishi_UpDate(Player& e_);
	//営業開始の表示のプロトタイプ宣言
	void eigyoukaishi_Render(Player& e_);	
	//統計初期化のプロトタイプ宣言
	void toukei_Initialize();
	//客を出現させる関数のプロトタイプ宣言
	void customer_Appear();
	//客の初期化のプロトタイプ宣言
	void customer_Initialize(Chara& c_,Seat& s_);
	//客の表示のプロトタイプ宣言
	void customer_Render(Chara& c_);
	//客の行動のプロトタイプ宣言
	void customer_UpDate(Chara& c_, Seat & set_, Chara& lf_, Sushi & asu_, Chara& fu_);
	//客の入店、お帰りのプロトタイプ宣言
	void customer_Respawn(Chara& c_, Sushi& s_, Chara& lf_, Seat& se_, Chara& fu_);
	//メニューの初期化のプロトタイプ宣言
	void menu_Initialize(Seat& s_, float x_, float y_);
	//メニューの表示のプロトタイプ宣言
	void menu_Render(Seat& s_);
	//メニューの行動のプロトタイプ宣言
	void menu_UpDate(Seat& s_, POINT p_);
	//客A初期化のプロトタイプ宣言
	void customerA_Initialize(Chara& c_, float x_, float y_);
	//客Aの表示のプロトタイプ宣言
	void customerA_Render(Chara& c_);
	//客Aアニメーションのプロトタイプ宣言
	void customerA_Anim(Chara&  c_);
	//客Aの動作プロトタイプ宣言
	void customerA_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi & asu_, Chara& fu_);
	//客B初期化のプロトタイプ宣言
	void customerB_Initialize(Chara& c_, float x_, float y_);
	//客Bの表示のプロトタイプ宣言
	void customerB_Render(Chara& c_);
	//客Bアニメーションのプロトタイプ宣言
	void customerB_Anim(Chara&  c_);
	//客Bの動作プロトタイプ宣言
	void customerB_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi & asu_, Chara& fu_);
	//客C初期化のプロトタイプ宣言
	void customerC_Initialize(Chara& c_, float x_, float y_);
	//客Cの表示のプロトタイプ宣言
	void customerC_Render(Chara& c_);
	//客Cアニメーションのプロトタイプ宣言
	void customerC_Anim(Chara&  c_);
	//客Cの動作プロトタイプ宣言
	void customerC_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi& asu_, Chara& fu_);
	//客D初期化のプロトタイプ宣言
	void customerD_Initialize(Chara& c_, float x_, float y_);
	//客Dの表示のプロトタイプ宣言
	void customerD_Render(Chara& c_);
	//客Dアニメーションのプロトタイプ宣言
	void customerD_Anim(Chara&  c_);
	//客Dの動作プロトタイプ宣言
	void customerD_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi & asu_, Chara& fu_);
	//客の好み寿司の初期化のプロトタイプ宣言
	void customerlovefood_Initialize(Chara& lf_, float x_, float y_, Chara&  c_, Chara& fu_, float fx_, float fy_);
	//客の好み寿司の表示のプロトタイプ宣言
	void customerlovefood_Render(Chara& lf_,Chara& c_, Chara& fu_);
	//客の好み寿司チェックプロトタイプ宣言
	void customerlovefood_Anim(Chara& lf_, Chara& fu_);
	//寿司まとめチェックのプロトタイプ宣言
	void allsushi_Anim(Sushi& asu_);
	//寿司まとめの初期化のプロトタイプ宣言
	void allsushi_Initialize(float x_, float y_);
	//寿司まとめの表示のプロトタイプ宣言
	void allsushi_Render(Sushi& asu_);
	//寿司まとめの行動のプロトタイプ宣言
	void allsushi_UpDate(Sushi& asu_, POINT p_);
	//寿司廃棄処理のプロトタイプ宣言
	void allsushi_dispose(Sushi& asu_, POINT p_);
	//回転のレーン初期化のプロトタイプ宣言
	void kaitenren_Initialize(Seat& ka_, float x_, float y_);
	//回転のレーンの行動プロトタイプ宣言
	void kaitenren_move(Seat& ka_);
	//回転のレーン表示のプロトタイプ宣言
	void kaitenren_Render(Seat& ka_);
	//電話初期化のプロトタイプ宣言
	void phone_Initialize(Seat& ph_, float x_, float y_);
	//電話カウントプロトタイプ宣言
	void phone_UpDate(Seat& ph_,POINT p_);
	//電話表示のプロトタイプ宣言
	void phone_Render(Seat& ph_);
	//運送の車初期化のプロトタイプ宣言
	void toragu_Initialize(Seat& trg_, float x_, float y_);
	//運送の車の表示のプロトタイプ宣言
	void toragu_Render(Seat& trg_);
	//運送の車アニメーションのプロトタイプ宣言
	void toragu_Anim(Seat& trg_);
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
		DG::Image_Create("haikeiImg", "./data/image/haikei.png");						//背景
		DG::Image_Create("menuImg", "./data/image/menu.png");							//メニュー
		DG::Image_Create("menu_setsumeiImg", "./data/image/menu_setsumei.png");			//メニュー説明
		DG::Image_Create("phoneImg", "./data/image/phone.png");							//電話
		DG::Image_Create("toraguImg", "./data/image/toragu.png");						//運送の車
		DG::Image_Create("shisyouImg", "./data/image/shisyou.png");						//師匠
		DG::Image_Create("eigyoukaishiImg", "./data/image/eigyoukaishi.png");			//営業開始
		DG::Image_Create("fukidashiImg", "./data/image/fukidashi.png");					//吹き出し
		DG::Image_Create("tsukuriImg", "./data/image/tsukuri.png");						//作りアニメーション
		DG::Image_Create("phone_messageImg", "./data/image/phone_message.png");			//電話メッセージ
		DG::Image_Create("makisuImg", "./data/image/sushi_makisu.png");					//巻きす
		DG::Image_Create("kaitenrenImg", "./data/image/kaitenren.png");					//回転レーン
		DG::Image_Create("customerImg", "./data/image/customer.png");					//客
		DG::Font_Create("FontA", "HGS創英角ｺﾞｼｯｸUB", 12, 20);							//文字表示用のフォントを作成
		DG::Font_Create("FontB", "HGS創英角ｺﾞｼｯｸUB", 8, 10);

		//PG実行時の時刻を利用して毎回違う乱数表を選択する
		srand((unsigned int)time(NULL));

		//背景の初期化
		haikei_Initialize(haikei,0,0);

		//回転レーンの初期化
		kaitenren_Initialize(kaitenren,5,6);

		//巻きす初期化
		makisu_Initialize(makisu, 7, 13);

		//統計の初期化
		toukei_Initialize();

		//寿司のまとめ初期化
		for (int i = 0; i < Maxvalue; ++i) {	//Maxvalue
			allsushi[i].active = false;
		}

		//師匠の初期化
		shisyou_Initialize(shisyou,10,11);

		//メニューの初期化
		menu_Initialize(menu,16,11);

		//客を出現させる
		customer_Appear();

		//客の好み寿司の初期化
		for (int i = 0; i < Maxvalue; ++i) {
			customerlovefood_Initialize(lovefood[i], customer[i].x +1, customer[i].y-1, customer[i],fukidashi[i], customer[i].x+0.5f, customer[i].y - 2);
		}

		//電話の初期化
		phone_Initialize(phone,22,14);

		//運送の車初期化
		toragu_Initialize(toragu,0,0);

		//プレイヤーの初期化
		player_Initialize(player);

		//寿司配列情報の初期化
		Sushi_Initialize(sushi);

		//寿司情報を読み込む
		Sushi_Load(sushi, 1);

		//営業開始の初期化
		eigyoukaishi_Initialize(eigyoukaishi);
	}
	//-----------------------------------------------------------------------------
	//解放処理
	//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
	//-----------------------------------------------------------------------------
	void Finalize()
	{
		DG::Image_Erase("sushisozaiImg");
		DG::Image_Erase("De_sushisozaiImg");
		DG::Image_Erase("haikeiImg");
		DG::Image_Erase("menuImg");
		DG::Image_Erase("menu_setsumeiImg");
		DG::Image_Erase("phoneImg");
		DG::Image_Erase("toraguImg");
		DG::Image_Erase("shisyouImg");
		DG::Image_Erase("eigyoukaishiImg");
		DG::Image_Erase("fukidashiImg");
		DG::Image_Erase("tsukuriImg");
		DG::Image_Erase("phone_messageImg");
		DG::Image_Erase("makisuImg");
		DG::Image_Erase("kaitenrenImg");
		DG::Image_Erase("customerImg");
		DG::Image_Erase("customerImg");
		DG::Font_Erase("FontA");
		DG::Font_Erase("FontB");
	}
	//-----------------------------------------------------------------------------
	//実行処理
	//機能概要：ゲームの１フレームに当たる処理
	//-----------------------------------------------------------------------------
	TaskFlag UpDate()
	{
		in1 = DI::GPad_GetState("P1");
		//マウスカーソルの位置を得る
		mouse = DI::Mouse_GetState();
		POINT pos;
		pos.x = mouse.cursorPos.x / 32;
		pos.y = mouse.cursorPos.y / 32;

		//営業開始ーのカウント
		eigyoukaishi_UpDate(eigyoukaishi);

		if (eigyoukaishi.active == false) {
			if (mouse.LB.down) {
				//範囲内でクリックしたらクリック回数が増えていく
				if (pos.x >= 2 && pos.x <= 4 && pos.y >= 12 && pos.y <= 14) {
					//マップチェック
					Sushi_Check(sushi, pos, neta[KURIKU]);	//(kuriku)
					neta[KURIKU].kaisu++;				//クリック回数
				}
				//巻きす行動
				makisu_UpDate(makisu, pos, sushi);
			}

			//寿司まとめ行動
			for (int i = 0; i < Maxvalue; ++i) {
				allsushi_UpDate(allsushi[i], pos);
			}

			//客の行動
			for (int i = 0; i < Maxvalue; ++i) {
				customer_UpDate(customer[i], seiki[i], lovefood[i], allsushi[i], fukidashi[i]);
			}

			//電話カウント
			phone_UpDate(phone, pos);

			//メニューの行動
			menu_UpDate(menu, pos);


			//プレイヤーのカウント
			player_UpDate(player);
		}

		TaskFlag rtv = Task_Game;
		if (player.gameoverFlag==true) {
			remove("data\\Savefile.txt");
			rtv = Task_Gameover;	//次のタスクをゲームオーバーへ
		}
		if (player.timeOver == true) {
			ofstream save("data\\Savefile.txt", std::ios::trunc);
			save << player.shikin << endl;
			save << player.ninki << endl;
			save.close();
			rtv = Task_Outsidebusinesshours;	//次のタスクを営業時間外へ
		}
		if (player.clearFlag == true) {
			remove("data\\Savefile.txt");
			rtv = Task_Gameclear;	//次のタスクをゲームクリアへ
		}
		return rtv;
	}
	//-----------------------------------------------------------------------------
	//描画処理
	//機能概要：ゲームの１フレームに当たる表示処理
	//-----------------------------------------------------------------------------
	void Render()
	{
		//背景の表示
		haikei_Render(haikei);

		//巻きす表示
		makisu_Render(makisu);

		//ネタの表示
		Sushi_Render(sushi);


		//師匠の表示
		shisyou_Render(shisyou);

		//回転レーンの表示
		kaitenren_Render(kaitenren);

		//客の表示
		for (int i = 0; i < Maxvalue; ++i) {
			customer_Render(customer[i]);
		}

		//客の好み寿司の表示
		for (int i = 0; i < Maxvalue; ++i) {
			customerlovefood_Render(lovefood[i],customer[i],fukidashi[i]);
		}

		//寿司のまとめ表示
		for (int i = 0; i < 20; ++i) {		//Maxvalue
			allsushi_Render(allsushi[i]);
		}

		//メニューの表示
		menu_Render(menu);

		//電話表示
		phone_Render(phone);

		//運送の車の表示
		toragu_Render(toragu);

		//プレイヤーの表示
		player_Render(player);

		//営業開始の表示
		eigyoukaishi_Render(eigyoukaishi);

	}
	//--------------------------------------------------------------------------------------------------------------------------------
	//関数
	//--------------------------------------------------------------------------------------------------------------------------------
	//プレイヤーの初期化
	void player_Initialize(Player& pl_)
	{
		pl_.clearFlag = false;			//ゲームクリア判定
		pl_.gameoverFlag = false;		//ゲームオーバー判定
		pl_.timeOver = false;			//営業終了判定
		pl_.timeCnt = 0;
		pl_.timeStart = 8;				//営業開始時間は8時
		pl_.jikan = 0;
		pl_.shikin = 200;				//プレイヤーの資金初期化
		pl_.ninki = 6;					//プレイヤーの人気初期化

		// 資金と人気の読み込み
		// ただし、セーブファイルがなかった場合、新しく作る
		ifstream savefile("data\\Savefile.txt", std::ios::in);
		if (!savefile.is_open()) {
			ofstream save("data\\Savefile.txt", std::ios::trunc);
			if (!save.is_open()) {
				OutputDebugString("Save file creation failed!");
				return;
			}
			save << pl_.shikin << endl;
			save << pl_.ninki << endl;
			save.close();
		}
		else {
			char buf[100];
			savefile.getline(buf, 100);
			pl_.shikin = atoi(buf);
			savefile.getline(buf, 100);
			pl_.ninki = atoi(buf);
			savefile.close();
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------
	//プレイヤーのカウント
	void player_UpDate(Player& pl_)
	{
		if (in1.ST.down==true) {
			pl_.shikin += 500;
		}
		if (in1.SE.down == true) {
			pl_.ninki += 5;
		}

		//営業時間カウント
		pl_.timeCnt++;
		if (pl_.timeCnt % 960 == 0 && pl_.clearFlag == false && pl_.timeOver == false) {	//現実の4分間ほどはゲームの8時から24まで//960
			pl_.jikan++;
		}
		if ((pl_.timeStart + pl_.jikan) == 24) {
			pl_.timeOver = true;
		}

		//ゲームクリア判定
		if (pl_.ninki>100) {
			pl_.clearFlag = true;
		}

		//ゲームオーバー判定
		if (pl_.shikin < 0 || pl_.ninki < 0) {
			pl_.gameoverFlag = true;
		}

	}
	//--------------------------------------------------------------------------------------------------------------------------------
	//プレイヤーの表示
	void player_Render(Player& pl_)
	{
		//営業時間表示
		ML::Box2D textBox(200, 0, 480, 32);
		string text = "時間：" + to_string(pl_.timeStart + pl_.jikan) + "時";
		DG::Font_Draw("FontA", textBox, text, ML::Color(1, 1, 1, 1));

		//プレイヤーの資金表示
		ML::Box2D moneyBox(400, 0, 480, 32);
		string money = "資金：" + to_string(pl_.shikin);
		DG::Font_Draw("FontA", moneyBox, money, ML::Color(1, 1, 1, 1));

		//プレイヤーの人気表示
		ML::Box2D ninkiBox(600, 0, 480, 32);
		string ninki = "人気：" + to_string(pl_.ninki);
		DG::Font_Draw("FontA", ninkiBox, ninki, ML::Color(1, 1, 1, 1));
	}
	//--------------------------------------------------------------------------------------------------------------------------------
	//師匠の初期化
	void shisyou_Initialize(Player& siy_, float x_, float y_)
	{
		siy_.active = true;
		siy_.x = x_;
		siy_.y = y_;
		siy_.animCnt = 0;
		siy_.Cnt = 0;
	}
	//----------------------------------------------------------------
	//師匠の表示
	void shisyou_Render(Player& siy_)
	{
		if (siy_.active == true) {
			shisyou_Anim(siy_);
			ML::Box2D draw = siy_.drawBase.OffsetCopy(float(siy_.x) * 32, float(siy_.y) * 32);
			DG::Image_Draw("shisyouImg", draw, siy_.src);
		}
		if (siy_.active == false) {
			shisyou_Anim(siy_);
			ML::Box2D draw = siy_.drawBase.OffsetCopy((float(siy_.x) * 32)-125, float(siy_.y) * 32);
			DG::Image_Draw("tsukuriImg", draw, siy_.src);
		}
	}
	//----------------------------------------------------------------
	//師匠のアニメーション
	void shisyou_Anim(Player& siy_)
	{
		siy_.animCnt++;
		if (siy_.active == true) {
			siy_.drawBase = ML::Box2D(0, 0, 192, 192);
			switch ((siy_.animCnt / 60) % 2) {
			case 0:siy_.src = ML::Box2D(0, 0, 224, 224); break;
			case 1:siy_.src = ML::Box2D(224, 0, 224, 224); break;
			}
		}
		if (siy_.active == false) {
			shisyou.Cnt++;
			siy_.drawBase = ML::Box2D(0, 0, 160,160);
			switch ((siy_.animCnt / 5) % 5) {
			case 0:siy_.src = ML::Box2D(0, 0, 64, 64); break;
			case 1:siy_.src = ML::Box2D(64, 0, 64,64); break;
			case 2:siy_.src = ML::Box2D(128, 0, 64, 64); break;
			case 3:siy_.src = ML::Box2D(192, 0, 64, 64); break;
			case 4:siy_.src = ML::Box2D(256, 0, 64, 64); break;
			}
		}
		if (siy_.Cnt > 30) {
			siy_.active = true;
			siy_.Cnt = 0;
		}
	}
	//----------------------------------------------------------------
	//客の初期化
	void customer_Initialize(Chara& c_,Seat& s_)
	{	//客の席の処理
		if (s_.isEmpty==true) {
				s_.isEmpty = false;
				c_.typeNum = rand() % 4;					//客のタイプを乱数で決まる
				switch (c_.typeNum) {
				case 0: customerA_Initialize(c_, s_.pos.x, s_.pos.y); break;
				case 1: customerB_Initialize(c_, s_.pos.x, s_.pos.y); break;
				case 2: customerC_Initialize(c_, s_.pos.x, s_.pos.y); break;
				case 3: customerD_Initialize(c_, s_.pos.x, s_.pos.y); break;
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
		case 0: customerA_Render(c_); break;
		case 1: customerB_Render(c_); break;
		case 2: customerC_Render(c_); break;
		case 3: customerD_Render(c_); break;
		}
	}
	//----------------------------------------------------------------
	//客の行動
	void customer_UpDate(Chara& c_, Seat & set_, Chara& lf_, Sushi& asu_, Chara& fu_)
	{
		switch (c_.typeNum) {
		case 0:
			customerA_UpDate(c_, set_,lf_, asu_,fu_);
			break;
		case 1:
			customerB_UpDate(c_, set_,lf_, asu_,fu_);
			break;
		case 2:
			customerC_UpDate(c_, set_,lf_, asu_,fu_);
			break;
		case 3:
			customerD_UpDate(c_, set_,lf_, asu_,fu_);
			break;
		}
		//客アニメーションの行動
		c_.animCnt++;
	}
	//----------------------------------------------------------------
	//客の入店、お帰り処理
	void customer_Respawn(Chara & c_, Sushi & asu_, Chara& lf_, Seat & set_,Chara & fu_)
	{
		set_.isEmpty = false;
		c_.active = false;
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
		customerlovefood_Initialize(lf_, c_.x+1,c_.y-1,c_,fu_, c_.x +0.5f, c_.y - 2);

		//c_.active = false;			//客がお会計
		//s_.active = false;			//寿司が消える
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
			switch ((c_.animCnt / 10) % 4){
			case 0:c_.src = ML::Box2D(0, 0, 96, 96); break;
			case 1:c_.src = ML::Box2D(96, 0, 96, 96); break;
			case 2:c_.src = ML::Box2D(0, 0, 96, 96); break;
			case 3:c_.src = ML::Box2D(192, 0, 96, 96); break;
			}
			break;
		case Dead:
			switch ((c_.animCnt / 7) % 2) {
			case 0:c_.src = ML::Box2D(192, 768, 96, 96); break;
			case 1:c_.src = ML::Box2D(0, 864, 96, 96); break;
			}
			break;
		case Happy:
			switch ((c_.animCnt / 7) % 2) {
			case 0:c_.src = ML::Box2D(96,672, 96, 96); break;
			case 1:c_.src = ML::Box2D(192, 672, 96, 96); break;
			}
			break;
		case Angry:
			switch ((c_.animCnt / 10) % 3){
			case 0:c_.src = ML::Box2D(192, 288, 96, 96); break;
			case 1:c_.src = ML::Box2D(0, 384, 96, 96); break;
			case 2:c_.src = ML::Box2D(192, 288, 96, 96); break;
			}
			break;
		}
	}
	//----------------------------------------------------------------
	//客Aの動作
	void customerA_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi& asu_, Chara& fu_)
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
		if (c_.kaunto > 2500) {
			//プレイヤーの人気が－１
			player.ninki--;
			if (set_.isEmpty == true) {
				//客の入店、お帰り処理
				customer_Respawn(c_, asu_, lf_, set_,fu_);
			}
			set_.isEmpty = true;
		}
	}
	//----------------------------------------------------------------
	//客Bの動作
	void customerB_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi& asu_, Chara& fu_)
	{
		if (c_.kaunto == 300){
			c_.active = true;
			if (c_.active == false){
				c_.kaunto = 0;
				c_.kaunto++;
			}
		}
		if (c_.kaunto>1100){
			c_.motion = Angry;
		}
		if (c_.kaunto > 2600) {
			//プレイヤーの人気が－１
			player.ninki--;
			if (set_.isEmpty == true) {
				//客の入店、お帰り処理
				customer_Respawn(c_, asu_, lf_, set_,fu_);
			}
			set_.isEmpty = true;
		}
		//客カウント
		c_.kaunto++;
	}
	//----------------------------------------------------------------
	//客Cの動作
	void customerC_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi& asu_, Chara& fu_)
	{
		//客カウント
		c_.kaunto++;
		if (c_.kaunto == 400){
			c_.active = true;
		}
		if (c_.kaunto > 1200){
			c_.motion = Angry;
		}
		if (c_.kaunto > 2700) {
			//プレイヤーの人気が－１
			player.ninki--;
			if (set_.isEmpty == true) {
				//客の入店、お帰り処理
				customer_Respawn(c_, asu_, lf_, set_,fu_);
			}
			set_.isEmpty = true;
		}
	}
	//----------------------------------------------------------------
	//客Dの動作
	void customerD_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi& asu_, Chara& fu_)
	{
		//客カウント
		c_.kaunto++;
		if (c_.kaunto == 100){
			c_.active = true;
		}
		//客が待つ時間は遅過ぎると怒る
		if (c_.kaunto>900){
			c_.motion = Angry;
		}
		if (c_.kaunto > 2400) {
			//プレイヤーの人気が－１
			player.ninki--;
			if (set_.isEmpty == true) {
				//客の入店、お帰り処理
				customer_Respawn(c_, asu_, lf_, set_,fu_);
			}
			set_.isEmpty = true;
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
			switch ((c_.animCnt / 10) % 4){
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
			switch ((c_.animCnt / 10) % 3) {
			case 0:c_.src = c_.src = ML::Box2D(96, 864, 96, 96); break;
			case 1:c_.src = c_.src = ML::Box2D(192,864, 96, 96); break;
			case 2:c_.src = c_.src = ML::Box2D(96, 864, 96, 96); break;
			}
			break;
		case Angry:
			switch ((c_.animCnt / 10) % 3){
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
			switch ((c_.animCnt / 10) % 4){
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
			switch ((c_.animCnt / 10) % 3) {
			case 0:c_.src = c_.src = ML::Box2D(0, 768, 96, 96); break;
			case 1:c_.src = c_.src = ML::Box2D(96, 768, 96, 96); break;
			case 2:c_.src = c_.src = ML::Box2D(0, 768, 96, 96); break;
			}
			break;
		case Angry:
			switch ((c_.animCnt / 10) % 3){
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
			switch ((c_.animCnt / 30) % 3){
			case 0:c_.src = ML::Box2D(0, 480, 96, 96); break;
			case 1:c_.src = ML::Box2D(96, 480, 96, 96); break;
			case 2:c_.src = ML::Box2D(96, 480, 96, 96); break;
			}
			break;
		case Dead:c_.src = ML::Box2D(192, 480, 96, 96); break;
			break;
		case Happy:
			switch ((c_.animCnt / 10) % 2) {
			case 0:c_.src = c_.src = ML::Box2D(0, 576, 96, 96); break;
			case 1:c_.src = c_.src = ML::Box2D(96, 576, 96, 96); break;
			}
			break;
		case Angry:
			switch ((c_.animCnt / 30) % 2){
			case 0:c_.src = ML::Box2D(192, 576, 96, 96); break;
			case 1:c_.src = ML::Box2D(0, 672, 96, 96); break;
			}
			break;
		}
	}
	//----------------------------------------------------------------
	//客の好み寿司の初期化
	void customerlovefood_Initialize(Chara& lf_, float x_, float y_, Chara&  c_, Chara& fu_, float fx_, float fy_)
	{
		lf_.x = x_;
		lf_.y = y_;
		fu_.x = fx_;
		fu_.y = fy_;
		fu_.active = c_.active;
		lf_.active = c_.active;
		lf_.number = 9 + rand() % 8;		//9番から寿司になる
	}
	//----------------------------------------------------------------
	//客の好み寿司の表示
	void customerlovefood_Render(Chara& lf_, Chara& c_, Chara& fu_)
	{
		if (c_.active == true) {
			customerlovefood_Anim(lf_,fu_);
			ML::Box2D drawA = fu_.drawBase.OffsetCopy(float(fu_.x) * 32, float(fu_.y) * 32);
			DG::Image_Draw("fukidashiImg", drawA, fu_.src);

			ML::Box2D draw = lf_.drawBase.OffsetCopy(float(lf_.x) * 32, float(lf_.y) * 32);
			DG::Image_Draw("sushisozaiImg", draw, lf_.src);
		}
	}
	//----------------------------------------------------------------
	//客の好み寿司チェック
	void customerlovefood_Anim(Chara& lf_,Chara& fu_)
	{
		lf_.drawBase = ML::Box2D(0, -12, 32, 32);
		switch (lf_.number) {
		case 9:lf_.src = ML::Box2D(32, 64, 32, 32);  break;			//マクロ寿司
		case 10:lf_.src = ML::Box2D(64, 64, 32, 32); break;			//サーモン寿司
		case 11:lf_.src = ML::Box2D(96, 64, 32, 32); break;			//玉子寿司
		case 12:lf_.src = ML::Box2D(0, 96, 32, 32); break;			//いくら寿司
		case 13:lf_.src = ML::Box2D(32, 96, 32, 32); break;			//きゅうり寿司
		case 14:lf_.src = ML::Box2D(64, 96, 32, 32); break;			//鉄火寿司
		case 15:lf_.src = ML::Box2D(96, 96, 32, 32); break;			//甘海老寿司
		case 16:lf_.src = ML::Box2D(0, 128, 32, 32); break;			//海老寿司
		//case 14:lf_.src = ML::Box2D(160, 0, 32, 32); break;
		}
		//吹き出しの表示
		fu_.drawBase = ML::Box2D(0, 0, 64, 64);
		fu_.src = ML::Box2D(0, 0, 64, 64);

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
				allsushi[i].hitFlag = false;
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
		enum {
			makurosushi=9, samonsushi = 10,tamakosushi = 11,ikurasushi=12,
			kyuurisushi=13,teekasushi=14,amaebisushi=15,ebisushi=16
		};
		switch (asu_.motion) {
		case Normal:													//寿司は新鮮な状態
			asu_.drawBase = ML::Box2D(0, -12, 32, 32);
			switch (asu_.number){										
			case makurosushi:asu_.src = ML::Box2D(32, 64, 32, 32); break;			//マクロ寿司
			case samonsushi:asu_.src = ML::Box2D(64, 64, 32, 32); break;			//サーモン寿司
			case tamakosushi:asu_.src = ML::Box2D(96, 64, 32, 32); break;			//玉子寿司
			case ikurasushi:asu_.src = ML::Box2D(0, 96, 32, 32); break;				//いくら寿司
			case kyuurisushi:asu_.src = ML::Box2D(32, 96, 32, 32); break;			//きゅうり寿司
			case teekasushi:asu_.src = ML::Box2D(64, 96, 32, 32); break;			//鉄火寿司
			case amaebisushi:asu_.src = ML::Box2D(96,96, 32, 32); break;			//甘海老寿司
			case ebisushi:asu_.src = ML::Box2D(0, 128, 32, 32); break;				//海老寿司
			}
			break;
		case Dead:														//寿司は新鮮じゃない状態
			asu_.drawBase = ML::Box2D(0, -12, 32, 32);
			switch (asu_.number) {
			case makurosushi:														//新鮮じゃないマクロ寿司
				switch ((asu_.animCnt / 7) % 4){
				case 0:asu_.src = ML::Box2D(0,0, 32, 32); break;
				case 1:asu_.src = ML::Box2D(32, 0, 32, 32); break;
				case 2:asu_.src = ML::Box2D(64, 0, 32, 32); break;
				case 3:asu_.src = ML::Box2D(96, 0, 32, 32); break;
				}
				break;
			case samonsushi:													//新鮮じゃないサーモン寿司
				switch ((asu_.animCnt / 7) % 4) {
				case 0:asu_.src = ML::Box2D(0, 32, 32, 32); break;
				case 1:asu_.src = ML::Box2D(32, 32, 32, 32); break;
				case 2:asu_.src = ML::Box2D(64, 32, 32, 32); break;
				case 3:asu_.src = ML::Box2D(96, 32, 32, 32); break;
				}
				break;
			case tamakosushi:													//新鮮じゃない玉子寿司
				switch ((asu_.animCnt / 7) % 4) {
				case 0:asu_.src = ML::Box2D(0, 64, 32, 32); break;
				case 1:asu_.src = ML::Box2D(32, 64, 32, 32); break;
				case 2:asu_.src = ML::Box2D(64, 64, 32, 32); break;
				case 3:asu_.src = ML::Box2D(96, 64, 32, 32); break;
				}
				break;
			case ikurasushi:													//新鮮じゃないいくら寿司
				switch ((asu_.animCnt / 7) % 4) {
				case 0:asu_.src = ML::Box2D(0, 96, 32, 32); break;
				case 1:asu_.src = ML::Box2D(32, 96, 32, 32); break;
				case 2:asu_.src = ML::Box2D(64, 96, 32, 32); break;
				case 3:asu_.src = ML::Box2D(96, 96, 32, 32); break;
				}
				break;
			case kyuurisushi:													//新鮮じゃないきゅうり寿司
				switch ((asu_.animCnt / 7) % 4) {
				case 0:asu_.src = ML::Box2D(0, 128, 32, 32); break;
				case 1:asu_.src = ML::Box2D(32, 128, 32, 32); break;
				case 2:asu_.src = ML::Box2D(64, 128, 32, 32); break;
				case 3:asu_.src = ML::Box2D(96, 128, 32, 32); break;
				}
				break;
			case teekasushi:													//新鮮じゃない鉄火寿司
				switch ((asu_.animCnt / 7) % 4) {
				case 0:asu_.src = ML::Box2D(0, 160, 32, 32); break;
				case 1:asu_.src = ML::Box2D(32, 160, 32, 32); break;
				case 2:asu_.src = ML::Box2D(64, 160, 32, 32); break;
				case 3:asu_.src = ML::Box2D(96, 160, 32, 32); break;
				}
				break;
			case amaebisushi:													//新鮮じゃない甘海老寿司
				switch ((asu_.animCnt / 7) % 4) {
				case 0:asu_.src = ML::Box2D(0, 192, 32, 32); break;
				case 1:asu_.src = ML::Box2D(32, 192, 32, 32); break;
				case 2:asu_.src = ML::Box2D(64, 192, 32, 32); break;
				case 3:asu_.src = ML::Box2D(96, 192, 32, 32); break;
				}
				break;
			case ebisushi:														//新鮮じゃない海老寿司
				switch ((asu_.animCnt / 7) % 4) {
				case 0:asu_.src = ML::Box2D(0, 224, 32, 32); break;
				case 1:asu_.src = ML::Box2D(32, 224, 32, 32); break;
				case 2:asu_.src = ML::Box2D(64, 224, 32, 32); break;
				case 3:asu_.src = ML::Box2D(96, 224, 32, 32); break;
				}
				break;
			}
			break;
		}
	}
	//----------------------------------------------------------------
	//寿司まとめ行動
	void allsushi_UpDate(Sushi& asu_, POINT p_)
	{
		//POINT pre = { asu_.x,asu_.y };	//移動前
		asu_.moveCnt++;
		if (asu_.hitFlag == false) {
			if (asu_.active == true) {
				float speedCnt;					//移動速度
				speedCnt = 0.05f;				//0.06f
				if (asu_.moveCnt < 180) {
					asu_.x -= speedCnt;
				}
				if (asu_.moveCnt > 170 && asu_.moveCnt < 250) {
					asu_.y -= speedCnt;
				}
				if (asu_.moveCnt > 240 && asu_.moveCnt < 620) {
					asu_.x += speedCnt;
				}
				if (asu_.moveCnt > 610 && asu_.moveCnt <690) {
					asu_.y += speedCnt;
				}
				if (asu_.moveCnt > 680 && asu_.moveCnt <880) {		//870
					asu_.x -= speedCnt;
				}
				if (asu_.moveCnt > 880) {
					asu_.motion = Dead;
					asu_.moveCnt = 0;
				}
			}
		}

		//寿司と客の当たり判定
		if (asu_.active == true) {
			ML::Box2D me = asu_.hitBase.OffsetCopy(float(asu_.x )* 32.0f,float(asu_.y)* 32.0f);
			//me.x = asu_.x;
			//me.y = asu_.y;
			//寿司は客と当たり判定をさせる
			for (int c = 0; c < Maxvalue; ++c) {
				if (customer[c].active == true) {											//客はいる場合
					//寿司自身の当たり判定の写真を用意（現在位置）
					ML::Box2D you = customer[c].hitBase.OffsetCopy(customer[c].x * 32, customer[c].y * 32);
					//接触判定
					if (true == you.Hit(me) && lovefood[c].number == asu_.number){			//自分と接触してるか相手に判断してもらう
						switch (asu_.motion){
						case Normal:
							customer[c].motion = Happy;
							if (customer[c].motion == Happy) {
								asu_.Cnt++;
								//客が新鮮な寿司を食べたら、笑う
								if (customer[c].motion == Happy && asu_.Cnt>35) {
									//客の入店、お帰り処理
									customer_Respawn(customer[c], asu_, lovefood[c], seiki[c],fukidashi[c]);
									asu_.active = false;		//寿司が消える
									player.shikin += neta[SUSHI].money;
									player.ninki++;			//人気+1
									asu_.Cnt = 0;
								}
							}
							break;
						case Dead:
							customer[c].motion = Dead;
							if (customer[c].motion == Dead) {
								asu_.Cnt++;
								//客が新鮮じゃない寿司を食べたら、泣く
								if (customer[c].motion == Dead && asu_.Cnt>45) {		//35
									//客の入店、お帰り処理
									customer_Respawn(customer[c], asu_, lovefood[c], seiki[c],fukidashi[c]);
									asu_.active = false;		//寿司が消える
									player.shikin -= neta[TAMAKO].money;
									player.ninki-=5;
									asu_.Cnt = 0;
								}
							}
							break;
						}

						////客が遅く待たせればお金が半分になる
						//if (customer[c].motion == Angry) {
						//	//客の入店、お帰り処理
						//	customer_Respawn(customer[c], asu_, lovefood[c], seiki[c]);
						//	player.shikin += (neta[SUSHI].money/2);
						//}

						seiki[c].isEmpty = true;
						break;
					}
					//寿司廃棄処理
					allsushi_dispose(asu_, p_);
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
		c_.Cnt = 0;
	}
	//----------------------------------------------------------------
	//巻きす行動
	void makisu_UpDate(Sushi& c_, POINT p_, Sushi& md_)
	{
		//neta[KURIKU].kaisu++;				//クリック回数
		int bangou;
		//範囲内チェック
		if (p_.x >= 7 && p_.x < 10 && p_.y >= 12 && p_.y <= 15) {
			for (int i = 12; i <= 14; ++i) {
				for (int j = 7; j <= 9; ++j) {
					bangou = md_.arr[i][j];
					switch (bangou) {
					case 0: neta[SYARI].kaisu++; break;				//シャリ
					case 1:	neta[TAMAKO].kaisu++; break;			//たまこ
					case 2:	neta[SAMON].kaisu++; break;				//サーモン
					case 3:	neta[NORI].kaisu++; break;				//のり
					case 4:	neta[MAKURO].kaisu++; break;			//まくろ
					case 5:	neta[IKURA].kaisu++; break;				//いくら
					case 6:	neta[KYUURI].kaisu++; break;			//きゅうり
					case 7:	neta[AMAEBI].kaisu++; break;			//甘海老
					case 8:	neta[EBI].kaisu++; break;				//海老
					}
					md_.arr[i][j] = 24;			//クリックしたら、空欄になる
				}
			}

			//寿司の合成処理
			//シャリ=2、まくろ=2なら、マクロ寿司（9番）
			if (neta[SYARI].kaisu == 2 && neta[MAKURO].kaisu == 2) {
				shisyou.active = false;
				allsushi[CheckDisabledSushi()].number = 9;
			}

			//シャリ=2、サーモン=2、サーモン寿司（10番）
			else if (neta[SYARI].kaisu == 2 && neta[SAMON].kaisu == 2) {
				shisyou.active = false;
				allsushi[CheckDisabledSushi()].number = 10;
			}
			//シャリ=2、たまこ=2、玉子寿司（11番）
			else if (neta[SYARI].kaisu == 2 && neta[TAMAKO].kaisu == 2) {
				shisyou.active = false;
				allsushi[CheckDisabledSushi()].number = 11;
			}
			//シャリ=2、いくら=2、のり=1なら、いくら寿司（12番）
			else if (neta[SYARI].kaisu == 2 && neta[IKURA].kaisu == 2 && neta[NORI].kaisu == 1&& neta[KYUURI].kaisu==1) {
				allsushi[CheckDisabledSushi()].number = 12;
			}
			//シャリ=4、のり=1、胡瓜=1なら、きゅうり寿司（13番）
			else if (neta[SYARI].kaisu == 4&& neta[NORI].kaisu == 1 && neta[KYUURI].kaisu == 1) {
				allsushi[CheckDisabledSushi()].number = 13;
			}
			//シャリ=4、のり=1、マクロ=1なら、鉄火寿司（14番）
			else if (neta[SYARI].kaisu == 4 && neta[NORI].kaisu == 1 && neta[MAKURO].kaisu == 1) {
				allsushi[CheckDisabledSushi()].number = 14;
			}
			//シャリ=1、甘海老=1なら、甘海老寿司（15番）
			else if (neta[SYARI].kaisu == 1&& neta[AMAEBI].kaisu == 1) {
				allsushi[CheckDisabledSushi()].number = 15;
			}
			//シャリ=2、海老=2なら、海老寿司（16番）
			else if (neta[SYARI].kaisu == 2 && neta[EBI].kaisu == 2) {
				allsushi[CheckDisabledSushi()].number = 16;
			}

			//寿司を出す
			allsushi_Initialize(14,10);
			//寿司を作ったら、各ネーター計算0になる
			neta[SYARI].kaisu = 0, neta[SAMON].kaisu = 0, neta[TAMAKO].kaisu = 0;
			neta[MAKURO].kaisu = 0, neta[IKURA].kaisu = 0, neta[NORI].kaisu=0;
			neta[KYUURI].kaisu = 0, neta[AMAEBI].kaisu=0, neta[EBI].kaisu=0;

			//クリック回数も0に戻す
			neta[KURIKU].kaisu = 0;
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
			if (neta[SYARI].nokori >0) {			//シャリ残り>0なら
				switch (ku_.kaisu){					//クリックした回数
				case 0:md_.arr[p_.y][p_.x + 5] = 0;break;
				case 1:md_.arr[p_.y][p_.x + 6] = 0;break;
				case 2:md_.arr[p_.y][p_.x + 7] = 0;break;
				case 3:md_.arr[p_.y + 1][p_.x + 5] = 0;break;
				case 4:md_.arr[p_.y + 1][p_.x + 6] = 0;break;
				case 5:md_.arr[p_.y + 1][p_.x + 7] = 0;break;
				case 6:md_.arr[p_.y + 2][p_.x + 5] = 0;break;
				case 7:md_.arr[p_.y + 2][p_.x + 6] = 0;break;
				case 8:md_.arr[p_.y + 2][p_.x + 7] = 0;break;
				}
				neta[SYARI].nokori--;
			}
			break;
			//玉子をクリックしたら、コピーする
		case 1:
			if (neta[TAMAKO].nokori >0) {			//玉子残り>0なら
				switch (ku_.kaisu) {				//クリックした回数
				case 0:md_.arr[p_.y][p_.x + 4] = 1; break;
				case 1:md_.arr[p_.y][p_.x + 5] = 1; break;
				case 2:md_.arr[p_.y][p_.x + 6] = 1; break;
				case 3:md_.arr[p_.y + 1][p_.x + 4] = 1; break;
				case 4:md_.arr[p_.y + 1][p_.x + 5] = 1; break;
				case 5:md_.arr[p_.y + 1][p_.x + 6] = 1; break;
				case 6:md_.arr[p_.y + 2][p_.x + 4] = 1; break;
				case 7:md_.arr[p_.y + 2][p_.x + 5] = 1; break;
				case 8:md_.arr[p_.y + 2][p_.x + 6] = 1; break;
				}
				neta[TAMAKO].nokori--;
			}
			break;
			//サーモンをクリックしたら、コピーする
		case 2:
			if (neta[SAMON].nokori >0) {			//サーモン残り>0なら
				switch (ku_.kaisu) {				//クリックした回数
				case 0:md_.arr[p_.y][p_.x + 3] = 2; break;
				case 1:md_.arr[p_.y][p_.x + 4] = 2; break;
				case 2:md_.arr[p_.y][p_.x + 5] = 2; break;
				case 3:md_.arr[p_.y + 1][p_.x + 3] = 2; break;
				case 4:md_.arr[p_.y + 1][p_.x + 4] = 2; break;
				case 5:md_.arr[p_.y + 1][p_.x + 5] = 2; break;
				case 6:md_.arr[p_.y + 2][p_.x + 3] = 2; break;
				case 7:md_.arr[p_.y + 2][p_.x + 4] = 2; break;
				case 8:md_.arr[p_.y + 2][p_.x + 5] = 2; break;
				}
				neta[SAMON].nokori--;
			}
			break;
			//海苔をクリックしたら、コピーする
		case 3:
			if (neta[NORI].nokori >0) {			//海苔残り>0なら
				switch (ku_.kaisu) {				//クリックした回数
				case 0:md_.arr[p_.y-1][p_.x + 5] = 3; break;
				case 1:md_.arr[p_.y-1][p_.x + 6] = 3; break;
				case 2:md_.arr[p_.y-1][p_.x + 7] = 3; break;
				case 3:md_.arr[p_.y][p_.x + 5] = 3; break;
				case 4:md_.arr[p_.y][p_.x + 6] = 3; break;
				case 5:md_.arr[p_.y][p_.x + 7] = 3; break;
				case 6:md_.arr[p_.y + 1][p_.x + 5] = 3; break;
				case 7:md_.arr[p_.y + 1][p_.x + 6] = 3; break;
				case 8:md_.arr[p_.y + 1][p_.x + 7] = 3; break;
				}
				neta[NORI].nokori--;
			}
			break;
			//まくろをクリックしたら、コピーする
		case 4:
			if (neta[MAKURO].nokori >0) {			//まくろ残り>0なら
				switch (ku_.kaisu) {				//クリックした回数
				case 0:md_.arr[p_.y - 1][p_.x + 4] = 4; break;
				case 1:md_.arr[p_.y - 1][p_.x + 5] = 4; break;
				case 2:md_.arr[p_.y - 1][p_.x + 6] = 4; break;
				case 3:md_.arr[p_.y][p_.x + 4] = 4; break;
				case 4:md_.arr[p_.y][p_.x + 5] = 4; break;
				case 5:md_.arr[p_.y][p_.x + 6] = 4; break;
				case 6:md_.arr[p_.y + 1][p_.x + 4] = 4; break;
				case 7:md_.arr[p_.y + 1][p_.x + 5] = 4; break;
				case 8:md_.arr[p_.y + 1][p_.x + 6] = 4; break;
				}
				neta[MAKURO].nokori--;
			}
			break;
			//いくらをクリックしたら、コピーする
		case 5:
			if (neta[IKURA].nokori >0) {			//いくら残り>0なら
				switch (ku_.kaisu) {				//クリックした回数
				case 0:md_.arr[p_.y - 1][p_.x + 3] = 5; break;
				case 1:md_.arr[p_.y - 1][p_.x + 4] = 5; break;
				case 2:md_.arr[p_.y - 1][p_.x + 5] = 5; break;
				case 3:md_.arr[p_.y][p_.x + 3] = 5; break;
				case 4:md_.arr[p_.y][p_.x + 4] = 5; break;
				case 5:md_.arr[p_.y][p_.x + 5] = 5; break;
				case 6:md_.arr[p_.y + 1][p_.x + 3] = 5; break;
				case 7:md_.arr[p_.y + 1][p_.x + 4] = 5; break;
				case 8:md_.arr[p_.y + 1][p_.x + 5] = 5; break;
				}
				neta[IKURA].nokori--;
			}
			break;
			//きゅうりをクリックしたら、コピーする
		case 6:
			if (neta[KYUURI].nokori >0) {			//胡瓜残り>0なら
				switch (ku_.kaisu) {				//クリックした回数
				case 0:md_.arr[p_.y - 2][p_.x + 5] = 6; break;
				case 1:md_.arr[p_.y - 2][p_.x + 6] = 6; break;
				case 2:md_.arr[p_.y - 2][p_.x + 7] = 6; break;
				case 3:md_.arr[p_.y-1][p_.x + 5] = 6; break;
				case 4:md_.arr[p_.y-1][p_.x + 6] = 6; break;
				case 5:md_.arr[p_.y-1][p_.x + 7] = 6; break;
				case 6:md_.arr[p_.y ][p_.x + 5] = 6; break;
				case 7:md_.arr[p_.y][p_.x + 6] = 6; break;
				case 8:md_.arr[p_.y][p_.x + 7] = 6; break;
				}
				neta[KYUURI].nokori--;
			}
			break;
			//きゅうりをクリックしたら、コピーする
		case 7:
			if (neta[AMAEBI].nokori >0) {			//甘海老残り>0なら
				switch (ku_.kaisu) {				//クリックした回数
				case 0:md_.arr[p_.y - 2][p_.x + 4] = 7; break;
				case 1:md_.arr[p_.y - 2][p_.x + 5] = 7; break;
				case 2:md_.arr[p_.y - 2][p_.x + 6] = 7; break;
				case 3:md_.arr[p_.y - 1][p_.x + 4] = 7; break;
				case 4:md_.arr[p_.y - 1][p_.x + 5] = 7; break;
				case 5:md_.arr[p_.y - 1][p_.x + 6] = 7; break;
				case 6:md_.arr[p_.y][p_.x + 4] = 7; break;
				case 7:md_.arr[p_.y][p_.x + 5] = 7; break;
				case 8:md_.arr[p_.y][p_.x + 6] = 7; break;
				}
				neta[AMAEBI].nokori--;
			}
			break;
			//海老をクリックしたら、コピーする
		case 8:
			if (neta[EBI].nokori >0) {				//海老残り>0なら
				switch (ku_.kaisu) {				//クリックした回数
				case 0:md_.arr[p_.y - 2][p_.x + 3] = 8; break;
				case 1:md_.arr[p_.y - 2][p_.x + 4] = 8; break;
				case 2:md_.arr[p_.y - 2][p_.x + 5] = 8; break;
				case 3:md_.arr[p_.y - 1][p_.x + 3] = 8; break;
				case 4:md_.arr[p_.y - 1][p_.x + 4] = 8; break;
				case 5:md_.arr[p_.y - 1][p_.x + 5] = 8; break;
				case 6:md_.arr[p_.y][p_.x + 3] = 8; break;
				case 7:md_.arr[p_.y][p_.x + 4] = 8; break;
				case 8:md_.arr[p_.y][p_.x + 5] = 8; break;
				}
				neta[EBI].nokori--;
			}
			break;
		}
		//クリック回数を８超えたら
		if (ku_.kaisu > 8) { ku_.kaisu = -1; }
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
				if (md_.arr[y][x] < NETA_MAXNUM && neta[md_.arr[y][x]].nokori >=0) {
					ML::Box2D  draw(0, 0, 32, 32);
					draw.Offset(x * 32, y * 32);		//表示位置を調整
					DG::Image_Draw("sushisozaiImg", draw, md_.chip[md_.arr[y][x]]);

					//残り個数も表示
					if (md_.arr[y][x] >= 0 && md_.arr[y][x] <= 9) {
						ML::Box2D textBox(-7, 20, 32, 32);		//残り個数の位置
						textBox.Offset(x * 32, y * 32);			//表示位置を調整
						string text = to_string(neta[md_.arr[y][x]].nokori);
						DG::Font_Draw("FontB", textBox, text, ML::Color(1,1,1,1));		//白色
					}
				}
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
		}
	}
	//----------------------------------------------------------------
	//回転レーンの初期化
	void kaitenren_Initialize(Seat& ka_, float x_, float y_)
	{
		ka_.x = x_;
		ka_.y = y_;
		ka_.moveCnt = 0;
	}
	//----------------------------------------------------------------
	//回転レーンの行動
	void kaitenren_move(Seat& ka_)
	{
		ka_.moveCnt++;
		ka_.drawBase = ML::Box2D(0, -10, 640, 160);
		switch ((ka_.moveCnt / 10) % 2) {
		case 0:ka_.src = ML::Box2D(0, 0, 640, 160); break;
		case 1:ka_.src = ML::Box2D(640, 0, 640, 160); break;
		}
	}
	//----------------------------------------------------------------
	//回転レーンの表示
	void kaitenren_Render(Seat& ka_)
	{
		kaitenren_move(ka_);
		ML::Box2D draw = ka_.drawBase.OffsetCopy(float(ka_.x) * 32, float(ka_.y) * 32);
		DG::Image_Draw("kaitenrenImg", draw, ka_.src);
	}
	//----------------------------------------------------------------
	//統計初期化
	void toukei_Initialize()
	{
		//マウスでクリック回数
		neta[KURIKU].kaisu = 0;
		//それぞれのネタ残り個数
		neta[SYARI].nokori =10;
		neta[TAMAKO].nokori =10;
		neta[SAMON].nokori =10;
		neta[NORI].nokori = 10;
		neta[MAKURO].nokori = 10;
		neta[IKURA].nokori = 10;
		neta[KYUURI].nokori = 10;
		neta[AMAEBI].nokori = 10;
		neta[EBI].nokori = 10;
		//寿司の値段
		neta[SUSHI].money = 50;
		//ネタの値段
		neta[SYARI].money =50;
		neta[TAMAKO].money = 100;
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
	//----------------------------------------------------------------
	//寿司廃棄処理
	void allsushi_dispose(Sushi& asu_, POINT p_)
	{
		//if (asu_.active == true) {
		//	ML::Box2D me = asu_.hitBase.OffsetCopy(float(asu_.x)* 32.0f, float(asu_.y)* 32.0f);
		//	//if (mouse.LB.on) {
		//		if (asu_.motion == Dead) {
		//			me.x = asu_.x;
		//			me.y = asu_.y;
		//			//POINT now = { asu_.x,asu_.y };		//移動後
		//			if (me.x <= p_.x &&p_.x < me.x + me.w&&me.y <= p_.y && p_.y < me.y + me.h) {
		//				if (mouse.LB.on) {
		//					asu_.hitFlag = true;
		//				}
		//			}
		//			//if (asu_.hitFlag == true) {
		//			//	//asu_.x = 0;
		//			//	//asu_.y = 0;
		//			//	asu_.x = float(p_.x);
		//			//	asu_.y = float(p_.y);
		//			//}
		//			//どうやって寿司を元の位置に戻らせるの？？？？
		//			//else
		//			//{
		//			//	asu_.hitFlag = false;
		//			//}
		//		}
		//	//}
		//}
	}
	//----------------------------------------------------------------
	//メニューの初期化
	void menu_Initialize(Seat& s_, float x_, float y_)
	{
		s_.active = true;
		s_.x = x_;
		s_.y = y_;
	}
	//----------------------------------------------------------------
	//メニューの表示
	void menu_Render(Seat& s_)
	{
		if (s_.active == true) {
			ML::Box2D draw(0,0,32,128);
			s_.hitBase = draw;
			draw.Offset(s_.x * 32, s_.y * 32);
			ML::Box2D src(0,0,32,128);
			DG::Image_Draw("menuImg", draw, src);
		}
		if(s_.active == false){
			ML::Box2D draw(0, 0, 800, 320);
			s_.hitBase = draw;
			draw.Offset(s_.x * 4, s_.y * 11);
			ML::Box2D src(0, 0, 1000, 400);
			DG::Image_Draw("menu_setsumeiImg", draw, src);
		}
	}
	//----------------------------------------------------------------
	//メニューのカウント
	void menu_UpDate(Seat& s_, POINT p_)
	{
		POINT maus;
		maus.x = mouse.cursorPos.x;
		maus.y = mouse.cursorPos.y;
		ML::Box2D me = s_.hitBase.OffsetCopy(float(s_.x)* 32.0f, float(s_.y)* 32.0f);
		if (mouse.LB.down) {
			if (me.x <= maus.x &&maus.x< me.x + me.w&&me.y <= maus.y&&maus.y< me.y + me.h) {
				s_.active = false;
			}
		}
		if (s_.active == false) {
			//閉じるボタン
			if ((p_.x >= 24 && p_.x <= 25 && p_.y >= 5 && p_.y <= 6)) {
				if (mouse.LB.down) {
					s_.active = true;
				}
			}
		}
	}
	//----------------------------------------------------------------
	//電話初期化
	void phone_Initialize(Seat& ph_, float x_, float y_)
	{
		ph_.active = true;
		ph_.x = x_;
		ph_.y = y_;
	}
	//----------------------------------------------------------------
	//電話のカウント
	void phone_UpDate(Seat& ph_, POINT p_)
	{
		POINT maus;
		maus.x = mouse.cursorPos.x;
		maus.y = mouse.cursorPos.y;
		ML::Box2D me = ph_.hitBase.OffsetCopy(float(ph_.x)* 32.0f, float(ph_.y)* 32.0f);
		if (mouse.LB.down) {
			if (me.x <= maus.x &&maus.x< me.x + me.w&&me.y <= maus.y&&maus.y< me.y + me.h) {
					ph_.active = false;
			}
		}

		if (ph_.active == false) {
			//シャリ補充
			if ((p_.x >= 6 && p_.x <= 7 && p_.y >= 6 && p_.y <= 7)) {
				if (mouse.LB.down){
					toragu.active = true;
					neta[SYARI].nokori = 10;
					player.shikin -= neta[SYARI].money;
				}
			}
			//玉子補充
			if ((p_.x >=9 && p_.x <=10 && p_.y >= 6 && p_.y <= 7)) {
				if (mouse.LB.down) {
					toragu.active = true;
					neta[TAMAKO].nokori = 10;
					player.shikin -= neta[TAMAKO].money;
				}
			}
			//サーモン補充
			if ((p_.x >= 12 && p_.x <= 13 && p_.y >= 6 && p_.y <= 7)) {
				if (mouse.LB.down) {
					toragu.active = true;
					neta[SAMON].nokori = 10;
					player.shikin -= neta[TAMAKO].money;
				}
			}
			//のり補充
			if ((p_.x >= 15 && p_.x <= 16 && p_.y >= 6 && p_.y <= 7)) {
				if (mouse.LB.down) {
					toragu.active = true;
					neta[NORI].nokori = 10;
					player.shikin -= neta[TAMAKO].money;
				}
			}
			//マクロ補充
			if ((p_.x >= 18 && p_.x <= 19 && p_.y >= 6 && p_.y <= 7)) {
				if (mouse.LB.down) {
					toragu.active = true;
					neta[MAKURO].nokori = 10;
					player.shikin -= neta[TAMAKO].money;
				}
			}
			//いくら補充
			if ((p_.x >= 21 && p_.x <= 22 && p_.y >= 6 && p_.y <= 7)) {
				if (mouse.LB.down) {
					toragu.active = true;
					neta[IKURA].nokori = 10;
					player.shikin -= neta[TAMAKO].money;
				}
			}
			//きゅうり補充
			if ((p_.x >= 6 && p_.x <= 7 && p_.y >= 9 && p_.y <= 10)) {
				if (mouse.LB.down) {
					toragu.active = true;
					neta[KYUURI].nokori = 10;
					player.shikin -= neta[TAMAKO].money;
				}
			}
			//甘海老補充
			if ((p_.x >= 9 && p_.x <= 10 && p_.y >= 9 && p_.y <= 10)) {
				if (mouse.LB.down) {
					toragu.active = true;
					neta[AMAEBI].nokori = 10;
					player.shikin -= neta[TAMAKO].money;
				}
			}
			//海老補充
			if ((p_.x >= 12 && p_.x <= 13 && p_.y >= 9 && p_.y <= 10)) {
				if (mouse.LB.down) {
					toragu.active = true;
					neta[EBI].nokori = 10;
					player.shikin -= neta[TAMAKO].money;
				}
			}
			//閉じるボタン
			if ((p_.x >= 24 && p_.x <= 25 && p_.y >= 5 && p_.y <= 6)) {
				if (mouse.LB.down) {
					ph_.active = true;
				}
			}
		}
	}
	//----------------------------------------------------------------
	//電話表示
	void phone_Render(Seat& ph_)
	{
		if (ph_.active == true) {
			ML::Box2D draw(0,0, 64, 64);
			ph_.hitBase = draw;
			draw.Offset(ph_.x*32, ph_.y*32);
			ML::Box2D src(0, 0, 64, 64);
			DG::Image_Draw("phoneImg", draw, src);
		}
		if (ph_.active == false) {
			ML::Box2D draw(0,0,800,320);
			ph_.hitBase = draw;
			draw.Offset(ph_.x*3, ph_.y*9);			//電話説明の位置
			ML::Box2D src(0, 0, 1000, 400);
			DG::Image_Draw("phone_messageImg", draw, src);
		}
	}
	//----------------------------------------------------------------
	//背景の初期化
	void haikei_Initialize(Player& h_, float x_, float y_)
	{
		h_.x = x_;
		h_.y = y_;
	}
	//----------------------------------------------------------------
	//背景の表示
	void haikei_Render(Player& h_)
	{
		ML::Box2D draw(0, 0, 960,544);
		h_.hitBase = draw;
		draw.Offset(h_.x, h_.y);
		ML::Box2D src(0, 0,960, 544);
		DG::Image_Draw("haikeiImg", draw, src);
	}
	//----------------------------------------------------------------
	//運送の車初期化
	void toragu_Initialize(Seat& trg_, float x_, float y_)
	{
		trg_.active = false;
		trg_.x = x_;
		trg_.y = y_;
		trg_.animCnt = 0;
		trg_.kaunto = 0;
	}
	//----------------------------------------------------------------
	//運送の車の表示
	void toragu_Render(Seat& trg_)
	{
		if (trg_.active == true) {
			toragu_Anim(trg_);
			ML::Box2D draw = trg_.drawBase.OffsetCopy(float(trg_.x) * 32, float(trg_.y) * 32);
			DG::Image_Draw("toraguImg", draw, trg_.src);
		}
	}
	//----------------------------------------------------------------
	//運送の車アニメーション
	void toragu_Anim(Seat& trg_)
	{
		trg_.animCnt++;
		trg_.kaunto++;
		trg_.drawBase = ML::Box2D(0,0,960, 544);
		switch ((trg_.animCnt /6) % 2) {
		case 0:trg_.src = ML::Box2D(0,0,960,544); break;
		case 1:trg_.src = ML::Box2D(960,0,960,544); break;
		}
		//60フレームになったら、運送の車が消える
		if (trg_.kaunto == 60) {
			trg_.active = false;
			trg_.kaunto = 0;
		}
	}
	//----------------------------------------------------------------
	//営業開始の初期化
	void eigyoukaishi_Initialize(Player& e_)
	{
		//営業開始イージング(個有名、移動の種類、開始値、終了値、移動する時間)
		easing::Set("BACKOUT", easing::EASINGTYPE::BACKIN ,0, 1000, 150);			//BACKOUT
		easing::Start("BACKOUT");
		e_.active = true;
	}
	//----------------------------------------------------------------
	//営業開始のカウント
	void eigyoukaishi_UpDate(Player& e_)
	{
		//イージングの種類によって移動する
		e_.x = easing::GetPos("BACKOUT");
		if (e_.x > 999) {
			e_.active = false;
		}
	}
	//----------------------------------------------------------------
	//営業開始の表示
	void eigyoukaishi_Render(Player& e_)
	{
		if (e_.active == true) {
			ML::Box2D draw(0, 0, 480, 272);
			draw.Offset(e_.x, e_.y+120);
			ML::Box2D src(0, 0, 480, 272);
			DG::Image_Draw("eigyoukaishiImg", draw, src);
		}
	}
}
