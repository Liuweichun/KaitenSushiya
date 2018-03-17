#pragma once
#include "Scene.h"

//�Q�[���N���A�N���X
class GameOver :public Scene
{
protected:
	//�Q�[�����
	DI::Mouse mouse;

	//�\���̌^��`
	struct Object {
	};
	Object overstage;
	Object haikei;

public:
	explicit GameOver() {}
	virtual ~GameOver() {}

	virtual void InitializeResource();
	virtual bool Initialize();
	virtual void Update();
	virtual void Render();
	virtual void Finalize();
	//-----------------------------------------------------------------------------
	//�N���X�̃����o�֐�
	//-----------------------------------------------------------------------------
	//�Q�[���I�[�o�[�̕\���̃v���g�^�C�v�錾
	void overstage_Render(Object& ob_);
	//�w�i�̕\���̃v���g�^�C�v�錾
	void haikei_Render(Object& ob_);
};