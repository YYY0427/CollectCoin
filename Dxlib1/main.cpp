#include <DxLib.h>
#include <cassert>
#include "Scene/SceneManager.h"
#include "Scene/TitleScene.h"
#include "InputState.h"
#include "Game.h"

//int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// window���[�h�ݒ�
	ChangeWindowMode(Game::kWindowMode);

	// ��ʃT�C�Y�̐ݒ�
	SetGraphMode(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, Game::kColorDepth);

	// �E�C���h�E���ݒ�
	SetMainWindowText(Game::TITLE_TEXT);

	// �ق��̃E�B���h�E��I�����Ă��Ă������悤�ɂ���
	SetAlwaysRunFlag(false);

	// �E�B���h�E�̃T�C�Y��ύX�\�ɂ���
	SetWindowSizeChangeEnableFlag(true);

	// ������
	if (DxLib_Init() == -1) 
	{
		return -1;
	}

	// ********** �t�H���g�̃��[�h **********
	LPCSTR font_path = "Data/font/PixelMplus10-Regular.ttf"; // �ǂݍ��ރt�H���g�t�@�C���̃p�X
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0)
	{
	}
	else
	{
		// �t�H���g�Ǎ��G���[����
		MessageBox(NULL, "�t�H���g�Ǎ����s", " ", MB_OK);	// ���b�Z�[�W�{�b�N�X���g���Čx���\��
	}

	SetDrawScreen(DX_SCREEN_BACK);

	InputState input;
	SceneManager sceneManager;
	sceneManager.ChangeScene(new TitleScene(sceneManager));

	while (ProcessMessage() != -1) 
	{
		LONGLONG time = GetNowHiPerformanceCount();

		ClearDrawScreen();

		input.Update();

		sceneManager.Update(input);

		sceneManager.Draw();

		ScreenFlip();

		if (CheckHitKey(KEY_INPUT_ESCAPE)) break;

		// fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667)
		{

		}
	}

	// ********** �t�H���g�̃A�����[�h **********
	if (RemoveFontResourceEx(font_path, FR_PRIVATE, NULL))
	{
	}
	else
	{
		MessageBox(NULL, "remove failure", "", MB_OK);
	}

	DxLib_End();

	return 0;
}