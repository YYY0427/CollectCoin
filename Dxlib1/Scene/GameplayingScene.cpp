#include "GameplayingScene.h"
#include "../InputState.h"
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
#include <DxLib.h>
#include <cassert>

namespace
{
	// プレイヤーの初期位置
	constexpr int TUTORIAL_PLAYER_START_INDEX_X = 5;
	constexpr int TUTORIAL_PLAYER_START_INDEX_Y = 5;

	constexpr int PLAYER_START_INDEX_X = 9;
	constexpr int PLAYER_START_INDEX_Y = 16;

	// 敵(スケルトン)の初期位置
	constexpr int TUTORIAL_SKELETON_START_INDEX_X = 5;
	constexpr int TUTORIAL_SKELETON_START_INDEX_Y = 1;

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
	const char* const GAMECLEAR_STRING = "GAME CLEAR!";

	// ゲームオーバー文字列
	const char* const GAMEOVER_STRING = "GAME OVER...";

	// 選択肢の数
	constexpr int CHOICE_NUM = 3; 

	// 選択肢の箱座標
	constexpr int CB_WIDTH_1 = 400;												//枠の幅
	constexpr int CB_HEIGHT_1 = 100;											//枠の高さ
	constexpr int CB_START_X_1 = (Game::SCREEN_WIDTH / 2) - (CB_WIDTH_1 / 2);	//枠のスタートX座標
	constexpr int CB_START_Y_1= Game::SCREEN_HEIGHT / 2;						//枠のスタートY座標

	constexpr int CB_WIDTH_2 = 400;												
	constexpr int CB_HEIGHT_2 = 100;											
	constexpr int CB_START_X_2 = (Game::SCREEN_WIDTH / 2) - (CB_WIDTH_1 / 2);	
	constexpr int CB_START_Y_2 = Game::SCREEN_HEIGHT / 2 + 115;					

	constexpr int CB_WIDTH_3 = 400;												
	constexpr int CB_HEIGHT_3 = 100;											
	constexpr int CB_START_X_3 = (Game::SCREEN_WIDTH / 2) - (CB_WIDTH_1 / 2);	
	constexpr int CB_START_Y_3 = Game::SCREEN_HEIGHT / 2 + 230;	

	// フェードの速度
	constexpr int FADE_INTERVAL = 60;
	constexpr int BGM_FADE_INTERVAL = 60;
	constexpr int GAMEOVER_FADE_INTERVAL = 60;
	constexpr int GAMECLEAR_FADE_INTERVAL = 60;
}

GameplayingScene::GameplayingScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&GameplayingScene::FadeInUpdate),
	life_(0),
	waitTimer_(0),
	gameOverTimer_(0),
	gameClearTimer_(0),
	preparTimer_(0),
	quakeTimer_(0),
	quakeX_(0.0f),
	quakeY_(0.0f),
	stage_(0),
	currentInputIndex_(0),
	stringWidth_(0),
	stringHeight_(0),
	fadeTimer_(0),
	fadeValue_(0),
	bgmFadeTimer_(0),
	bgmFadeValue_(0),
	gameOverFadeTimer_(0),
	gameOverStringFadeValue_(0),
	gameClearFadeTimer_(0),
	gameClearStringFadeValue_(0),
	tempScreenH_(-1),
	gameOverH_(-1),
	gameOverShadowH_(-1),
	gameClearH_(-1),
	readyH_(-1),
	gameClearShadowH_(-1),
	cursor1H_(-1),
	cursor2H_(-1),
	cursor3H_(-1),
	cursor4H_(-1),
	playH_(-1),
	retryH_(-1),
	backH_(-1),
	skeletonH_(-1),
	slimeH_(-1),
	ghostH_(-1),
	golemH_(-1),
	lifeH_(-1),
	coinH_(-1),
	nowaponPlayerH_(-1),
	waponPlayerH_(-1),
	deadPlayerH_(-1),
	attackPlayerH_(-1),
	mapChipH_(-1),
	backGraphH_(-1),
	sordH_(-1),
	doorH_(-1),
	isGameClear_(false),
	faideEnabled_(false),
	playerDeadSound_(false),
	isNextStage_(false),
	isTitile_(false),
	isRetry_(false),
	isCoinEnabled_(false)
{
	Init();
}

GameplayingScene::~GameplayingScene()
{
	End();
}

