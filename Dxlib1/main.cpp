#include <DxLib.h>
#include <cassert>
#include "Scene/SceneManager.h"
#include "Scene/TitleScene.h"
#include "InputState.h"
#include "game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// window���[�h�ݒ�
	ChangeWindowMode(Game::kWindowMode);

	if (DxLib_Init() == -1) 
	{
		return -1;
	}

	// �E�C���h�E���ݒ�
	SetMainWindowText(L"�p�b�N�}��");
	// ��ʃT�C�Y�̐ݒ�
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);

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

	DxLib_End();

	return 0;
}