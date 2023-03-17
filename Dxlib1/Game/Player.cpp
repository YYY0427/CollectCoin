#include "Player.h"
#include "Field.h"
#include "../DrawFunctions.h"
#include "../InputState.h"
#include "Skeleton.h"
#include "../game.h"
#include "EnemyBase.h"
#include "../SoundManager.h"
#include <algorithm>
#include <DxLib.h>

namespace
{
	// 通常のプレイヤーの移動スピード(何倍か)
	constexpr float NORMAL_SPEED = 1.4f;

	// パワーエサを取得した場合の移動スピード(何倍か)
	constexpr float GET_FEED_SPEED = 1.7f;

	// パワーエサを取得した場合持続時間(何秒か)
	constexpr int FEED_DURATION = 60 * 10;

	// プレイヤーを残り何割でフラッシュさせるかの割合
	constexpr float FLASH_RATIO = 0.8f;

	constexpr float GRAVITY = 1.0f;

	constexpr float JUMP_ACC = -10.0f;

	constexpr float QUAKE = 5.0f;
}

Player::Player(int normalH, int waponH, int deadH, int attackH, int indexX, int indexY, int stage) :
	stage_(stage),
	angle_(0.0f),
	kX_(0), kY_(0),
	moveTimer_(0),
	moveDirection_(0),
	feedGetNum_(0),
	moveInterval_(Field::CHIP_SIZE),
	powerFeedTimer_(0),
	speed2_(1.0f),
	imgIdX_(0),
	deadImgIdx_(0),
	attackImgIdx_(0),
	wantMoveDirection_(0),
	imgY_(0),
	timer_(0),
	soundVolume_(0),
	clearTimer_(0),
	isPowerFeed_(false),
	isDead_(false),
	isAnimeEnd_(false),
	isPowerFeed2_(false),
	isEnabled_(true),
	isIntrusion_(false),
	isTurnFlag_(false),
	isFlash_(false),
	enemyKill_(false),
	soundMin_(false),
	vec_(0.0f, 0.0f)
{
	normalBgmVolume_ = SoundManager::GetInstance().GetBGMVolume();
	powerUpBgmVolume_ = 0;

	// 初期座標
	indexX_ = indexX;
	indexY_ = indexY;

	// 画像
	normalH_ = normalH;
	waponH_ = waponH;
	deathH_ = deadH;
	attackH_ = attackH;
	handle_ = normalH_;

	powerUpBgmH_ = LoadSoundMem("Data/sound/BGM/powerUp.mp3");
	StartMusic();

	// 画像サイズの取得
	GetGraphSizeF(deathH_, &deathImgSize_.x, &deathImgSize_.y);
	GetGraphSizeF(attackH_, &attackImgSize_.x, &attackImgSize_.y);

	// インデックスの座標から初期座標を求める
	pos_.x = (indexX_ * Field::CHIP_SIZE) + (Field::CHIP_SIZE / 2 + Field::DISPLAY_POS_X);
	pos_.y=  (indexY_ * Field::CHIP_SIZE) + (Field::CHIP_SIZE / 2 + Field::DISPLAY_POS_Y);

	speed_ = NORMAL_SPEED;

	if (stage >= 1)
	{
		pEnemy_.resize(EnemyBase::enemy_num);
	}
	else
	{
		pEnemy_.resize(1);
	}
}

Player::~Player()
{
	DeleteSoundMem(powerUpBgmH_);
}

void Player::SetEnemy(std::shared_ptr<EnemyBase> enemy, int i, int stage)
{
	if (stage >= 1)
	{
		pEnemy_.resize(EnemyBase::enemy_num);
	}
	else
	{
		pEnemy_.resize(1);
	}
	pEnemy_[i] = enemy;
}

void Player::Init(int stage)
{
	StageCheck(stage);
	handle_ = normalH_;
	angle_ = 0.0f;
	kX_ = 0; 
	kY_ = 0;
	moveTimer_ = 0;
	moveDirection_ = 0;
	feedGetNum_ = 0;
	moveInterval_ = Field::CHIP_SIZE;
	powerFeedTimer_ = 0;
	speed2_ = 1.0f;
	imgIdX_ = 0;
	deadImgIdx_ = 0;
	attackImgIdx_ = 0;
	wantMoveDirection_ = 0;
	isPowerFeed_ = false;
	isDead_ = false;
	isAnimeEnd_ = false;
	isPowerFeed2_ = false;
	isEnabled_ = true;
	isIntrusion_ = false;
	isTurnFlag_ = false;
	isFlash_ = false;
}

