#pragma once
#include "Scene.h"

//�Q�[���N���A�N���X
class GameClear:public Scene
{
protected:
	//�Q�[�����
	DI::Mouse mouse;

	//�\���̌^��`
	struct Object {
	};
	Object gameclear;
	Object haikei;

public:
	explicit GameClear() {}
	virtual ~GameClear() {}

	virtual void InitializeResource();
	virtual bool Initialize();
	virtual void Update();
	virtual void Render();
	virtual void Finalize();
	//-----------------------------------------------------------------------------
	//�N���X�̃����o�֐�
	//-----------------------------------------------------------------------------
	//�Q�[���N���A�̕\���̃v���g�^�C�v�錾
	void gameclear_Render(Object& ob_);
	//�w�i�̕\���̃v���g�^�C�v�錾
	void haikei_Render(Object& ob_);
};

