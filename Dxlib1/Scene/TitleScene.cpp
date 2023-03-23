#include "TitleScene.h"
#include "../InputState.h"
#include "SceneManager.h"
#include "GameplayingScene.h"
#include "OptionScene.h"
#include "../DrawFunctions.h"
#include "../Game.h"
#include "../Game/Ghost.h"
#include "../Game/Golem.h"
#include "../Game/Skeleton.h"
#include "../Game/Slime.h"
#include "../Game/Player.h"
#include "../SoundManager.h"
#include "../Game/BackGround.h"
#include "Information.h"
#include <DxLib.h>

namespace
{
	// 選択肢１の座標
	constexpr int CB_WIDTH_1 = 400;												// 枠の幅
	constexpr int CB_HEIGHT_1 = 100;											// 枠の高さ
	constexpr int CB_START_X_1 = (Game::SCREEN_WIDTH / 2) - (CB_WIDTH_1 / 2);	// 枠の開始X座標
	constexpr int CB_START_Y_1 = Game::SCREEN_HEIGHT / 2 + 100;					// 枠の開始Y座標
	
	// 選択肢２の座標
	constexpr int CB_WIDTH_2 = 120;												
	constexpr int CB_HEIGHT_2 = 100;											
	constexpr int CB_START_X_2 = 600;	
	constexpr int CB_START_Y_2 = Game::SCREEN_HEIGHT / 2 + 215;					

	// 選択肢３の座標
	constexpr int CB_WIDTH_3 = 120;												
	constexpr int CB_HEIGHT_3 = 100;											
	constexpr int CB_START_X_3 = (Game::SCREEN_WIDTH / 2) - (CB_WIDTH_3 / 2);	
	constexpr int CB_START_Y_3 = Game::SCREEN_HEIGHT / 2 + 215;					

	// 選択肢４の座標
	constexpr int CB_WIDTH_4 = 120;												
	constexpr int CB_HEIGHT_4 = 100;											
	constexpr int CB_START_X_4 = 880;	
	constexpr int CB_START_Y_4 = Game::SCREEN_HEIGHT / 2 + 215;					

	// キャラクターの拡大率
	constexpr float PLAYER_SCALE = 3.0f;
	constexpr float SKELETON_SCALE = 3.0f;
	constexpr float SLIME_SCALE = 3.0f;
	constexpr float GHOST_SCALE = 3.0f;
	constexpr float GOLEM_SCALE = 3.0f;

	// タイトル名
	const char* const TITLE_STRING = "これくとコイン";

	// フェードする速さ
	constexpr int FAIDE_INTERVAL = 60;

	// キャラクターの移動速度
	constexpr int MOVE_SPEED = 3;
}

void TitleScene::FadeInUpdate(const InputState& input)
{
	// BGMのフェード
	SetVolumeMusic(static_cast<int>(255.0f / 60.0f * static_cast<float>(60 - fadeTimer_) * 
		(static_cast<float>(SoundManager::GetInstance().GetBGMVolume() / 255.0f))));

	// 画面のフェード
	fadeValue_ = static_cast<int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(FAIDE_INTERVAL)));

	if (--fadeTimer_ == 0)
	{
		updateFunc_ = &TitleScene::NormalUpdate;
		fadeValue_ = 0;
	}
}

