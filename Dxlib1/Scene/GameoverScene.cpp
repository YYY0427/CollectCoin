#include "GameoverScene.h"
#include "../InputState.h"
#include "TitleScene.h"
#include "SceneManager.h"
#include "../DrawFunctions.h"
#include <DxLib.h>

void GameoverScene::FadeInUpdate(const InputState& input)
{
	fadeValue_ = 255 * static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval);
	if (--fadeTimer_ == 0)
	{
		updateFunc_ = &GameoverScene::NormalUpdate;
	}
}

void GameoverScene::NormalUpdate(const InputState& input)
{
	if (input.IsTriggered(InputType::next))
	{
		updateFunc_ = &GameoverScene::FadeOutUpdate;
		fadeColor_ = 0xff0000;
	}
	if (input.IsTriggered(InputType::prev))
	{
		manager_.ChangeScene(new TitleScene(manager_));
		return;
	}
}

void GameoverScene::FadeOutUpdate(const InputState& input)
{
	fadeValue_ = 255 * static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval);
	if (++fadeTimer_ == fade_interval)
	{
		manager_.ChangeScene(new TitleScene(manager_));
		return;
	}
}

GameoverScene::GameoverScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&GameoverScene::FadeInUpdate)
{
	gameOverH_ = my::MyLoadGraph(L"Data/img/GameOver.png");
}


void GameoverScene::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void GameoverScene::Draw()
{
	DrawString(0, 0, L"GameOverScene", 0xffffff, true);
	DrawRotaGraph(320, 240, 0.5, 0.0, gameOverH_, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, 640, 480, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
