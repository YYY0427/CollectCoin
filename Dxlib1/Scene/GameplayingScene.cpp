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
#include "../Game/Skeleton.h"
#include "../Game/Slime.h"
#include "../Game/Ghost.h"
#include "../Game/Golem.h"
#include "../Game/EnemyBase.h"
#include "../Game.h"
#include "../Game/Map.h"
#include "../Game/BackGround.h"
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

	// 準備中
	const char* const REDY_STRING = "REDEY";

	// ゲームクリアー
	const char* const GAMECLEAR_STRING = "GAME CLEAR!!!";

	// ゲームオーバー文字列
	const char* const GAMEOVER_STRING = "GAME OVER!!!";
}

GameplayingScene::GameplayingScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&GameplayingScene::FadeInUpdate),
	life_(1),
	timer_(0),
	gameOverTimer_(0),
	gameClearTimer_(0),
	preparTimer_(0),
	isGameClear_(false),
	faideEnabled_(false)
{
	// フォントの作成
	gameOverH_ = CreateFontToHandle("PixelMplus10", 20, 10);
	gameClearH_ = CreateFontToHandle("PixelMplus10", 20, 10);
	readyH_ = CreateFontToHandle("PixelMplus10", 20, 10);

	// サウンドのロード
	int coinSoundH = my::MyLoadSound("Data/sound/coin.wav");
	int sordSoundH = my::MyLoadSound("Data/sound/sord.wav");
	int powerDownSoundH = my::MyLoadSound("Data/sound/powerDown.wav");
	killSoundH_ = my::MyLoadSound("Data/sound/kill.wav");
	enemyAttackSoundH_ = my::MyLoadSound("Data/sound/enemyAttack.wav");

	// 画像のロード
	int nowaponPlayerH = my::MyLoadGraph("Data/img/game/nowapon-player.png");
	int waponPlayerH = my::MyLoadGraph("Data/img/game/wapon-player.png");
	int deadPlayerH = my::MyLoadGraph("Data/img/game/player-deth.png");
	int attackPlayerH = my::MyLoadGraph("Data/img/game/player-attack.png");

	int skeletonH = my::MyLoadGraph("Data/img/game/skeleton_walk.png");
 	int slimeH = my::MyLoadGraph("Data/img/game/slime.png");
	int ghostH = my::MyLoadGraph("Data/img/game/whiteGhost.png");
	int golemH = my::MyLoadGraph("Data/img/game/golem.png");

	int mapChipH = my::MyLoadGraph("Data/img/game/mapchip.png");
	int backGraph = my::MyLoadGraph("Data/img/game/Gray.png");
	lifeH_ = my::MyLoadGraph("Data/img/game/hart.png");


	pEnemy_[EnemyBase::skeleton] = std::make_shared<Skeleton>(
								skeletonH,					// 画像ハンドル
								SKELETON_START_INDEX_X,		// 初期座標X
								SKELETON_START_INDEX_Y);	// 初期座標Y
	pEnemy_[EnemyBase::slime] = std::make_shared<Slime>(
								slimeH,						// 画像ハンドル
								SLIME_START_INDEX_X,		// 初期座標X
								SLIME_START_INDEX_Y);		// 初期座標Y
	pEnemy_[EnemyBase::ghost] = std::make_shared<Ghost>(
								ghostH,						// 画像ハンドル
								GHOST_START_INDEX_X,		// 初期座標X
								GHOST_START_INDEX_Y);		// 初期座標Y
	pEnemy_[EnemyBase::golem] = std::make_shared<Golem>(
								golemH,						// 画像ハンドル
								GOLEM_START_INDEX_X,		// 初期座標X
								GOLEM_START_INDEX_Y);		// 初期座標Y

	pPlayer_ = std::make_shared<Player>(nowaponPlayerH, waponPlayerH, deadPlayerH, attackPlayerH,
										powerDownSoundH, PLAYER_START_INDEX_X, PLAYER_START_INDEX_Y);
	pField_ = std::make_shared<Field>(coinSoundH, sordSoundH);
	pMap_ = std::make_shared<Map>(mapChipH);
	pBackGround_ = std::make_shared<BackGround>(backGraph);

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

GameplayingScene::~GameplayingScene()
{
	
}

void GameplayingScene::FadeInUpdate(const InputState& input)
{
	for (auto& enemy : pEnemy_)
	{
		enemy->SetEnabled(false);
	}

	pPlayer_->SetEnabled(false);

	fadeValue_ = 255 * (static_cast<float>(fadeTimer_)) / static_cast<float>(fade_interval);
	ChangeVolumeSoundMem(255 - fadeValue_, bgmSoundH_);
	if (--fadeTimer_ == 0)
	{
		faideEnabled_ = true;

		fadeValue_ = 0;

		preparTimer_ = 120;
		updateFunc_ = &GameplayingScene::PrepareUpdate;
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

	pBackGround_->Update();

	pField_->Updata();

	pPlayer_->Update(input);

	for (auto& enemy : pEnemy_)
	{
		enemy->Update();
	}

	for (auto& enemy : pEnemy_)
	{
		// プレイヤーと敵の当たり判定
		if (Colision(enemy, enemy->GetSizeX(), enemy->GetSizeY()))
		{
			// 敵がイジケ状態ではないときに敵と当たった場合
			if (!enemy->GetIzike())
			{
				// 残機 - 1
				life_--;

				// プレイヤーの死亡フラグを立てる
				pPlayer_->SetDead(true);

				//// すべての敵を消す
				//for (auto& enemy : pEnemy_)
				//{
				//	enemy->SetEnabled(false);
				//}

				fadeColor_ = 0xff0000;

				PlaySoundMem(enemyAttackSoundH_, DX_PLAYTYPE_BACK);

				// 残機がなかった場合
				if (life_ <= 0)
				{
					updateFunc_ = &GameplayingScene::GameOverUpdate;
				}
				else if (life_ > 0)
				{
					// プレイヤー死亡演出移行
					updateFunc_ = &GameplayingScene::PlayerDeadUpdate;
				}
			}
			// 敵がイジケ状態の場合に敵と当たった場合敵を殺す
			else
			{
				// 敵の死亡フラグを立てる
				enemy->SetDead(true);

				PlaySoundMem(killSoundH_, DX_PLAYTYPE_BACK);

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
	pPlayer_->SetKill(true);

	pPlayer_->EnemyKillUpdate();

	if (pPlayer_->GetAnimeEnd())
	{
		pPlayer_->SetAnimeEnd(false);
		pPlayer_->SetKill(false);
		pPlayer_->SetAttackIdx(0);

		updateFunc_ = &GameplayingScene::NormalUpdate;
	}
}

void GameplayingScene::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void GameplayingScene::Draw()
{
	// 背景の描画
	pBackGround_->Draw();

	// マップの描画
	pMap_->Draw();

	// フィールド(剣、コイン)の描画
	pField_->Draw();

	// 敵の描画
	for (auto& enemy : pEnemy_)
	{
		enemy->Draw();
	}

	// プレイヤーの描画
	pPlayer_->Draw();

	// ゲームオーバー
	if (isGameOver_)
	{
		int width = GetDrawStringWidthToHandle(GAMEOVER_STRING, strlen(GAMEOVER_STRING), gameOverH_);

		// ゲームオーバー文字の表示
		DrawStringToHandle((Game::kScreenWidth / 2) - (width / 2), Game::kScreenHeight / 2 + 40,
			GAMEOVER_STRING, 0xffffff, gameOverH_, false);
	}
	// ゲームクリア
	if (isGameClear_)
	{
		int width = GetDrawStringWidthToHandle(GAMECLEAR_STRING, strlen(GAMECLEAR_STRING), gameOverH_);

		// ゲームクリア文字の表示
		DrawStringToHandle((Game::kScreenWidth / 2) - (width / 2), Game::kScreenHeight / 2 + 40,
			GAMECLEAR_STRING, 0xffffff, gameClearH_, false);
	}
	if (preparTimer_ > 0)
	{
		int width = GetDrawStringWidthToHandle(REDY_STRING, strlen(REDY_STRING), readyH_);
		// 準備中文字の表示
		DrawStringToHandle((Game::kScreenWidth / 2) - (width / 2), Game::kScreenHeight / 2 + 40,
			REDY_STRING, 0xffffff, readyH_, false);
	}

	// 残機の描画
	for (int i = 0; i < life_; i++)
	{
		int x = Game::kScreenWidth / 2 + 100 + (i * 40);

		DrawRectRotaGraph(x, Game::kScreenHeight - 30, 0, 0, 16, 16, 2.0f, 0.0f, lifeH_, true);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameplayingScene::GameClearUpdate(const InputState& input)
{
	isGameClear_ = true;
	gameClearTimer_++;
	if (gameClearTimer_ % 180 == 0)
	{
		updateFunc_ = &GameplayingScene::FadeOutUpdate;
	}
}

void GameplayingScene::GameOverUpdate(const InputState& input)
{
	if (timer_++ > 60)
	{
		// すべての敵を消す
		for (auto& enemy : pEnemy_)
		{
			enemy->SetEnabled(false);
		}

		if (!pPlayer_->GetAnimeEnd())
		{
			pPlayer_->DeadUpdate();
		}
		else
		{
			isGameOver_ = true;
			gameOverTimer_++;

			if (gameOverTimer_ % 180 == 0)
			{
				updateFunc_ = &GameplayingScene::FadeOutUpdate;
			}
		}
	}
}

void GameplayingScene::PlayerDeadUpdate(const InputState& input)
{
	if (timer_++ > 60)
	{
		// すべての敵を消す
		for (auto& enemy : pEnemy_)
		{
			enemy->SetEnabled(false);
		}

		pPlayer_->DeadUpdate();
	}

	if (pPlayer_->GetAnimeEnd())
	{
		updateFunc_ = &GameplayingScene::FadeOutUpdate;
	}
}

void GameplayingScene::FadeOutUpdate(const InputState& input)
{
	fadeTimer_++;
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));

	ChangeVolumeSoundMem(255 - fadeValue_, bgmSoundH_);

	// ゲームオーバー
	if (fadeTimer_ > fade_interval && isGameOver_)
	{
		manager_.ChangeScene(new TitleScene(manager_));
		return;
	}
	// ゲームクリアー
	else if (fadeTimer_ > fade_interval && isGameClear_)
	{
		manager_.ChangeScene(new TitleScene(manager_));
		return;
	}
	// 残機が１減った
	else if (fadeTimer_ > fade_interval && life_ > 0)
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

void GameplayingScene::PrepareUpdate(const InputState& input)
{
	preparTimer_--;
	if (preparTimer_ <= 0)
	{
		updateFunc_ = &GameplayingScene::NormalUpdate;
		preparTimer_ = 0;
	}
}

void GameplayingScene::SetInit()
{
	fadeTimer_ = fade_interval;
	fadeValue_ = 255;
	timer_ = 0;

}

bool GameplayingScene::Colision(std::shared_ptr<EnemyBase>enemy, int width, int height)
{
	float playerLeft = pPlayer_->GetPos().x;
	float playerRight = pPlayer_->GetPos().x + width;
	float playerTop = pPlayer_->GetPos().y;
	float playerBottom = pPlayer_->GetPos().y + width;

	float enemyLeft = enemy->GetPos().x;
	float enemyRight = enemy->GetPos().x + height;
	float enemyTop = enemy->GetPos().y;
	float enemyBottom = enemy->GetPos().y + height;

	if (playerLeft > enemyRight)	return false;
	if (playerRight < enemyLeft)	return false;
	if (playerTop > enemyBottom)	return false;
	if (playerBottom < enemyTop)	return false;

	return true;
}