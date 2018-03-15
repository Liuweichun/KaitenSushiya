#include "MyPG.h"
#include "MyGameMain.h"
#include <time.h>
#include "easing.h"

//�Q�[���{��
namespace Game
{
	//�Q�[�����
	DI::Mouse mouse;
	DI::VGamePad  in1;
	enum Motion { Non, Normal, Dead, Happy,Angry };

	//���i�̍\����
	struct Sushi {
		float x;
		float y;
		int arr[17][30];
		ML::Box2D chip[24];
		int animCnt;				//���i�A�j���[�V����
		bool active;				//�������f
		ML::Box2D hitBase;			//�����蔻��p
		bool hitFlag;				//�����蔻��
		Motion motion;
		ML::Box2D drawBase;
		ML::Box2D src;
		int number;					//�ԍ�
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
	//int newcustomer = 0;

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
		int number;					//�ԍ�
		int typeNum;				//�q�^�C�v
		int kaunto;					//�J�E���g
		bool hitFlag;
	};

	//�v�Z�\����
	struct Keisan {
		int kaisu;		//��
		int nokori;		//�c���
		int money;		//�l�i
	};
	enum {
		//�l�^
		SYARI,			//�V����
		TAMAKO,			//�ʎq
		SAMON,			//�T�[�}��
		NORI,			//�̂�
		MAKURO,			//�܂���
		IKURA,			//������
		KYUURI,			//���イ��
		AMAEBI,			//�ÊC�V
		EBI,			//�C�V
		//�v�Z
		SUSHI,
		MAKI,
		KURIKU,			//�N���b�N
		NETA_MAXNUM,	//�ő吔
	};
	Keisan neta[NETA_MAXNUM];		//�l�^�̍ő吔

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


	//------------------------------------------------------------------------------
	//�֐��̃v���g�^�C�v�錾
	//------------------------------------------------------------------------------
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
	void customer_Initialize(Chara& c_,Seat& s_);
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
	void customerlovefood_Render(Chara& lf_,Chara& c_, Chara& fu_);
	//�q�̍D�ݎ��i�`�F�b�N�v���g�^�C�v�錾
	void customerlovefood_Anim(Chara& lf_, Chara& fu_);
	//���i�܂Ƃ߃`�F�b�N�̃v���g�^�C�v�錾
	void allsushi_Anim(Sushi& asu_);
	//���i�܂Ƃ߂̏������̃v���g�^�C�v�錾
	void allsushi_Initialize(float x_, float y_);
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
	void phone_UpDate(Seat& ph_,POINT p_);
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

