#include "OptionScene.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "../game.h"
#include "../DrawFunctions.h"
#include "../SoundManager.h"
#include <DxLib.h>

namespace
{
	// 影
	constexpr int WH_WIDTH = Game::SCREEN_WIDTH;	// 画面の幅
	constexpr int WH_HEIGHT = Game::SCREEN_HEIGHT;	// 画面の高さ
	constexpr int WH_START_X = 0;					// 画面の左
	constexpr int WH_START_Y = 0;					// 画面の枠上

	// オプションウィンドウ
	constexpr int OW_WIDTH_1 = 700;												// オプション枠の幅
	constexpr int OW_HEIGHT_1 = 500;											// オプション枠の高さ
	constexpr int OW_START_X_1 = (Game::SCREEN_WIDTH / 2) - (OW_WIDTH_1 / 2);	// オプション枠の左
	constexpr int OW_START_Y_1 = (Game::SCREEN_HEIGHT / 2) - (OW_HEIGHT_1 / 2);	// オプション枠上

	// 選択肢の数
	constexpr int NAME_COUNT = 2;

	// オプション
	constexpr const char* const OPTION = "OPTION";
}

OptionScene::OptionScene(SceneManager& manager):
	Scene(manager),
	controllerH_(-1),
	noteH_(-1),
	bigH_(-1),
	midleH_(-1),
	noneH_(-1),
	cursor1H_(-1),
	cursor2H_(-1),
	cursor3H_(-1),
	cursor4H_(-1),
	teachH_(-1),
	optionH_(-1),
	speakerH_(-1),
	currentInputIndex_(0)
{
	Init();
}

OptionScene::~OptionScene()
{
	End();
}

void OptionScene::Init()
{
	// 画像のロード
	controllerH_ = my::MyLoadGraph("Data/img/game/controller.png");
	noteH_ = my::MyLoadGraph("Data/img/note.png");
	bigH_ = my::MyLoadGraph("Data/img/speaker.png");
	midleH_ = my::MyLoadGraph("Data/img/speaker_midle.png");
	noneH_ = my::MyLoadGraph("Data/img/speaker_none.png");
	cursor1H_ = my::MyLoadGraph("Data/img/game/cursor1.png");
	cursor2H_ = my::MyLoadGraph("Data/img/game/cursor2.png");
	cursor3H_ = my::MyLoadGraph("Data/img/game/cursor3.png");
	cursor4H_ = my::MyLoadGraph("Data/img/game/cursor4.png");

	// フォントのロード
	teachH_ = CreateFontToHandle("PixelMplus10", 25, 0);
	optionH_ = CreateFontToHandle("PixelMplus10", 40, 0);

	// 初期化
	speakerH_ = bigH_;
	currentInputIndex_ = 0;
}

void OptionScene::End()
{
	DeleteGraph(controllerH_);
	DeleteGraph(noteH_);
	DeleteGraph(bigH_);
	DeleteGraph(midleH_);
	DeleteGraph(noneH_);
	DeleteGraph(cursor1H_);
	DeleteGraph(cursor2H_);
	DeleteGraph(cursor3H_);
	DeleteGraph(cursor4H_);
	DeleteFontToHandle(teachH_);
	DeleteFontToHandle(optionH_);
}

void OptionScene::Update(const InputState& input)
{
	//上下で回る処理
	if (input.IsTriggered(InputType::up))
	{
		SoundManager::GetInstance().Play("cursor");
		currentInputIndex_ = ((currentInputIndex_ - 1) + NAME_COUNT) % NAME_COUNT;
	}
	else if (input.IsTriggered(InputType::down))
	{
		SoundManager::GetInstance().Play("cursor");
		currentInputIndex_ = (currentInputIndex_ + 1) % NAME_COUNT;
	}

	auto& soundMgr = SoundManager::GetInstance();
	// BGM
	if (input.IsTriggered(InputType::right) && currentInputIndex_ == 0)
	{
		soundMgr.SetBGMVolume((std::min)(soundMgr.GetBGMVolume() + 25, 255));
	}
	if (input.IsTriggered(InputType::left) && currentInputIndex_ == 0)
	{
		soundMgr.SetBGMVolume((std::max)(soundMgr.GetBGMVolume() - 25, 0));
	}
	// SE
	if (input.IsTriggered(InputType::right) && currentInputIndex_ == 1)
	{
		soundMgr.SetSEVolume((std::min)(soundMgr.GetSEVolume() + 25, 255));
	}
	if (input.IsTriggered(InputType::left) && currentInputIndex_ == 1)
	{
		soundMgr.SetSEVolume((std::max)(soundMgr.GetSEVolume() - 25, 0));
	}
	// タイトル画面に戻る
	if (input.IsTriggered(InputType::prev))
	{
		soundMgr.SaveSoundConfig();
		manager_.PopScene();
	}
}

