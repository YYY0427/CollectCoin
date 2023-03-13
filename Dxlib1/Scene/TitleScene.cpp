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
#include <DxLib.h>

namespace
{
	constexpr int PLAYER_SCALE = 3.0f;
	constexpr int SKELETON_SCALE = 3.0f;
	constexpr int SLIME_SCALE = 3.0f;
	constexpr int GHOST_SCALE = 3.0f;
	constexpr int GOLEM_SCALE = 3.0f;

	// タイトル
	const char* const TITLE_STRING = "ぼくの自信作！！";

	// 選択肢
	const char* const SELECTON1_STRING = "GAME START";
	const char* const SELECTON2_STRING = "OPTION";
	const char* const SELECTON3_STRING = "EXIT";

	// 選択肢の数
	constexpr int SELECTION_NUM = 3;

	// フェードする速さ
	constexpr int FAIDE_INTERVAL = 60;
}

void TitleScene::FadeInUpdate(const InputState& input)
{
	SetVolumeMusic(static_cast<int>(255.0f / 60.0f * static_cast<float>(60 - fadeTimer_)));
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(FAIDE_INTERVAL));
	if (--fadeTimer_ == 0)
	{
		updateFunc_ = &TitleScene::NormalUpdate;
		fadeValue_ = 0;
	}
}

void TitleScene::NormalUpdate(const InputState& input)
{
	pBackGround_->Update();

	if (!isTurnFlag_)
	{
		pos_.x += 3;
	}
	else
	{
		pos_.x -= 3;
	}
	if (pos_.x > Game::kScreenWidth - 200)
	{
		isEnabled_ = true;
		isTurnFlag_ = true;
		playerH_ = waponPlayerH_;
	}
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

	//選択肢を上下で回す処理
	if (input.IsTriggered(InputType::up))
	{
		SoundManager::GetInstance().Play("cursor");
		currentInputIndex_ = ((currentInputIndex_ - 1) + SELECTION_NUM) % SELECTION_NUM;
	}
	else if (input.IsTriggered(InputType::down))
	{
		SoundManager::GetInstance().Play("cursor");
		currentInputIndex_ = (currentInputIndex_ + 1) % SELECTION_NUM;
	}

	if (currentInputIndex_ == start)
		startH_ = selectionH_;
	else
		startH_ = normalSelectionH_;

	if (currentInputIndex_ == option)
		optionH_ = selectionH_;
	else
		optionH_ = normalSelectionH_;

	if (currentInputIndex_ == exsit)
		exsitH_ = selectionH_;
	else
		exsitH_ = normalSelectionH_;

	//次へのボタンが押されたら次のシーンへ行く
	if (input.IsTriggered(InputType::next))
	{
		SoundManager::GetInstance().Play("decision");
		if (currentInputIndex_ == start)
		{
			decisionIndex_ = start;
		}
		else if (currentInputIndex_ == option)
		{
			decisionIndex_ = option;
		}
		else if (currentInputIndex_ == exsit)
		{
			decisionIndex_ = exsit;
		}

		updateFunc_ = &TitleScene::FadeOutUpdate;
	}
}

void TitleScene::FadeOutUpdate(const InputState& input)
{
	fadeTimer_++;
	fadeValue_ = 255 * (static_cast<float>(fadeTimer_) / static_cast<float>(FAIDE_INTERVAL));
	SetVolumeMusic(255 - fadeValue_);
	if (fadeTimer_ == FAIDE_INTERVAL && decisionIndex_ == start)
	{
		StopMusic();
		manager_.ChangeScene(new GameplayingScene(manager_));
		return;
	}
	else if (fadeTimer_ == FAIDE_INTERVAL && decisionIndex_ == option)
	{
		StopMusic();
		manager_.ChangeScene(new OptionScene(manager_));
		return;
	}
	else if (fadeTimer_ == FAIDE_INTERVAL && decisionIndex_ == exsit)
	{
	
		DxLib_End();
	}
}