void GameplayingScene::Init()
{
	// 画面揺らす用の画像の作成
	int sw, sh, bit;					// 画面幅　画面高　ビット数
	GetScreenState(&sw, &sh, &bit);		// 幅と高さを取得
	tempScreenH_ = MakeScreen(sw, sh);	// 加工用画面を用意
	assert(tempScreenH_ >= 0);			// 作れなっかた場合ここで停止

	// フォントの作成
	gameOverH_ = CreateFontToHandle("PixelMplus10", 100, 9);
	gameOverShadowH_ = CreateFontToHandle("PixelMplus10", 102, 9);
	gameClearH_ = CreateFontToHandle("PixelMplus10", 100, 9);
	gameClearShadowH_ = CreateFontToHandle("PixelMplus10", 102, 9);
	readyH_ = CreateFontToHandle("PixelMplus10", 100, 9);

	// 文字の幅、文字の高さの取得
	stringWidth_ = GetDrawStringWidthToHandle(GAMECLEAR_STRING, static_cast<int>(strlen(GAMECLEAR_STRING)), gameClearH_);
	stringHeight_ = GetFontSizeToHandle(gameClearH_);

	// ゲームクリア時の文字座標の設定
	gameClearPos_.x = static_cast<float>((Game::SCREEN_WIDTH / 2) - (stringWidth_ / 2));
	gameClearPos_.y = static_cast<float>((Game::SCREEN_HEIGHT / 2) - 220);

	// 画像のロード
	nowaponPlayerH_ = my::MyLoadGraph("Data/img/game/nowapon-player.png");
	waponPlayerH_ = my::MyLoadGraph("Data/img/game/wapon-player_gold.png");
	deadPlayerH_ = my::MyLoadGraph("Data/img/game/player-deth1.png");
	attackPlayerH_ = my::MyLoadGraph("Data/img/game/player-attack_gold.png");
	cursor1H_ = my::MyLoadGraph("Data/img/game/cursor1.png");
	cursor2H_ = my::MyLoadGraph("Data/img/game/cursor2.png");
	cursor3H_ = my::MyLoadGraph("Data/img/game/cursor3.png");
	cursor4H_ = my::MyLoadGraph("Data/img/game/cursor4.png");
	playH_ = my::MyLoadGraph("Data/img/play.png");
	retryH_ = my::MyLoadGraph("Data/img/retry.png");
	backH_ = my::MyLoadGraph("Data/img/back.png");
	skeletonH_ = my::MyLoadGraph("Data/img/game/skeleton_monokuro.png");
	slimeH_ = my::MyLoadGraph("Data/img/game/slime_monokuro.png");
	ghostH_ = my::MyLoadGraph("Data/img/game/ghost_monokuro.png");
	golemH_ = my::MyLoadGraph("Data/img/game/golem_monokuro.png");
	mapChipH_ = my::MyLoadGraph("Data/img/game/mapchip1.png");
	backGraphH_ = my::MyLoadGraph("Data/img/game/Gray.png");
	sordH_ = my::MyLoadGraph("Data/img/game/sord_gold.png");
	doorH_ = my::MyLoadGraph("Data/img/game/door.png");
	lifeH_ = my::MyLoadGraph("Data/img/game/hart.png");
	coinH_ = my::MyLoadGraph("Data/img/game/coin.png");

	// 初期化
	fadeTimer_ = FADE_INTERVAL;
	fadeValue_ = 255;
	bgmFadeTimer_ = FADE_INTERVAL;
	bgmFadeValue_ = 255;
	gameOverFadeTimer_ = 0;
	gameOverStringFadeValue_ = 0;
	gameClearFadeTimer_ = 0;
	gameClearStringFadeValue_ = 0;
	life_ = 0;
	waitTimer_ = 0;
	gameOverTimer_ = 0;
	gameClearTimer_ = 0;
	preparTimer_ = 0;
	quakeTimer_ = 0;
	quakeX_ = 0.0f;
	quakeY_ = 0.0f;
	currentInputIndex_ = 0;
	isGameClear_ = false;
	faideEnabled_ = false;
	playerDeadSound_ = false;
	isNextStage_ = false;
	isTitile_ = false;
	isRetry_ = false;
	isCoinEnabled_ = false;

	// ステージ
	stage_ = tutorial;		// どのステージ
	StageCheck(stage_);		// ステージによって座標の変更

	// メモリの確保
	pPlayer_ = std::make_shared<Player>(nowaponPlayerH_, waponPlayerH_, deadPlayerH_, attackPlayerH_, playerStartPosX_, playerStartPosY_, stage_);
	pField_ = std::make_shared<Field>(sordH_, doorH_, coinH_, stage_);
	pMap_ = std::make_shared<Map>(mapChipH_, stage_);
	pBackGround_ = std::make_shared<BackGround>(backGraphH_);

	// ポインタの設定
	pBackGround_->SetPlayer(pPlayer_);
	pPlayer_->SetField(pField_);
	pField_->SetPlayer(pPlayer_);
	pMap_->SetField(pField_);
	pField_->StageCheck(stage_);
	for (auto& enemy : pEnemy_)
	{
		enemy->SetPlayer(pPlayer_);
		enemy->SetField(pField_);
		enemy->Init(stage_);
	}

	// ステージによって敵の数を変更
	for (int i = 0; i < enemyNum_; i++)
	{
		pPlayer_->SetEnemy(pEnemy_[i], i, stage_);
		pField_->SetEnemy(pEnemy_[i], i);
	}

	// BGMを鳴らす
	SoundManager::GetInstance().PlayMusic("Data/sound/BGM/game.mp3");
	SetVolumeMusic(0);
}

