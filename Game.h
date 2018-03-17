#pragma once
#include "Scene.h"
#include "easing.h"

//�Q�[���{�҃N���X
class Game : public Scene {
protected:
	//�Q�[�����
	DI::Mouse mouse;
	DI::VGamePad  in1;
	enum Motion { Non, Normal, Dead, Happy, Angry };
	enum SushiNumber{
		Syari, Tamako, Samon, Nori, Makuro,Ikura, Kyuuri, Amaebi, Ebi,
		MakuroSushi, SamonSushi, TamakoSushi, IkuraSushi,
		KyuuriSushi, TeekaSushi, AmaebiSushi, EbiSushi,Kuuran,Neta_Maxnum,	//�ő吔
	};

	//���i�̍\����
	struct Sushi {
		float x;
		float y;
		float befX;
		float befY;
		int arr[17][30];
		ML::Box2D chip[24];
		int animCnt;				//���i�A�j���[�V����
		bool active;				//�������f
		ML::Box2D hitBase;			//�����蔻��p
		bool hitFlag;				//�����蔻��
		Motion motion;
		SushiNumber sushinumber;
		ML::Box2D drawBase;
		ML::Box2D src;
		//int number;					//�ԍ�
		int moveCnt;
		int Cnt;
	};

	//�ȁA�ݔ��̍\����
	struct Seat {
		ML::Vec2 pos;
		bool isEmpty;				//�Ȃ̔��f�p
		float x;
		float y;
		bool active;
		ML::Box2D hitBase;			//�����蔻��p
		int moveCnt;
		ML::Box2D drawBase;
		ML::Box2D src;
		int animCnt;				//�A�j���[�V����
		int kaunto;					//�J�E���g
	};

	const static int Maxvalue = 6;			//�ȁA�q�̐����T����,��ȏ󋵁��U
	int emptySeat = Maxvalue;				//��ȏ󋵁��U


	//�ȁA�X�ݔ��̐ݒ�ϐ�
	Seat seiki[Maxvalue]{
		{ ML::Vec2(21,3),true },
	{ ML::Vec2(16,3),true },
	{ ML::Vec2(11,3),true },
	{ ML::Vec2(6,3),true },
	{ ML::Vec2(2.3f,6),true },
	{ ML::Vec2(25, 6),true }
	};

	//�q�̍\����
	struct Chara {
		float x;
		float y;
		int animCnt;				//�q�A�j���[�V����
		bool active;				//�q�����݂����f
		ML::Box2D hitBase;			//�����蔻��
		float speed;				//�ړ����x
		Motion motion;
		ML::Box2D drawBase;
		ML::Box2D src;
		SushiNumber sushinumber;
		int typeNum;				//�q�^�C�v
		//int number;
		int kaunto;					//�J�E���g
		bool hitFlag;
	};

	//�v�Z�\����
	struct Keisan {
		int kaisu;		//��
		int nokori;		//�c���
		int money;		//�l�i
	};
	//enum {
	//	//�l�^
	//	SYARI,			//�V����
	//	TAMAKO,			//�ʎq
	//	SAMON,			//�T�[�}��
	//	NORI,			//�̂�
	//	MAKURO,			//�܂���
	//	IKURA,			//������
	//	KYUURI,			//���イ��
	//	AMAEBI,			//�ÊC�V
	//	EBI,			//�C�V
	//	////�v�Z
	//	//SUSHI,
	//	//MAKI,
	//	//KURIKU,			//�N���b�N
	//	//NETA_MAXNUM,	//�ő吔
	//};
	Keisan neta[Neta_Maxnum];		//�l�^�̍ő吔
	Keisan keisan;

	//�v���C���[�̍\����
	struct Player {
		bool clearFlag;			//�N���A
		bool gameoverFlag;		//gameover
		bool timeOver;			//�c�ƏI������
		int timeCnt;
		int timeStart;			//�c�ƊJ�n����
		int jikan;
		int shikin;				//�v���C���[����
		int ninki;				//�v���C���[�l�C
		ML::Box2D hitBase;
		ML::Box2D drawBase;
		ML::Box2D src;
		int animCnt;			//�A�j���[�V����
		bool active;
		float x;
		float y;
		int Cnt;
	};
	//�v���C���[�ϐ�
	Player player;
	Player shisyou;				//�t��
	Player haikei;				//�w�i
	Player eigyoukaishi;		//�c�ƊJ�n

