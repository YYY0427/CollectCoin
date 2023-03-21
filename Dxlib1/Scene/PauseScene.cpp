#include "PauseScene.h"
#include "DxLib.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "../Game.h"
#include <cmath>

namespace
{
	constexpr int WS_WIDTH = Game::SCREEN_WIDTH;	// 幅
	constexpr int WS_HEIGHT = Game::SCREEN_HEIGHT;	// 高さ
	constexpr int WS_START_Y = 0;					// 開始位置y座標
	constexpr int WS_START_X = 0;					// 開始位置x座標

	// ポーズ中のメッセージ
	const char* const PAUSE_STRING = "PAUSED";
	constexpr int FONT_SIZE = 80;
}

PauseScene::PauseScene(SceneManager& manager) :
	Scene(manager),
	rate_(0.0),
	isPlusOrMinus(false),
	pauseWidth_(0),
	selectionH_(-1),
	stringGraphH_(-1)
{
	Init();
}

PauseScene::~PauseScene()
{
	End();
}

void PauseScene::Init()
{
	// 文字の拡大率の初期化
	rate_ = 0.75;

	// フォントのロード
	selectionH_ = CreateFontToHandle("PixelMplus10", FONT_SIZE, 9);

	// 文字列の幅の取得
	pauseWidth_ = GetDrawStringWidthToHandle(PAUSE_STRING, static_cast<int>(strlen(PAUSE_STRING)), selectionH_);

	// 文字列を画像化
	stringGraphH_ = MakeScreen(pauseWidth_, FONT_SIZE, true);
	SetDrawScreen(stringGraphH_);
	DrawStringToHandle(0, 0, PAUSE_STRING, 0xffffff, selectionH_, false);
	SetDrawScreen(DX_SCREEN_BACK);
}

void PauseScene::End()
{
	DeleteFontToHandle(selectionH_);
	DeleteGraph(stringGraphH_);
}

void PauseScene::Update(const InputState& input)
{
	// 文字の拡大縮小処理
	if (rate_ > 1.0)
	{
		isPlusOrMinus = false;
	}
	else if (rate_ < 0.75)
	{
		isPlusOrMinus = true;
	}
	if (isPlusOrMinus)
	{
		rate_ += 0.005;
	}
	else
	{
		rate_ -= 0.005;
	}

	// 戻る
	if (input.IsTriggered(InputType::pause))
	{
		manager_.PopScene();
		return;
	}
}

void PauseScene::Draw()
{
	// 影
 	SetDrawBlendMode(DX_BLENDMODE_MULA, 150);
	DrawBox(WS_START_Y, WS_START_X, WS_START_Y + WS_WIDTH, WS_START_X + WS_HEIGHT, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	

	// ポーズ文字列の表示
	DrawRotaGraph(((WS_START_Y + WS_WIDTH) / 2) - (pauseWidth_ / 2) + 130, (WS_START_X + WS_HEIGHT) / 2 - 200,
		rate_, 0.0f, stringGraphH_, true, false);
}