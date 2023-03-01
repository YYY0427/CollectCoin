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
#include "../Game/Map.h"
#include <DxLib.h>

namespace
{
	// プレイヤーの初期位置
	constexpr int PLAYER_START_INDEX_X = 9;
	constexpr int PLAYER_START_INDEX_Y = 16;

	// 敵(スケルトン)の初期位置
	constexpr int SKELETON_START_INDEX_X = 9;
	constexpr int SKELETON_START_INDEX_Y = 8;

	// 敵(スライム)の初期位置
	constexpr int SLIME_START_INDEX_X = 8;
	constexpr int SLIME_START_INDEX_Y = 10;

	// 敵(幽霊)の初期位置
	constexpr int GHOST_START_INDEX_X = 9;
	constexpr int GHOST_START_INDEX_Y = 10;

	// 敵(ゴーレム)の初期位置
	constexpr int GOLEM_START_INDEX_X = 10;
	constexpr int GOLEM_START_INDEX_Y = 10;
}

GameplayingScene::GameplayingScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&GameplayingScene::FadeInUpdate),
	life_(3),
	timer_(0),
	clearOrOver_(false),
	faideEnabled_(false)
{
	int nowaponPlayerH = my::MyLoadGraph(L"Data/img/game/nowapon-player.png");
	int waponPlayerH = my::MyLoadGraph(L"Data/img/game/wapon-player.png");
	int deadPlayerH = my::MyLoadGraph(L"Data/img/game/player-deth.png");

	int skeletonH = my::MyLoadGraph(L"Data/img/game/skeleton_walk.png");
 	int slimeH = my::MyLoadGraph(L"Data/img/game/slime.png");
	int ghostH = my::MyLoadGraph(L"Data/img/game/whiteGhost.png");
	int golemH = my::MyLoadGraph(L"Data/img/game/golem.png");

	int mapChipH = my::MyLoadGraph(L"Data/img/game/mapchip.png");
	hartH_ = my::MyLoadGraph(L"Data/img/game/hart.png");


	pEnemy_[EnemyBase::skeleton] = std::make_shared<BlinkyEnemy>(
								skeletonH,					// 画像ハンドル
								SKELETON_START_INDEX_X,		// 初期座標X
								SKELETON_START_INDEX_Y);	// 初期座標Y
	pEnemy_[EnemyBase::slime] = std::make_shared<InkyEnemy>(
								slimeH,						// 画像ハンドル
								SLIME_START_INDEX_X,		// 初期座標X
								SLIME_START_INDEX_Y);		// 初期座標Y
	pEnemy_[EnemyBase::ghost] = std::make_shared<CrydeEnemy>(
								ghostH,						// 画像ハンドル
								GHOST_START_INDEX_X,		// 初期座標X
								GHOST_START_INDEX_Y);		// 初期座標Y
	pEnemy_[EnemyBase::golem] = std::make_shared<PinkyEnemy>(
								golemH,						// 画像ハンドル
								GOLEM_START_INDEX_X,		// 初期座標X
								GOLEM_START_INDEX_Y);		// 初期座標Y

	pPlayer_ = std::make_shared<Player>(nowaponPlayerH, waponPlayerH, deadPlayerH,
								PLAYER_START_INDEX_X, PLAYER_START_INDEX_Y);
	pField_ = std::make_shared<Field>();
	pMap_ = std::make_shared<Map>(mapChipH);

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
		faideEnabled_ = true;

		fadeValue_ = 0;

		updateFunc_ = &GameplayingScene::NormalUpdate;
	}
}

void GameplayingScene::NormalUpdate(const InputState& input)
{
	if (faideEnabled_)
	{
		for (auto& enemy : pEnemy_)
		{
			enemy->SetEnabled(true);
		}

		pPlayer_->SetEnabled(true);

		faideEnabled_ = false;
	}

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
	// マップの描画
	pMap_->Draw();

	// フィールド(剣、コイン)の描画
	pField_->Draw();

	// プレイヤーの描画
	pPlayer_->Draw();

	// 敵の描画
	for (auto& enemy : pEnemy_)
	{
		enemy->Draw();
	}

	// 残機の描画
	for (int i = 0; i < life_; i++)
	{
		int x = Game::kScreenWidth / 2 + 100 + (i * 40);

		DrawRectRotaGraph(x, Game::kScreenHeight - 16, 0, 0, 16, 16, 2.0f, 0.0f, hartH_, true);
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
	pPlayer_->DeadUpdate();

	if (pPlayer_->GetAnimeEnd())
	{
		updateFunc_ = &GameplayingScene::FadeOutUpdate;
	}
}

void GameplayingScene::PlayerDeadUpdate(const InputState& input)
{
	pPlayer_->DeadUpdate();

	if (pPlayer_->GetAnimeEnd())
	{
		isAnimeEnd_ = true;
		updateFunc_ = &GameplayingScene::FadeOutUpdate;
	}
}

void GameplayingScene::FadeOutUpdate(const InputState& input)
{
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));

	fadeTimer_++;

	// ゲームオーバー
	if (fadeTimer_ > fade_interval && !clearOrOver_ && life_ == 0)
	{
		manager_.ChangeScene(new TitleScene(manager_));
		return;
	}
	// ゲームクリアー
	else if (fadeTimer_ > fade_interval && clearOrOver_)
	{
		manager_.ChangeScene(new GameclearScene(manager_));
		return;
	}
	// 残機が１減った
	else if (fadeTimer_ > fade_interval && isAnimeEnd_)
	{
		for (auto& enemy : pEnemy_)
		{
			enemy->Init();
			enemy->SetInit();
		}

		// 初期化
		pPlayer_->Init();

		pField_->Init();

		SetInit();

		updateFunc_ = &GameplayingScene::FadeInUpdate;
	}
}

void GameplayingScene::SetInit()
{
	isAnimeEnd_ = false;
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