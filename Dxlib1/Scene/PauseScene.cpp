#include "PauseScene.h"
#include "DxLib.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "KeyConfigScene.h"
#include "../Game.h"

PauseScene::PauseScene(SceneManager& manager) :
	Scene(manager)
{
	selectionH_ = CreateFontToHandle(NULL, 15, 10);
}

void PauseScene::Update(const InputState& input)
{
	if (input.IsTriggered(InputType::pause))
	{
		manager_.PopScene();
		return;
	}
}

void PauseScene::Draw()
{
	//constexpr int pw_width = 400	//ポーズ枠の幅
	//constexpr int pw_height = 300;	//ポーズ枠の高さ
	//constexpr int pw_start_x = (640 - pw_width) / 2;	//ポーズ枠の左
	//constexpr int pw_start_y = (480 - pw_height) / 2;	//ポーズ枠上

	constexpr int pw_width = Game::kScreenWidth;	//ポーズ枠の幅
	constexpr int pw_height = Game::kScreenHeight;	//ポーズ枠の高さ
	constexpr int pw_start_x = 0;	//ポーズ枠の左
	constexpr int pw_start_y = 0;	//ポーズ枠上

//	SetDrawBlendMode(DX_BLENDMODE_MULA, 196);
 	SetDrawBlendMode(DX_BLENDMODE_MULA, 150);
	//ポーズウィンドウセロファン(黒い)
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//通常描画に戻す

	//ポーズウィンドウ枠線
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0xffffff, false);

	//ポーズ中メッセージ
	int width1 = GetDrawStringWidthToHandle(L"PAUSED", 6, selectionH_);
	DrawStringToHandle(((pw_start_x + pw_width) / 2) - (width1 / 2), (pw_start_y + pw_height) / 2 + 40,
		L"PAUSED", 0xffffff, selectionH_, false);
}

