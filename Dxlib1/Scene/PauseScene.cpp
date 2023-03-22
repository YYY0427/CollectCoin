#include "PauseScene.h"
#include "DxLib.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "../Game.h"
#include "../DrawFunctions.h"
#include "../SoundManager.h"
#include "TitleScene.h"
#include "OptionScene.h"
#include <cmath>

namespace
{
	// 画面全体の影
	constexpr int WS_WIDTH = Game::SCREEN_WIDTH;	// 幅
	constexpr int WS_HEIGHT = Game::SCREEN_HEIGHT;	// 高さ
	constexpr int WS_START_Y = 0;					// 開始位置y座標
	constexpr int WS_START_X = 0;					// 開始位置x座標

	// 選択肢の箱座標
	constexpr int CB_WIDTH_1 = 400;												//枠の幅
	constexpr int CB_HEIGHT_1 = 100;											//枠の高さ
	constexpr int CB_START_X_1 = (Game::SCREEN_WIDTH / 2) - (CB_WIDTH_1 / 2);	//枠のスタートX座標
	constexpr int CB_START_Y_1 = Game::SCREEN_HEIGHT / 2;						//枠のスタートY座標

	constexpr int CB_WIDTH_2 = 400;
	constexpr int CB_HEIGHT_2 = 100;
	constexpr int CB_START_X_2 = (Game::SCREEN_WIDTH / 2) - (CB_WIDTH_1 / 2);
	constexpr int CB_START_Y_2 = Game::SCREEN_HEIGHT / 2 + 115;

	constexpr int CB_WIDTH_3 = 400;
	constexpr int CB_HEIGHT_3 = 100;
	constexpr int CB_START_X_3 = (Game::SCREEN_WIDTH / 2) - (CB_WIDTH_1 / 2);
	constexpr int CB_START_Y_3 = Game::SCREEN_HEIGHT / 2 + 230;

	// ポーズ中のメッセージ
	const char* const PAUSE_STRING = "PAUSED";
	constexpr int FONT_SIZE = 90;

	// 選択肢の数
	constexpr int CHOICE_NUM = 3;

	// フェードの速度
	constexpr int FADE_INTERVAL = 60;
}

PauseScene::PauseScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&PauseScene::NormalUpdate),
	rate_(0.0),
	isPlusOrMinus(false),
	isTitle_(false),
	pauseWidth_(0),
	currentInputIndex_(0),
	fadeValue_(0),
	fadeTimer_(0),
	controllerH_(-1),
	teachH_(-1),
	selectionH_(-1),
	backH_(-1),
	playH_(-1),
	stringGraphH_(-1),
	cursor1H_(-1),
	cursor2H_(-1),
	cursor3H_(-1),
	cursor4H_(-1),
	settingH_(-1),
	cursor1Pos_(0, 0),
	cursor2Pos_(0, 0),
	cursor3Pos_(0, 0),
	cursor4Pos_(0, 0)
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

	// 画像のロード
	cursor1H_ = my::MyLoadGraph("Data/img/game/cursor1.png");
	cursor2H_ = my::MyLoadGraph("Data/img/game/cursor2.png");
	cursor3H_ = my::MyLoadGraph("Data/img/game/cursor3.png");
	cursor4H_ = my::MyLoadGraph("Data/img/game/cursor4.png");
	playH_ = my::MyLoadGraph("Data/img/play.png");
	backH_ = my::MyLoadGraph("Data/img/back.png");
	settingH_ = my::MyLoadGraph("Data/img/setting.png");
	controllerH_ = my::MyLoadGraph("Data/img/game/controller.png");
	teachH_ = CreateFontToHandle("PixelMplus10", 25, 0);

	// 文字列を画像化
	stringGraphH_ = MakeScreen(pauseWidth_, FONT_SIZE, true);
	SetDrawScreen(stringGraphH_);
	DrawStringToHandle(0, 0, PAUSE_STRING, 0xffffff, selectionH_, false);
	SetDrawScreen(DX_SCREEN_BACK);

	// 初期化
	fadeTimer_ = 0;
	fadeValue_ = 0;
	currentInputIndex_ = 0;
	isPlusOrMinus = false;
	isTitle_ = false;
}

void PauseScene::End()
{
	DeleteFontToHandle(selectionH_);
	DeleteFontToHandle(teachH_);
	DeleteGraph(stringGraphH_);
	DeleteGraph(cursor1H_);
	DeleteGraph(cursor2H_);
	DeleteGraph(cursor3H_);
	DeleteGraph(cursor4H_);
	DeleteGraph(controllerH_);
}

