#pragma once
#include "Scene.h"
#include "easing.h"

//�^�C�g���N���X
class Title : public Scene{
protected:
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

public:
	explicit Title() {}
	virtual ~Title() {}

	virtual void InitializeResource();
	virtual bool Initialize();
	virtual void Update();
	virtual void Render();
	virtual void Finalize();
	//-----------------------------------------------------------------------------
	//�N���X�̃����o�֐�
	//-----------------------------------------------------------------------------
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
};