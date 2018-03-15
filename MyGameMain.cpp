#include "MyPG.h"
#include "MyGameMain.h"
#include "easing.h"

//�Q�[�����
TaskFlag			actTask, nextTask; 

//-----------------------------------------------------------------------------
//����������
//�@�\�T�v�F�v���O�����N�����ɂP����s�����i�f�ނȂǂ̏������s���j
//-----------------------------------------------------------------------------
void  MyGameMain_Initalize()
{
	//�L�[�{�[�h�̓��͂��󂯎��I�u�W�F�N�g�𐶐�����
	DI::AnalogAxisKB	ls = { DIK_LEFT, DIK_RIGHT, DIK_UP, DIK_DOWN };
	DI::AnalogAxisKB	rs = { DIK_NUMPAD4, DIK_NUMPAD6, DIK_NUMPAD8, DIK_NUMPAD2 };
	DI::KeyDatas_KB	 key = {
		{ DIK_Z, DI::But::B1 },{ DIK_X, DI::But::B2 },{ DIK_C, DI::But::B3 },
		{ DIK_V, DI::But::B4 },{ DIK_A, DI::But::SE },{ DIK_S, DI::But::ST },
		{ DIK_Q, DI::But::L1 },{ DIK_W, DI::But::R1 },{ DIK_E, DI::But::L2 },
		{ DIK_R, DI::But::R2 },{ DIK_D, DI::But::L3 },{ DIK_NUMPAD5, DI::But::R3 }, 
	};
	//�Q�[���p�b�h�p�����[�^
	DI::KeyDatas_GP	but = {
		{ DI::GPB::B01, DI::But::B1 },{ DI::GPB::B02, DI::But::B2 },
		{ DI::GPB::B03, DI::But::B3 },{ DI::GPB::B04, DI::But::B4 },
		{ DI::GPB::B05, DI::But::L1 },{ DI::GPB::B06, DI::But::R1 },
		{ DI::GPB::B07, DI::But::L2 },{ DI::GPB::B08, DI::But::R2 },
		{ DI::GPB::B09, DI::But::SE },{ DI::GPB::B10, DI::But::ST },
		{ DI::GPB::B11, DI::But::L3 },{ DI::GPB::B12, DI::But::R3 },
	};
	DI::GPad_CreateKBGP("P1", ls, rs, key, 0, but);

	actTask  = Task_Non;	//������Ԃ͎��s���̃^�X�N�𖳌��ɂ��Ă���
	nextTask = Task_Title;	//���̃^�X�N���^�C�g���ɂ��邱�Ƃōŏ��Ƀ^�C�g�������s�����
}
//-----------------------------------------------------------------------------
//�������
//�@�\�T�v�F�v���O�����I�����ɂP����s�����i�f�ނȂǂ̉�����s���j
//-----------------------------------------------------------------------------
void  MyGameMain_Finalize()
{
	DI::GPad_Erase("P1");	//�����I�ɉ���������ꍇ��
}


//-----------------------------------------------------------------------------
//�O���t�@�C�����
namespace  Title {//�^�C�g���^�X�N
	extern  void  Initialize();
	extern  TaskFlag  UpDate();
	extern  void  Render();
	extern  void  Finalize();
}
namespace  Game {//�Q�[���{�҃^�X�N
	extern  void  Initialize();
	extern  TaskFlag  UpDate();
	extern  void  Render();
	extern  void  Finalize();
}
namespace  Gameover {//�Q�[���I�[�o�[�^�X�N
	extern  void  Initialize();
	extern  TaskFlag  UpDate();
	extern  void  Render();
	extern  void  Finalize();
}
namespace  Outsidebusinesshours {//�X�c�ƊO�^�X�N
	extern  void  Initialize();
	extern  TaskFlag  UpDate();
	extern  void  Render();
	extern  void  Finalize();
}
namespace  Gamesetsumei {//�Q�[�������^�X�N
	extern  void  Initialize();
	extern  TaskFlag  UpDate();
	extern  void  Render();
	extern  void  Finalize();
}
namespace  Gameclear {//�Q�[���N���A�^�X�N
	extern  void  Initialize();
	extern  TaskFlag  UpDate();
	extern  void  Render();
	extern  void  Finalize();
}
//�ȉ��K�v�ɉ����Ēǉ�
//-----------------------------------------------------------------------------
//�X�V����
//�@�\�T�v�F�Q�[���̂P�t���[���ɓ����鏈��
//-----------------------------------------------------------------------------
void  MyGameMain_UpDate()
{
	//1�t���[���Ɉ��
	easing::UpDate();

	//���Ɏ��s����^�X�N���A���̃^�X�N�ƈ�v���Ȃ�
	if (actTask != nextTask) {
		//�����s���̃^�X�N�̏I���������Ăт���
		switch (actTask) {
		case Task_Title:					Title::Finalize();	break;
		case Task_Game:						Game::Finalize();	break;
		case Task_Gameover:					Gameover::Finalize();	break;
		case Task_Outsidebusinesshours:		Outsidebusinesshours::Finalize();	break;
		case Task_Gamesetsumei:				Gamesetsumei::Finalize();	break;
		case Task_Gameclear:				Gameclear::Finalize();	break;
			//�ȉ��K�v�ɉ����Ēǉ�
		}
		//���̃^�X�N�Ɉڂ�
		actTask = nextTask;
		//���̃^�X�N�̏������������Ăт���
		switch (actTask) {
		case Task_Title:					Title::Initialize();	break;
		case Task_Game:						Game::Initialize();		break;
		case Task_Gameover:					Gameover::Initialize();		break;
		case Task_Outsidebusinesshours:		Outsidebusinesshours::Initialize();		break;
		case Task_Gamesetsumei:				Gamesetsumei::Initialize();		break;
		case Task_Gameclear:				Gameclear::Initialize();		break;
			//�ȉ��K�v�ɉ����Ēǉ�
		}
	}
	//���݂̃^�X�N�̎��s�E�X�V�������Ăт���
	switch (actTask) {
	case Task_Title:					nextTask = Title::UpDate();		break;
	case Task_Game:						nextTask = Game::UpDate();		break;
	case Task_Gameover:					nextTask = Gameover::UpDate();		break;
	case Task_Outsidebusinesshours:		nextTask = Outsidebusinesshours::UpDate();		break;
	case Task_Gamesetsumei:				nextTask = Gamesetsumei::UpDate();		break;
	case Task_Gameclear:				nextTask = Gameclear::UpDate();		break;
		//�ȉ��K�v�ɉ����Ēǉ�
	}
}
//-----------------------------------------------------------------------------
//�`�揈��
//�@�\�T�v�F�Q�[���̂P�t���[���ɓ�����\������
//-----------------------------------------------------------------------------
void  MyGameMain_Render2D()
{
	switch (actTask){
		case Task_Title:					Title::Render();	break;
		case Task_Game:						Game::Render();		break;
		case Task_Gameover:					Gameover::Render();		break;
		case Task_Outsidebusinesshours:		Outsidebusinesshours::Render();		break;
		case Task_Gamesetsumei:				Gamesetsumei::Render();		break;
		case Task_Gameclear:				Gameclear::Render();		break;
		//�ȉ��K�v�ɉ����Ēǉ�
	}
}
