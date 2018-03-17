#include"MyGameMain.h"
#include"OutsideBusinessHours.h"

//-----------------------------------------------------------------------------
//���\�[�X����
//-----------------------------------------------------------------------------
void OutsideBusinessHours::InitializeResource()
{
	DG::Image_Create("eigyoujikanngaiImg", "./data/image/eigyoujikanngai.png");		//�c�Ǝ��ԊO
	DG::Image_Create("haikei_blackImg", "./data/image/haikei_black.png");			//�w�i
}
//-----------------------------------------------------------------------------
//����������
//�@�\�T�v�F�v���O�����N�����ɂP����s�����i�f�ނȂǂ̏������s���j
//-----------------------------------------------------------------------------
bool OutsideBusinessHours::Initialize()
{
	if (!Scene::Initialize()) {
		return false;
	}
	return true;
}
//-----------------------------------------------------------------------------
//�X�V����
//�@�\�T�v�F�Q�[���̂P�t���[���ɓ����鏈��
//-----------------------------------------------------------------------------
void OutsideBusinessHours::Update()
{
	//�}�E�X�J�[�\���̈ʒu�𓾂�
	mouse = DI::Mouse_GetState();

	if (mouse.LB.down) {
		currentScene = scenes["Game"];		//���̃^�X�N���Q�[���{�҂�
		currentScene->Initialize();
	}
}
//-----------------------------------------------------------------------------
//�`�揈��
//�@�\�T�v�F�Q�[���̂P�t���[���ɓ�����\������
//-----------------------------------------------------------------------------
void OutsideBusinessHours::Render()
{
	//�w�i�̕\��
	haikei_Render(haikei);
	//�c�Ǝ��ԊO�̕\��
	eigyougaistage_Render(eigyougaistage);
}
//-----------------------------------------------------------------------------
//�������
//�@�\�T�v�F�v���O�����I�����ɂP����s�����i�f�ނȂǂ̉�����s���j
//-----------------------------------------------------------------------------
void OutsideBusinessHours::Finalize()
{
	DG::Image_Erase("eigyoujikanngaiImg");
	DG::Image_Erase("haikei_blackImg");
}
//--------------------------------------------------------------------------------------------------------------------------------
//�֐�
//--------------------------------------------------------------------------------------------------------------------------------
//�c�Ǝ��ԊO�̕\��
void OutsideBusinessHours::eigyougaistage_Render(Object& ob_)
{
	ML::Box2D draw(0, 0, 960, 544);
	ML::Box2D src(0, 0, 960, 544);
	DG::Image_Draw("eigyoujikanngaiImg", draw, src);
}
//--------------------------------------------------------------------------------------------------------------------------------
//�w�i�̕\��
void OutsideBusinessHours::haikei_Render(Object& ob_)
{
	ML::Box2D draw(0, 0, 960, 544);
	ML::Box2D src(0, 0, 960, 544);
	DG::Image_Draw("haikei_blackImg", draw, src);
}