void PauseScene::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void PauseScene::Draw()
{
	// 影
 	SetDrawBlendMode(DX_BLENDMODE_MULA, 150);
	DrawBox(WS_START_Y, WS_START_X, WS_START_Y + WS_WIDTH, WS_START_X + WS_HEIGHT, 0x000000, true);
	DrawBox(0, Game::SCREEN_HEIGHT - 60, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	

	// ポーズ文字列の表示
	DrawRotaGraph(((WS_START_Y + WS_WIDTH) / 2) - (pauseWidth_ / 2) + 130, (WS_START_X + WS_HEIGHT) / 2 - 200,
		rate_, 0.0f, stringGraphH_, true, false);

	// 選択肢
	DrawRoundRect(CB_START_X_1 - 3, CB_START_Y_1 - 3, CB_START_X_1 + CB_WIDTH_1 + 3, CB_START_Y_1 + CB_HEIGHT_1 + 3, 5, 5, 0x000000, true);
	DrawRoundRect(CB_START_X_2 - 3, CB_START_Y_2 - 3, CB_START_X_2 + CB_WIDTH_2 + 3, CB_START_Y_2 + CB_HEIGHT_2 + 3, 5, 5, 0x000000, true);
	DrawRoundRect(CB_START_X_3 - 3, CB_START_Y_3 - 3, CB_START_X_3 + CB_WIDTH_3 + 3, CB_START_Y_3 + CB_HEIGHT_3 + 3, 5, 5, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawRoundRect(CB_START_X_1, CB_START_Y_1, CB_START_X_1 + CB_WIDTH_1, CB_START_Y_1 + CB_HEIGHT_1, 5, 5, 0xffffff, true);
	DrawRoundRect(CB_START_X_2, CB_START_Y_2, CB_START_X_2 + CB_WIDTH_2, CB_START_Y_2 + CB_HEIGHT_2, 5, 5, 0xffffff, true);
	DrawRoundRect(CB_START_X_3, CB_START_Y_3, CB_START_X_3 + CB_WIDTH_3, CB_START_Y_3 + CB_HEIGHT_3, 5, 5, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawRoundRect(CB_START_X_1, CB_START_Y_1, CB_START_X_1 + CB_WIDTH_1, CB_START_Y_1 + CB_HEIGHT_1 - 5, 5, 5, 0xffffff, true);
	DrawRoundRect(CB_START_X_2, CB_START_Y_2, CB_START_X_2 + CB_WIDTH_2, CB_START_Y_2 + CB_HEIGHT_2 - 5, 5, 5, 0xffffff, true);
	DrawRoundRect(CB_START_X_3, CB_START_Y_3, CB_START_X_3 + CB_WIDTH_3, CB_START_Y_3 + CB_HEIGHT_3 - 5, 5, 5, 0xffffff, true);

	// アイコン
	DrawRotaGraph(Game::SCREEN_WIDTH / 2, (Game::SCREEN_HEIGHT / 2) + 50, 1.0f, 0.0f, playH_, true);
	DrawRotaGraph(Game::SCREEN_WIDTH / 2, (Game::SCREEN_HEIGHT / 2) + 165, 1.0f, 0.0f, settingH_, true);
	DrawRotaGraph(Game::SCREEN_WIDTH / 2, (Game::SCREEN_HEIGHT / 2) + 280, 1.0f, 0.0f, backH_, true);

	// カーソル
	DrawRotaGraph(static_cast<int>(cursor1Pos_.x), static_cast<int>(cursor1Pos_.y), 0.2f, 0.0f, cursor1H_, true);
	DrawRotaGraph(static_cast<int>(cursor2Pos_.x), static_cast<int>(cursor2Pos_.y), 0.2f, 0.0f, cursor2H_, true);
	DrawRotaGraph(static_cast<int>(cursor3Pos_.x), static_cast<int>(cursor3Pos_.y), 0.2f, 0.0f, cursor3H_, true);
	DrawRotaGraph(static_cast<int>(cursor4Pos_.x), static_cast<int>(cursor4Pos_.y), 0.2f, 0.0f, cursor4H_, true);

	// A けってい
	DrawRectRotaGraph(Game::SCREEN_WIDTH / 2 + 570, Game::SCREEN_HEIGHT - 30, 0, 0, 16, 16, 2.0f, 0.0f, controllerH_, true);
	DrawStringToHandle(Game::SCREEN_WIDTH / 2 + 600, Game::SCREEN_HEIGHT - 45, "けってい", 0xffffff, teachH_, false);

	// ↑↓←→　せんたく
	DrawRectRotaGraph(Game::SCREEN_WIDTH / 2 + 250, Game::SCREEN_HEIGHT - 30, 128, 0, 16, 16, 2.0f, 0.0f, controllerH_, true);
	DrawRectRotaGraph(Game::SCREEN_WIDTH / 2 + 255 + 32, Game::SCREEN_HEIGHT - 30, 144, 0, 16, 16, 2.0f, 0.0f, controllerH_, true);
	DrawRectRotaGraph(Game::SCREEN_WIDTH / 2 + 260 + 64, Game::SCREEN_HEIGHT - 30, 160, 0, 16, 16, 2.0f, 0.0f, controllerH_, true);
	DrawRectRotaGraph(Game::SCREEN_WIDTH / 2 + 265 + 96, Game::SCREEN_HEIGHT - 30, 176, 0, 16, 16, 2.0f, 0.0f, controllerH_, true);
	DrawStringToHandle(Game::SCREEN_WIDTH / 2 + 270 + 128, Game::SCREEN_HEIGHT - 45, "せんたく", 0xffffff, teachH_, false);

	// フェイドアウト
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void PauseScene::NormalUpdate(const InputState& input)
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

	//上下で回る処理
	if (input.IsTriggered(InputType::up))
	{
		SoundManager::GetInstance().Play("cursor");
		currentInputIndex_ = ((currentInputIndex_ - 1) + CHOICE_NUM) % CHOICE_NUM;
	}
	else if (input.IsTriggered(InputType::down))
	{
		SoundManager::GetInstance().Play("cursor");
		currentInputIndex_ = (currentInputIndex_ + 1) % CHOICE_NUM;
	}

	if (input.IsTriggered(InputType::next))
	{
		SoundManager::GetInstance().Play("decision");

		if (currentInputIndex_ == 0)
		{
			manager_.PopScene();
			return;
		}
		else if (currentInputIndex_ == 1)
		{
			manager_.PushScene(new OptionScene(manager_));
			return;
		}
		else
		{
			isTitle_ = true;
		}
		updateFunc_ = &PauseScene::FadeOutUpdate;
	}

	// カーソルの移動
	if (currentInputIndex_ == 0)
	{
		cursor1Pos_.x = CB_START_X_1;
		cursor1Pos_.y = CB_START_Y_1;

		cursor2Pos_.x = CB_START_X_1 + CB_WIDTH_1;
		cursor2Pos_.y = CB_START_Y_1;

		cursor3Pos_.x = CB_START_X_1 + CB_WIDTH_1;
		cursor3Pos_.y = CB_START_Y_1 + CB_HEIGHT_1;

		cursor4Pos_.x = CB_START_X_1;
		cursor4Pos_.y = CB_START_Y_1 + CB_HEIGHT_1;
	}
	else if (currentInputIndex_ == 1)
	{
		cursor1Pos_.x = CB_START_X_2;
		cursor1Pos_.y = CB_START_Y_2;

		cursor2Pos_.x = CB_START_X_2 + CB_WIDTH_2;
		cursor2Pos_.y = CB_START_Y_2;

		cursor3Pos_.x = CB_START_X_2 + CB_WIDTH_2;
		cursor3Pos_.y = CB_START_Y_2 + CB_HEIGHT_2;

		cursor4Pos_.x = CB_START_X_2;
		cursor4Pos_.y = CB_START_Y_2 + CB_HEIGHT_2;

	}
	else if (currentInputIndex_ == 2)
	{
		cursor1Pos_.x = CB_START_X_3;
		cursor1Pos_.y = CB_START_Y_3;

		cursor2Pos_.x = CB_START_X_3 + CB_WIDTH_3;
		cursor2Pos_.y = CB_START_Y_3;

		cursor3Pos_.x = CB_START_X_3 + CB_WIDTH_3;
		cursor3Pos_.y = CB_START_Y_3 + CB_HEIGHT_3;

		cursor4Pos_.x = CB_START_X_3;
		cursor4Pos_.y = CB_START_Y_3 + CB_HEIGHT_3;
	}
}

void PauseScene::FadeOutUpdate(const InputState& input)
{
	fadeTimer_++;
	fadeValue_ = static_cast<int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(FADE_INTERVAL)));

	// タイトル画面に戻る
	if (fadeTimer_ > FADE_INTERVAL && isTitle_)
	{
		manager_.ChangeScene(new TitleScene(manager_));
		return;
	}
}