void GameplayingScene::End()
{
	DeleteFontToHandle(gameOverH_);
	DeleteFontToHandle(gameOverShadowH_);
	DeleteFontToHandle(gameClearH_);
	DeleteFontToHandle(gameClearShadowH_);
	DeleteFontToHandle(readyH_);
	DeleteGraph(lifeH_);
	DeleteGraph(coinH_);
	DeleteGraph(tempScreenH_);
	DeleteGraph(skeletonH_);
	DeleteGraph(slimeH_);
	DeleteGraph(ghostH_);
	DeleteGraph(golemH_);
	DeleteGraph(playH_);
	DeleteGraph(retryH_);
	DeleteGraph(backH_);
	DeleteGraph(cursor1H_);
	DeleteGraph(cursor2H_);
	DeleteGraph(cursor3H_);
	DeleteGraph(cursor4H_);
	DeleteGraph(nowaponPlayerH_);
	DeleteGraph(waponPlayerH_);
	DeleteGraph(deadPlayerH_);
	DeleteGraph(attackPlayerH_);
	DeleteGraph(mapChipH_);
	DeleteGraph(backGraphH_);
	DeleteGraph(sordH_);
	DeleteGraph(doorH_);
}

void GameplayingScene::FadeInUpdate(const InputState& input)
{
	// フェード中はキャラクターを表示しない
	pPlayer_->SetEnabled(false);
	for (auto& enemy : pEnemy_)
	{
		enemy->SetEnabled(false);
	}

	// BGMのフェード処理
	SetVolumeMusic(static_cast<int>(255.0f / 60.0f * static_cast<float>(60 - fadeTimer_) * 
		(static_cast<float>(SoundManager::GetInstance().GetBGMVolume() / 255.0f))));

	// 画面のフェード処理
	fadeValue_ = 255 * (static_cast<int>(fadeTimer_)) / static_cast<int>(FADE_INTERVAL);

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
	// フェードが終わったのでキャラクターを表示
	if (faideEnabled_)
	{
		for (auto& enemy : pEnemy_)
		{
			enemy->SetEnabled(true);
		}

		pPlayer_->SetEnabled(true);

		faideEnabled_ = false;
	}

	// 背景の更新処理
	pBackGround_->Update(pPlayer_->GetPowerFeed());

	// フィールドの更新処理
	pField_->Updata();

	// プレイヤーの更新処理
	pPlayer_->Update(input);

	// 敵の更新処理
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
				// 残機がある場合は少し揺らす
				else
				{
					quakeX_ = 20.0f;
					quakeTimer_ = 60;
				}

				// プレイヤーの死亡フラグを立てる
				pPlayer_->SetDead(true);

				// SEを鳴らす
				SoundManager::GetInstance().Play("enemyAttack");

				// ジングルを鳴らすフラグを立てる
				playerDeadSound_ = true;

				// 残機がなかった場合
				if (life_ <= 0)
				{
					// ゲームオーバー演出に移行
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

				// SEを鳴らす
				SoundManager::GetInstance().Play("kill");

				// 敵死亡演出に移行
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

	// 敵の死亡演出
	pPlayer_->EnemyKillUpdate();

	if (pPlayer_->GetAnimeEnd())
	{
		// 初期化
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

	if (isCoinEnabled_)
	{
		for (auto& coin : pCoin_)
		{
			coin->Draw();
		}
	}

	// 残機の描画
	for (int i = 0; i < life_; i++)
	{
		int x = Game::SCREEN_WIDTH / 2 + 250 + (-i * 40);

		DrawRectRotaGraph(x, Game::SCREEN_HEIGHT - 30, 0, 0, 16, 16, 2.5f, 0.0f, lifeH_, true);
	}

	// ゲームオーバー
	if (isGameOver_)
	{
		int stringWidth = GetDrawStringWidthToHandle(GAMEOVER_STRING, static_cast<int>(strlen(GAMEOVER_STRING)), gameOverH_);
		int stringHeight = GetFontSizeToHandle(gameOverH_);

		int width = (Game::SCREEN_WIDTH / 2) - (stringWidth / 2);
		int height = (Game::SCREEN_HEIGHT / 2) - (stringHeight / 2);

		// ゲームオーバー文字の表示
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (gameOverStringFadeValue_ * 100) / 255);
		DrawBox(0, 0, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, gameOverStringFadeValue_);
		DrawStringToHandle(width + 47, height,
			GAMEOVER_STRING, 0x000000, gameOverShadowH_, false);
		DrawStringToHandle(width + 50, height,
			GAMEOVER_STRING, 0xffffff, gameOverH_, false);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	// ゲームクリア
	if (isGameClear_ )
	{
		// ゲームクリア文字の表示
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (gameClearStringFadeValue_ * 100) / 255);
		DrawBox(0, 0, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, gameClearStringFadeValue_);
		DrawStringToHandle(static_cast<int>(gameClearPos_.x - 3), static_cast<int>(gameClearPos_.y),
			GAMECLEAR_STRING, 0x000000, gameClearShadowH_, false);
		DrawStringToHandle(static_cast<int>(gameClearPos_.x), static_cast<int>(gameClearPos_.y),
			GAMECLEAR_STRING, 0xffffff, gameClearH_, false);

		// 最後のステージをクリアした場合箱を表示しない
		if (stage_ != stage_num - 1)
		{
			// 選択肢
			DrawRoundRect(CB_START_X_1 - 3, CB_START_Y_1 - 3, CB_START_X_1 + CB_WIDTH_1 + 3, CB_START_Y_1 + CB_HEIGHT_1 + 3, 5, 5, 0x000000, true);
			DrawRoundRect(CB_START_X_2 - 3, CB_START_Y_2 - 3, CB_START_X_2 + CB_WIDTH_2 + 3, CB_START_Y_2 + CB_HEIGHT_2 + 3, 5, 5, 0x000000, true);
			DrawRoundRect(CB_START_X_3 - 3, CB_START_Y_3 - 3, CB_START_X_3 + CB_WIDTH_3 + 3, CB_START_Y_3 + CB_HEIGHT_3 + 3, 5, 5, 0x000000, true);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			DrawRoundRect(CB_START_X_1, CB_START_Y_1, CB_START_X_1 + CB_WIDTH_1, CB_START_Y_1 + CB_HEIGHT_1, 5, 5, 0xffffff, true);
			DrawRoundRect(CB_START_X_2, CB_START_Y_2, CB_START_X_2 + CB_WIDTH_2, CB_START_Y_2 + CB_HEIGHT_2, 5, 5, 0xffffff, true);
			DrawRoundRect(CB_START_X_3, CB_START_Y_3, CB_START_X_3 + CB_WIDTH_3, CB_START_Y_3 + CB_HEIGHT_3, 5, 5, 0xffffff, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			DrawRoundRect(CB_START_X_1, CB_START_Y_1, CB_START_X_1 + CB_WIDTH_1, CB_START_Y_1 + CB_HEIGHT_1 - 5, 5, 5, 0xffffff, true);
			DrawRoundRect(CB_START_X_2, CB_START_Y_2, CB_START_X_2 + CB_WIDTH_2, CB_START_Y_2 + CB_HEIGHT_2 - 5, 5, 5, 0xffffff, true);
			DrawRoundRect(CB_START_X_3, CB_START_Y_3, CB_START_X_3 + CB_WIDTH_3, CB_START_Y_3 + CB_HEIGHT_3 - 5, 5, 5, 0xffffff, true);

			// カーソル
			DrawRotaGraph(static_cast<int>(cursor1Pos_.x), static_cast<int>(cursor1Pos_.y), 0.2f, 0.0f, cursor1H_, true);
			DrawRotaGraph(static_cast<int>(cursor2Pos_.x), static_cast<int>(cursor2Pos_.y), 0.2f, 0.0f, cursor2H_, true);
			DrawRotaGraph(static_cast<int>(cursor3Pos_.x), static_cast<int>(cursor3Pos_.y), 0.2f, 0.0f, cursor3H_, true);
			DrawRotaGraph(static_cast<int>(cursor4Pos_.x), static_cast<int>(cursor4Pos_.y), 0.2f, 0.0f, cursor4H_, true);

			// アイコン
			DrawRotaGraph(Game::SCREEN_WIDTH / 2, (Game::SCREEN_HEIGHT / 2) + 50, 1.0f, 0.0f, playH_, true);
			DrawRotaGraph(Game::SCREEN_WIDTH / 2, (Game::SCREEN_HEIGHT / 2) + 165, 0.8f, 0.0f, retryH_, true);
			DrawRotaGraph(Game::SCREEN_WIDTH / 2, (Game::SCREEN_HEIGHT / 2) + 280, 1.0f, 0.0f, backH_, true);

		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (preparTimer_ > 0)
	{
		int width = GetDrawStringWidthToHandle(REDY_STRING, static_cast<int>(strlen(REDY_STRING)), readyH_);
		int height = GetFontSizeToHandle(readyH_);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawBox(0, 0, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// 準備中文字の表示
		DrawStringToHandle((Game::SCREEN_WIDTH / 2) - (width / 2), (Game::SCREEN_HEIGHT / 2) - (height / 2),
			REDY_STRING, 0xffffff, readyH_, false);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(static_cast<int>(quakeX_), static_cast<int>(quakeY_), tempScreenH_, false);
	if (quakeTimer_ > 0)
	{
		GraphFilter(tempScreenH_, DX_GRAPH_FILTER_MONO, 0, 7);
		DrawGraph(static_cast<int>(quakeX_), static_cast<int>(quakeY_), tempScreenH_, false);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void GameplayingScene::GameClearUpdate(const InputState& input)
{
	// ゲームクリア時のコインを増やす
	isCoinEnabled_ = true;
	if (gameClearTimer_ % 1 == 0)
	{
		pCoin_.push_back(std::make_shared<Coin>(coinH_));
		pCoin_.push_back(std::make_shared<Coin>(coinH_));
	}

	// 不要になったコインの削除
	auto coin = std::remove_if(pCoin_.begin(), pCoin_.end(), [](const std::shared_ptr<Coin>& coin)
		{
			return coin->IsEnabled();
		});
	pCoin_.erase(coin, pCoin_.end());

	// コインの演出の更新処理
	for (auto& coin : pCoin_)
	{
		coin->Update();
	}
	
	// ゲームクリア時のプレイヤーの演出
	pPlayer_->ClearUpdate();

	// ゲームクリア後指定の時間が経ったら一度だけジングルを鳴らす
	gameClearTimer_++;
	if (gameClearTimer_ >= 60 && !isGameClear_)
	{
		SoundManager::GetInstance().PlayJingle("Data/sound/BGM/gameClear.ogg");
		gameClearTimer_ = 0;
		isGameClear_ = true;

		// 最後のステージをクリアした場合クリアの文字列の位置の変更
		if (stage_ == stage_num - 1)
		{
			gameClearPos_.x = static_cast<float>((Game::SCREEN_WIDTH / 2) - (stringWidth_ / 2));
			gameClearPos_.y = static_cast<float>((Game::SCREEN_HEIGHT / 2) - (stringHeight_ / 2) + 20);
		}
	}
	
	// 音楽が鳴っていた場合
	if (isGameClear_)
	{
		// 文字のフェード
		gameClearStringFadeValue_ = static_cast<int>(255 * (static_cast<float>(gameClearFadeTimer_)) / static_cast<float>(GAMECLEAR_FADE_INTERVAL));

		// 最後のステージではない場合
		if (stage_ != stage_num - 1)
		{
			gameClearPos_.y -= 0.5f;
			if (gameClearPos_.y <= static_cast<float>((Game::SCREEN_HEIGHT / 2) - 230))
			{
				gameClearPos_.y = static_cast<float>((Game::SCREEN_HEIGHT / 2) - 230);
			}
		}
		else
		{
			gameClearPos_.y -= 0.5f;
			if (gameClearPos_.y <= static_cast<float>((Game::SCREEN_HEIGHT / 2) - (stringHeight_ / 2)))
			{
				gameClearPos_.y = static_cast<float>((Game::SCREEN_HEIGHT / 2) - (stringHeight_ / 2));
			}
		}

		if (++gameClearFadeTimer_ >= 100)
		{
			gameClearFadeTimer_ = 100;
		
			// 最後のステージをクリアした場合すぐフェードしてタイトル画面に戻る
			if (stage_ == stage_num - 1 && !CheckMusic())
			{
				isTitile_ = true;
				updateFunc_ = &GameplayingScene::FadeOutUpdate;
			}

			if (stage_ != stage_num - 1)
			{
				//上下で回る処理
				if (input.IsTriggered(InputType::up))
				{
					SoundManager::GetInstance().Play("cursor");
					currentInputIndex_ = ((currentInputIndex_ - 1) + CHOICE_NUM) % CHOICE_NUM;
				}
				else if (input.IsTriggered(InputType::down))
				{
					SoundManager::GetInstance().Play("cursor");
					currentInputIndex_ = (currentInputIndex_ + 1) % CHOICE_NUM;
				}

				// カーソルの移動
				CursorMove();

				// 決定ボタンが押されたとき
				if (input.IsTriggered(InputType::next))
				{
					// SEを鳴らす
					SoundManager::GetInstance().Play("decision");

					// 初期化
					gameClearTimer_ = 0;

					// 選択された選択肢のフラグを立てる
					if (currentInputIndex_ == 0)
					{
						isNextStage_ = true;
					}
					else if (currentInputIndex_ == 1)
					{
						isRetry_ = true;
					}
					else
					{
						isTitile_ = true;
					}

					// フェードアウトに移行
					updateFunc_ = &GameplayingScene::FadeOutUpdate;
				}
			}
		}
	}
}

void GameplayingScene::GameOverUpdate(const InputState& input)
{
	// 画面を揺らす
	Quake();

	// 2秒待つ
	if (waitTimer_++ > 120)
	{
		// すべての敵を消す
		for (auto& enemy : pEnemy_)
		{
			enemy->SetEnabled(false);
		}

		if (!pPlayer_->GetAnimeEnd())
		{
			// プレイヤーの死亡演出
			pPlayer_->DeadUpdate();

			if (playerDeadSound_)
			{
				// ジングルを鳴らす
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
			gameOverStringFadeValue_ = static_cast<int>(255 * (static_cast<float>(gameOverFadeTimer_)) / static_cast<float>(GAMEOVER_FADE_INTERVAL));

			if (++gameOverFadeTimer_ >= 100)
			{
				gameOverFadeTimer_ = 100;

				if (++gameOverTimer_ >= 180)
				{
					gameOverTimer_ = 0;
					waitTimer_ = 0;
					updateFunc_ = &GameplayingScene::FadeOutUpdate;
				}
			}
		}
	}
}

void GameplayingScene::Quake()
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
}

void GameplayingScene::CursorMove()
{
	if (currentInputIndex_ == 0)
	{
		cursor1Pos_.x = CB_START_X_1;
		cursor1Pos_.y = CB_START_Y_1;

		cursor2Pos_.x = CB_START_X_1 + CB_WIDTH_1;
		cursor2Pos_.y = CB_START_Y_1;

		cursor3Pos_.x = CB_START_X_1 + CB_WIDTH_1;
		cursor3Pos_.y = CB_START_Y_1 + CB_HEIGHT_1;

		cursor4Pos_.x = CB_START_X_1;
		cursor4Pos_.y = CB_START_Y_1 + CB_HEIGHT_1;
	}
	else if (currentInputIndex_ == 1)
	{
		cursor1Pos_.x = CB_START_X_2;
		cursor1Pos_.y = CB_START_Y_2;

		cursor2Pos_.x = CB_START_X_2 + CB_WIDTH_2;
		cursor2Pos_.y = CB_START_Y_2;

		cursor3Pos_.x = CB_START_X_2 + CB_WIDTH_2;
		cursor3Pos_.y = CB_START_Y_2 + CB_HEIGHT_2;

		cursor4Pos_.x = CB_START_X_2;
		cursor4Pos_.y = CB_START_Y_2 + CB_HEIGHT_2;

	}
	else if (currentInputIndex_ == 2)
	{
		cursor1Pos_.x = CB_START_X_3;
		cursor1Pos_.y = CB_START_Y_3;

		cursor2Pos_.x = CB_START_X_3 + CB_WIDTH_3;
		cursor2Pos_.y = CB_START_Y_3;

		cursor3Pos_.x = CB_START_X_3 + CB_WIDTH_3;
		cursor3Pos_.y = CB_START_Y_3 + CB_HEIGHT_3;

		cursor4Pos_.x = CB_START_X_3;
		cursor4Pos_.y = CB_START_Y_3 + CB_HEIGHT_3;
	}
}

void GameplayingScene::PlayerDeadUpdate(const InputState& input)
{
	// 画面を揺らす
	Quake();

	if (waitTimer_++ > 60)
	{
		// すべての敵を消す
		for (auto& enemy : pEnemy_)
		{
			enemy->SetEnabled(false);
		}
		
		// ジングルを１回だけ鳴らす
		if (playerDeadSound_)
		{
			SoundManager::GetInstance().PlayJingle("Data/sound/BGM/playerDead.wav");
			playerDeadSound_ = false;
		}

		// プレイヤーの死亡演出の実行
		if (!pPlayer_->GetAnimeEnd())
		{
			pPlayer_->DeadUpdate();
		}
	}

	// プレイヤーの死亡演出が終わり、
	// ジングルが鳴り終わったらフェードアウトに移行
	if (pPlayer_->GetAnimeEnd() && !CheckMusic())
	{
		updateFunc_ = &GameplayingScene::FadeOutUpdate;
	}
}

void GameplayingScene::FadeOutUpdate(const InputState& input)
{
	// 画面のフェード処理
	fadeTimer_++;
	fadeValue_ = static_cast<int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(FADE_INTERVAL)));

	// 音楽のフェード処理
	SetVolumeMusic(static_cast<int>((std::max)(SoundManager::GetInstance().GetBGMVolume() - fadeValue_, 0)));

	// ゲームオーバー
	if (fadeTimer_ > FADE_INTERVAL && isGameOver_)
	{
		manager_.ChangeScene(new TitleScene(manager_));
		return;
	}
	// 次のステージへ
	else if (fadeTimer_ > FADE_INTERVAL && isNextStage_)
	{
		isNextStage_ = false;
		stage_ += 1;
		SetInit();
		updateFunc_ = &GameplayingScene::FadeInUpdate;
		return;
	}
	// タイトル画面に戻る
	else if (fadeTimer_ > FADE_INTERVAL && isTitile_)
	{
		isTitile_ = false;
		manager_.ChangeScene(new TitleScene(manager_));
		return;
	}
	// リトライ
	else if (fadeTimer_ > FADE_INTERVAL && isRetry_)
	{
		SetInit();
		updateFunc_ = &GameplayingScene::FadeInUpdate;
	}
	// 残機が１減った
	else if (fadeTimer_ > FADE_INTERVAL && life_ > 0)
	{
		SetDeadInit();
		updateFunc_ = &GameplayingScene::FadeInUpdate;
	}
}

void GameplayingScene::PrepareUpdate(const InputState& input)
{
	// 2秒間止める
	preparTimer_--;
	if (preparTimer_ <= 0)
	{
		updateFunc_ = &GameplayingScene::NormalUpdate;
		preparTimer_ = 0;
	}
}

void GameplayingScene::SetInit()
{
	// コインの削除
	pCoin_.clear();

	//ゲームクリアの文字列の初期化
	gameClearPos_.x = static_cast<float>((Game::SCREEN_WIDTH / 2) - (stringWidth_ / 2));
	gameClearPos_.y = static_cast<float>((Game::SCREEN_HEIGHT / 2) - 220);

	// フェード関係の初期化
	fadeTimer_ = FADE_INTERVAL;
	fadeValue_ = 255;
	gameClearFadeTimer_ = 0;
	gameClearStringFadeValue_ = 0;
	gameOverFadeTimer_ = 0;
	gameOverStringFadeValue_ = 0;

	// タイマーの初期化
	waitTimer_ = 0;

	// フラグの初期化 
	isTitile_ = false;
	isCoinEnabled_ = false;
	isNextStage_ = false;
	isRetry_ = false;
	isGameClear_ = false;
	isGameOver_ = false;

	// 現在のステージの確認
	StageCheck(stage_);

	// プレイヤーの初期化
	pPlayer_->Init(stage_);

	// 集めたコインの枚数の初期化
	pField_->SetCoinNum(0);

	// 敵の初期化
	for (auto& enemy : pEnemy_)
	{
		enemy->Init(stage_);	// 敵全体の初期化
		enemy->SetInit(stage_);	// ステージによって敵それぞれを初期化
	}

	// ステージによってサイズを変更
	pMap_->StageCheck(stage_);

	// フィールドの初期化
	pField_->StageCheck(stage_);

	// ポインタの設定
	for (auto& enemy : pEnemy_)
	{
		enemy->SetPlayer(pPlayer_);
		enemy->SetField(pField_);
	}
	for (int i = 0; i < enemyNum_; i++)
	{
		pPlayer_->SetEnemy(pEnemy_[i], i, stage_);
		pField_->SetEnemy(pEnemy_[i], i);
	}
	pBackGround_->SetPlayer(pPlayer_);
	pField_->SetPlayer(pPlayer_);
	pPlayer_->SetField(pField_);
	pMap_->SetField(pField_);

	// BGMを鳴らす
	SetVolumeMusic(0);
	SoundManager::GetInstance().PlayMusic("Data/sound/BGM/game.mp3");
	pPlayer_->StartMusic();
}

void GameplayingScene::SetDeadInit()
{
	// フェード関係の初期化
	fadeTimer_ = FADE_INTERVAL;
	fadeValue_ = 255;
	waitTimer_ = 0;
	
	// フラグの初期化
	isTitile_ = false;
	isCoinEnabled_ = false;
	isNextStage_ = false;
	isRetry_ = false;
	isGameClear_ = false;
	isGameOver_ = false;

	// プレイヤーの初期化
	pPlayer_->Init(stage_);

	// 敵の初期化
	for (auto& enemy : pEnemy_)
	{
		enemy->Init(stage_);
		enemy->SetInit(stage_);
	}

	// BGMを鳴らす
	SetVolumeMusic(0);
	SoundManager::GetInstance().PlayMusic("Data/sound/BGM/game.mp3");
	pPlayer_->StartMusic();
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

void GameplayingScene::StageCheck(int stage)
{
	switch (stage)
	{
	case 0:
		playerStartPosX_ = TUTORIAL_PLAYER_START_INDEX_X;
		playerStartPosY_ = TUTORIAL_PLAYER_START_INDEX_Y;
		skeletonStartPosX_ = TUTORIAL_SKELETON_START_INDEX_X;
		skeletonStartPosY_ = TUTORIAL_SKELETON_START_INDEX_Y;
		break;
	case 1:
		playerStartPosX_ = PLAYER_START_INDEX_X;
		playerStartPosY_ = PLAYER_START_INDEX_Y;
		skeletonStartPosX_ = SKELETON_START_INDEX_X;
		skeletonStartPosY_ = SKELETON_START_INDEX_Y;
		slimeStartPosX_ = SLIME_START_INDEX_X;
		slimeStartPosY_ = SLIME_START_INDEX_Y;
		ghostStartPosX_ = GHOST_START_INDEX_X;
		ghostStartPosY_ = GHOST_START_INDEX_Y;
		golemStartPosX_ = GOLEM_START_INDEX_X;
		golemStartPosY_ = GOLEM_START_INDEX_Y;
		break;
	default:
		break;
	}

	if (stage >= 1)
	{
		enemyNum_ = EnemyBase::enemy_num;
		pEnemy_.resize(enemyNum_);
		pEnemy_[EnemyBase::skeleton] = std::make_shared<Skeleton>(
			skeletonH_,
			skeletonStartPosX_,
			skeletonStartPosY_,
			stage_);
		pEnemy_[EnemyBase::slime] = std::make_shared<Slime>(
			slimeH_,
			slimeStartPosX_,
			slimeStartPosY_,
			stage_);
		pEnemy_[EnemyBase::ghost] = std::make_shared<Ghost>(
			ghostH_,
			ghostStartPosX_,
			ghostStartPosY_,
			stage_);
		pEnemy_[EnemyBase::golem] = std::make_shared<Golem>(
			golemH_,
			golemStartPosX_,
			golemStartPosY_,
			stage_);
	}
	else
	{
		enemyNum_ = 1;
		pEnemy_.resize(enemyNum_);
		pEnemy_[EnemyBase::skeleton] = std::make_shared<Skeleton>(
			skeletonH_,					// 画像ハンドル
			skeletonStartPosX_,			// 初期座標X
			skeletonStartPosY_,			// 初期座標Y 
			stage_);
	}
}
