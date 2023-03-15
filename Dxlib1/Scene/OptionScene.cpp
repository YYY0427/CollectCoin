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
	constexpr int pw_width = Game::kScreenWidth;	// 画面の幅
	constexpr int pw_height = Game::kScreenHeight;	// 画面の高さ
	constexpr int pw_start_x = 0;					// 画面の左
	constexpr int pw_start_y = 0;					// 画面の枠上

	constexpr int pw_width_1 = 700;												// オプション枠の幅
	constexpr int pw_height_1 = 500;											// オプション枠の高さ
	constexpr int pw_start_x_1 = (Game::kScreenWidth / 2) - (pw_width_1 / 2);	// オプション枠の左
	constexpr int pw_start_y_1 = (Game::kScreenHeight / 2) - (pw_height_1 / 2);	// オプション枠上

	constexpr int nameCount = 2;

	// オプション
	constexpr const char* const OPTION = "OPTION";
}

OptionScene::OptionScene(SceneManager& manager):
	Scene(manager)
{
	controllerH_ = my::MyLoadGraph("Data/img/game/controller.png");
	noteH_ = my::MyLoadGraph("Data/img/note.png");
	bigH_ = my::MyLoadGraph("Data/img/speaker.png");
	midleH_ = my::MyLoadGraph("Data/img/speaker_midle.png");
	noneH_ = my::MyLoadGraph("Data/img/speaker_none.png");
	cursor1H_ = my::MyLoadGraph("Data/img/game/cursor1.png");
	cursor2H_ = my::MyLoadGraph("Data/img/game/cursor2.png");
	cursor3H_ = my::MyLoadGraph("Data/img/game/cursor3.png");
	cursor4H_ = my::MyLoadGraph("Data/img/game/cursor4.png");

	teachH_ = CreateFontToHandle("PixelMplus10", 25, 0);
	optionH_ = CreateFontToHandle("PixelMplus10", 40, 0);

	speakerH_ = bigH_;
	currentInputIndex_ = 0;
}

void OptionScene::Update(const InputState& input)
{
	//上下で回る処理
	if (input.IsTriggered(InputType::up))
	{
		SoundManager::GetInstance().Play("cursor");
		currentInputIndex_ = ((currentInputIndex_ - 1) + nameCount) % nameCount;
	}
	else if (input.IsTriggered(InputType::down))
	{
		SoundManager::GetInstance().Play("cursor");
		currentInputIndex_ = (currentInputIndex_ + 1) % nameCount;
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
		manager_.PopScene();
		return;
	}
}

