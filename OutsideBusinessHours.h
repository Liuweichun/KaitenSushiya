#pragma once
#include "Scene.h"

//�c�Ǝ��ԊO�N���X
class OutsideBusinessHours :public Scene
{
protected:
	//�Q�[�����
	DI::Mouse mouse;

	//�\���̌^��`
	struct Object {
	};
	Object eigyougaistage;
	Object haikei;

public:
	explicit OutsideBusinessHours() {}
	virtual ~OutsideBusinessHours() {}

	virtual void InitializeResource();
	virtual bool Initialize();
	virtual void Update();
	virtual void Render();
	virtual void Finalize();
	//-----------------------------------------------------------------------------
	//�N���X�̃����o�֐�
	//-----------------------------------------------------------------------------
	//�c�Ǝ��ԊO�̕\���̃v���g�^�C�v�錾
	void eigyougaistage_Render(Object& ob_);
	//�w�i�̕\���̃v���g�^�C�v�錾
	void haikei_Render(Object& ob_);
};
