#include "MyPG.h"
#include "MyGameMain.h"
#include "easing.h"

//�^�C�g�����
namespace Title
{
	//�Q�[�����
	DI::Mouse mouse;

	//Alldata�\����
	struct Alldata {
		float x;
		float y;
		int animCnt;				//�A�j���[�V����
		ML::Box2D drawBase;
		ML::Box2D src;
		ML::Box2D hitBase;			//�����蔻��p
		bool active;				//�������f
	};

	//Alldata�ϐ�
	Alldata title;					//�^�C�g��
	Alldata haikei;					//�w�i
	Alldata shisyou;				//�t��
	Alldata playbutton;				//�Q�[���J�n�{�^��

	//------------------------------------------------------------------------------
	//�֐��̃v���g�^�C�v�錾
	//------------------------------------------------------------------------------
	//�^�C�g���̏������̃v���g�^�C�v�錾
	void title_Initialize(Alldata& a_);
	//�^�C�g���̃J�E���g�̃v���g�^�C�v�錾
	void title_UpDate(Alldata& a_);
	//�^�C�g���̕\���̃v���g�^�C�v�錾
	void title_Render(Alldata& a_);
	//�w�i�̕\���̃v���g�^�C�v�錾
	void haikei_Render(Alldata& a_);
	//�t���̏������̃v���g�^�C�v�錾
	void shisyou_Initialize(Alldata& a_, float x_, float y_);
	//�t���̕\���̃v���g�^�C�v�錾
	void shisyou_Render(Alldata& a_);
	//�t���̃A�j���[�V�����v���g�^�C�v�錾
	void shisyou_Anim(Alldata& a_);
	//�Q�[���J�n�{�^���̏������̃v���g�^�C�v�錾
	void playbutton_Initialize(Alldata& a_, float x_, float y_);
	//�Q�[���J�n�{�^���̃J�E���g�̃v���g�^�C�v�錾
	void playbutton_UpDate(Alldata& a_, POINT p_);
	//�Q�[���J�n�{�^���̕\���̃v���g�^�C�v�錾
	void playbutton_Render(Alldata& a_);

