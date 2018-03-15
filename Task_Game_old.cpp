#include "MyPG.h"
#include "MyGameMain.h"
#include <time.h>

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
		bool active;				//�݌ɔ��f
		ML::Box2D hitBase;			//�����蔻��p
		bool hitFlag;				//�����蔻��
		Motion motion;
		ML::Box2D drawBase;
		ML::Box2D src;
		int number;					//�ԍ�
		int moveCnt;
		int Cnt;
	};

	//�Ȃ̍\����
	struct Seat {
		ML::Vec2 pos;
		bool isEmpty;				//�Ȃ̔��f�p
	};

	const static int Maxvalue = 6;			//�ȁA�q�̐����T����
	int emptySeat = Maxvalue;				//��ȏ󋵁��U
	//int newcustomer = 0;

	//�Ȃ̐ݒ�ϐ�
	Seat seiki[Maxvalue]{
		{ ML::Vec2(17,3),true },
		{ ML::Vec2(13,3),true },
		{ ML::Vec2(9,3),true },
		{ ML::Vec2(5,3),true },
		{ ML::Vec2(21,3),true },
		{ ML::Vec2(24, 7),true }
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
	};

	//�l�^�A���i�ϐ�
	Sushi sushi;				//�l�^
	Sushi allsushi[Maxvalue];	//���i�̂܂Ƃ�			
	Sushi makisu;				//������
	Sushi kaiten[2];			//��]

	//�q�ϐ�
	Chara customer[Maxvalue];
	Chara lovefood[Maxvalue];

	//�v�Z�ϐ�
	Keisan kuriku;		//�N���b�N�v�Z�p
	Keisan syari;		//�V�����v�Z�p
	Keisan tamako;		//�ʎq�v�Z�p
	Keisan samon;		//�T�[�}���v�Z�p
	Keisan makuro;		//�܂���v�Z�p
	Keisan nori;		//�̂�v�Z�p
	Keisan ikura;		//������v�Z�p
	Keisan kyuuri;		//���イ��v�Z�p


	bool clearFlag;		//�N���A
	bool timeOver;		//�Q�[���I�[�o�[
	int timeCnt;
	int timeLeft;
	int jikan;


	//------------------------------------------------------------------------------
	//�֐��̃v���g�^�C�v�錾
	//------------------------------------------------------------------------------
	//���v�������̃v���g�^�C�v�錾
	void toukei_Initialize();

	//�q���o��������֐��̃v���g�^�C�v�錾
	void customer_Appear();
	//�q�̏������̃v���g�^�C�v�錾
	void customer_Initialize(Chara& c_,Seat& s_);
	//�q�̕\���̃v���g�^�C�v�錾
	void customer_Render(Chara& c_);
	//�q�̍s���̃v���g�^�C�v�錾
	void customer_UpDate(Chara& c_, Seat & set_, Chara& lf_, Sushi & asu_);

	void customer_Respawn(Chara& c_, Sushi& s_, Chara& lf_, Seat& se_);

	//�qA�������̃v���g�^�C�v�錾
	void customerA_Initialize(Chara& c_, float x_, float y_);
	//�qA�̕\���̃v���g�^�C�v�錾
	void customerA_Render(Chara& c_);
	//�qA�A�j���[�V�����̃v���g�^�C�v�錾
	void customerA_Anim(Chara&  c_);
	//�qA�̓���v���g�^�C�v�錾
	void customerA_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi & asu_);

	//�qB�������̃v���g�^�C�v�錾
	void customerB_Initialize(Chara& c_, float x_, float y_);
	//�qB�̕\���̃v���g�^�C�v�錾
	void customerB_Render(Chara& c_);
	//�qB�A�j���[�V�����̃v���g�^�C�v�錾
	void customerB_Anim(Chara&  c_);
	//�qB�̓���v���g�^�C�v�錾
	void customerB_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi & asu_);


	//�qC�������̃v���g�^�C�v�錾
	void customerC_Initialize(Chara& c_, float x_, float y_);
	//�qC�̕\���̃v���g�^�C�v�錾
	void customerC_Render(Chara& c_);
	//�qC�A�j���[�V�����̃v���g�^�C�v�錾
	void customerC_Anim(Chara&  c_);
	//�qC�̓���v���g�^�C�v�錾
	void customerC_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi& asu_);

	//�qD�������̃v���g�^�C�v�錾
	void customerD_Initialize(Chara& c_, float x_, float y_);
	//�qD�̕\���̃v���g�^�C�v�錾
	void customerD_Render(Chara& c_);
	//�qD�A�j���[�V�����̃v���g�^�C�v�錾
	void customerD_Anim(Chara&  c_);
	//�qD�̓���v���g�^�C�v�錾
	void customerD_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi & asu_);




	//�q�̍D�ݎ��i�̏������̃v���g�^�C�v�錾
	void customerlovefood_Initialize(Chara& lf_, float x_, float y_);
	//�q�̍D�ݎ��i�̕\���̃v���g�^�C�v�錾
	void customerlovefood_Render(Chara& lf_);
	//�q�̍D�ݎ��i�`�F�b�N�v���g�^�C�v�錾
	void customerlovefood_Anim(Chara& lf_);




	//���i�܂Ƃ߃`�F�b�N�̃v���g�^�C�v�錾
	void allsushi_Anim(Sushi& asu_);
	//���i�܂Ƃ߂̏������̃v���g�^�C�v�錾
	void allsushi_Initialize(float x_, float y_);
	//���i�܂Ƃ߂̕\���̃v���g�^�C�v�錾
	void allsushi_Render(Sushi& asu_);
	//���i�܂Ƃ߂̍s���̃v���g�^�C�v�錾
	void allsushi_UpDate(Sushi& asu_/*,Chara& c_,Chara& lv_*/);




	//��]�̃��[���������̃v���g�^�C�v�錾
	void kaiten_Initialize(Sushi& k_, float x_, float y_);
	//��]�̃��[���̍s���v���g�^�C�v�錾
	void kaiten_move(Sushi& k_);
	//��]�̃��[���\���̃v���g�^�C�v�錾
	void kaiten_Render(Sushi& k_);


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
		DG::Image_Create("makurosushiImg", "./data/image/sushi_makurosushi.png");		//�}�N�����i
		DG::Image_Create("tamakosushiImg", "./data/image/sushi_tamakosushi.png");		//���܂����i
		DG::Image_Create("samonsushiImg", "./data/image/sushi_samonsushi.png");			//�T�[�������i
		DG::Image_Create("kyuuriImg", "./data/image/sushi_kyuuri.png");					//���イ��
		DG::Image_Create("makisuImg", "./data/image/sushi_makisu.png");					//������
		DG::Image_Create("makisu01Img", "./data/image/sushi_makisu01.png");				//������
		DG::Image_Create("kaitenImg", "./data/image/kaiten.png");						//��]
		DG::Image_Create("customerImg", "./data/image/customer.png");					//�q
		DG::Image_Create("ClearImg", "./data/image/Clear.bmp");			//�N���A
		DG::Image_Create("GameoverImg", "./data/image/Gameover.bmp");			//�N���A
		DG::Font_Create("FontA", "MS �S�V�b�N", 12, 24);					//�����\���p�̃t�H���g���쐬

		//PG���s���̎����𗘗p���Ė���Ⴄ�����\��I������
		srand((unsigned int)time(NULL));

		//��]�̏�����
		for (int k = 0; k < 2; k++) {
			kaiten_Initialize(kaiten[k],-4, 6);
		}

		//������������
		makisu_Initialize(makisu, 7, 13);

		//���v�̏�����
		toukei_Initialize();

		//���i�̂܂Ƃߏ�����
		for (int i = 0; i < Maxvalue; ++i) {
			allsushi[i].active = false;
		}

		//�q���o��������
		customer_Appear();

		//�q�̍D�ݎ��i�̏�����
		for (int i = 0; i < Maxvalue; ++i) {
			customerlovefood_Initialize(lovefood[i], customer[i].x +1, customer[i].y-1);
		}

		//�N���A�̏�����
		clearFlag = false;
		//�Q�[���I�[�o�[�̏�����
		bool timeOver = false;
		int timeCnt = 0;
		timeLeft = 60;
		jikan = 0;

		//���i�z����̏�����
		Sushi_Initialize(sushi);
		//���i����ǂݍ���
		Sushi_Load(sushi, 1);
	}
	//-----------------------------------------------------------------------------
	//�������
	//�@�\�T�v�F�v���O�����I�����ɂP����s�����i�f�ނȂǂ̉�����s���j
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
	//���s����
	//�@�\�T�v�F�Q�[���̂P�t���[���ɓ����鏈��
	//-----------------------------------------------------------------------------
	TaskFlag UpDate()
	{
		//�}�E�X�J�[�\���̈ʒu�𓾂�
		mouse = DI::Mouse_GetState();
		POINT pos;
		pos.x = mouse.cursorPos.x / 32;
		pos.y = mouse.cursorPos.y / 32;


		if (mouse.LB.down) {
			//�͈͓��ŃN���b�N������N���b�N�񐔂������Ă���
			if (pos.x > 1 && pos.x < 5 && pos.y>11 && pos.y < 15){
				//�}�b�v�`�F�b�N
				Sushi_Check(sushi, pos, kuriku);
				kuriku.kaisu++;				//�N���b�N��
			}
			//�������s��
			makisu_UpDate(makisu, pos, sushi);
		}



		//���i�܂Ƃߍs��
		for (int i = 0; i < Maxvalue; ++i) {
			allsushi_UpDate(allsushi[i]);
		}

		//�q�̍s��
		for (int i = 0; i < Maxvalue; ++i) {
			customer_UpDate(customer[i], seiki[i],lovefood[i],allsushi[i]);
		}

		//��]���[���̍s��
		for (int k = 0; k < 2; k++) {
			kaiten_move(kaiten[k]);
		}


		//�N���A���ĂȂ��Ƃ��̂݁A�v���C���𓮂���
		//if (clearFlag == false&& (timeLeft - time) > 0)
		//{
		//	//player�̍s��
		//	player_UpDate(player);
		//}

		//�������ԃJ�E���g
		timeCnt++;
		if (timeCnt % 60 == 0 && clearFlag == false && timeOver == false) {
			jikan++;
		}
		if ((timeLeft - jikan) <= 0) {
			timeOver = true;
		}

		TaskFlag rtv = Task_Game;//��肠�������݂̃^�X�N���w��
		if (true == in1.ST.down) {
			rtv = Task_Title;	//���̃^�X�N���^�C�g����
		}
		return rtv;
	}
	//-----------------------------------------------------------------------------
	//�`�揈��
	//�@�\�T�v�F�Q�[���̂P�t���[���ɓ�����\������
	//-----------------------------------------------------------------------------
	void Render()
	{
		//�������\��
		makisu_Render(makisu);

		//�l�^�̕\��
		Sushi_Render(sushi);


		//��]�̕\��
		for (int k = 0; k < 2; k++) {
			kaiten_Render(kaiten[k]);
		}


		//�q�̕\��
		for (int i = 0; i < Maxvalue; ++i) {
			customer_Render(customer[i]);
		}

		//�q�̍D�ݎ��i�̕\��
		for (int i = 0; i < Maxvalue; ++i) {
			customerlovefood_Render(lovefood[i]);
		}

		//���i�̂܂Ƃߕ\��
		for (int i = 0; i < Maxvalue; ++i) {
			allsushi_Render(allsushi[i]);
		}

		//�N���A�\��
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

		//��������
		ML::Box2D textBox(0, 0, 480, 32);
		string text = "�������ԁF" + to_string(timeLeft - jikan) + "�b";
		DG::Font_Draw("FontA", textBox, text, ML::Color(1, 1, 1, 1));
	}
	//----------------------------------------------------------------
	//�֐�
	//----------------------------------------------------------------
	//�q�̏�����
	void customer_Initialize(Chara& c_,Seat& s_)
	{	//�q�̐Ȃ̏���
		if (s_.isEmpty==true) {
				s_.isEmpty = false;
				c_.typeNum = rand() % 4;					//�q�̃^�C�v�𗐐��Ō��܂�
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
	//�q�̍s��
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
		//�q�A�j���[�V�����̍s��
		c_.animCnt++;
	}
	//----------------------------------------------------------------
	//�q�̓��X�A���A�菈��
	void customer_Respawn(Chara & c_, Sushi & asu_, Chara& lf_, Seat & set_)
	{
		set_.isEmpty = false;
		c_.active = false;
		asu_.active = false;				//���i��������
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
		customerlovefood_Initialize(lf_, c_.x+1,c_.y-1);

		//c_.active = false;			//�q������v
		//s_.active = false;				//���i��������
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
			switch ((c_.animCnt / 10) % 10)
			{
			case 0:c_.src = ML::Box2D(0, 0, 96, 96); break;
			case 1:c_.src = ML::Box2D(96, 0, 96, 96); break;
			case 2:c_.src = ML::Box2D(0, 0, 96, 96); break;
			case 3:c_.src = ML::Box2D(192, 0, 96, 96); break;
			}
			break;
		case Dead:
			//��
			//c_.drawBase = ML::Box2D(-32, -8, 64, 32);
			//c_.src = ML::Box2D(192, 96, 64, 32);
			break;
		case Happy:
			//��
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
	//�qA�̓���
	void customerA_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi& asu_)
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
		//if (c_.kaunto > 1100) {
		//	for (int c = 0; c < Maxvalue; ++c) {
		//		if (seiki[c].isEmpty == true) {
		//			//�q�̓��X�A���A�菈��
		//			customer_Respawn(c_, allsushi[c], lf_, set_);
		//		}
		//		seiki[c].isEmpty = true;
		//		break;
		//	}
		//}
	}
	//----------------------------------------------------------------
	//�qB�̓���
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
		//			//�q�̓��X�A���A�菈��
		//			customer_Respawn(c_, allsushi[c], lf_, set_);
		//		}
		//		seiki[c].isEmpty = true;
		//		break;
		//	}
		//}
		//�q�J�E���g
		c_.kaunto++;
	}
	//----------------------------------------------------------------
	//�qC�̓���
	void customerC_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi& asu_)
	{
		//�q�J�E���g
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
		//			//�q�̓��X�A���A�菈��
		//			customer_Respawn(c_, allsushi[c], lf_, set_);
		//		}
		//		seiki[c].isEmpty = true;
		//		break;
		//	}
		//}
	}
	//----------------------------------------------------------------
	//�qD�̓���
	void customerD_UpDate(Chara&  c_, Seat & set_, Chara& lf_, Sushi& asu_)
	{
		//�q�J�E���g
		c_.kaunto++;
		if (c_.kaunto == 100){
			c_.active = true;
		}
		//�q���҂��Ԃ͒x�߂���Ɠ{��
		if (c_.kaunto>1000){
			c_.motion = Angry;
		}
		if (c_.kaunto > 1500) {
			//for (int c = 0; c < Maxvalue; ++c) {
				if (set_.isEmpty == true) {
					//�q�̓��X�A���A�菈��
					customer_Respawn(c_, asu_, lf_, set_);
				}
				set_.isEmpty = true;
				//break;
			//}
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
			switch ((c_.animCnt / 10) % 10){
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
			//��
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
			switch ((c_.animCnt / 10) % 10)
			{
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
			//��
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
	//�q�̍D�ݎ��i�̏�����
	void customerlovefood_Initialize(Chara& lf_, float x_, float y_)
	{
		lf_.x = x_;
		lf_.y = y_;
		lf_.active = true;
		lf_.number = 9 + rand() % 5;
	}
	//----------------------------------------------------------------
	//�q�̍D�ݎ��i�̕\��
	void customerlovefood_Render(Chara& lf_)
	{
		if (lf_.active == true) {
			customerlovefood_Anim(lf_);
			ML::Box2D draw = lf_.drawBase.OffsetCopy(float(lf_.x) * 32, float(lf_.y) * 32);
			DG::Image_Draw("sushisozaiImg", draw, lf_.src);
		}
	}
	//----------------------------------------------------------------
	//�q�̍D�ݎ��i�`�F�b�N
	void customerlovefood_Anim(Chara& lf_)
	{
		lf_.drawBase = ML::Box2D(0, -12, 32, 32);
		switch (lf_.number) {
		case 9:lf_.src = ML::Box2D(32, 64, 32, 32);  break;			//�}�N�����i
		case 10:lf_.src = ML::Box2D(64, 64, 32, 32); break;			//�T�[�������i
		case 11:lf_.src = ML::Box2D(96, 64, 32, 32); break;			//�ʎq���i
		case 12:lf_.src = ML::Box2D(0, 96, 32, 32); break;			//��������i
		case 13:lf_.src = ML::Box2D(32, 96, 32, 32); break;			//���イ����i
		//case 13:lf_.src = ML::Box2D(32, 32, 32, 32); break;
		//case 14:lf_.src = ML::Box2D(160, 0, 32, 32); break;
		}
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
		//enum {
		//	makurosushi = 9, samonsushi = 10,tamakosushi = 11,
		//};
		switch (asu_.motion) {
		case Normal:
			asu_.drawBase = ML::Box2D(0, -12, 32, 32);
			switch (asu_.number){
			case 9:asu_.src = ML::Box2D(32, 64, 32, 32); break;			//�}�N�����i
			case 10:asu_.src = ML::Box2D(64, 64, 32, 32); break;		//�T�[�������i
			case 11:asu_.src = ML::Box2D(96, 64, 32, 32); break;		//�ʎq���i
			case 12:asu_.src = ML::Box2D(0, 96, 32, 32); break;			//��������i
			case 13:asu_.src = ML::Box2D(32, 96, 32, 32); break;		//���イ����i
			case 14:asu_.src = ML::Box2D(160, 0, 32, 32); break;
			case 15:asu_.src = ML::Box2D(160, 0, 32, 32); break;
			}
			break;

		case Dead:
			asu_.drawBase = ML::Box2D(0, -12, 32, 32);
			switch (asu_.number) {
			case 9:														//�}�N�����i
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
	//���i�܂Ƃߍs��
	void allsushi_UpDate(Sushi& asu_)
	{
		asu_.moveCnt++;
		if (asu_.active == true) {
			float speedCnt;					//�ړ����x
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
		//���i�Ƌq�̓����蔻��
		if (asu_.active == true) {
			ML::Box2D me = asu_.hitBase.OffsetCopy(asu_.x * 32, asu_.y * 32);
			//���i�͋q�Ɠ����蔻���������
			for (int c = 0; c < Maxvalue; ++c) {
				if (customer[c].active == true) {											//�q����ꍇ
					//���i���g�̓����蔻��̎ʐ^��p�Ӂi���݈ʒu�j
					ML::Box2D you = customer[c].hitBase.OffsetCopy(customer[c].x * 32, customer[c].y * 32);
					//�ڐG����

					if (true == you.Hit(me) && lovefood[c].number == asu_.number){			//�����ƐڐG���Ă邩����ɔ��f���Ă��炤
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
						//�q���V�N�Ȏ��i��H�ׂ���A�΂�
						if (customer[c].motion ==Happy&&asu_.Cnt>35) {
							//�q�̓��X�A���A�菈��
							customer_Respawn(customer[c], asu_, lovefood[c], seiki[c]);
							asu_.Cnt=0;
						}
						//�q���V�N����Ȃ����i��H�ׂ���A����
						if(customer[c].motion == Dead && asu_.Cnt>35){
							//�q�̓��X�A���A�菈��
							customer_Respawn(customer[c], asu_, lovefood[c], seiki[c]);
							asu_.Cnt = 0;
						}
						//�q�̓��X�A���A�菈��
						//customer_Respawn(customer[c], asu_, lovefood[c], seiki[c]);
						seiki[c].isEmpty = true;
						break;
					}
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
	}
	//----------------------------------------------------------------
	//�������s��
	void makisu_UpDate(Sushi& c_, POINT p_, Sushi& md_)
	{
		int bangou;
		//�͈͓��`�F�b�N
		if (p_.x >= 7 && p_.x < 10 && p_.y >= 12 && p_.y <= 15) {
			for (int i = 12; i <= 14; ++i) {
				for (int j = 7; j <= 9; ++j) {
					bangou = md_.arr[i][j];
					switch (bangou) {
					case 0:					//�V����
						syari.kaisu++;
						break;
					case 1:					//���܂�
						tamako.kaisu++;
						break;
					case 2:
						samon.kaisu++;		//�T�[����
						break;
					case 3:
						nori.kaisu++;		//�̂�
						break;
					case 4:
						makuro.kaisu++;		//�܂���
						break;
					case 5:
						ikura.kaisu++;		//������
						break;
					case 6:
						kyuuri.kaisu++;		//���イ��
						break;
					}
					md_.arr[i][j] = 24;			//�N���b�N������A�󗓂ɂȂ�
				}
			}
			//�V����=2�A���܂�=1�A�ʎq���i�i11�ԁj
			if (syari.kaisu == 2 && tamako.kaisu == 1) {
				allsushi[CheckDisabledSushi()].number = 11;
				allsushi_Initialize(3,6);
			}
			//�V����=2�A�T�[����=1�A�T�[�������i�i10�ԁj
			if (syari.kaisu == 2 && samon.kaisu == 1) {
				allsushi[CheckDisabledSushi()].number = 10;
				allsushi_Initialize(3, 6);
			}
			//�V����=2�A�܂���=1�Ȃ�A�}�N�����i�i9�ԁj
			if (syari.kaisu == 2 && makuro.kaisu == 1) {
				allsushi[CheckDisabledSushi()].number = 9;
				allsushi_Initialize(3, 6);
			}
			//�V����=2�A������=1�A�̂�=1�Ȃ�A��������i�i12�ԁj
			if (syari.kaisu == 2 && ikura.kaisu == 1 && nori.kaisu == 1&&kyuuri.kaisu==1) {
				allsushi[CheckDisabledSushi()].number = 12;
				allsushi_Initialize(3, 6);
			}
			//�V����=3�A�̂�=1�Ȃ�A�̂�=1�Ȃ�A���イ����i�i13�ԁj
			if (syari.kaisu == 3&& nori.kaisu == 1 && kyuuri.kaisu == 1) {
				allsushi[CheckDisabledSushi()].number = 13;
				allsushi_Initialize(3, 6);
			}

			//���i���������A�e�l�[�^�[�v�Z0�ɂȂ�
			syari.kaisu = 0, samon.kaisu = 0, tamako.kaisu = 0;
			makuro.kaisu = 0, ikura.kaisu = 0, nori.kaisu=0;
			kyuuri.kaisu = 0;
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
		else {
			ML::Box2D draw(-14, -46, 128, 128);
			c_.hitBase = draw;
			draw.Offset(c_.x * 32, c_.y * 32);
			ML::Box2D src(0, 0, 718, 759);
			DG::Image_Draw("makisu01Img", draw, src);
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
			//�ʎq���N���b�N������A�R�s�[����
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
			//�T�[�������N���b�N������A�R�s�[����
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
			//�C�ۂ��N���b�N������A�R�s�[����
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
			//�܂�����N���b�N������A�R�s�[����
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
			//��������N���b�N������A�R�s�[����
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
			//���イ����N���b�N������A�R�s�[����
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
				ML::Box2D  draw(0, 0, 32, 32);
				draw.Offset(x * 32, y * 32);		//�\���ʒu�𒲐�
				DG::Image_Draw("sushisozaiImg", draw, md_.chip[md_.arr[y][x]]);
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
			//ML::Box2D draw = md_.chip[c].OffsetCopy(md_.x, md_.y);
		}
	}
	//----------------------------------------------------------------
	//��]���[���̏�����
	void kaiten_Initialize(Sushi& k_, float x_, float y_)
	{
		k_.x = x_;
		k_.y = y_;
		k_.moveCnt = 0;
	}
	//----------------------------------------------------------------
	//��]���[���̍s��
	void kaiten_move(Sushi& k_)
	{
		k_.moveCnt++;
		if (k_.moveCnt == 100) {
			k_.moveCnt = 0;
		}
	}
	//----------------------------------------------------------------
	//��]���[���̕\��
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
	//���v������
	void toukei_Initialize()
	{
		//�}�E�X�ŃN���b�N��
		kuriku.kaisu = 0;
		//�V�����v�Z�p
		syari.kaisu = 0;
		//�ʎq�v�Z�p
		tamako.kaisu = 0;
		//�T�[�}���v�Z�p
		samon.kaisu = 0;
		//�܂���v�Z�p
		makuro.kaisu = 0;
		//�̂�v�Z�p
		nori.kaisu = 0;
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
}