void TitleScene::NormalUpdate(const InputState& input)
{
	// 背景処理
	pBackGround_->Update(false);

	// キャラクターの移動処理
	if (!isTurnFlag_)
	{
		pos_.x += MOVE_SPEED;
	}
	else
	{
		pos_.x -= MOVE_SPEED;
	}

	// 特定の位置で画像を変え、移動方向を反転させる
	if (pos_.x > Game::SCREEN_WIDTH - 200)
	{
		isEnabled_ = true;
		isTurnFlag_ = true;
		playerH_ = waponPlayerH_;
	}
	// 画面外に出たら初期化し、描画しない
	else if (pos_.x < -200)
	{
		isEnabled_ = false;
		isTurnFlag_ = false;
		playerH_ = nowaponPlayerH_;
	}

	// アニメーション処理
	playerImgIdx_ = (playerImgIdx_ + 1) % (Player::ANIME_FRAME_SPEED * Player::ANIME_FRAME_NUM);
	skeletonImgIdx_ = (skeletonImgIdx_ + 1) % (Skeleton::ANIME_FRAME_SPEED * Skeleton::ANIME_FRAME_NUM);
	slimeImgIdx_ = (slimeImgIdx_ + 1) % (Slime::ANIME_FRAME_SPEED * Slime::ANIME_FRAME_NUM);
	ghostImgIdx_ = (ghostImgIdx_ + 1) % (Ghost::ANIME_FRAME_SPEED * Ghost::ANIME_FRAME_NUM);
	golemImgIdx_ = (golemImgIdx_ + 1) % (Ghost::ANIME_FRAME_SPEED * Ghost::ANIME_FRAME_NUM);

	// 選択肢を回す
	if (currentInputIndex_ == start)
	{
		// カーソルの移動
		CursorMove(start);

		if (input.IsTriggered(InputType::down))
		{
			// SE
			SoundManager::GetInstance().Play("cursor");

			// 選択肢の入れ替え
			currentInputIndex_ = credit;
		}
	}
	else if (currentInputIndex_ == option)
	{
		CursorMove(option);

		if (input.IsTriggered(InputType::up))
		{
			SoundManager::GetInstance().Play("cursor");
			currentInputIndex_ = start;
		}
		else if (input.IsTriggered(InputType::right))
		{
			SoundManager::GetInstance().Play("cursor");
			currentInputIndex_ = credit;
		}
	}
	else if (currentInputIndex_ == credit)
	{
		CursorMove(credit);

		if (input.IsTriggered(InputType::up))
		{
			SoundManager::GetInstance().Play("cursor");
			currentInputIndex_ = start;
		}
		else if (input.IsTriggered(InputType::right))
		{
			SoundManager::GetInstance().Play("cursor");
			currentInputIndex_ = exit;
		}
		else if (input.IsTriggered(InputType::left))
		{
			SoundManager::GetInstance().Play("cursor");
			currentInputIndex_ = option;
		}
	}
	else if (currentInputIndex_ == exit)
	{
		CursorMove(exit);

		if (input.IsTriggered(InputType::up))
		{
			SoundManager::GetInstance().Play("cursor");
			currentInputIndex_ = start;
		}
		else if (input.IsTriggered(InputType::left))
		{
			SoundManager::GetInstance().Play("cursor");
			currentInputIndex_ = credit;
		}
	}

	//次へのボタンが押されたら次のシーンへ行く
	if (input.IsTriggered(InputType::next))
	{
		// SE
		SoundManager::GetInstance().Play("decision");

		// optionとcreditはフェードしない
		// startとexitはフェード処理に移行
		if (currentInputIndex_ == start)
		{
			decisionIndex_ = start;
		}
		else if (currentInputIndex_ == option)
		{
			manager_.PushScene(new OptionScene(manager_));
			return;
		}
		else if (currentInputIndex_ == credit)
		{
			manager_.PushScene(new Information(manager_));
			return;
		}
		else if (currentInputIndex_ == exit)
		{
			decisionIndex_ = exit;
		}

		updateFunc_ = &TitleScene::FadeOutUpdate;
	}
}

void TitleScene::FadeOutUpdate(const InputState& input)
{
	// 画面のフェード
	fadeTimer_++;
	fadeValue_ = static_cast<int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(FAIDE_INTERVAL)));

	// BGMのフェード
	SetVolumeMusic(static_cast<int>((std::max)(SoundManager::GetInstance().GetBGMVolume() - fadeValue_, 0)));

	// ゲームスタート
	if (fadeTimer_ == FAIDE_INTERVAL && decisionIndex_ == start)
	{
		StopMusic();
		manager_.ChangeScene(new GameplayingScene(manager_));
		return;
	}
	// ゲーム終了
	else if (fadeTimer_ == FAIDE_INTERVAL && decisionIndex_ == exit)
	{
		DxLib_End();
	}
}