	//-----------------------------------------------------------------------------
	//����������
	//�@�\�T�v�F�v���O�����N�����ɂP����s�����i�f�ނȂǂ̏������s���j
	//-----------------------------------------------------------------------------
	void Initialize()
	{
		DG::Image_Create("TitleImg", "./data/image/Title.png");
		DG::Image_Create("Title_haikeiImg", "./data/image/Title_haikei.png");
		DG::Image_Create("shisyouImg", "./data/image/shisyou.png");						//�t��
		DG::Image_Create("kaishibotanImg", "./data/image/kaishibotan.png");				//�Q�[���J�n�{�^��

		//�^�C�g���̏�����
		title_Initialize(title);
		//�t���̏�����
		shisyou_Initialize(shisyou, 0, 8);
		//�Q�[���J�n�{�^���̏�����
		playbutton_Initialize(playbutton,10,10);
	}
	//-----------------------------------------------------------------------------
	//�������
	//�@�\�T�v�F�v���O�����I�����ɂP����s�����i�f�ނȂǂ̉�����s���j
	//-----------------------------------------------------------------------------
	void Finalize()
	{
		DG::Image_Erase("TitleImg");
		DG::Image_Erase("Title_haikeiImg");
		DG::Image_Erase("shisyouImg");
		DG::Image_Erase("kaishibotanImg");
	}
	//-----------------------------------------------------------------------------
	//�X�V����
	//�@�\�T�v�F�Q�[���̂P�t���[���ɓ����鏈��
	//-----------------------------------------------------------------------------
	TaskFlag UpDate()
	{
		//�}�E�X�J�[�\���̈ʒu�𓾂�
		mouse = DI::Mouse_GetState();
		POINT pos;
		pos.x = mouse.cursorPos.x;
		pos.y = mouse.cursorPos.y;

		DI::VGamePad  in1 = DI::GPad_GetState("P1");

		//�^�C�g���̃J�E���g
		title_UpDate(title);

		//�Q�[���J�n�{�^���̃J�E���g
		playbutton_UpDate(playbutton,pos);

		TaskFlag rtv = Task_Title;//��肠�������݂̃^�X�N���w��
		if (mouse.LB.down&&playbutton.active==false) {
			rtv = Task_Gamesetsumei;	//���̃^�X�N���Q�[��������
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

		//�^�C�g���̕\��
		title_Render(title);

		//�t���̕\��
		shisyou_Render(shisyou);

		//�Q�[���J�n�{�^���̕\��
		playbutton_Render(playbutton);

	}
	//--------------------------------------------------------------------------------------------------------------------------------
	//�֐�
	//--------------------------------------------------------------------------------------------------------------------------------
	//�^�C�g���̏�����
	void title_Initialize(Alldata& a_)
	{
		//�^�C�g���C�[�W���̏�����(�L���A�ړ��̎�ށA�J�n�l�A�I���l�A�ړ����鎞��)
		easing::Set("BACKOUT", easing::EASINGTYPE::BACKOUT, -270, 260, 100);			//BACKOUT
		easing::Start("BACKOUT");
	}
	//--------------------------------------------------------------------------------------------------------------------------------
	//�^�C�g���̃J�E���g
	void title_UpDate(Alldata& a_)
	{
		//�C�[�W���O�̎�ނɂ���Ĉړ�����
		a_.x = easing::GetPos("BACKOUT");
	}
	//--------------------------------------------------------------------------------------------------------------------------------
	//�^�C�g���̕\��
	void title_Render(Alldata& a_)
	{
		ML::Box2D draw(0, 0, 480,272);	
		draw.Offset(a_.x, a_.y);
		ML::Box2D src(0, 0, 480, 272);
		DG::Image_Draw("TitleImg", draw, src);
	}
	//--------------------------------------------------------------------------------------------------------------------------------
	//�w�i�̕\��
	void haikei_Render(Alldata& a_)
	{
		ML::Box2D draw(0, 0, 960,544);
		ML::Box2D src(0, 0, 960, 544);
		DG::Image_Draw("Title_haikeiImg", draw, src);
	}
	//--------------------------------------------------------------------------------------------------------------------------------
	//�t���̏�����
	void shisyou_Initialize(Alldata& a_, float x_, float y_)
	{
		a_.x = x_;
		a_.y = y_;
		a_.animCnt = 0;
	}
	//--------------------------------------------------------------------------------------------------------------------------------
	//�t���̕\��
	void shisyou_Render(Alldata& a_)
	{
		shisyou_Anim(a_);
		ML::Box2D draw = a_.drawBase.OffsetCopy(float(a_.x) * 32, float(a_.y) * 32);
		DG::Image_Draw("shisyouImg", draw, a_.src);
	}
	//--------------------------------------------------------------------------------------------------------------------------------
	//�t���̃A�j���[�V����
	void shisyou_Anim(Alldata& a_)
	{
		a_.animCnt++;
		a_.drawBase = ML::Box2D(0, 0, 288, 288);
		switch ((a_.animCnt / 60) % 2) {
		case 0:a_.src = ML::Box2D(0, 0, 224, 224); break;
		case 1:a_.src = ML::Box2D(224, 0, 224, 224); break;
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------
	//�Q�[���J�n�{�^���̏�����
	void playbutton_Initialize(Alldata& a_, float x_, float y_)
	{
		a_.active = true;
		a_.x = x_;
		a_.y = y_;
	}
	//--------------------------------------------------------------------------------------------------------------------------------
	//�Q�[���J�n�{�^���̃J�E���g
	void playbutton_UpDate(Alldata& a_,POINT p_)
	{
		ML::Box2D me = a_.hitBase.OffsetCopy(float(a_.x)* 32.0f, float(a_.y)* 32.0f);
		if (me.x <= p_.x && p_.x < me.x + me.w&&me.y <= p_.y && p_.y <me.y + me.h){
			a_.active = false;
		}
		else{
			a_.active = true;
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------
	//�Q�[���J�n�{�^���̕\��
	void playbutton_Render(Alldata& a_)
	{
		if (a_.active == true) {
			ML::Box2D draw(0, 0, 288, 128);
			a_.hitBase = draw;
			draw.Offset(a_.x * 32, a_.y * 32);
			ML::Box2D src(0, 0, 384,160);
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
}