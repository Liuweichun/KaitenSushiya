#pragma once
#include "MyPG.h"

enum TaskFlag
{
	Task_Non,							//�^�X�N�����I�����
	Task_Title,							//�^�C�g��
	Task_Game,							//�Q�[���{��
	Task_Gameover,						//�Q�[���I�[�o�[
	Task_Outsidebusinesshours,			//�X�c�Ǝ��ԊO
	Task_Gamesetsumei,					//�Q�[������
	Task_Gameclear,						//�Q�[���N���A

	//�ȉ��K�v�ɉ����Ēǉ�
};

extern  void  MyGameMain_Finalize( );
extern  void  MyGameMain_Initalize( );
extern  void  MyGameMain_UpDate( );
extern  void  MyGameMain_Render2D( );
