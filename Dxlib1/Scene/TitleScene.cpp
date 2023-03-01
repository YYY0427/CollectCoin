#include "TitleScene.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "GameplayingScene.h"
#include "OptionScene.h"
#include "../DrawFunctions.h"
#include "../Game.h"
#include <DxLib.h>

namespace
{
	constexpr int fade_interval = 60;

	// 選択肢の数
	constexpr int SELECTION_NUM = 3;
}

void TitleScene::FadeInUpdate(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));

	if (--fadeTimer_ == 0)
	{
		updateFunc_ = &TitleScene::NormalUpdate;
		fadeValue_ = 0;
	}
}

void TitleScene::NormalUpdate(const InputState& input)
{
	//選択肢を上下で回す処理
	if (input.IsTriggered(InputType::up))
	{
		currentInputIndex_ = ((currentInputIndex_ - 1) + SELECTION_NUM) % SELECTION_NUM;
	}
	else if (input.IsTriggered(InputType::down))
	{
		currentInputIndex_ = (currentInputIndex_ + 1) % SELECTION_NUM;
	}

	if (currentInputIndex_ == start)
		startH_ = selectionH_;
	else
		startH_ = normalSelectionH_;

	if (currentInputIndex_ == option)
		optionH_ = selectionH_;
	else
		optionH_ = normalSelectionH_;

	if (currentInputIndex_ == exsit)
		exsitH_ = selectionH_;
	else
		exsitH_ = normalSelectionH_;

	//次へのボタンが押されたら次のシーンへ行く
	if (input.IsTriggered(InputType::next))
	{
		if (currentInputIndex_ == start)
		{
			decisionIndex_ = start;
		}
		else if (currentInputIndex_ == option)
		{
			decisionIndex_ = option;
		}
		else if (currentInputIndex_ == exsit)
		{
			decisionIndex_ = exsit;
		}

		updateFunc_ = &TitleScene::FadeOutUpdate;
	}
}

void TitleScene::FadeOutUpdate(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));

	fadeTimer_++;

	if (fadeTimer_ == fade_interval && decisionIndex_ == start)
	{
		manager_.ChangeScene(new GameplayingScene(manager_));
		return;
	}
	else if (fadeTimer_ == fade_interval && decisionIndex_ == option)
	{
		manager_.ChangeScene(new OptionScene(manager_));
		return;
	}
	else if (fadeTimer_ == fade_interval && decisionIndex_ == exsit)
	{
		DxLib_End();
	}
}

TitleScene::TitleScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&TitleScene::FadeInUpdate),
	fadeTimer_(fade_interval),
	fadeValue_(255),
	currentInputIndex_(0),
	decisionIndex_(0)
{
	normalSelectionH_ = CreateFontToHandle(NULL, 30, 10);
	selectionH_ = CreateFontToHandle(NULL, 40, 10);
	titleH_ = CreateFontToHandle(NULL, 150, 10);

	startH_ = normalSelectionH_;
	optionH_ = normalSelectionH_;
	exsitH_ = normalSelectionH_;
}

TitleScene::~TitleScene()
{

}

void TitleScene::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void TitleScene::Draw()
{
	DrawString(0, 0, L"TitleScene", 0xffffff, true);

	int width1 = GetDrawStringWidthToHandle(L"GAME START", 10, startH_);
	int width2 = GetDrawStringWidthToHandle(L"OPTION", 6, optionH_);
	int width3 = GetDrawStringWidthToHandle(L"EXSIT", 5, exsitH_);
	int width4 = GetDrawStringWidthToHandle(L"SORD MAN", 8, titleH_);

	DrawStringToHandle((Game::kScreenWidth / 2) - (width1 / 2), Game::kScreenHeight / 2 + 150,
		L"GAME START", 0xffffff, startH_, false);
	DrawStringToHandle((Game::kScreenWidth / 2) - (width2 / 2), Game::kScreenHeight / 2 + 225,
		L"OPTION", 0xffffff, optionH_, false);
	DrawStringToHandle((Game::kScreenWidth / 2) - (width3 / 2), Game::kScreenHeight / 2 + 300,
		L"EXSIT", 0xffffff, exsitH_, false);

	DrawStringToHandle((Game::kScreenWidth / 2) - (width4 / 2), 100, L"SORD MAN", 0xffffff, titleH_, false);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}