TitleScene::TitleScene(SceneManager& manager) :
	Scene(manager),
	updateFunc_(&TitleScene::FadeInUpdate),
	fadeTimer_(FAIDE_INTERVAL),
	fadeValue_(255),
	currentInputIndex_(0),
	decisionIndex_(0),
	imgY_(0),
	pos_(0, Game::kScreenHeight / 2 - Player::HEIGHT)
{
	// 画像のロード
	sordH_ = my::MyLoadGraph("Data/img/game/sord_gold.png");
	nowaponPlayerH_ = my::MyLoadGraph("Data/img/game/nowapon-player.png");
	waponPlayerH_ = my::MyLoadGraph("Data/img/game/wapon-player_gold.png");
	skeletonH_ = my::MyLoadGraph("Data/img/game/skeleton_monokuro.png");
	slimeH_ = my::MyLoadGraph("Data/img/game/slime_monokuro.png");
	ghostH_ = my::MyLoadGraph("Data/img/game/ghost_monokuro.png");
	golemH_ = my::MyLoadGraph("Data/img/game/golem_monokuro.png");
	controller_ = my::MyLoadGraph("Data/img/game/controller.png");
	int backGraph = my::MyLoadGraph("Data/img/game/Gray.png");

	normalSelectionH_ = CreateFontToHandle("PixelMplus10", 30, 10);
	selectionH_ = CreateFontToHandle("PixelMplus10", 40, 10);
	titleH_ = CreateFontToHandle("PixelMplus10", 150, 10);
	teachH_ = CreateFontToHandle("PixelMplus10", 25, 10);

	pBackGround_ = std::make_shared<BackGround>(backGraph);
	pPlayer_ = std::make_shared <Player>(1,1,1,1,1,1);

	pBackGround_->SetPlayer(pPlayer_);

	startH_ = normalSelectionH_;
	optionH_ = normalSelectionH_;
	exsitH_ = normalSelectionH_;

	playerH_ = nowaponPlayerH_;

	SetVolumeMusic(0);
	PlayMusic("Data/sound/BGM/titleBgm.mp3", DX_PLAYTYPE_LOOP);
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void TitleScene::Draw()
{
	int width1 = GetDrawStringWidthToHandle(SELECTON1_STRING, strlen(SELECTON1_STRING), startH_);
	int width2 = GetDrawStringWidthToHandle(SELECTON2_STRING, strlen(SELECTON2_STRING), optionH_);
	int width3 = GetDrawStringWidthToHandle(SELECTON3_STRING, strlen(SELECTON3_STRING), exsitH_);
	int width4 = GetDrawStringWidthToHandle(TITLE_STRING, strlen(TITLE_STRING) , titleH_);

	pBackGround_->Draw();

	// 影
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawStringToHandle((Game::kScreenWidth / 2) - (width4 / 2) + 10, 100 + 10, TITLE_STRING, 0x000000, titleH_, false);
	DrawStringToHandle((Game::kScreenWidth / 2) - (width1 / 2) + 5, Game::kScreenHeight / 2 + 150 + 5,
		SELECTON1_STRING, 0x000000, startH_, false);
	DrawStringToHandle((Game::kScreenWidth / 2) - (width2 / 2) + 5, Game::kScreenHeight / 2 + 225 + 5,
		SELECTON2_STRING, 0x000000, optionH_, false);
	DrawStringToHandle((Game::kScreenWidth / 2) - (width3 / 2) + 5, Game::kScreenHeight / 2 + 300 + 5,
		SELECTON3_STRING, 0x000000, exsitH_, false);
	DrawStringToHandle(Game::kScreenWidth / 2 + 600 + 5, Game::kScreenHeight - 45 + 5, "けってい", 0x000000, teachH_, false);
	DrawStringToHandle(Game::kScreenWidth / 2 + 270 + 128 + 5, Game::kScreenHeight - 45 + 5, "せんたく", 0x000000, teachH_, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// タイトル
	DrawStringToHandle((Game::kScreenWidth / 2) - (width4 / 2), 100, TITLE_STRING, 0xffffff, titleH_, false);

	// 選択肢
	DrawStringToHandle((Game::kScreenWidth / 2) - (width1 / 2), Game::kScreenHeight / 2 + 150,
		SELECTON1_STRING, 0xffffff, startH_, false);
	DrawStringToHandle((Game::kScreenWidth / 2) - (width2 / 2), Game::kScreenHeight / 2 + 225,
		SELECTON2_STRING, 0xffffff, optionH_, false);
	DrawStringToHandle((Game::kScreenWidth / 2) - (width3 / 2), Game::kScreenHeight / 2 + 300,
		SELECTON3_STRING, 0xffffff, exsitH_, false);

	// A けってい
	DrawRectRotaGraph(Game::kScreenWidth / 2 + 570, Game::kScreenHeight - 30, 0, 0, 16, 16, 2.0f, 0.0f, controller_, true);
	DrawStringToHandle(Game::kScreenWidth / 2 + 600, Game::kScreenHeight - 45, "けってい", 0xffffff, teachH_, false);

	// ↑↓←→　せんたく
	DrawRectRotaGraph(Game::kScreenWidth / 2 + 250, Game::kScreenHeight - 30, 128, 0, 16, 16, 2.0f, 0.0f, controller_, true);
	DrawRectRotaGraph(Game::kScreenWidth / 2 + 255 + 32, Game::kScreenHeight - 30, 144, 0, 16, 16, 2.0f, 0.0f, controller_, true);
	DrawRectRotaGraph(Game::kScreenWidth / 2 + 260 + 64, Game::kScreenHeight - 30, 160, 0, 16, 16, 2.0f, 0.0f, controller_, true);
	DrawRectRotaGraph(Game::kScreenWidth / 2 + 265 + 96, Game::kScreenHeight - 30, 176, 0, 16, 16, 2.0f, 0.0f, controller_, true);
	DrawStringToHandle(Game::kScreenWidth / 2 + 270 + 128, Game::kScreenHeight - 45, "せんたく", 0xffffff, teachH_, false);


	if (!isTurnFlag_)
	{
		// 剣の表示
		DrawRectRotaGraph(Game::kScreenWidth - 200, Game::kScreenHeight / 2 - 32, 0, 0, 16, 16, 3.0f, 0.0f, sordH_, true);
	}

	// プレイヤー画像の表示
	int playerImgX = (playerImgIdx_ / Player::ANIME_FRAME_SPEED) * Player::WIDTH;
	DrawRectRotaGraph(pos_.x, pos_.y,		// 座標
		playerImgX, 0,								// 切り取り左上
		Player::WIDTH, Player::HEIGHT,					// 幅、高さ
		PLAYER_SCALE, 0.0f,							// 拡大率、回転角度
		playerH_, true, isTurnFlag_);							// 画像のハンドル、透過するか

	// スケルトン画像の表示
	int skeletonImgX = (skeletonImgIdx_ / Skeleton::ANIME_FRAME_SPEED) * Skeleton::WIDTH;
	DrawRectRotaGraph(pos_.x - 100, pos_.y,		// 座標
		skeletonImgX, Skeleton::HEIGHT * 3,								// 切り取り左上
		Skeleton::WIDTH, Skeleton::HEIGHT,					// 幅、高さ
		SKELETON_SCALE, 0.0f,								// 拡大率、回転角度
		skeletonH_, true, isTurnFlag_);							// 画像のハンドル、透過するか

	// ゴーレムの画像の表示
	int golemImgX = (golemImgIdx_ / Golem::ANIME_FRAME_SPEED) * Golem::WIDTH;
	DrawRectRotaGraph(pos_.x - 150, pos_.y,		// 座標
		golemImgX, Golem::HEIGHT * 3,							// 切り取り左上
		Golem::WIDTH, Golem::HEIGHT,				// 幅、高さ
		GOLEM_SCALE, 0.0f,							// 拡大率、回転角度
		golemH_, true, isTurnFlag_);						// 画像のハンドル、透過するか

	// ゴーストの画像の表示
	int ghostImgX = (ghostImgIdx_ / Ghost::ANIME_FRAME_SPEED) * Ghost::WIDTH;
	DrawRectRotaGraph(pos_.x - 200, pos_.y,		// 座標
		ghostImgX, Ghost::HEIGHT * 3,							// 切り取り左上
		Ghost::WIDTH, Ghost::HEIGHT,				// 幅、高さ
		GHOST_SCALE, 0.0f,							// 拡大率、回転角度
		ghostH_, true, isTurnFlag_);						// 画像のハンドル、透過するか

	// スライムの画像の表示
	int slimeImgX = (slimeImgIdx_ / Slime::ANIME_FRAME_SPEED) * Slime::WIDTH;
	DrawRectRotaGraph(pos_.x - 250, pos_.y,		// 座標
		slimeImgX, Slime::HEIGHT * 3,							// 切り取り左上
		Slime::WIDTH, Slime::HEIGHT,				// 幅、高さ
		SLIME_SCALE, 0.0f,							// 拡大率、回転角度
		slimeH_, true, isTurnFlag_);						// 画像のハンドル、透過するか

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}