	//-----------------------------------------------------------------------------
	//����������
	//�@�\�T�v�F�v���O�����N�����ɂP����s�����i�f�ނȂǂ̏������s���j
	//-----------------------------------------------------------------------------
	void Initialize()
	{
		DG::Image_Create("sushisozaiImg", "./data/image/sushisozai01.png");				//���i�A�l�^�[
		DG::Image_Create("De_sushisozaiImg", "./data/image/De_sushisozai.png");			//���������i
		DG::Image_Create("haikeiImg", "./data/image/haikei.png");						//�w�i
		DG::Image_Create("menuImg", "./data/image/menu.png");							//���j���[
		DG::Image_Create("menu_setsumeiImg", "./data/image/menu_setsumei.png");			//���j���[����
		DG::Image_Create("phoneImg", "./data/image/phone.png");							//�d�b
		DG::Image_Create("toraguImg", "./data/image/toragu.png");						//�^���̎�
		DG::Image_Create("shisyouImg", "./data/image/shisyou.png");						//�t��
		DG::Image_Create("eigyoukaishiImg", "./data/image/eigyoukaishi.png");			//�c�ƊJ�n
		DG::Image_Create("fukidashiImg", "./data/image/fukidashi.png");					//�����o��
		DG::Image_Create("tsukuriImg", "./data/image/tsukuri.png");						//���A�j���[�V����
		DG::Image_Create("phone_messageImg", "./data/image/phone_message.png");			//�d�b���b�Z�[�W
		DG::Image_Create("makisuImg", "./data/image/sushi_makisu.png");					//������
		DG::Image_Create("kaitenrenImg", "./data/image/kaitenren.png");					//��]���[��
		DG::Image_Create("customerImg", "./data/image/customer.png");					//�q
		DG::Font_Create("FontA", "HGS�n�p�p�޼��UB", 12, 20);							//�����\���p�̃t�H���g���쐬
		DG::Font_Create("FontB", "HGS�n�p�p�޼��UB", 8, 10);

		//PG���s���̎����𗘗p���Ė���Ⴄ�����\��I������
		srand((unsigned int)time(NULL));

		//�w�i�̏�����
		haikei_Initialize(haikei,0,0);

		//��]���[���̏�����
		kaitenren_Initialize(kaitenren,5,6);

		//������������
		makisu_Initialize(makisu, 7, 13);

		//���v�̏�����
		toukei_Initialize();

		//���i�̂܂Ƃߏ�����
		for (int i = 0; i < Maxvalue; ++i) {	//Maxvalue
			allsushi[i].active = false;
		}

		//�t���̏�����
		shisyou_Initialize(shisyou,10,11);

		//���j���[�̏�����
		menu_Initialize(menu,16,11);

		//�q���o��������
		customer_Appear();

		//�q�̍D�ݎ��i�̏�����
		for (int i = 0; i < Maxvalue; ++i) {
			customerlovefood_Initialize(lovefood[i], customer[i].x +1, customer[i].y-1, customer[i],fukidashi[i], customer[i].x+0.5f, customer[i].y - 2);
		}

		//�d�b�̏�����
		phone_Initialize(phone,22,14);

		//�^���̎ԏ�����
		toragu_Initialize(toragu,0,0);

		//�v���C���[�̏�����
		player_Initialize(player);

		//���i�z����̏�����
		Sushi_Initialize(sushi);

		//���i����ǂݍ���
		Sushi_Load(sushi, 1);

		//�c�ƊJ�n�̏�����
		eigyoukaishi_Initialize(eigyoukaishi);
	}
	//-----------------------------------------------------------------------------
	//�������
	//�@�\�T�v�F�v���O�����I�����ɂP����s�����i�f�ނȂǂ̉�����s���j
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
	//���s����
	//�@�\�T�v�F�Q�[���̂P�t���[���ɓ����鏈��
	//-----------------------------------------------------------------------------
	TaskFlag UpDate()
	{
		in1 = DI::GPad_GetState("P1");
		//�}�E�X�J�[�\���̈ʒu�𓾂�
		mouse = DI::Mouse_GetState();
		POINT pos;
		pos.x = mouse.cursorPos.x / 32;
		pos.y = mouse.cursorPos.y / 32;

		//�c�ƊJ�n�[�̃J�E���g
		eigyoukaishi_UpDate(eigyoukaishi);

		if (eigyoukaishi.active == false) {
			if (mouse.LB.down) {
				//�͈͓��ŃN���b�N������N���b�N�񐔂������Ă���
				if (pos.x >= 2 && pos.x <= 4 && pos.y >= 12 && pos.y <= 14) {
					//�}�b�v�`�F�b�N
					Sushi_Check(sushi, pos, neta[KURIKU]);	//(kuriku)
					neta[KURIKU].kaisu++;				//�N���b�N��
				}
				//�������s��
				makisu_UpDate(makisu, pos, sushi);
			}

			//���i�܂Ƃߍs��
			for (int i = 0; i < Maxvalue; ++i) {
				allsushi_UpDate(allsushi[i], pos);
			}

			//�q�̍s��
			for (int i = 0; i < Maxvalue; ++i) {
				customer_UpDate(customer[i], seiki[i], lovefood[i], allsushi[i], fukidashi[i]);
			}

			//�d�b�J�E���g
			phone_UpDate(phone, pos);

			//���j���[�̍s��
			menu_UpDate(menu, pos);


			//�v���C���[�̃J�E���g
			player_UpDate(player);
		}

		TaskFlag rtv = Task_Game;
		if (player.gameoverFlag==true) {
			remove("data\\Savefile.txt");
			rtv = Task_Gameover;	//���̃^�X�N���Q�[���I�[�o�[��
		}
		if (player.timeOver == true) {
			ofstream save("data\\Savefile.txt", std::ios::trunc);
			save << player.shikin << endl;
			save << player.ninki << endl;
			save.close();
			rtv = Task_Outsidebusinesshours;	//���̃^�X�N���c�Ǝ��ԊO��
		}
		if (player.clearFlag == true) {
			remove("data\\Savefile.txt");
			rtv = Task_Gameclear;	//���̃^�X�N���Q�[���N���A��
		}
		return rtv;
	}
	//-----------------------------------------------------------------------------
	//�`�揈��
	//�@�\�T�v�F�Q�[���̂P�t���[���ɓ�����\������
	//-----------------------------------------------------------------------------
	void Render()
	{
		//�w�i�̕\��
		haikei_Render(haikei);

		//�������\��
		makisu_Render(makisu);

		//�l�^�̕\��
		Sushi_Render(sushi);


		//�t���̕\��
		shisyou_Render(shisyou);

		//��]���[���̕\��
		kaitenren_Render(kaitenren);

		//�q�̕\��
		for (int i = 0; i < Maxvalue; ++i) {
			customer_Render(customer[i]);
		}

		//�q�̍D�ݎ��i�̕\��
		for (int i = 0; i < Maxvalue; ++i) {
			customerlovefood_Render(lovefood[i],customer[i],fukidashi[i]);
		}

		//���i�̂܂Ƃߕ\��
		for (int i = 0; i < 20; ++i) {		//Maxvalue
			allsushi_Render(allsushi[i]);
		}

		//���j���[�̕\��
		menu_Render(menu);

		//�d�b�\��
		phone_Render(phone);

		//�^���̎Ԃ̕\��
		toragu_Render(toragu);

		//�v���C���[�̕\��
		player_Render(player);

		//�c�ƊJ�n�̕\��
		eigyoukaishi_Render(eigyoukaishi);

	}
	//--------------------------------------------------------------------------------------------------------------------------------
	//�֐�
	//--------------------------------------------------------------------------------------------------------------------------------
	//�v���C���[�̏�����
	void player_Initialize(Player& pl_)
	{
		pl_.clearFlag = false;			//�Q�[���N���A����
		pl_.gameoverFlag = false;		//�Q�[���I�[�o�[����
		pl_.timeOver = false;			//�c�ƏI������
		pl_.timeCnt = 0;
		pl_.timeStart = 8;				//�c�ƊJ�n���Ԃ�8��
		pl_.jikan = 0;
		pl_.shikin = 200;				//�v���C���[�̎���������
		pl_.ninki = 6;					//�v���C���[�̐l�C������

		// �����Ɛl�C�̓ǂݍ���
		// �������A�Z�[�u�t�@�C�����Ȃ������ꍇ�A�V�������
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
	//�v���C���[�̃J�E���g
	void player_UpDate(Player& pl_)
	{
		if (in1.ST.down==true) {
			pl_.shikin += 500;
		}
		if (in1.SE.down == true) {
			pl_.ninki += 5;
		}

		//�c�Ǝ��ԃJ�E���g
		pl_.timeCnt++;
		if (pl_.timeCnt % 960 == 0 && pl_.clearFlag == false && pl_.timeOver == false) {	//������4���Ԃقǂ̓Q�[����8������24�܂�//960
			pl_.jikan++;
		}
		if ((pl_.timeStart + pl_.jikan) == 24) {
			pl_.timeOver = true;
		}

		//�Q�[���N���A����
		if (pl_.ninki>100) {
			pl_.clearFlag = true;
		}

		//�Q�[���I�[�o�[����
		if (pl_.shikin < 0 || pl_.ninki < 0) {
			pl_.gameoverFlag = true;
		}

	}
	//--------------------------------------------------------------------------------------------------------------------------------
	//�v���C���[�̕\��
	void player_Render(Player& pl_)
	{
		//�c�Ǝ��ԕ\��
		ML::Box2D textBox(200, 0, 480, 32);
		string text = "���ԁF" + to_string(pl_.timeStart + pl_.jikan) + "��";
		DG::Font_Draw("FontA", textBox, text, ML::Color(1, 1, 1, 1));

		//�v���C���[�̎����\��
		ML::Box2D moneyBox(400, 0, 480, 32);
		string money = "�����F" + to_string(pl_.shikin);
		DG::Font_Draw("FontA", moneyBox, money, ML::Color(1, 1, 1, 1));

		//�v���C���[�̐l�C�\��
		ML::Box2D ninkiBox(600, 0, 480, 32);
		string ninki = "�l�C�F" + to_string(pl_.ninki);
		DG::Font_Draw("FontA", ninkiBox, ninki, ML::Color(1, 1, 1, 1));
	}
	//--------------------------------------------------------------------------------------------------------------------------------
	//�t���̏�����
	void shisyou_Initialize(Player& siy_, float x_, float y_)
	{
		siy_.active = true;
		siy_.x = x_;
		siy_.y = y_;
		siy_.animCnt = 0;
		siy_.Cnt = 0;
	}
	//----------------------------------------------------------------
	//�t���̕\��
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
	//�t���̃A�j���[�V����
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
	//�q�̏�����
	void customer_Initialize(Chara& c_,Seat& s_)
	{	//�q�̐Ȃ̏���
		if (s_.isEmpty==true) {
				s_.isEmpty = false;
				c_.typeNum = rand() % 4;					//�q�̃^�C�v�𗐐��Ō��܂�
				switch (c_.typeNum) {
				case 0: customerA_Initialize(c_, s_.pos.x, s_.pos.y); break;
				case 1: customerB_Initialize(c_, s_.pos.x, s_.pos.y); break;
				case 2: customerC_Initialize(c_, s_.pos.x, s_.pos.y); break;
				case 3: customerD_Initialize(c_, s_.pos.x, s_.pos.y); break;
				}
				s_.isEmpty = true;
				/*emptySeat -= 1;*/
		}
		//���@B
		//if (emptySeat > 0) {
		//	int newcustomer = 0;
		//	do {
		//		newcustomer = rand() % Maxvalue;
		//	} while (!seiki[newcustomer].isEmpty);
		//	seiki[newcustomer].isEmpty = false;
		//		c_.typeNum = rand() % 3;					//�q�̃^�C�v�𗐐��Ō��܂�
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
	//�q���o��������
	void customer_Appear()
	{
		for (int i = 0; i < Maxvalue; ++i) {
			customer_Initialize(customer[i],seiki[i]);
		}
	}
	//----------------------------------------------------------------
	//�q�̕\��
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
	//�q�̍s��
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
		//�q�A�j���[�V�����̍s��
		c_.animCnt++;
	}
	//----------------------------------------------------------------
	//�q�̓��X�A���A�菈��
	void customer_Respawn(Chara & c_, Sushi & asu_, Chara& lf_, Seat & set_,Chara & fu_)
	{
		set_.isEmpty = false;
		c_.active = false;
		lf_.active = false;			//�q�̍D�ݎ��i��������

		//�܂��q���o��������
		c_.typeNum = rand() % 4;				//�q�̃^�C�v�𗐐��Ō��܂�
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
		//�܂��q�̍D�ݎ��i���o��������
		customerlovefood_Initialize(lf_, c_.x+1,c_.y-1,c_,fu_, c_.x +0.5f, c_.y - 2);

		//c_.active = false;			//�q������v
		//s_.active = false;			//���i��������
		//lf_.active = false;			//�q�̍D�ݎ��i��������
		//se_.isEmpty = true;
	
		//emptySeat++;
		////�܂��q���o��������
		//c_.typeNum = rand() % 4;					//�q�̃^�C�v�𗐐��Ō��܂�
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
		////�܂��q�̍D�ݎ��i���o��������
		//customerlovefood_Initialize(lf_, c_.x + 3, c_.y);
		//���[�v�񂵂Ă��A�V�������[�v�ɂȂ�̂ŁA�Ȃ̏�Ԕ��f�ł��Ȃ��Ȃ�H�H�H���̏�Ԃ��ƁA����̋q�̓�����ɂȂ����玟�̋q���o����q���d�˂�
		//�ǂ�����ē����蔻�f�����̐Ȃ̏�Ԃ�true�ɂȂ��āAemptySeat+1�ɂȂ��̂��Q�H�H�H
	}
	//----------------------------------------------------------------
	//�qA�̏�����
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
	//�qA�̕\��
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
	//�qA�A�j���[�V����
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
	//�qA�̓���
	void customerA_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi& asu_, Chara& fu_)
	{
		//�q�J�E���g
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
			//�v���C���[�̐l�C���|�P
			player.ninki--;
			if (set_.isEmpty == true) {
				//�q�̓��X�A���A�菈��
				customer_Respawn(c_, asu_, lf_, set_,fu_);
			}
			set_.isEmpty = true;
		}
	}
	//----------------------------------------------------------------
	//�qB�̓���
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
			//�v���C���[�̐l�C���|�P
			player.ninki--;
			if (set_.isEmpty == true) {
				//�q�̓��X�A���A�菈��
				customer_Respawn(c_, asu_, lf_, set_,fu_);
			}
			set_.isEmpty = true;
		}
		//�q�J�E���g
		c_.kaunto++;
	}
	//----------------------------------------------------------------
	//�qC�̓���
	void customerC_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi& asu_, Chara& fu_)
	{
		//�q�J�E���g
		c_.kaunto++;
		if (c_.kaunto == 400){
			c_.active = true;
		}
		if (c_.kaunto > 1200){
			c_.motion = Angry;
		}
		if (c_.kaunto > 2700) {
			//�v���C���[�̐l�C���|�P
			player.ninki--;
			if (set_.isEmpty == true) {
				//�q�̓��X�A���A�菈��
				customer_Respawn(c_, asu_, lf_, set_,fu_);
			}
			set_.isEmpty = true;
		}
	}
	//----------------------------------------------------------------
	//�qD�̓���
	void customerD_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi& asu_, Chara& fu_)
	{
		//�q�J�E���g
		c_.kaunto++;
		if (c_.kaunto == 100){
			c_.active = true;
		}
		//�q���҂��Ԃ͒x�߂���Ɠ{��
		if (c_.kaunto>900){
			c_.motion = Angry;
		}
		if (c_.kaunto > 2400) {
			//�v���C���[�̐l�C���|�P
			player.ninki--;
			if (set_.isEmpty == true) {
				//�q�̓��X�A���A�菈��
				customer_Respawn(c_, asu_, lf_, set_,fu_);
			}
			set_.isEmpty = true;
		}
	}
	//----------------------------------------------------------------
	//�qB������
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
	//�qB�̕\��
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
	//�qB�A�j���[�V����
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
			//��
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
	//�qC������
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
	//�qC�̕\��
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
	//�qC�A�j���[�V����
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
			//��
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
	//�qD�̏�����
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
	//�qD�̕\��
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
	//�qD�A�j���[�V����
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
	//�q�̍D�ݎ��i�̏�����
	void customerlovefood_Initialize(Chara& lf_, float x_, float y_, Chara&  c_, Chara& fu_, float fx_, float fy_)
	{
		lf_.x = x_;
		lf_.y = y_;
		fu_.x = fx_;
		fu_.y = fy_;
		fu_.active = c_.active;
		lf_.active = c_.active;
		lf_.number = 9 + rand() % 8;		//9�Ԃ�����i�ɂȂ�
	}
	//----------------------------------------------------------------
	//�q�̍D�ݎ��i�̕\��
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
	//�q�̍D�ݎ��i�`�F�b�N
	void customerlovefood_Anim(Chara& lf_,Chara& fu_)
	{
		lf_.drawBase = ML::Box2D(0, -12, 32, 32);
		switch (lf_.number) {
		case 9:lf_.src = ML::Box2D(32, 64, 32, 32);  break;			//�}�N�����i
		case 10:lf_.src = ML::Box2D(64, 64, 32, 32); break;			//�T�[�������i
		case 11:lf_.src = ML::Box2D(96, 64, 32, 32); break;			//�ʎq���i
		case 12:lf_.src = ML::Box2D(0, 96, 32, 32); break;			//��������i
		case 13:lf_.src = ML::Box2D(32, 96, 32, 32); break;			//���イ����i
		case 14:lf_.src = ML::Box2D(64, 96, 32, 32); break;			//�S�Ύ��i
		case 15:lf_.src = ML::Box2D(96, 96, 32, 32); break;			//�ÊC�V���i
		case 16:lf_.src = ML::Box2D(0, 128, 32, 32); break;			//�C�V���i
		//case 14:lf_.src = ML::Box2D(160, 0, 32, 32); break;
		}
		//�����o���̕\��
		fu_.drawBase = ML::Box2D(0, 0, 64, 64);
		fu_.src = ML::Box2D(0, 0, 64, 64);

	}
	//----------------------------------------------------------------
	//���i�̂܂Ƃߏ�����
	void allsushi_Initialize(float x_, float y_)
	{
		for (int i = 0; i < Maxvalue; ++i) {
			if (allsushi[i].active == false) {
				//���i�𐶐�����
				allsushi[i].active = true;
				allsushi[i].x = x_;
				allsushi[i].y = y_;
				allsushi[i].motion = Normal;
				allsushi[i].moveCnt = 0;
				allsushi[i].hitBase = ML::Box2D(0, -12, 32, 32);
				allsushi[i].animCnt = 0;
				allsushi[i].Cnt = 0;
				allsushi[i].hitFlag = false;
				break;						//���i�����[�v�𔲂���
			}
		}
	}
	//----------------------------------------------------------------
	//���i�܂Ƃ߂̕\��
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
	//���i�܂Ƃ߃`�F�b�N
	void allsushi_Anim(Sushi& asu_)
	{
		enum {
			makurosushi=9, samonsushi = 10,tamakosushi = 11,ikurasushi=12,
			kyuurisushi=13,teekasushi=14,amaebisushi=15,ebisushi=16
		};
		switch (asu_.motion) {
		case Normal:													//���i�͐V�N�ȏ��
			asu_.drawBase = ML::Box2D(0, -12, 32, 32);
			switch (asu_.number){										
			case makurosushi:asu_.src = ML::Box2D(32, 64, 32, 32); break;			//�}�N�����i
			case samonsushi:asu_.src = ML::Box2D(64, 64, 32, 32); break;			//�T�[�������i
			case tamakosushi:asu_.src = ML::Box2D(96, 64, 32, 32); break;			//�ʎq���i
			case ikurasushi:asu_.src = ML::Box2D(0, 96, 32, 32); break;				//��������i
			case kyuurisushi:asu_.src = ML::Box2D(32, 96, 32, 32); break;			//���イ����i
			case teekasushi:asu_.src = ML::Box2D(64, 96, 32, 32); break;			//�S�Ύ��i
			case amaebisushi:asu_.src = ML::Box2D(96,96, 32, 32); break;			//�ÊC�V���i
			case ebisushi:asu_.src = ML::Box2D(0, 128, 32, 32); break;				//�C�V���i
			}
			break;
		case Dead:														//���i�͐V�N����Ȃ����
			asu_.drawBase = ML::Box2D(0, -12, 32, 32);
			switch (asu_.number) {
			case makurosushi:														//�V�N����Ȃ��}�N�����i
				switch ((asu_.animCnt / 7) % 4){
				case 0:asu_.src = ML::Box2D(0,0, 32, 32); break;
				case 1:asu_.src = ML::Box2D(32, 0, 32, 32); break;
				case 2:asu_.src = ML::Box2D(64, 0, 32, 32); break;
				case 3:asu_.src = ML::Box2D(96, 0, 32, 32); break;
				}
				break;
			case samonsushi:													//�V�N����Ȃ��T�[�������i
				switch ((asu_.animCnt / 7) % 4) {
				case 0:asu_.src = ML::Box2D(0, 32, 32, 32); break;
				case 1:asu_.src = ML::Box2D(32, 32, 32, 32); break;
				case 2:asu_.src = ML::Box2D(64, 32, 32, 32); break;
				case 3:asu_.src = ML::Box2D(96, 32, 32, 32); break;
				}
				break;
			case tamakosushi:													//�V�N����Ȃ��ʎq���i
				switch ((asu_.animCnt / 7) % 4) {
				case 0:asu_.src = ML::Box2D(0, 64, 32, 32); break;
				case 1:asu_.src = ML::Box2D(32, 64, 32, 32); break;
				case 2:asu_.src = ML::Box2D(64, 64, 32, 32); break;
				case 3:asu_.src = ML::Box2D(96, 64, 32, 32); break;
				}
				break;
			case ikurasushi:													//�V�N����Ȃ���������i
				switch ((asu_.animCnt / 7) % 4) {
				case 0:asu_.src = ML::Box2D(0, 96, 32, 32); break;
				case 1:asu_.src = ML::Box2D(32, 96, 32, 32); break;
				case 2:asu_.src = ML::Box2D(64, 96, 32, 32); break;
				case 3:asu_.src = ML::Box2D(96, 96, 32, 32); break;
				}
				break;
			case kyuurisushi:													//�V�N����Ȃ����イ����i
				switch ((asu_.animCnt / 7) % 4) {
				case 0:asu_.src = ML::Box2D(0, 128, 32, 32); break;
				case 1:asu_.src = ML::Box2D(32, 128, 32, 32); break;
				case 2:asu_.src = ML::Box2D(64, 128, 32, 32); break;
				case 3:asu_.src = ML::Box2D(96, 128, 32, 32); break;
				}
				break;
			case teekasushi:													//�V�N����Ȃ��S�Ύ��i
				switch ((asu_.animCnt / 7) % 4) {
				case 0:asu_.src = ML::Box2D(0, 160, 32, 32); break;
				case 1:asu_.src = ML::Box2D(32, 160, 32, 32); break;
				case 2:asu_.src = ML::Box2D(64, 160, 32, 32); break;
				case 3:asu_.src = ML::Box2D(96, 160, 32, 32); break;
				}
				break;
			case amaebisushi:													//�V�N����Ȃ��ÊC�V���i
				switch ((asu_.animCnt / 7) % 4) {
				case 0:asu_.src = ML::Box2D(0, 192, 32, 32); break;
				case 1:asu_.src = ML::Box2D(32, 192, 32, 32); break;
				case 2:asu_.src = ML::Box2D(64, 192, 32, 32); break;
				case 3:asu_.src = ML::Box2D(96, 192, 32, 32); break;
				}
				break;
			case ebisushi:														//�V�N����Ȃ��C�V���i
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
	//���i�܂Ƃߍs��
	void allsushi_UpDate(Sushi& asu_, POINT p_)
	{
		//POINT pre = { asu_.x,asu_.y };	//�ړ��O
		asu_.moveCnt++;
		if (asu_.hitFlag == false) {
			if (asu_.active == true) {
				float speedCnt;					//�ړ����x
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

		//���i�Ƌq�̓����蔻��
		if (asu_.active == true) {
			ML::Box2D me = asu_.hitBase.OffsetCopy(float(asu_.x )* 32.0f,float(asu_.y)* 32.0f);
			//me.x = asu_.x;
			//me.y = asu_.y;
			//���i�͋q�Ɠ����蔻���������
			for (int c = 0; c < Maxvalue; ++c) {
				if (customer[c].active == true) {											//�q�͂���ꍇ
					//���i���g�̓����蔻��̎ʐ^��p�Ӂi���݈ʒu�j
					ML::Box2D you = customer[c].hitBase.OffsetCopy(customer[c].x * 32, customer[c].y * 32);
					//�ڐG����
					if (true == you.Hit(me) && lovefood[c].number == asu_.number){			//�����ƐڐG���Ă邩����ɔ��f���Ă��炤
						switch (asu_.motion){
						case Normal:
							customer[c].motion = Happy;
							if (customer[c].motion == Happy) {
								asu_.Cnt++;
								//�q���V�N�Ȏ��i��H�ׂ���A�΂�
								if (customer[c].motion == Happy && asu_.Cnt>35) {
									//�q�̓��X�A���A�菈��
									customer_Respawn(customer[c], asu_, lovefood[c], seiki[c],fukidashi[c]);
									asu_.active = false;		//���i��������
									player.shikin += neta[SUSHI].money;
									player.ninki++;			//�l�C+1
									asu_.Cnt = 0;
								}
							}
							break;
						case Dead:
							customer[c].motion = Dead;
							if (customer[c].motion == Dead) {
								asu_.Cnt++;
								//�q���V�N����Ȃ����i��H�ׂ���A����
								if (customer[c].motion == Dead && asu_.Cnt>45) {		//35
									//�q�̓��X�A���A�菈��
									customer_Respawn(customer[c], asu_, lovefood[c], seiki[c],fukidashi[c]);
									asu_.active = false;		//���i��������
									player.shikin -= neta[TAMAKO].money;
									player.ninki-=5;
									asu_.Cnt = 0;
								}
							}
							break;
						}

						////�q���x���҂�����΂����������ɂȂ�
						//if (customer[c].motion == Angry) {
						//	//�q�̓��X�A���A�菈��
						//	customer_Respawn(customer[c], asu_, lovefood[c], seiki[c]);
						//	player.shikin += (neta[SUSHI].money/2);
						//}

						seiki[c].isEmpty = true;
						break;
					}
					//���i�p������
					allsushi_dispose(asu_, p_);
				}
			}
		}
		//���i�A�j���[�V�����̍s��
		asu_.animCnt++;
	}
	//----------------------------------------------------------------
	//������������
	void makisu_Initialize(Sushi& c_, float x_, float y_)
	{
		c_.active = true;
		c_.x = x_;
		c_.y = y_;
		c_.Cnt = 0;
	}
	//----------------------------------------------------------------
	//�������s��
	void makisu_UpDate(Sushi& c_, POINT p_, Sushi& md_)
	{
		//neta[KURIKU].kaisu++;				//�N���b�N��
		int bangou;
		//�͈͓��`�F�b�N
		if (p_.x >= 7 && p_.x < 10 && p_.y >= 12 && p_.y <= 15) {
			for (int i = 12; i <= 14; ++i) {
				for (int j = 7; j <= 9; ++j) {
					bangou = md_.arr[i][j];
					switch (bangou) {
					case 0: neta[SYARI].kaisu++; break;				//�V����
					case 1:	neta[TAMAKO].kaisu++; break;			//���܂�
					case 2:	neta[SAMON].kaisu++; break;				//�T�[����
					case 3:	neta[NORI].kaisu++; break;				//�̂�
					case 4:	neta[MAKURO].kaisu++; break;			//�܂���
					case 5:	neta[IKURA].kaisu++; break;				//������
					case 6:	neta[KYUURI].kaisu++; break;			//���イ��
					case 7:	neta[AMAEBI].kaisu++; break;			//�ÊC�V
					case 8:	neta[EBI].kaisu++; break;				//�C�V
					}
					md_.arr[i][j] = 24;			//�N���b�N������A�󗓂ɂȂ�
				}
			}

			//���i�̍�������
			//�V����=2�A�܂���=2�Ȃ�A�}�N�����i�i9�ԁj
			if (neta[SYARI].kaisu == 2 && neta[MAKURO].kaisu == 2) {
				shisyou.active = false;
				allsushi[CheckDisabledSushi()].number = 9;
			}

			//�V����=2�A�T�[����=2�A�T�[�������i�i10�ԁj
			else if (neta[SYARI].kaisu == 2 && neta[SAMON].kaisu == 2) {
				shisyou.active = false;
				allsushi[CheckDisabledSushi()].number = 10;
			}
			//�V����=2�A���܂�=2�A�ʎq���i�i11�ԁj
			else if (neta[SYARI].kaisu == 2 && neta[TAMAKO].kaisu == 2) {
				shisyou.active = false;
				allsushi[CheckDisabledSushi()].number = 11;
			}
			//�V����=2�A������=2�A�̂�=1�Ȃ�A��������i�i12�ԁj
			else if (neta[SYARI].kaisu == 2 && neta[IKURA].kaisu == 2 && neta[NORI].kaisu == 1&& neta[KYUURI].kaisu==1) {
				allsushi[CheckDisabledSushi()].number = 12;
			}
			//�V����=4�A�̂�=1�A�ӉZ=1�Ȃ�A���イ����i�i13�ԁj
			else if (neta[SYARI].kaisu == 4&& neta[NORI].kaisu == 1 && neta[KYUURI].kaisu == 1) {
				allsushi[CheckDisabledSushi()].number = 13;
			}
			//�V����=4�A�̂�=1�A�}�N��=1�Ȃ�A�S�Ύ��i�i14�ԁj
			else if (neta[SYARI].kaisu == 4 && neta[NORI].kaisu == 1 && neta[MAKURO].kaisu == 1) {
				allsushi[CheckDisabledSushi()].number = 14;
			}
			//�V����=1�A�ÊC�V=1�Ȃ�A�ÊC�V���i�i15�ԁj
			else if (neta[SYARI].kaisu == 1&& neta[AMAEBI].kaisu == 1) {
				allsushi[CheckDisabledSushi()].number = 15;
			}
			//�V����=2�A�C�V=2�Ȃ�A�C�V���i�i16�ԁj
			else if (neta[SYARI].kaisu == 2 && neta[EBI].kaisu == 2) {
				allsushi[CheckDisabledSushi()].number = 16;
			}

			//���i���o��
			allsushi_Initialize(14,10);
			//���i���������A�e�l�[�^�[�v�Z0�ɂȂ�
			neta[SYARI].kaisu = 0, neta[SAMON].kaisu = 0, neta[TAMAKO].kaisu = 0;
			neta[MAKURO].kaisu = 0, neta[IKURA].kaisu = 0, neta[NORI].kaisu=0;
			neta[KYUURI].kaisu = 0, neta[AMAEBI].kaisu=0, neta[EBI].kaisu=0;

			//�N���b�N�񐔂�0�ɖ߂�
			neta[KURIKU].kaisu = 0;
		}
	}
	//----------------------------------------------------------------
	//�������\��
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
	//���i�P�ʂ̈ړ��`�F�b�N
	void Sushi_Check(Sushi& md_, POINT p_, Keisan& ku_)
	{
		//�}�b�v�z��O�̃f�[�^���N���b�N�����邱�Ƃ�h�~
		if (p_.x < 0) { return; }
		if (p_.y < 0) { return; }
		if (p_.x >= 30) { return; }
		if (p_.y >= 17) { return; }

		//�}�E�X�ɑ΂��鏈��
		int chip = md_.arr[p_.y][p_.x];
		//�V����=0�A���܂�=1�A�T�[����=2�A�̂�=3
		switch (chip) {
		//�V�������N���b�N������A�R�s�[����
		case 0:
			if (neta[SYARI].nokori >0) {			//�V�����c��>0�Ȃ�
				switch (ku_.kaisu){					//�N���b�N������
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
			//�ʎq���N���b�N������A�R�s�[����
		case 1:
			if (neta[TAMAKO].nokori >0) {			//�ʎq�c��>0�Ȃ�
				switch (ku_.kaisu) {				//�N���b�N������
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
			//�T�[�������N���b�N������A�R�s�[����
		case 2:
			if (neta[SAMON].nokori >0) {			//�T�[�����c��>0�Ȃ�
				switch (ku_.kaisu) {				//�N���b�N������
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
			//�C�ۂ��N���b�N������A�R�s�[����
		case 3:
			if (neta[NORI].nokori >0) {			//�C�ێc��>0�Ȃ�
				switch (ku_.kaisu) {				//�N���b�N������
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
			//�܂�����N���b�N������A�R�s�[����
		case 4:
			if (neta[MAKURO].nokori >0) {			//�܂���c��>0�Ȃ�
				switch (ku_.kaisu) {				//�N���b�N������
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
			//��������N���b�N������A�R�s�[����
		case 5:
			if (neta[IKURA].nokori >0) {			//������c��>0�Ȃ�
				switch (ku_.kaisu) {				//�N���b�N������
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
			//���イ����N���b�N������A�R�s�[����
		case 6:
			if (neta[KYUURI].nokori >0) {			//�ӉZ�c��>0�Ȃ�
				switch (ku_.kaisu) {				//�N���b�N������
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
			//���イ����N���b�N������A�R�s�[����
		case 7:
			if (neta[AMAEBI].nokori >0) {			//�ÊC�V�c��>0�Ȃ�
				switch (ku_.kaisu) {				//�N���b�N������
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
			//�C�V���N���b�N������A�R�s�[����
		case 8:
			if (neta[EBI].nokori >0) {				//�C�V�c��>0�Ȃ�
				switch (ku_.kaisu) {				//�N���b�N������
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
		//�N���b�N�񐔂��W��������
		if (ku_.kaisu > 8) { ku_.kaisu = -1; }
	}
	//----------------------------------------------------------------
	//���i����ǂݍ���
	bool Sushi_Load(Sushi& md_, int n_)
	{
		//�t�@�C���p�X�����
		string filePath = "./data/map/Map" + to_string(n_) + ".txt";

		//�t�@�C�����J���i�ǂݍ��݁j
		ifstream fin(filePath);
		if (!fin) { return false; }		//�ǂݍ��ݎ��s

		//�z��Ƀf�[�^����荞��
		for (int y = 0; y < 17; ++y) {
			for (int x = 0; x < 30; ++x) {
				fin >> md_.arr[y][x];
			}
		}
		//�t�@�C�������
		fin.close();
		return true;
	}
	//----------------------------------------------------------------
	//���i�\��
	void Sushi_Render(Sushi&md_)
	{
		for (int y = 0; y < 17; ++y){
			for (int x = 0; x < 30; ++x) {
				if (md_.arr[y][x] < NETA_MAXNUM && neta[md_.arr[y][x]].nokori >=0) {
					ML::Box2D  draw(0, 0, 32, 32);
					draw.Offset(x * 32, y * 32);		//�\���ʒu�𒲐�
					DG::Image_Draw("sushisozaiImg", draw, md_.chip[md_.arr[y][x]]);

					//�c������\��
					if (md_.arr[y][x] >= 0 && md_.arr[y][x] <= 9) {
						ML::Box2D textBox(-7, 20, 32, 32);		//�c����̈ʒu
						textBox.Offset(x * 32, y * 32);			//�\���ʒu�𒲐�
						string text = to_string(neta[md_.arr[y][x]].nokori);
						DG::Font_Draw("FontB", textBox, text, ML::Color(1,1,1,1));		//���F
					}
				}
			}
		}
	}
	//----------------------------------------------------------------
	//���i������
	void Sushi_Initialize(Sushi&md_)
	{
		//�f�[�^�̃R�s�[
		for (int y = 0; y < 17; ++y){
			for (int x = 0; x < 30; ++x){
				md_.arr[y][x];
			}
		}
		//���i�`�b�v���̏�����
		for (int c = 0; c < 24; ++c){
			int x = (c % 4);
			int y = (c / 4);
			md_.chip[c] = ML::Box2D(x * 32, y * 32, 32, 32);
		}
	}
	//----------------------------------------------------------------
	//��]���[���̏�����
	void kaitenren_Initialize(Seat& ka_, float x_, float y_)
	{
		ka_.x = x_;
		ka_.y = y_;
		ka_.moveCnt = 0;
	}
	//----------------------------------------------------------------
	//��]���[���̍s��
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
	//��]���[���̕\��
	void kaitenren_Render(Seat& ka_)
	{
		kaitenren_move(ka_);
		ML::Box2D draw = ka_.drawBase.OffsetCopy(float(ka_.x) * 32, float(ka_.y) * 32);
		DG::Image_Draw("kaitenrenImg", draw, ka_.src);
	}
	//----------------------------------------------------------------
	//���v������
	void toukei_Initialize()
	{
		//�}�E�X�ŃN���b�N��
		neta[KURIKU].kaisu = 0;
		//���ꂼ��̃l�^�c���
		neta[SYARI].nokori =10;
		neta[TAMAKO].nokori =10;
		neta[SAMON].nokori =10;
		neta[NORI].nokori = 10;
		neta[MAKURO].nokori = 10;
		neta[IKURA].nokori = 10;
		neta[KYUURI].nokori = 10;
		neta[AMAEBI].nokori = 10;
		neta[EBI].nokori = 10;
		//���i�̒l�i
		neta[SUSHI].money = 50;
		//�l�^�̒l�i
		neta[SYARI].money =50;
		neta[TAMAKO].money = 100;
	}
	//----------------------------------------------------------------
	//���i�̔ԍ��̃`�F�b�N
	int CheckDisabledSushi()
	{
		//*@brief		���ݔ�g�p���̂����̔ԍ��𒲂ׂ܂�
		//	*	@return		�g������i�̔ԍ�
		for (int i = 0; i < _countof(allsushi); ++i)
			if (!allsushi[i].active)
				return i;
		return -1;
	}
	//----------------------------------------------------------------
	//���i�p������
	void allsushi_dispose(Sushi& asu_, POINT p_)
	{
		//if (asu_.active == true) {
		//	ML::Box2D me = asu_.hitBase.OffsetCopy(float(asu_.x)* 32.0f, float(asu_.y)* 32.0f);
		//	//if (mouse.LB.on) {
		//		if (asu_.motion == Dead) {
		//			me.x = asu_.x;
		//			me.y = asu_.y;
		//			//POINT now = { asu_.x,asu_.y };		//�ړ���
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
		//			//�ǂ�����Ď��i�����̈ʒu�ɖ߂点��́H�H�H�H
		//			//else
		//			//{
		//			//	asu_.hitFlag = false;
		//			//}
		//		}
		//	//}
		//}
	}
	//----------------------------------------------------------------
	//���j���[�̏�����
	void menu_Initialize(Seat& s_, float x_, float y_)
	{
		s_.active = true;
		s_.x = x_;
		s_.y = y_;
	}
	//----------------------------------------------------------------
	//���j���[�̕\��
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
	//���j���[�̃J�E���g
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
			//����{�^��
			if ((p_.x >= 24 && p_.x <= 25 && p_.y >= 5 && p_.y <= 6)) {
				if (mouse.LB.down) {
					s_.active = true;
				}
			}
		}
	}
	//----------------------------------------------------------------
	//�d�b������
	void phone_Initialize(Seat& ph_, float x_, float y_)
	{
		ph_.active = true;
		ph_.x = x_;
		ph_.y = y_;
	}
	//----------------------------------------------------------------
	//�d�b�̃J�E���g
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
			//�V������[
			if ((p_.x >= 6 && p_.x <= 7 && p_.y >= 6 && p_.y <= 7)) {
				if (mouse.LB.down){
					toragu.active = true;
					neta[SYARI].nokori = 10;
					player.shikin -= neta[SYARI].money;
				}
			}
			//�ʎq��[
			if ((p_.x >=9 && p_.x <=10 && p_.y >= 6 && p_.y <= 7)) {
				if (mouse.LB.down) {
					toragu.active = true;
					neta[TAMAKO].nokori = 10;
					player.shikin -= neta[TAMAKO].money;
				}
			}
			//�T�[������[
			if ((p_.x >= 12 && p_.x <= 13 && p_.y >= 6 && p_.y <= 7)) {
				if (mouse.LB.down) {
					toragu.active = true;
					neta[SAMON].nokori = 10;
					player.shikin -= neta[TAMAKO].money;
				}
			}
			//�̂��[
			if ((p_.x >= 15 && p_.x <= 16 && p_.y >= 6 && p_.y <= 7)) {
				if (mouse.LB.down) {
					toragu.active = true;
					neta[NORI].nokori = 10;
					player.shikin -= neta[TAMAKO].money;
				}
			}
			//�}�N����[
			if ((p_.x >= 18 && p_.x <= 19 && p_.y >= 6 && p_.y <= 7)) {
				if (mouse.LB.down) {
					toragu.active = true;
					neta[MAKURO].nokori = 10;
					player.shikin -= neta[TAMAKO].money;
				}
			}
			//�������[
			if ((p_.x >= 21 && p_.x <= 22 && p_.y >= 6 && p_.y <= 7)) {
				if (mouse.LB.down) {
					toragu.active = true;
					neta[IKURA].nokori = 10;
					player.shikin -= neta[TAMAKO].money;
				}
			}
			//���イ���[
			if ((p_.x >= 6 && p_.x <= 7 && p_.y >= 9 && p_.y <= 10)) {
				if (mouse.LB.down) {
					toragu.active = true;
					neta[KYUURI].nokori = 10;
					player.shikin -= neta[TAMAKO].money;
				}
			}
			//�ÊC�V��[
			if ((p_.x >= 9 && p_.x <= 10 && p_.y >= 9 && p_.y <= 10)) {
				if (mouse.LB.down) {
					toragu.active = true;
					neta[AMAEBI].nokori = 10;
					player.shikin -= neta[TAMAKO].money;
				}
			}
			//�C�V��[
			if ((p_.x >= 12 && p_.x <= 13 && p_.y >= 9 && p_.y <= 10)) {
				if (mouse.LB.down) {
					toragu.active = true;
					neta[EBI].nokori = 10;
					player.shikin -= neta[TAMAKO].money;
				}
			}
			//����{�^��
			if ((p_.x >= 24 && p_.x <= 25 && p_.y >= 5 && p_.y <= 6)) {
				if (mouse.LB.down) {
					ph_.active = true;
				}
			}
		}
	}
	//----------------------------------------------------------------
	//�d�b�\��
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
			draw.Offset(ph_.x*3, ph_.y*9);			//�d�b�����̈ʒu
			ML::Box2D src(0, 0, 1000, 400);
			DG::Image_Draw("phone_messageImg", draw, src);
		}
	}
	//----------------------------------------------------------------
	//�w�i�̏�����
	void haikei_Initialize(Player& h_, float x_, float y_)
	{
		h_.x = x_;
		h_.y = y_;
	}
	//----------------------------------------------------------------
	//�w�i�̕\��
	void haikei_Render(Player& h_)
	{
		ML::Box2D draw(0, 0, 960,544);
		h_.hitBase = draw;
		draw.Offset(h_.x, h_.y);
		ML::Box2D src(0, 0,960, 544);
		DG::Image_Draw("haikeiImg", draw, src);
	}
	//----------------------------------------------------------------
	//�^���̎ԏ�����
	void toragu_Initialize(Seat& trg_, float x_, float y_)
	{
		trg_.active = false;
		trg_.x = x_;
		trg_.y = y_;
		trg_.animCnt = 0;
		trg_.kaunto = 0;
	}
	//----------------------------------------------------------------
	//�^���̎Ԃ̕\��
	void toragu_Render(Seat& trg_)
	{
		if (trg_.active == true) {
			toragu_Anim(trg_);
			ML::Box2D draw = trg_.drawBase.OffsetCopy(float(trg_.x) * 32, float(trg_.y) * 32);
			DG::Image_Draw("toraguImg", draw, trg_.src);
		}
	}
	//----------------------------------------------------------------
	//�^���̎ԃA�j���[�V����
	void toragu_Anim(Seat& trg_)
	{
		trg_.animCnt++;
		trg_.kaunto++;
		trg_.drawBase = ML::Box2D(0,0,960, 544);
		switch ((trg_.animCnt /6) % 2) {
		case 0:trg_.src = ML::Box2D(0,0,960,544); break;
		case 1:trg_.src = ML::Box2D(960,0,960,544); break;
		}
		//60�t���[���ɂȂ�����A�^���̎Ԃ�������
		if (trg_.kaunto == 60) {
			trg_.active = false;
			trg_.kaunto = 0;
		}
	}
	//----------------------------------------------------------------
	//�c�ƊJ�n�̏�����
	void eigyoukaishi_Initialize(Player& e_)
	{
		//�c�ƊJ�n�C�[�W���O(�L���A�ړ��̎�ށA�J�n�l�A�I���l�A�ړ����鎞��)
		easing::Set("BACKOUT", easing::EASINGTYPE::BACKIN ,0, 1000, 150);			//BACKOUT
		easing::Start("BACKOUT");
		e_.active = true;
	}
	//----------------------------------------------------------------
	//�c�ƊJ�n�̃J�E���g
	void eigyoukaishi_UpDate(Player& e_)
	{
		//�C�[�W���O�̎�ނɂ���Ĉړ�����
		e_.x = easing::GetPos("BACKOUT");
		if (e_.x > 999) {
			e_.active = false;
		}
	}
	//----------------------------------------------------------------
	//�c�ƊJ�n�̕\��
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
