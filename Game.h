#pragma once
#include "Scene.h"
#include "easing.h"

//ゲーム本編クラス
class Game : public Scene {
protected:
	//ゲーム情報
	DI::Mouse mouse;
	DI::VGamePad  in1;
	enum Motion { Non, Normal, Dead, Happy, Angry };
	enum SushiNumber{
		Syari, Tamako, Samon, Nori, Makuro,Ikura, Kyuuri, Amaebi, Ebi,
		MakuroSushi, SamonSushi, TamakoSushi, IkuraSushi,
		KyuuriSushi, TeekaSushi, AmaebiSushi, EbiSushi,Kuuran,Neta_Maxnum,	//最大数
	};

	//寿司の構造体
	struct Sushi {
		float x;
		float y;
		float befX;
		float befY;
		int arr[17][30];
		ML::Box2D chip[24];
		int animCnt;				//寿司アニメーション
		bool active;				//生存判断
		ML::Box2D hitBase;			//当たり判定用
		bool hitFlag;				//当たり判定
		Motion motion;
		SushiNumber sushinumber;
		ML::Box2D drawBase;
		ML::Box2D src;
		//int number;					//番号
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
		SushiNumber sushinumber;
		int typeNum;				//客タイプ
		//int number;
		int kaunto;					//カウント
		bool hitFlag;
	};

	//計算構造体
	struct Keisan {
		int kaisu;		//回数
		int nokori;		//残り個数
		int money;		//値段
	};
	//enum {
	//	//ネタ
	//	SYARI,			//シャリ
	//	TAMAKO,			//玉子
	//	SAMON,			//サーマン
	//	NORI,			//のり
	//	MAKURO,			//まくろ
	//	IKURA,			//いくら
	//	KYUURI,			//きゅうり
	//	AMAEBI,			//甘海老
	//	EBI,			//海老
	//	////計算
	//	//SUSHI,
	//	//MAKI,
	//	//KURIKU,			//クリック
	//	//NETA_MAXNUM,	//最大数
	//};
	Keisan neta[Neta_Maxnum];		//ネタの最大数
	Keisan keisan;

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

public:
	explicit Game() {}
	virtual ~Game() {}

	virtual void InitializeResource();
	virtual bool Initialize();
	virtual void Update();
	virtual void Render();
	virtual void Finalize();
	//-----------------------------------------------------------------------------
	//クラスのメンバ関数
	//-----------------------------------------------------------------------------
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
	void customer_Initialize(Chara& c_, Seat& s_);
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
	void customerlovefood_Render(Chara& lf_, Chara& c_, Chara& fu_);
	//客の好み寿司チェックプロトタイプ宣言
	void customerlovefood_Anim(Chara& lf_, Chara& fu_);
	//寿司まとめアニメーションのプロトタイプ宣言
	void allsushi_Anim(Sushi& asu_);
	//寿司まとめの初期化のプロトタイプ宣言
	void allsushi_Initialize(float x_, float y_, int saraidx_);
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
	void phone_UpDate(Seat& ph_, POINT p_);
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
};