void Player::Update(const InputState& input)
{
	// 移動のインターバルのカウント
	moveTimer_++;

	// 現在の座標を仮座標で保存
	kX_ = indexX_;
	kY_ = indexY_;

	// プレイヤーが移動したい方向を記憶
	// 上 1
	if (input.IsTriggered(InputType::up))
	{
		wantMoveDirection_ = up;
	} 
	// 下 2
	if (input.IsTriggered(InputType::down))
	{
		wantMoveDirection_ = down;
	}
	// 左 3
	if (input.IsTriggered(InputType::left))
	{
		wantMoveDirection_ = left;
	}
	// 右 4
	if (input.IsTriggered(InputType::right))
	{
		wantMoveDirection_ = right;
	}

	moveInterval_ = Field::CHIP_SIZE / speed_;

	// 移動のインターバル				// 移動する方向に壁がない場合移動
	if (moveTimer_ % moveInterval_ == 0 && !Colision(moveDirection_))
	{
		// 移動処理
		switch (moveDirection_)
		{
		case up:
			indexY_--;
			break;
		case down:
			indexY_++;
			break;
		case left:
			indexX_--;
			break;
		case right:
			indexX_++;
			break;
		default:
			break;
		};

		// プレイヤーが移動したい方向に壁がない場合移動を実行
		if (wantMoveDirection_ == up)
		{
			if (!pField_->IsBlock(indexY_ - 1, indexX_))
			{
				// 方向の切り替え
				imgY_ = 32;
				moveDirection_ = wantMoveDirection_;
			}
		}
		else if (wantMoveDirection_ == down)
		{
			if (!pField_->IsBlock(indexY_ + 1, indexX_) && pField_->Intrusion(indexY_ + 1, indexX_, isIntrusion_))
			{
				//方向の切り替え
				imgY_ = 96;
				moveDirection_ = wantMoveDirection_;
			}
		}
		else if (wantMoveDirection_ == left)
		{
			if (!pField_->IsBlock(indexY_, indexX_ - 1))
			{
				//方向の切り替え
				imgY_ = 64;
				moveDirection_ = wantMoveDirection_;
			}
		}
		else if (wantMoveDirection_ == right)
		{
			if (!pField_->IsBlock(indexY_, indexX_ + 1))
			{
				// 方向の切り替え
				imgY_ = 0;
				moveDirection_ = wantMoveDirection_;
			}
		}

		// 初期化
		moveTimer_ = 0;
	}
	
	// 壁に当たっていない場合
	if (!Colision(moveDirection_))
	{
		// 座標計算
		PosCalculation();
	}
	else
	{
		// 壁に当たっているので移動を行わない
		moveDirection_ = 0;
	}

	// エサとの当たり判定
	if (pField_->IsFeed(indexY_, indexX_))
	{
		//エサの数のカウント
		feedGetNum_++;
	}
	
	// パワーエサをとった場合の計算
	SpeedCalculation();

	// ワープチェック
	indexX_ = pField_->Warp(kY_, kX_, indexY_, indexX_);

	// 移動している場合処理を行う
	if (moveDirection_)
	{
		// アニメーション処理
		imgIdX_ = (imgIdX_ + 1) % (ANIME_FRAME_SPEED * ANIME_FRAME_NUM);
	}
}

