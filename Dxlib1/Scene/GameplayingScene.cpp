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


GameplayingScene::GameplayingScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&GameplayingScene::FadeInUpdate)
{
	pField_ = std::make_shared<Field>();
	pPlayer_ = std::make_shared<Player>();
	pChasingEnemy_ = std::make_shared<ChasingEnemy>();
}


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

	// ポーズシーン切り替え
	if (input.IsTriggered(InputType::pause))
	{
		manager_.PushScene(new PauseScene(manager_));
	}

	// ゲームクリア判定
	if (pField_->IsGameClearCheck())
	{
		// ゲームクリア演出に移行
		updateFunc_ = &GameplayingScene::GameClearDraw;
		fadeColor_ = 0xff0000;
	}

	// ゲームオーバー判定
	if (Colision())
	{
		// プレイヤーの死亡フラグを立てる
		pPlayer_->SetDead(true);

		pChasingEnemy_->SetEnabled(true);

		// ゲームオーバー演出に移行
		updateFunc_ = &GameplayingScene::GameOverDraw;
		fadeColor_ = 0xff0000;
	}
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

void GameplayingScene::GameClearDraw(const InputState& input)
{
	updateFunc_ = &GameplayingScene::GameClearFadeOutUpdate;
}

void GameplayingScene::GameOverDraw(const InputState& input)
{
	pPlayer_->DeadUpdate();

	if (pPlayer_->GetEnd())
	{
		updateFunc_ = &GameplayingScene::GameOverFadeOutUpdate;
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

bool GameplayingScene::Colision()
{
	float playerLeft = pPlayer_->GetPos().x;
	float playerRight = pPlayer_->GetPos().x + 16.0f;
	float playerTop = pPlayer_->GetPos().y;
	float playerBottom = pPlayer_->GetPos().y + 16.0f;

	float enemyLeft = pChasingEnemy_->GetPos().x;
	float enemyRight = pChasingEnemy_->GetPos().x + 16.0f;
	float enemyTop = pChasingEnemy_->GetPos().y;
	float enemyBottom = pChasingEnemy_->GetPos().y + 16.0f;

	if (playerLeft > enemyRight)	return false;
	if (playerRight < enemyLeft)	return false;
	if (playerTop > enemyBottom)	return false;
	if (playerBottom < enemyTop)	return false;

	return true;
}