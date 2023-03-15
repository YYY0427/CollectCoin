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
#include "../SoundManager.h"
#include "../Game/Coin.h"
#include "../Particle.h"
#include <DxLib.h>
#include <cassert>

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
	const char* const GAMEOVER_STRING = "GAME OVER...";
}

GameplayingScene::GameplayingScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&GameplayingScene::FadeInUpdate),
	life_(3),
	timer_(0),
	gameOverTimer_(0),
	gameClearTimer_(0),
	preparTimer_(0),
	quakeTimer_(0),
	quakeX_(0.0f),
	quakeY_(0.0f),
	isGameClear_(false),
	faideEnabled_(false),
	playerDeadSound_(false)
{
	// 画面幅　画面高　ビット数
	int sw, sh, bit;
	// 幅と高さを取得
	GetScreenState(&sw, &sh, &bit);
	// 加工用画面を用意
	tempScreenH_ = MakeScreen(sw, sh);
	// 作れなっかた場合ここで停止
	assert(tempScreenH_ >= 0);

	// フォントの作成
	gameOverH_ = CreateFontToHandle("PixelMplus10", 50, 30);
	gameOverShadowH_ = CreateFontToHandle("PixelMplus10", 51, 30);
	gameClearH_ = CreateFontToHandle("PixelMplus10", 50, 30);
	gameClearShadowH_ = CreateFontToHandle("PixelMplus10", 51, 30);
	readyH_ = CreateFontToHandle("PixelMplus10", 20, 10);

	// 画像のロード
	int nowaponPlayerH = my::MyLoadGraph("Data/img/game/nowapon-player.png");
	int waponPlayerH = my::MyLoadGraph("Data/img/game/wapon-player_gold.png");
	int deadPlayerH = my::MyLoadGraph("Data/img/game/player-deth1.png");
	int attackPlayerH = my::MyLoadGraph("Data/img/game/player-attack_gold.png");

	int skeletonH = my::MyLoadGraph("Data/img/game/skeleton_monokuro.png");
 	int slimeH = my::MyLoadGraph("Data/img/game/slime_monokuro.png");
	int ghostH = my::MyLoadGraph("Data/img/game/ghost_monokuro.png");
	int golemH = my::MyLoadGraph("Data/img/game/golem_monokuro.png");

	int mapChipH = my::MyLoadGraph("Data/img/game/mapchip1.png");
	int backGraph = my::MyLoadGraph("Data/img/game/Gray.png");

	int sordH_ =  my::MyLoadGraph("Data/img/game/sord_gold.png");
	int doorH_ =  my::MyLoadGraph("Data/img/game/door.png");
	lifeH_ = my::MyLoadGraph("Data/img/game/hart.png");
	coinH_ = my::MyLoadGraph("Data/img/game/coin.png");


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

	pPlayer_ = std::make_shared<Player>(nowaponPlayerH, waponPlayerH, deadPlayerH, attackPlayerH, PLAYER_START_INDEX_X, PLAYER_START_INDEX_Y);
	pField_ = std::make_shared<Field>(sordH_, doorH_, coinH_);
	pMap_ = std::make_shared<Map>(mapChipH);
	pBackGround_ = std::make_shared<BackGround>(backGraph);

	for (auto& circle : pParticle_)
	{
		circle = std::make_shared<Particle>();
	}

	for (auto& enemy : pEnemy_)
	{
		enemy->SetPlayer(pPlayer_);
		enemy->SetField(pField_);
	}
	
	pBackGround_->SetPlayer(pPlayer_);
	pPlayer_->SetField(pField_);
	pField_->SetPlayer(pPlayer_);

	for (int i = 0; i < EnemyBase::enemy_num; i++)
	{
		pPlayer_->SetEnemy(pEnemy_[i], i);
		pField_->SetEnemy(pEnemy_[i], i);
	}

	SoundManager::GetInstance().PlayMusic("Data/sound/BGM/game.mp3");
	SetVolumeMusic(0);
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
	SetVolumeMusic(static_cast<int>(255.0f / 60.0f * static_cast<float>(60 - fadeTimer_)) * (static_cast<float>(SoundManager::GetInstance().GetBGMVolume() / 255.0f)));
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_)) / static_cast<float>(fade_interval);
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
			// プレイヤーがイジケ状態ではないときに敵と当たった場合
			if (!enemy->GetIzike())
			{
				// BGMを止める
				StopMusic();
				pPlayer_->StopMusic();

				// 残機 - 1
				life_--;

				// ゲームオーバーの場合画面の揺れを大きくする
				if (life_ <= 0)
				{
					quakeX_ = 40.0f;
					quakeY_ = 20.0f;
					quakeTimer_ = 80;
				}
				else
				{
					quakeX_ = 20.0f;
					quakeTimer_ = 60;
				}

				// プレイヤーの死亡フラグを立てる
				pPlayer_->SetDead(true);

				fadeColor_ = 0xff0000;

				SoundManager::GetInstance().Play("enemyAttack");

				playerDeadSound_ = true;

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

				SoundManager::GetInstance().Play("kill");

				updateFunc_ = &GameplayingScene::EnemyDeadUpdate;
			}
		}
	}

	if (quakeTimer_ > 0)
	{
		quakeX_ = -quakeX_;
		quakeX_ *= 0.95f;
		quakeY_ = -quakeY_;
		quakeY_ *= 0.95f;

		--quakeTimer_;
	}
	else
	{
		quakeX_ = 0.0f;
		quakeY_ = 0.0f;
	}

	// ポーズシーン切り替え
	if (input.IsTriggered(InputType::pause))
	{
		manager_.PushScene(new PauseScene(manager_));
	}

	// ゲームクリア判定
	if (pField_->IsGameClearCheck())
	{
		// BGMを止める
		StopMusic();
		pPlayer_->StopMusic();

		// ゲームクリア演出に移行
		updateFunc_ = &GameplayingScene::GameClearUpdate;
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
	// 加工用スクリーンハンドルをセット
	SetDrawScreen(tempScreenH_);

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

	for (auto& coin : pCoin_)
	{
		coin->Draw();
	}

	for (auto& circle : pParticle_)
	{
		circle->Draw();
	}

	// ゲームオーバー
	if (isGameOver_)
	{
		int stringWidth = GetDrawStringWidthToHandle(GAMEOVER_STRING, strlen(GAMEOVER_STRING), gameOverH_);
		int stringHeight = GetFontSizeToHandle(gameOverH_);

		int width = (Game::kScreenWidth / 2) - (stringWidth / 2);
		int height = (Game::kScreenHeight / 2) - (stringHeight / 2);

		// ゲームオーバー文字の表示
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, gameOverFadeValue_);
		DrawStringToHandle(width - 3, height,
			GAMEOVER_STRING, 0x000000, gameOverShadowH_, false);
		DrawStringToHandle(width, height,
			GAMEOVER_STRING, 0xffffff, gameOverH_, false);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	// ゲームクリア
	if (isGameClear_)
	{
		int stringWidth = GetDrawStringWidthToHandle(GAMECLEAR_STRING, strlen(GAMECLEAR_STRING), gameClearH_);
		int stringHeight = GetFontSizeToHandle(gameClearH_);

		int width = (Game::kScreenWidth / 2) - (stringWidth / 2);
		int height = (Game::kScreenHeight / 2) - (stringHeight / 2);

		// ゲームクリア文字の表示
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, gameClearFadeValue_);
		DrawStringToHandle(width - 3, height,
			GAMECLEAR_STRING, 0x000000, gameClearShadowH_, false);
		DrawStringToHandle(width, height,
			GAMECLEAR_STRING, 0xffffff, gameClearH_, false);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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
		int x = Game::kScreenWidth / 2 + 250 + (-i * 40);

		DrawRectRotaGraph(x, Game::kScreenHeight - 30, 0, 0, 16, 16, 2.5f, 0.0f, lifeH_, true);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(quakeX_, quakeY_, tempScreenH_, false);
	if (quakeTimer_ > 0)
	{
		GraphFilter(tempScreenH_, DX_GRAPH_FILTER_MONO, 0, 7);
		DrawGraph(quakeX_, quakeY_, tempScreenH_, false);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void GameplayingScene::GameClearUpdate(const InputState& input)
{
	/*for (auto& circle : pParticle_)
	{
		circle->Update();
	}*/

	if (gameClearTimer_ % 1 == 0)
	{
		pCoin_.push_back(std::make_shared<Coin>(coinH_));
		pCoin_.push_back(std::make_shared<Coin>(coinH_));
	}
	
	for (auto& coin : pCoin_)
	{
		coin->Update();
	}

	gameClearTimer_++;
	if (gameClearTimer_ == 60 && !isGameClear_)
	{
		SoundManager::GetInstance().PlayJingle("Data/sound/BGM/gameClear.ogg");
	}
	else if (gameClearTimer_ > 120 && !CheckMusic())
	{
		isGameClear_ = true;
		gameClearTimer_ = 120;
	}

	pPlayer_->ClearUpdate();
	if (isGameClear_)
	{
		gameClearTimer_ -= 2;
		gameClearFadeValue_ = 255 * (static_cast<float>(gameClearFadeTimer_)) / static_cast<float>(game_clear_fade_interval);

		if (++gameClearFadeTimer_ >= 255)
		{
			gameClearFadeValue_ = 255;
			if (gameClearTimer_ <= 0)
			{
				updateFunc_ = &GameplayingScene::FadeOutUpdate;
				gameClearTimer_ = 0;
			}
		}
	}
}

void GameplayingScene::GameOverUpdate(const InputState& input)
{
	if (quakeTimer_ > 0)
	{
		quakeX_ = -quakeX_;
		quakeX_ *= 0.95f;
		quakeY_ = -quakeY_;
		quakeY_ *= 0.95f;

		--quakeTimer_;
	}
	else
	{
		quakeX_ = 0.0f;
		quakeY_ = 0.0f;
	}

	if (timer_++ > 120)
	{
		// すべての敵を消す
		for (auto& enemy : pEnemy_)
		{
			enemy->SetEnabled(false);
		}

		if (!pPlayer_->GetAnimeEnd())
		{
			pPlayer_->DeadUpdate();

			if (playerDeadSound_)
			{
				SoundManager::GetInstance().PlayJingle("Data/sound/BGM/gameOver.wav");
				playerDeadSound_ = false;
			}
		}
		else if (pPlayer_->GetAnimeEnd() && !CheckMusic())
		{
			isGameOver_ = true;
		}
		if (isGameOver_)
		{
			gameOverFadeValue_ = 255 * (static_cast<float>(gameOverFadeTimer_)) / static_cast<float>(game_over_fade_interval);
			if (++gameOverFadeTimer_ >= 255)
			{
				gameOverFadeValue_ = 255;

				if (gameOverTimer_++ % 180 == 0)
				{
					updateFunc_ = &GameplayingScene::FadeOutUpdate;
				}
			}
		}
	}
}

void GameplayingScene::PlayerDeadUpdate(const InputState& input)
{
	if (quakeTimer_ > 0)
	{
		quakeX_ = -quakeX_;
		quakeX_ *= 0.95f;
		--quakeTimer_;
	}
	else
	{
		quakeX_ = 0.0f;
	}

	if (timer_++ > 60)
	{
		// すべての敵を消す
		for (auto& enemy : pEnemy_)
		{
			enemy->SetEnabled(false);
		}
		
		if (playerDeadSound_)
		{
			SoundManager::GetInstance().PlayJingle("Data/sound/BGM/playerDead.wav");
			playerDeadSound_ = false;
		}

		if (!pPlayer_->GetAnimeEnd())
		{
			pPlayer_->DeadUpdate();
		}
	}

	if (pPlayer_->GetAnimeEnd() && !CheckMusic())
	{
		updateFunc_ = &GameplayingScene::FadeOutUpdate;
	}
}

void GameplayingScene::FadeOutUpdate(const InputState& input)
{
	fadeTimer_++;
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fade_interval));
	SetVolumeMusic(static_cast<float>((std::max)(SoundManager::GetInstance().GetBGMVolume() - fadeValue_, 0)));

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

		SetVolumeMusic(0);
		SoundManager::GetInstance().PlayMusic("Data/sound/BGM/game.mp3");

		pPlayer_->StartMusic();

		updateFunc_ = &GameplayingScene::FadeInUpdate;
	}
}

void GameplayingScene::PrepareUpdate(const InputState& input)
{
	/*if (!SoundManager::GetInstance().Check("gameStart"))
	{
		preparTimer_--;
	}*/
	preparTimer_--;
	if (preparTimer_ <= 0)
	{
		/*SetVolumeMusic(static_cast<int>(255.0f / 60.0f * static_cast<float>(60 - bgmFadeTimer_)));
		bgmFadeValue_ = 255 * (static_cast<float>(bgmFadeTimer_)) / static_cast<float>(bgm_fade_interval);
		if (--bgmFadeTimer_ == 0)
		{
			updateFunc_ = &GameplayingScene::NormalUpdate;
			preparTimer_ = 0;
		}*/

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