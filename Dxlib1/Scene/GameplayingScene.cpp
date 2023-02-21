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
#include "../Game/BlinkyEnemy.h"
#include "../Game/InkyEnemy.h"
#include "../Game/CrydeEnemy.h"
#include "../Game/PinkyEnemy.h"
#include "../Game/EnemyBase.h"
#include "../Game.h"
#include <DxLib.h>

GameplayingScene::GameplayingScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&GameplayingScene::FadeInUpdate),
	life_(3),
	timer_(0),
	clearOrOver_(false)
{
	playerH_ = my::MyLoadGraph(L"Data/img/game/Pacman16.png");
	int deadPlayerH = my::MyLoadGraph(L"Data/img/game/PacmanDeath16.png");
	pPlayer_ = std::make_shared<Player>(playerH_, deadPlayerH);

	pField_ = std::make_shared<Field>();

	int blinkyEnemyH_ = my::MyLoadGraph(L"Data/img/game/blinky.png");
	int inkyEnemyH_ = my::MyLoadGraph(L"Data/img/game/inky.png");
	int crydeEnemyH_ = my::MyLoadGraph(L"Data/img/game/cryde.png");
	int pinkyEnemyH_ = my::MyLoadGraph(L"Data/img/game/pinky.png");

	pEnemy_[EnemyBase::blinky] = std::make_shared<BlinkyEnemy>(blinkyEnemyH_, 9, 8);
	pEnemy_[EnemyBase::inky] = std::make_shared<InkyEnemy>(inkyEnemyH_, 8, 10);
	pEnemy_[EnemyBase::cryde] = std::make_shared<CrydeEnemy>(crydeEnemyH_, 9, 10);
	pEnemy_[EnemyBase::pinky] = std::make_shared<PinkyEnemy>(pinkyEnemyH_, 10, 10);

	for (auto& enemy : pEnemy_)
	{
		enemy->SetPlayer(pPlayer_);
		enemy->SetField(pField_);
	}
	
	pPlayer_->SetField(pField_);
	pField_->SetPlayer(pPlayer_);

	for (int i = 0; i < EnemyBase::enemy_num; i++)
	{
		pPlayer_->SetEnemy(pEnemy_[i], i);
		pField_->SetEnemy(pEnemy_[i], i);
	}
}


void GameplayingScene::FadeInUpdate(const InputState& input)
{
	for (auto& enemy : pEnemy_)
	{
		enemy->SetEnabled(false);
	}

	pPlayer_->SetEnabled(false);

	fadeValue_ = 255 * (static_cast<float>(fadeTimer_)) / static_cast<float>(fade_interval);
	if (--fadeTimer_ == 0)
	{
		for (auto& enemy : pEnemy_)
		{
			enemy->SetEnabled(true);
		}

		pPlayer_->SetEnabled(true);

		fadeValue_ = 0;

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
			// 敵がイジケ状態ではないときに敵と当たった場合
			if (!enemy->GetIzike())
			{
				// 残機 - 1
				life_--;

				// プレイヤーの死亡フラグを立てる
				pPlayer_->SetDead(true);

				// すべての敵を消す
				for (auto& enemy : pEnemy_)
				{
					enemy->SetEnabled(false);
				}

				// ゲームオーバー演出移行
				updateFunc_ = &GameplayingScene::PlayerDeadUpdate;
				fadeColor_ = 0xff0000;
			}
			// 敵がイジケ状態の場合に敵と当たった場合敵を殺す
			else
			{
				// 敵の死亡フラグを立てる
				enemy->SetDead(true);

				// プレイヤーとあたった敵を消す
				enemy->SetEnabled(false);

				// 敵を消す
				pPlayer_->SetEnabled(false);

				updateFunc_ = &GameplayingScene::EnemyDeadUpdate;
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
		updateFunc_ = &GameplayingScene::GameClearUpdate;
		fadeColor_ = 0xff0000;
	}
}

void GameplayingScene::EnemyDeadUpdate(const InputState& input)
{
	timer_++;

	if (timer_ % 60 == 0)
	{
		for (auto& enemy : pEnemy_)
		{
			enemy->SetEnabled(true);
		}

		pPlayer_->SetEnabled(true);

		timer_ = 0;

		updateFunc_ = &GameplayingScene::NormalUpdate;
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

	for (int i = 0; i < life_; i++)
	{
		int x = Game::kScreenWidth / 2 + i * 40;

		DrawRectRotaGraph(x, Game::kScreenHeight - 16, 32, 0, 16, 16, 2.0f, 0.0f, playerH_, true);
	}

	DrawString(0, 0, L"GamePlayingScene", 0xffffff, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameplayingScene::GameClearUpdate(const InputState& input)
{
	updateFunc_ = &GameplayingScene::FadeOutUpdate;
}

void GameplayingScene::GameOverUpdate(const InputState& input)
{
	updateFunc_ = &GameplayingScene::FadeOutUpdate;
}

void GameplayingScene::PlayerDeadUpdate(const InputState& input)
{
	pPlayer_->DeadUpdate();

	if (pPlayer_->GetAnimeEnd())
	{
		a_ = true;
		updateFunc_ = &GameplayingScene::FadeOutUpdate;
	}
}

void GameplayingScene::FadeOutUpdate(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));

	fadeTimer_++;

	// ゲーム
	if (fadeTimer_ > fade_interval && !clearOrOver_ && life_ == 0)
	{
		manager_.ChangeScene(new GameoverScene(manager_));
		return;
	}
	else if (fadeTimer_ > fade_interval && clearOrOver_)
	{
		manager_.ChangeScene(new GameclearScene(manager_));
		return;
	}
	else if (fadeTimer_ > fade_interval && a_)
	{
		for (auto& enemy : pEnemy_)
		{
			enemy->Init();
			enemy->SetInit();
		}

		pPlayer_->Init();

		pField_->Init();

		SetInit();

		updateFunc_ = &GameplayingScene::FadeInUpdate;
	}
}

void GameplayingScene::SetInit()
{
	a_ = false;
	fadeTimer_ = fade_interval;
	fadeValue_ = 255;
	timer_ = 0;
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