TitleScene::TitleScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&TitleScene::FadeInUpdate),
	fadeTimer_(0),
	fadeValue_(0),
	currentInputIndex_(0),
	decisionIndex_(0),
	imgY_(0),
	pos_(0, 0),
	sordH_(-1),
	nowaponPlayerH_(-1),
	waponPlayerH_(-1),
	skeletonH_(-1),
	slimeH_(-1),
	ghostH_(-1),
	golemH_(-1),
	controllerH_(-1),
	cursor1H_(-1),
	cursor2H_(-1),
	cursor3H_(-1),
	cursor4H_(-1),
	playerH_(-1),
	settingH_(-1),
	questionH_(-1),
	doorH_(-1),
	backGraphH_(-1),
	titleH_(-1),
	teachH_(-1)
{
	Init();
}

TitleScene::~TitleScene()
{
	End();
}

void TitleScene::Init()
{
	// 画像のロード
	sordH_ = my::MyLoadGraph("Data/img/game/sord_gold.png");
	nowaponPlayerH_ = my::MyLoadGraph("Data/img/game/nowapon-player_gold.png");
	waponPlayerH_ = my::MyLoadGraph("Data/img/game/wapon-player_gold.png");
	skeletonH_ = my::MyLoadGraph("Data/img/game/skeleton_monokuro.png");
	slimeH_ = my::MyLoadGraph("Data/img/game/slime_monokuro.png");
	ghostH_ = my::MyLoadGraph("Data/img/game/ghost_monokuro.png");
	golemH_ = my::MyLoadGraph("Data/img/game/golem_monokuro.png");
	controllerH_ = my::MyLoadGraph("Data/img/game/controller.png");
	cursor1H_ = my::MyLoadGraph("Data/img/game/cursor1.png");
	cursor2H_ = my::MyLoadGraph("Data/img/game/cursor2.png");
	cursor3H_ = my::MyLoadGraph("Data/img/game/cursor3.png");
	cursor4H_ = my::MyLoadGraph("Data/img/game/cursor4.png");
	playH_ = my::MyLoadGraph("Data/img/play.png");
	settingH_ = my::MyLoadGraph("Data/img/setting.png");
	questionH_ = my::MyLoadGraph("Data/img/information.png");
	doorH_ = my::MyLoadGraph("Data/img/door.png");
	backGraphH_ = my::MyLoadGraph("Data/img/game/Gray.png");

	// フォントのロード
	titleH_ = CreateFontToHandle("PixelMplus10", 150, 9);
	teachH_ = CreateFontToHandle("PixelMplus10", 25, 0);

	// メモリの確保
	pBackGround_ = std::make_shared<BackGround>(backGraphH_);

	// 初期化
	playerH_ = nowaponPlayerH_;
	fadeTimer_ = FAIDE_INTERVAL;
	fadeValue_ = 255;
	currentInputIndex_ = 0;
	decisionIndex_ = 0;
	imgY_ = 0;
	pos_ = { 0, Game::SCREEN_HEIGHT / 2 - Player::HEIGHT };

	// 音量を０にしてBGMを流す
	SetVolumeMusic(0);
	SoundManager::GetInstance().PlayMusic("Data/sound/BGM/titleBgm.mp3");
}

void TitleScene::End()
{
	DeleteGraph(sordH_);
	DeleteGraph(nowaponPlayerH_);
	DeleteGraph(waponPlayerH_);
	DeleteGraph(skeletonH_);
	DeleteGraph(slimeH_);
	DeleteGraph(ghostH_);
	DeleteGraph(golemH_);
	DeleteGraph(controllerH_);
	DeleteGraph(cursor1H_);
	DeleteGraph(cursor2H_);
	DeleteGraph(cursor3H_);
	DeleteGraph(cursor4H_);
	DeleteGraph(playH_);
	DeleteGraph(settingH_);
	DeleteGraph(questionH_);
	DeleteGraph(doorH_);
	DeleteGraph(backGraphH_);
	DeleteFontToHandle(titleH_);
	DeleteFontToHandle(teachH_);
}