void Player::Draw()
{
	if (!isDead_ && isEnabled_ && !enemyKill_)
	{
		if (isFlash_)
		{
			if ((powerFeedTimer_ / 10) % 2 == 0)
			{
				return;
			}
		}
		int imgX = (imgIdX_ / ANIME_FRAME_SPEED) * WIDTH;

		// プレイヤー画像の表示
		DrawRectRotaGraph(pos_.x, pos_.y - 5,		// 座標
						imgX, imgY_,				// 切り取り左上
						WIDTH, HEIGHT,				// 幅、高さ
						SCALE, 0.0f,				// 拡大率、回転角度
						handle_, true, isTurnFlag_);	// 画像のハンドル、透過するか
	}
	else if(isDead_ && isEnabled_ && !enemyKill_)
	{
		// 画像のインデックスを計算
		int imgX = (deadImgIdx_ / DEAD_ANIME_FRAME_SPEED) * WIDTH;

		// アニメーション画像の右端まで表示した場合
		if (imgX >= deathImgSize_.x - WIDTH)
		{
			// アニメーション終了フラグを立てる
			isAnimeEnd_ = true;
		}

		// ゲームオーバー時の画像を表示
		DrawRectRotaGraph(pos_.x,							// 座標
						  pos_.y,
						  imgX, 0,							// 切り取り左上
						  WIDTH, HEIGHT,					// 幅、高さ
						  SCALE, 0,							// 拡大率、回転角度
						  deathH_, true);					// 画像のハンドル、透過するか
	}
	else if (enemyKill_ && isEnabled_)
	{
		// 画像のインデックスを計算
		int imgX = (attackImgIdx_ / ATTACK_ANIME_FRAME_SPEED) * WIDTH;

		// アニメーション画像の右端まで表示した場合
		if (imgX >= attackImgSize_.x - WIDTH)
		{
			// アニメーション終了フラグを立てる
			isAnimeEnd_ = true;
		}

		// ゲームオーバー時の画像を表示
		DrawRectRotaGraph(pos_.x,							// 座標
			pos_.y,
			imgX, imgY_,							// 切り取り左上
			WIDTH, HEIGHT,					// 幅、高さ
			SCALE, 0,							// 拡大率、回転角度
			attackH_, true);					// 画像のハンドル、透過するか
	}
}

void Player::DeadUpdate()
{
	if (isDead_)
	{
		// アニメーション処理
		deadImgIdx_ = (deadImgIdx_ + 1) % (DEAD_ANIME_FRAME_SPEED * DEAD_ANIME_FRAME_NUM);
	}
}

void Player::EnemyKillUpdate()
{
	attackImgIdx_ = (attackImgIdx_ + 1) % (ATTACK_ANIME_FRAME_SPEED * ATTACK_ANIME_FRAME_NUM);
}

void Player::ClearUpdate()
{
	static Vec2 pos = pos_;
	clearTimer_++;

	if (pos.y == pos_.y && clearTimer_ > 80)
	{
		vec_.y = JUMP_ACC;
		clearTimer_ = 0;
	}
	
	vec_.y += GRAVITY;

	pos_ += vec_;

	if (pos_.y > pos.y)
	{
		pos_ = pos;
	}
}

bool Player::Colision(int direction)
{
	switch (direction)
	{
	case up:
		if (pField_->IsBlock(indexY_ - 1, indexX_))	return true;
		break;
	case down:
		if (pField_->IsBlock(indexY_ + 1, indexX_) || 
			!pField_->Intrusion(indexY_ + 1, indexX_, isIntrusion_))	return true;
		break;
	case left:
		if (pField_->IsBlock(indexY_, indexX_ - 1))	return true;
		break;
	case right:
		if (pField_->IsBlock(indexY_, indexX_ + 1)) return true;
		break;
	}
	return false;
}