void OptionScene::Draw()
{
	// 影の表示
	SetDrawBlendMode(DX_BLENDMODE_MULA, 150);
	DrawBox(pw_start_x, pw_start_y, pw_start_x + pw_width, pw_start_y + pw_height, 0x000000, true);
	DrawBox(pw_start_x_1 + 10, pw_start_y_1 + 10, pw_start_x_1 + pw_width_1 + 10, pw_start_y_1 + pw_height_1 + 10, 0x000000, true);
	DrawBox(0, Game::kScreenHeight - 60, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(pw_start_x_1, pw_start_y_1, pw_start_x_1 + pw_width_1, pw_start_y_1 + pw_height_1, GetColor(200, 200, 200), true);

	// オプション
	int width1 = GetDrawStringWidthToHandle(OPTION, strlen(OPTION), optionH_);
	DrawStringToHandle((Game::kScreenWidth / 2) - (width1 / 2), pw_start_y_1 + 50, OPTION, 0x000000, optionH_, false);

	// B もどる
	DrawRectRotaGraph(Game::kScreenWidth / 2 + 570, Game::kScreenHeight - 30, 32, 0, 16, 16, 2.0f, 0.0f, controllerH_, true);
	DrawStringToHandle(Game::kScreenWidth / 2 + 600, Game::kScreenHeight - 45, "もどる", 0xffffff, teachH_, false);

	// ↑↓←→　せんたく
	DrawRectRotaGraph(Game::kScreenWidth / 2 + 250, Game::kScreenHeight - 30, 128, 0, 16, 16, 2.0f, 0.0f, controllerH_, true);
	DrawRectRotaGraph(Game::kScreenWidth / 2 + 255 + 32, Game::kScreenHeight - 30, 144, 0, 16, 16, 2.0f, 0.0f, controllerH_, true);
	DrawRectRotaGraph(Game::kScreenWidth / 2 + 260 + 64, Game::kScreenHeight - 30, 160, 0, 16, 16, 2.0f, 0.0f, controllerH_, true);
	DrawRectRotaGraph(Game::kScreenWidth / 2 + 265 + 96, Game::kScreenHeight - 30, 176, 0, 16, 16, 2.0f, 0.0f, controllerH_, true);
	DrawStringToHandle(Game::kScreenWidth / 2 + 270 + 128, Game::kScreenHeight - 45, "せんたく", 0xffffff, teachH_, false);

	// BGM 
	DrawRotaGraph(pw_start_x + pw_width / 2 - 230, pw_start_y + pw_height / 2 - 50, 1.0f, 0.0f, noteH_, true);

	// SE
	DrawRotaGraph(pw_start_x + pw_width / 2 - 230, pw_start_y + pw_height / 2 + 100, 1.0f, 0.0f, speakerH_, true);

	// カーソル
	if (currentInputIndex_ == 0)
	{
		DrawRotaGraph(pw_start_x + pw_width / 2 - 230 - 32, pw_start_y + pw_height / 2 - 80, 0.2f, 0.0f, cursor1H_, true);
		DrawRotaGraph(pw_start_x + pw_width / 2 - 230 + 32, pw_start_y + pw_height / 2 - 80, 0.2f, 0.0f, cursor2H_, true);
		DrawRotaGraph(pw_start_x + pw_width / 2 - 230 + 32, pw_start_y + pw_height / 2 - 10, 0.2f, 0.0f, cursor3H_, true);
		DrawRotaGraph(pw_start_x + pw_width / 2 - 230 - 32, pw_start_y + pw_height / 2 - 10, 0.2f, 0.0f, cursor4H_, true);
	}
	else if (currentInputIndex_ == 1)
	{
		DrawRotaGraph(pw_start_x + pw_width / 2 - 230 - 32, pw_start_y + pw_height / 2 + 70, 0.2f, 0.0f, cursor1H_, true);
		DrawRotaGraph(pw_start_x + pw_width / 2 - 230 + 32, pw_start_y + pw_height / 2 + 70, 0.2f, 0.0f, cursor2H_, true);
		DrawRotaGraph(pw_start_x + pw_width / 2 - 230 + 32, pw_start_y + pw_height / 2 + 130, 0.2f, 0.0f, cursor3H_, true);
		DrawRotaGraph(pw_start_x + pw_width / 2 - 230 - 32, pw_start_y + pw_height / 2 + 130, 0.2f, 0.0f, cursor4H_, true);
	}

	auto& soundMgr = SoundManager::GetInstance();
	// BGMの設定
	DrawFormatString(pw_start_x + 20, pw_start_y + 50, 0xffffff, "BGM Volume = %d", soundMgr.GetBGMVolume());

	// SEの設定
	DrawFormatString(pw_start_x + 20, pw_start_y + 70, 0xffffff, "SE Volume = %d", soundMgr.GetSEVolume());

	int bgmNum = soundMgr.GetBGMVolume() / 25;
	int seNum = soundMgr.GetSEVolume() / 25;

	for (int i = 0; i < 10; i++)
	{
		int width = i * 40;
		// 枠組み
		DrawRoundRect(pw_start_x + pw_width / 2 - 100 + width, 370, pw_start_x + pw_width / 2 - 100 + 30 + width, 370 + 80, 5, 5, GetColor(255, 165, 0), false);
		DrawRoundRect(pw_start_x + pw_width / 2 - 100 + width, 500, pw_start_x + pw_width / 2 - 100 + 30 + width, 500 + 80, 5, 5, GetColor(255, 165, 0), false);
	}

	// BGM
	for (int i = 0; i < bgmNum; i++)
	{
		int width = i * 40;
		DrawRoundRect(pw_start_x + pw_width / 2 - 100 + width, 370, pw_start_x + pw_width / 2 - 100 + 30 + width, 370 + 80, 5, 5, GetColor(255, 165, 0), true);
	}
	// SE
	for (int i = 0; i < seNum; i++)
	{
		int width = i * 40;
		DrawRoundRect(pw_start_x + pw_width / 2 - 100 + width, 500, pw_start_x + pw_width / 2 - 100 + 30 + width, 500 + 80, 5, 5, GetColor(255, 165, 0), true);
	}
}