void TitleScene::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void TitleScene::Draw()
{
	// タイトル名の幅の取得
	int width = GetDrawStringWidthToHandle(TITLE_STRING, static_cast<int>(strlen(TITLE_STRING)) , titleH_);

	// 背景描画処理
	pBackGround_->Draw();

	// 影
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawStringToHandle((Game::SCREEN_WIDTH / 2) - (width / 2) + 5, 100 + 5, TITLE_STRING, 0x000000, titleH_, false);
	DrawStringToHandle(Game::SCREEN_WIDTH / 2 + 600 + 2, Game::SCREEN_HEIGHT - 45 + 2, "けってい", 0x000000, teachH_, false);
	DrawStringToHandle(Game::SCREEN_WIDTH / 2 + 270 + 128 + 2, Game::SCREEN_HEIGHT - 45 + 2, "せんたく", 0x000000, teachH_, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// タイトル
	DrawStringToHandle((Game::SCREEN_WIDTH / 2) - (width / 2), 100, TITLE_STRING, 0xffffff, titleH_, false);

	// 選択肢
	DrawRoundRect(CB_START_X_1 - 3, CB_START_Y_1 - 3, CB_START_X_1 + CB_WIDTH_1 + 3, CB_START_Y_1 + CB_HEIGHT_1 + 3, 5, 5, 0x000000, true);
	DrawRoundRect(CB_START_X_2 - 3, CB_START_Y_2 - 3, CB_START_X_2 + CB_WIDTH_2 + 3, CB_START_Y_2 + CB_HEIGHT_2 + 3, 5, 5, 0x000000, true);
	DrawRoundRect(CB_START_X_3 - 3, CB_START_Y_3 - 3, CB_START_X_3 + CB_WIDTH_3 + 3, CB_START_Y_3 + CB_HEIGHT_3 + 3, 5, 5, 0x000000, true);
	DrawRoundRect(CB_START_X_4 - 3, CB_START_Y_4 - 3, CB_START_X_4 + CB_WIDTH_4 + 3, CB_START_Y_4 + CB_HEIGHT_4 + 3, 5, 5, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawRoundRect(CB_START_X_1, CB_START_Y_1, CB_START_X_1 + CB_WIDTH_1, CB_START_Y_1 + CB_HEIGHT_1, 5, 5, 0xffffff, true);
	DrawRoundRect(CB_START_X_2, CB_START_Y_2, CB_START_X_2 + CB_WIDTH_2, CB_START_Y_2 + CB_HEIGHT_2, 5, 5, 0xffffff, true);
	DrawRoundRect(CB_START_X_3, CB_START_Y_3, CB_START_X_3 + CB_WIDTH_3, CB_START_Y_3 + CB_HEIGHT_3, 5, 5, 0xffffff, true);
	DrawRoundRect(CB_START_X_4, CB_START_Y_4, CB_START_X_4 + CB_WIDTH_4, CB_START_Y_4 + CB_HEIGHT_4, 5, 5, 0xffffff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawRoundRect(CB_START_X_1, CB_START_Y_1, CB_START_X_1 + CB_WIDTH_1, CB_START_Y_1 + CB_HEIGHT_1 - 5, 5, 5, 0xffffff, true);
	DrawRoundRect(CB_START_X_2, CB_START_Y_2, CB_START_X_2 + CB_WIDTH_2, CB_START_Y_2 + CB_HEIGHT_2 - 5, 5, 5, 0xffffff, true);
	DrawRoundRect(CB_START_X_3, CB_START_Y_3, CB_START_X_3 + CB_WIDTH_3, CB_START_Y_3 + CB_HEIGHT_3 - 5, 5, 5, 0xffffff, true);
	DrawRoundRect(CB_START_X_4, CB_START_Y_4, CB_START_X_4 + CB_WIDTH_4, CB_START_Y_4 + CB_HEIGHT_4 - 5, 5, 5, 0xffffff, true);

	// アイコン
	DrawRotaGraph(Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT / 2 + 150, 1.0f, 0.0f, playH_, true);
	DrawRotaGraph(Game::SCREEN_WIDTH / 2 - 140, Game::SCREEN_HEIGHT / 2 + 265, 1.0f, 0.0f, settingH_, true);
	DrawRotaGraph(Game::SCREEN_WIDTH / 2, Game::SCREEN_HEIGHT / 2 + 265, 1.0f, 0.0f, questionH_, true);
	DrawRotaGraph(Game::SCREEN_WIDTH / 2 + 140, Game::SCREEN_HEIGHT / 2 + 265, 1.0f, 0.0f, doorH_, true);

	// カーソル
	DrawRotaGraph(static_cast<int>(cursor1Pos_.x), static_cast<int>(cursor1Pos_.y), 0.2f, 0.0f, cursor1H_, true);
	DrawRotaGraph(static_cast<int>(cursor2Pos_.x), static_cast<int>(cursor2Pos_.y), 0.2f, 0.0f, cursor2H_, true);
	DrawRotaGraph(static_cast<int>(cursor3Pos_.x), static_cast<int>(cursor3Pos_.y), 0.2f, 0.0f, cursor3H_, true);
	DrawRotaGraph(static_cast<int>(cursor4Pos_.x), static_cast<int>(cursor4Pos_.y), 0.2f, 0.0f, cursor4H_, true);

	// A けってい
	DrawRectRotaGraph(Game::SCREEN_WIDTH / 2 + 570, Game::SCREEN_HEIGHT - 30, 0, 0, 16, 16, 2.0f, 0.0f, controllerH_, true);
	DrawStringToHandle(Game::SCREEN_WIDTH / 2 + 600, Game::SCREEN_HEIGHT - 45, "けってい", 0xffffff, teachH_, false);

	// ↑↓←→　せんたく
	DrawRectRotaGraph(Game::SCREEN_WIDTH / 2 + 250, Game::SCREEN_HEIGHT - 30, 128, 0, 16, 16, 2.0f, 0.0f, controllerH_, true);
	DrawRectRotaGraph(Game::SCREEN_WIDTH / 2 + 255 + 32, Game::SCREEN_HEIGHT - 30, 144, 0, 16, 16, 2.0f, 0.0f, controllerH_, true);
	DrawRectRotaGraph(Game::SCREEN_WIDTH / 2 + 260 + 64, Game::SCREEN_HEIGHT - 30, 160, 0, 16, 16, 2.0f, 0.0f, controllerH_, true);
	DrawRectRotaGraph(Game::SCREEN_WIDTH / 2 + 265 + 96, Game::SCREEN_HEIGHT - 30, 176, 0, 16, 16, 2.0f, 0.0f, controllerH_, true);
	DrawStringToHandle(Game::SCREEN_WIDTH / 2 + 270 + 128, Game::SCREEN_HEIGHT - 45, "せんたく", 0xffffff, teachH_, false);

	if (!isTurnFlag_)
	{
		// 剣の表示
		DrawRectRotaGraph(Game::SCREEN_WIDTH - 200, Game::SCREEN_HEIGHT / 2 - 32, 0, 0, 16, 16, 3.0f, 0.0f, sordH_, true);
	}

	// プレイヤー画像の表示
	int playerImgX = (playerImgIdx_ / Player::ANIME_FRAME_SPEED) * Player::WIDTH;	
	DrawRectRotaGraph(static_cast<int>(pos_.x),				// x座標	
		static_cast<int>(pos_.y),							// y座標
		playerImgX, 0,										// 切り取り左上
		Player::WIDTH, Player::HEIGHT,						// 幅、高さ
		PLAYER_SCALE, 0.0f,									// 拡大率、回転角度
		playerH_, true, isTurnFlag_);						// 画像のハンドル、透過するか、反転するか

	// スケルトン画像の表示
	int skeletonImgX = (skeletonImgIdx_ / Skeleton::ANIME_FRAME_SPEED) * Skeleton::WIDTH;
	DrawRectRotaGraph(static_cast<int>(pos_.x - 100), static_cast<int>(pos_.y),
		skeletonImgX, Skeleton::HEIGHT * 3,		
		Skeleton::WIDTH, Skeleton::HEIGHT,		
		SKELETON_SCALE, 0.0f,					
		skeletonH_, true, isTurnFlag_);			

	// ゴーレムの画像の表示
	int golemImgX = (golemImgIdx_ / Golem::ANIME_FRAME_SPEED) * Golem::WIDTH;
	DrawRectRotaGraph(static_cast<int>(pos_.x - 150), static_cast<int>(pos_.y),
		golemImgX, Golem::HEIGHT * 3,			
		Golem::WIDTH, Golem::HEIGHT,			
		GOLEM_SCALE, 0.0f,						
		golemH_, true, isTurnFlag_);			

	// ゴーストの画像の表示
	int ghostImgX = (ghostImgIdx_ / Ghost::ANIME_FRAME_SPEED) * Ghost::WIDTH;
	DrawRectRotaGraph(static_cast<int>(pos_.x - 200), static_cast<int>(pos_.y),
		ghostImgX, Ghost::HEIGHT * 3,			
		Ghost::WIDTH, Ghost::HEIGHT,			
		GHOST_SCALE, 0.0f,						
		ghostH_, true, isTurnFlag_);			

	// スライムの画像の表示
	int slimeImgX = (slimeImgIdx_ / Slime::ANIME_FRAME_SPEED) * Slime::WIDTH;
	DrawRectRotaGraph(static_cast<int>(pos_.x - 250), static_cast<int>(pos_.y),		
		slimeImgX, Slime::HEIGHT * 3,			
		Slime::WIDTH, Slime::HEIGHT,			
		SLIME_SCALE, 0.0f,						
		slimeH_, true, isTurnFlag_);			

	// 画面フェード
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::CursorMove(int selection)
{
	// 選択肢によってカーソルを移動
	switch (selection)
	{
	case start:
		cursor1Pos_.x = CB_START_X_1;
		cursor1Pos_.y = CB_START_Y_1;

		cursor2Pos_.x = CB_START_X_1 + CB_WIDTH_1;
		cursor2Pos_.y = CB_START_Y_1;

		cursor3Pos_.x = CB_START_X_1 + CB_WIDTH_1;
		cursor3Pos_.y = CB_START_Y_1 + CB_HEIGHT_1;

		cursor4Pos_.x = CB_START_X_1;
		cursor4Pos_.y = CB_START_Y_1 + CB_HEIGHT_1;
		break;
	case option:
		cursor1Pos_.x = CB_START_X_2;
		cursor1Pos_.y = CB_START_Y_2;

		cursor2Pos_.x = CB_START_X_2 + CB_WIDTH_2;
		cursor2Pos_.y = CB_START_Y_2;

		cursor3Pos_.x = CB_START_X_2 + CB_WIDTH_2;
		cursor3Pos_.y = CB_START_Y_2 + CB_HEIGHT_2;

		cursor4Pos_.x = CB_START_X_2;
		cursor4Pos_.y = CB_START_Y_2 + CB_HEIGHT_2;
		break;
	case credit:
		cursor1Pos_.x = CB_START_X_3;
		cursor1Pos_.y = CB_START_Y_3;

		cursor2Pos_.x = CB_START_X_3 + CB_WIDTH_3;
		cursor2Pos_.y = CB_START_Y_3;

		cursor3Pos_.x = CB_START_X_3 + CB_WIDTH_3;
		cursor3Pos_.y = CB_START_Y_3 + CB_HEIGHT_3;

		cursor4Pos_.x = CB_START_X_3;
		cursor4Pos_.y = CB_START_Y_3 + CB_HEIGHT_3;
		break;
	case exit:
		cursor1Pos_.x = CB_START_X_4;
		cursor1Pos_.y = CB_START_Y_4;

		cursor2Pos_.x = CB_START_X_4 + CB_WIDTH_4;
		cursor2Pos_.y = CB_START_Y_4;

		cursor3Pos_.x = CB_START_X_4 + CB_WIDTH_4;
		cursor3Pos_.y = CB_START_Y_4 + CB_HEIGHT_4;

		cursor4Pos_.x = CB_START_X_4;
		cursor4Pos_.y = CB_START_Y_4 + CB_HEIGHT_4;
		break;
	}
}
