#include <DxLib.h>
#include <cassert>
#include "Scene/SceneManager.h"
#include "Scene/TitleScene.h"
#include "InputState.h"
#include "Game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// windowモード設定
	ChangeWindowMode(Game::kWindowMode);

	// 画面サイズの設定
	SetGraphMode(Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, Game::kColorDepth);

	// ウインドウ名設定
	SetMainWindowText(Game::kTitleText);

	// 初期化
	if (DxLib_Init() == -1) 
	{
		return -1;
	}

	// ********** フォントのロード **********
	LPCSTR font_path = "Data/font/PixelMplus10-Regular.ttf"; // 読み込むフォントファイルのパス
	if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0)
	{
	}
	else
	{
		// フォント読込エラー処理
		MessageBox(NULL, "フォント読込失敗", " ", MB_OK);	// メッセージボックスを使って警告表示
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

		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{

		}
	}

	// ********** フォントのアンロード **********
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