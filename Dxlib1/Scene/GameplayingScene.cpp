#include "GameplayingScene.h"
#include "../InputState.h"
#include "GameoverScene.h"
#include "GameclearScene.h"
#include "TitleScene.h"
#include "SceneManager.h"
#include "../DrawFunctions.h"
#include "PauseScene.h"
#include"../Game/Player.h"
#include "../Game/Field.h"
#include "../Game/ChasingEnemy.h"
#include <DxLib.h>

void GameplayingScene::FadeInUpdate(const InputState& input)
{
	fadeValue_ = 255 * static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval);
	if (--fadeTimer_ == 0)
	{
		updateFunc_ = &GameplayingScene::NormalUpdate;
	}
}
void GameplayingScene::NormalUpdate(const InputState& input)
{
	pField_->Updata();

	pChasingEnemy_->Update();

	pPlayer_->Update(input);

	// ゲームクリア判定
	if (pField_->IsGameClearCheck())
	{
		updateFunc_ = &GameplayingScene::GameClearFadeOutUpdate;
		fadeColor_ = 0xff0000;
	}
	// ゲームオーバー判定
	if (Colision())
	{
		updateFunc_ = &GameplayingScene::GameOverFadeOutUpdate;
		fadeColor_ = 0xff0000;
	}
	if (input.IsTriggered(InputType::prev))
	{
		manager_.ChangeScene(new TitleScene(manager_));
		return;
	}
	if (input.IsTriggered(InputType::pause))
	{
		manager_.PushScene(new PauseScene(manager_));
	}
}

void GameplayingScene::GameClearFadeOutUpdate(const InputState& input)
{
	fadeValue_ = 255 * static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval);
	if (++fadeTimer_ == fade_interval)
	{
		manager_.ChangeScene(new GameclearScene(manager_));
		return;
	}
}

void GameplayingScene::GameOverFadeOutUpdate(const InputState& input)
{
	fadeValue_ = 255 * static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval);
	if (++fadeTimer_ == fade_interval)
	{
		manager_.ChangeScene(new GameoverScene(manager_));
		return;
	}
}

GameplayingScene::GameplayingScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&GameplayingScene::FadeInUpdate)
{
	pField_ = std::make_shared<Field>();
	pPlayer_ = std::make_shared<Player>();
	pChasingEnemy_ = std::make_shared<ChasingEnemy>();
}

void GameplayingScene::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void GameplayingScene::Draw()
{
	pField_->Draw();

	pChasingEnemy_->Draw();

	pPlayer_->Draw();

	DrawString(0, 0, L"GamePlayingScene", 0xffffff, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, 640, 480, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

bool GameplayingScene::Colision()
{
	float playerLeft = pPlayer_->GetPos().x;
	float playerRight = pPlayer_->GetPos().x + 16;
	float playerTop = pPlayer_->GetPos().y;
	float playerBottom = pPlayer_->GetPos().y + 16;

	float enemyLeft = pChasingEnemy_->GetPos().x;
	float enemyRight = pChasingEnemy_->GetPos().x + 16;
	float enemyTop = pChasingEnemy_->GetPos().y;
	float enemyBottom = pChasingEnemy_->GetPos().y + 16;

	if (playerLeft > enemyRight)	return false;
	if (playerRight < enemyLeft)	return false;
	if (playerTop > enemyBottom)	return false;
	if (playerBottom < enemyTop)	return false;

	return true;
}