void Player::SpeedCalculation()
{
	// パワーエサを取得していない状態でパワーエサを取得した
	if (!isPowerFeed_)
	{
		if (pField_->IsPowerFeed(indexY_, indexX_))
		{
			// パワーエサのフラグを立てる
			isPowerFeed_ = true;

			// プレイヤー画像を変更
			handle_ = waponH_;

			for (auto& enemy : pEnemy_)
			{
				// 敵のイジケ状態を開始
				enemy->SetIzike(true);
			}
		}
	}
	else
	{
		// プレイヤー画像を変更
		handle_ = waponH_;

		// パワーエサを取得している状態でパワーエサを取得した
		if (pField_->IsPowerFeed(indexY_, indexX_))
		{
			isPowerFeed2_ = true;
		}
	}

	// パワーエサを取得していた場合
	// 一定時間プレイヤーの移動を速くする
	if (isPowerFeed_)
	{
		// もともと流れていたBGMを小さくしていく
		SetVolumeMusic(normalBgmVolume_);
		normalBgmVolume_ -= 2;
		normalBgmVolume_ = (std::max)(normalBgmVolume_, 0);

		// 新しいBGMを徐々に大きくしていく
		ChangeVolumeSoundMem(powerUpBgmVolume_, powerUpBgmH_);
		powerUpBgmVolume_ += 2;
		powerUpBgmVolume_ = (std::min)(powerUpBgmVolume_, SoundManager::GetInstance().GetBGMVolume());

		// タイマーを開始
		powerFeedTimer_++;

		// 移動速度を変更
		speed_ = GET_FEED_SPEED;

		if (isPowerFeed2_)
		{
			powerFeedTimer_ = 1;
			isPowerFeed2_ = false;
		}

		// powerFeedTimerがFEED_DURATIONの特定の割合に達したら敵のアニメーションを変更
		if ((FEED_DURATION * FLASH_RATIO) < powerFeedTimer_)
		{
			// プレイヤーの点滅を開始
			isFlash_ = true;
		}
		else
		{
			isFlash_ = false;
		}

		// タイマーが指定した時間を経過した場合元の速度に戻す
		if (powerFeedTimer_ % FEED_DURATION == 0)
		{
			// 初期化
			powerFeedTimer_ = 0;
			isPowerFeed_ = false;
			isFlash_ = false;
			soundMin_ = true;

			for (auto& enemy : pEnemy_)
			{
				enemy->SetIzike(false);
			}

			SoundManager::GetInstance().Play("powerDown");

			// プレイヤー画像を変更
			handle_ = normalH_;

			// 元の移動速度に戻す
			speed_ = NORMAL_SPEED;
		}
	}
	else if(soundMin_ && !isPowerFeed_)
	{
		if (normalBgmVolume_ >= SoundManager::GetInstance().GetBGMVolume() && powerUpBgmVolume_ <= 0)
		{
			soundMin_ = false;
		}

		// もともと流れていたBGMを大きくしていく
		SetVolumeMusic(normalBgmVolume_);
		normalBgmVolume_ += 2;
		normalBgmVolume_ = (std::min)(normalBgmVolume_, SoundManager::GetInstance().GetBGMVolume());

		// 新しいBGMを徐々に小さくしていく
		ChangeVolumeSoundMem(powerUpBgmVolume_, powerUpBgmH_);
		powerUpBgmVolume_ -= 2;
		powerUpBgmVolume_ = (std::max)(powerUpBgmVolume_, 0);
	}
}

void Player::PosCalculation()
{
	// インデックス座標から座標を計算
	pos_.x = (indexX_ * Field::CHIP_SIZE) + (Field::CHIP_SIZE / 2 + Field::DISPLAY_POS_X);
	pos_.y = (indexY_ * Field::CHIP_SIZE) + (Field::CHIP_SIZE / 2 + Field::DISPLAY_POS_Y);

	// 向いている方向と移動速度によって座標を計算
	switch (moveDirection_) 
	{
	case up:
		pos_.y -= (moveTimer_ % moveInterval_) * speed_;
		break;
	case down:
		pos_.y += (moveTimer_ % moveInterval_) * speed_;
		break;
	case left:
		pos_.x -= (moveTimer_ % moveInterval_) * speed_;
		break;
	case right:
		pos_.x += (moveTimer_ % moveInterval_) * speed_;
		break;
	default:
		break;
	};
}

void Player::StartMusic()
{
	powerUpBgmVolume_ = 0;
	ChangeVolumeSoundMem(powerUpBgmVolume_, powerUpBgmH_);
	PlaySoundMem(powerUpBgmH_, DX_PLAYTYPE_LOOP);
}

void Player::StopMusic()
{
	StopSoundMem(powerUpBgmH_);
}

void Player::StageCheck(int stage)
{
	switch (stage)
	{
	case 0:
		indexY_ = 5;
		indexX_ = 5;
		break;
	case 1:
		indexX_ = 9;
		indexY_ = 16;
		break;
	}
}
