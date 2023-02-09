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
#include "../Game/CapriciousEnemy.h"
#include "../Game/InconsistentEnemy.h"
#include "../Game/PreemptiveEnemy.h"
#include "../Game/EnemyBase.h"
#include <DxLib.h>

GameplayingScene::GameplayingScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&GameplayingScene::FadeInUpdate)
{
	pField_ = std::make_shared<Field>();
	pPlayer_ = std::make_shared<Player>();

	int chasingEnemyH = my::MyLoadGraph(L"Data/img/game/blinky.png");
	int capriciousEnemyH = my::MyLoadGraph(L"Data/img/game/inky.png");
	int inconsistentEnemyH = my::MyLoadGraph(L"Data/img/game/cryde.png");
	int preemptiveEnemyH = my::MyLoadGraph(L"Data/img/game/pinky.png");

	pEnemy_[0] = std::make_shared<ChasingEnemy>(chasingEnemyH, 9, 8);
	pEnemy_[1] = std::make_shared<CapriciousEnemy>(capriciousEnemyH, 8, 10);
	pEnemy_[2] = std::make_shared<InconsistentEnemy>(inconsistentEnemyH, 9, 10);
	pEnemy_[3] = std::make_shared<PreemptiveEnemy>(preemptiveEnemyH, 10, 10);

	for (auto& enemy : pEnemy_)
	{
		enemy->SetPlayer(pPlayer_);
		enemy->SetField(pField_);
	}
	
	for (int i = 0; i < 4; i++)
	{
		pPlayer_->SetEnemy(pEnemy_[i], i);
	}

	pPlayer_->SetField(pField_);
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

	pPlayer_->Update(input);

	for (auto& enemy : pEnemy_)
	{
		enemy->Update();
	}

	for (auto& enemy : pEnemy_)
	{
		// プレイヤーと敵の当たり判定
		if (Colision(enemy))
		{
			// 敵がイジケ状態ではないときに敵と当たった場合死亡
			if (!enemy->GetIzike())
			{
				// プレイヤーの死亡フラグを立てる
				pPlayer_->SetDead(true);

				// 敵を消すフラグを立てる
				enemy->SetEnabled(true);

				// ゲームオーバー演出に移行
				updateFunc_ = &GameplayingScene::GameOverDraw;
				fadeColor_ = 0xff0000;
			}
			// 敵がイジケ状態の場合に敵と当たった場合敵を殺す
			else
			{
				// 敵の死亡フラグを立てる
				enemy->SetDead(true);
			}
		}
	}

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
}

void GameplayingScene::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void GameplayingScene::Draw()
{
	pField_->Draw();

	pPlayer_->Draw();

	for (auto& enemy : pEnemy_)
	{
		enemy->Draw();
	}

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

	if (pPlayer_->GetAnimeEnd())
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

bool GameplayingScene::Colision(std::shared_ptr<EnemyBase>enemy)
{
	float playerLeft = pPlayer_->GetPos().x;
	float playerRight = pPlayer_->GetPos().x + 16.0f;
	float playerTop = pPlayer_->GetPos().y;
	float playerBottom = pPlayer_->GetPos().y + 16.0f;

	float enemyLeft = enemy->GetPos().x;
	float enemyRight = enemy->GetPos().x + 16.0f;
	float enemyTop = enemy->GetPos().y;
	float enemyBottom = enemy->GetPos().y + 16.0f;

	if (playerLeft > enemyRight)	return false;
	if (playerRight < enemyLeft)	return false;
	if (playerTop > enemyBottom)	return false;
	if (playerBottom < enemyTop)	return false;

	return true;
}