#pragma once
#include "Scene.h"

//�Q�[�����[���N���X
class GameRule : public Scene 
{
protected:
	//�Q�[�����
	DI::Mouse mouse;

	//�\���̌^��`
	struct Object {
		bool active;			//�������f
	};
	Object haikei;

public:
	explicit GameRule() {}
	virtual ~GameRule() {}

	virtual void InitializeResource() override;
	virtual bool Initialize() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Finalize() override;
};