void OptionScene::Draw()
{
	// 影の表示
	SetDrawBlendMode(DX_BLENDMODE_MULA, 150);
	DrawBox(WH_START_X, WH_START_Y, WH_START_X + WH_WIDTH, WH_START_Y + WH_HEIGHT, 0x000000, true);
	DrawBox(OW_START_X_1 + 10, OW_START_Y_1 + 10, OW_START_X_1 + OW_WIDTH_1 + 10, OW_START_Y_1 + OW_HEIGHT_1 + 10, 0x000000, true);
	DrawBox(0, Game::SCREEN_HEIGHT - 60, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(OW_START_X_1, OW_START_Y_1, OW_START_X_1 + OW_WIDTH_1, OW_START_Y_1 + OW_HEIGHT_1, GetColor(200, 200, 200), true);

	// オプション
	int width = GetDrawStringWidthToHandle(OPTION, static_cast<int>(strlen(OPTION)), optionH_);
	DrawStringToHandle((Game::SCREEN_WIDTH / 2) - (width / 2), OW_START_Y_1 + 50, OPTION, 0x000000, optionH_, false);

	// B もどる
	DrawRectRotaGraph(Game::SCREEN_WIDTH / 2 + 570, Game::SCREEN_HEIGHT - 30, 32, 0, 16, 16, 2.0f, 0.0f, controllerH_, true);
	DrawStringToHandle(Game::SCREEN_WIDTH / 2 + 600, Game::SCREEN_HEIGHT - 45, "もどる", 0xffffff, teachH_, false);

	// ↑↓←→　せんたく
	DrawRectRotaGraph(Game::SCREEN_WIDTH / 2 + 250, Game::SCREEN_HEIGHT - 30, 128, 0, 16, 16, 2.0f, 0.0f, controllerH_, true);
	DrawRectRotaGraph(Game::SCREEN_WIDTH / 2 + 255 + 32, Game::SCREEN_HEIGHT - 30, 144, 0, 16, 16, 2.0f, 0.0f, controllerH_, true);
	DrawRectRotaGraph(Game::SCREEN_WIDTH / 2 + 260 + 64, Game::SCREEN_HEIGHT - 30, 160, 0, 16, 16, 2.0f, 0.0f, controllerH_, true);
	DrawRectRotaGraph(Game::SCREEN_WIDTH / 2 + 265 + 96, Game::SCREEN_HEIGHT - 30, 176, 0, 16, 16, 2.0f, 0.0f, controllerH_, true);
	DrawStringToHandle(Game::SCREEN_WIDTH / 2 + 270 + 128, Game::SCREEN_HEIGHT - 45, "せんたく", 0xffffff, teachH_, false);

	// BGM 
	DrawRotaGraph(WH_START_X + WH_WIDTH / 2 - 230, WH_START_Y + WH_HEIGHT / 2 - 50, 1.0f, 0.0f, noteH_, true);

	// SE
	DrawRotaGraph(WH_START_X + WH_WIDTH / 2 - 230, WH_START_Y + WH_HEIGHT / 2 + 100, 1.0f, 0.0f, speakerH_, true);

	// カーソル
	if (currentInputIndex_ == 0)
	{
		DrawRotaGraph(WH_START_X + WH_WIDTH / 2 - 230 - 32, WH_START_Y + WH_HEIGHT / 2 - 80, 0.2f, 0.0f, cursor1H_, true);
		DrawRotaGraph(WH_START_X + WH_WIDTH / 2 - 230 + 32, WH_START_Y + WH_HEIGHT / 2 - 80, 0.2f, 0.0f, cursor2H_, true);
		DrawRotaGraph(WH_START_X + WH_WIDTH / 2 - 230 + 32, WH_START_Y + WH_HEIGHT / 2 - 10, 0.2f, 0.0f, cursor3H_, true);
		DrawRotaGraph(WH_START_X + WH_WIDTH / 2 - 230 - 32, WH_START_Y + WH_HEIGHT / 2 - 10, 0.2f, 0.0f, cursor4H_, true);
	}
	else if (currentInputIndex_ == 1)
	{
		DrawRotaGraph(WH_START_X + WH_WIDTH / 2 - 230 - 32, WH_START_Y + WH_HEIGHT / 2 + 70, 0.2f, 0.0f, cursor1H_, true);
		DrawRotaGraph(WH_START_X + WH_WIDTH / 2 - 230 + 32, WH_START_Y + WH_HEIGHT / 2 + 70, 0.2f, 0.0f, cursor2H_, true);
		DrawRotaGraph(WH_START_X + WH_WIDTH / 2 - 230 + 32, WH_START_Y + WH_HEIGHT / 2 + 130, 0.2f, 0.0f, cursor3H_, true);
		DrawRotaGraph(WH_START_X + WH_WIDTH / 2 - 230 - 32, WH_START_Y + WH_HEIGHT / 2 + 130, 0.2f, 0.0f, cursor4H_, true);
	}

	auto& soundMgr = SoundManager::GetInstance();

	// 音量の数の取得
	int bgmNum = soundMgr.GetBGMVolume() / 25;
	int seNum = soundMgr.GetSEVolume() / 25;

	for (int i = 0; i < 10; i++)
	{
		int width = i * 40;

		// 枠組み
		DrawRoundRect(WH_START_X + WH_WIDTH / 2 - 100 + width, 370, WH_START_X + WH_WIDTH / 2 - 100 + 30 + width, 370 + 80, 5, 5, GetColor(255, 165, 0), false);
		DrawRoundRect(WH_START_X + WH_WIDTH / 2 - 100 + width, 500, WH_START_X + WH_WIDTH / 2 - 100 + 30 + width, 500 + 80, 5, 5, GetColor(255, 165, 0), false);
	}

	// BGM
	for (int i = 0; i < bgmNum; i++)
	{
		int width = i * 40;
		DrawRoundRect(WH_START_X + WH_WIDTH / 2 - 100 + width, 370, WH_START_X + WH_WIDTH / 2 - 100 + 30 + width, 370 + 80, 5, 5, GetColor(255, 165, 0), true);
	}
	// SE
	for (int i = 0; i < seNum; i++)
	{
		int width = i * 40;
		DrawRoundRect(WH_START_X + WH_WIDTH / 2 - 100 + width, 500, WH_START_X + WH_WIDTH / 2 - 100 + 30 + width, 500 + 80, 5, 5, GetColor(255, 165, 0), true);
	}
}