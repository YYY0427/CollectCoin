#include "PauseScene.h"
#include "DxLib.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "../Game.h"
#include <cmath>

namespace
{
	// ポーズ中のメッセージ
	const char* const PAUSE_STRING = "PAUSED";
}

PauseScene::PauseScene(SceneManager& manager) :
	Scene(manager),
	rate_(0.75),
	a_(false)
{
	selectionH_ = CreateFontToHandle("PixelMplus10", 25, 10);

	// 文字列の幅の取得
	pauseWidth_ = GetDrawStringWidthToHandle(PAUSE_STRING, strlen(PAUSE_STRING), selectionH_);

	// 文字列を画像化
	stringGraph_ = MakeScreen(pauseWidth_, 25, true);
	SetDrawScreen(stringGraph_);
	DrawStringToHandle(0, 0, PAUSE_STRING, 0xffffff, selectionH_, false);
	SetDrawScreen(DX_SCREEN_BACK);
}

PauseScene::~PauseScene()
{
	DeleteFontToHandle(selectionH_);
}

void PauseScene::Update(const InputState& input)
{
	if (rate_ > 1.0)
	{
		a_ = false;
	}
	else if (rate_ < 0.75)
	{
		a_ = true;
	}
	if (a_)
	{
		rate_ += 0.01;
	}
	else
	{
		rate_ -= 0.01;
	}

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

	constexpr int pw_width = Game::kScreenWidth;	// ポーズ枠の幅
	constexpr int pw_height = Game::kScreenHeight;	// ポーズ枠の高さ
	constexpr int pw_start_x = 0;					// ポーズ枠の左
	constexpr int pw_start_y = 0;					// ポーズ枠上

 	SetDrawBlendMode(DX_BLENDMODE_MULA, 150);
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	

	// ポーズウィンドウ枠線
//	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0xffffff, false);

	DrawRotaGraph(((pw_start_x + pw_width) / 2) - (pauseWidth_ / 2) + 40, (pw_start_y + pw_height) / 2 + 45,
		rate_, 0.0f, stringGraph_, true, false);
}