	//�ȁA�X�ݔ��̐ݒ�ϐ�
	Seat menu;					//���j���[
	Seat phone;					//�d�b
	Seat kaitenren;				//��]���[��
	Seat toragu;				//�^���̎�

	//�l�^�A���i�ϐ�
	Sushi sushi;				//�l�^
	Sushi allsushi[Maxvalue];	//���i�̂܂Ƃ�			
	Sushi makisu;				//������

	//�q�A�q�̍D�ݎ��i�ϐ�
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
	//�N���X�̃����o�֐�
	//-----------------------------------------------------------------------------
	//�v���C���[�̏������̃v���g�^�C�v�錾
	void player_Initialize(Player& pl_);
	//�v���C���[�̃J�E���g�̃v���g�^�C�v�錾
	void player_UpDate(Player& pl_);
	//�v���C���[�̕\���̃v���g�^�C�v�錾
	void player_Render(Player& pl_);
	//�w�i�̏������̃v���g�^�C�v�錾
	void haikei_Initialize(Player& h_, float x_, float y_);
	//�w�i�̕\���̃v���g�^�C�v�錾
	void haikei_Render(Player& h_);
	//�t���̏������̃v���g�^�C�v�錾
	void shisyou_Initialize(Player& siy_, float x_, float y_);
	//�t���̕\���̃v���g�^�C�v�錾
	void shisyou_Render(Player& siy_);
	//�t���̃A�j���[�V�����v���g�^�C�v�錾
	void shisyou_Anim(Player& siy_);
	//�c�ƊJ�n�̏������̃v���g�^�C�v�錾
	void eigyoukaishi_Initialize(Player& c_);
	//�c�ƊJ�n�̃J�E���g�̃v���g�^�C�v�錾
	void eigyoukaishi_UpDate(Player& e_);
	//�c�ƊJ�n�̕\���̃v���g�^�C�v�錾
	void eigyoukaishi_Render(Player& e_);
	//���v�������̃v���g�^�C�v�錾
	void toukei_Initialize();
	//�q���o��������֐��̃v���g�^�C�v�錾
	void customer_Appear();
	//�q�̏������̃v���g�^�C�v�錾
	void customer_Initialize(Chara& c_, Seat& s_);
	//�q�̕\���̃v���g�^�C�v�錾
	void customer_Render(Chara& c_);
	//�q�̍s���̃v���g�^�C�v�錾
	void customer_UpDate(Chara& c_, Seat & set_, Chara& lf_, Sushi & asu_, Chara& fu_);
	//�q�̓��X�A���A��̃v���g�^�C�v�錾
	void customer_Respawn(Chara& c_, Sushi& s_, Chara& lf_, Seat& se_, Chara& fu_);
	//���j���[�̏������̃v���g�^�C�v�錾
	void menu_Initialize(Seat& s_, float x_, float y_);
	//���j���[�̕\���̃v���g�^�C�v�錾
	void menu_Render(Seat& s_);
	//���j���[�̍s���̃v���g�^�C�v�錾
	void menu_UpDate(Seat& s_, POINT p_);
	//�qA�������̃v���g�^�C�v�錾
	void customerA_Initialize(Chara& c_, float x_, float y_);
	//�qA�̕\���̃v���g�^�C�v�錾
	void customerA_Render(Chara& c_);
	//�qA�A�j���[�V�����̃v���g�^�C�v�錾
	void customerA_Anim(Chara&  c_);
	//�qA�̓���v���g�^�C�v�錾
	void customerA_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi & asu_, Chara& fu_);
	//�qB�������̃v���g�^�C�v�錾
	void customerB_Initialize(Chara& c_, float x_, float y_);
	//�qB�̕\���̃v���g�^�C�v�錾
	void customerB_Render(Chara& c_);
	//�qB�A�j���[�V�����̃v���g�^�C�v�錾
	void customerB_Anim(Chara&  c_);
	//�qB�̓���v���g�^�C�v�錾
	void customerB_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi & asu_, Chara& fu_);
	//�qC�������̃v���g�^�C�v�錾
	void customerC_Initialize(Chara& c_, float x_, float y_);
	//�qC�̕\���̃v���g�^�C�v�錾
	void customerC_Render(Chara& c_);
	//�qC�A�j���[�V�����̃v���g�^�C�v�錾
	void customerC_Anim(Chara&  c_);
	//�qC�̓���v���g�^�C�v�錾
	void customerC_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi& asu_, Chara& fu_);
	//�qD�������̃v���g�^�C�v�錾
	void customerD_Initialize(Chara& c_, float x_, float y_);
	//�qD�̕\���̃v���g�^�C�v�錾
	void customerD_Render(Chara& c_);
	//�qD�A�j���[�V�����̃v���g�^�C�v�錾
	void customerD_Anim(Chara&  c_);
	//�qD�̓���v���g�^�C�v�錾
	void customerD_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi & asu_, Chara& fu_);
	//�q�̍D�ݎ��i�̏������̃v���g�^�C�v�錾
	void customerlovefood_Initialize(Chara& lf_, float x_, float y_, Chara&  c_, Chara& fu_, float fx_, float fy_);
	//�q�̍D�ݎ��i�̕\���̃v���g�^�C�v�錾
	void customerlovefood_Render(Chara& lf_, Chara& c_, Chara& fu_);
	//�q�̍D�ݎ��i�`�F�b�N�v���g�^�C�v�錾
	void customerlovefood_Anim(Chara& lf_, Chara& fu_);
	//���i�܂Ƃ߃A�j���[�V�����̃v���g�^�C�v�錾
	void allsushi_Anim(Sushi& asu_);
	//���i�܂Ƃ߂̏������̃v���g�^�C�v�錾
	void allsushi_Initialize(float x_, float y_, int saraidx_);
	//���i�܂Ƃ߂̕\���̃v���g�^�C�v�錾
	void allsushi_Render(Sushi& asu_);
	//���i�܂Ƃ߂̍s���̃v���g�^�C�v�錾
	void allsushi_UpDate(Sushi& asu_, POINT p_);
	//���i�p�������̃v���g�^�C�v�錾
	void allsushi_dispose(Sushi& asu_, POINT p_);
	//��]�̃��[���������̃v���g�^�C�v�錾
	void kaitenren_Initialize(Seat& ka_, float x_, float y_);
	//��]�̃��[���̍s���v���g�^�C�v�錾
	void kaitenren_move(Seat& ka_);
	//��]�̃��[���\���̃v���g�^�C�v�錾
	void kaitenren_Render(Seat& ka_);
	//�d�b�������̃v���g�^�C�v�錾
	void phone_Initialize(Seat& ph_, float x_, float y_);
	//�d�b�J�E���g�v���g�^�C�v�錾
	void phone_UpDate(Seat& ph_, POINT p_);
	//�d�b�\���̃v���g�^�C�v�錾
	void phone_Render(Seat& ph_);
	//�^���̎ԏ������̃v���g�^�C�v�錾
	void toragu_Initialize(Seat& trg_, float x_, float y_);
	//�^���̎Ԃ̕\���̃v���g�^�C�v�錾
	void toragu_Render(Seat& trg_);
	//�^���̎ԃA�j���[�V�����̃v���g�^�C�v�錾
	void toragu_Anim(Seat& trg_);
	//�������������̃v���g�^�C�v�錾
	void makisu_Initialize(Sushi& c_, float x_, float y_);
	//�������\���̃v���g�^�C�v�錾
	void makisu_Render(Sushi& c_);
	//�������s���̃v���g�^�C�v�錾
	void makisu_UpDate(Sushi& c_, POINT p_, Sushi& md_);
	//�l�^�P�ʂ̈ړ��`�F�b�N�v���g�^�C�v�錾
	void Sushi_Check(Sushi& su_, POINT p_, Keisan& ku_);
	//���i����ǂݍ��ރv���g�^�C�v�錾
	bool Sushi_Load(Sushi& su_, int n_);
	//���i�̕\���v���g�^�C�v�錾
	void Sushi_Render(Sushi&su_);
	//���i�������̃v���g�^�C�v�錾
	void Sushi_Initialize(Sushi&su_);
	//���i�̔ԍ��̃`�F�b�N�̃v���g�^�C�v�錾
	int CheckDisabledSushi();
};
