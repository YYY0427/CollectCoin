#include "Player.h"
#include "Field.h"
#include "../DrawFunctions.h"
#include "../InputState.h"
#include "BlinkyEnemy.h"
#include "../game.h"
#include "EnemyBase.h"
#include <DxLib.h>

namespace
{
	// 画像の幅
	constexpr int WIDTH = 16;

	// 画像の高さ
	constexpr int HEIGHT = 16;

	// 画像の拡大率
	constexpr float SCALE = 2.0f;

	// 通常のプレイヤーの移動スピード(何倍か)
	constexpr float NORMAL_SPEED = 1.4f;

	// パワーエサを取得した場合の移動スピード(何倍か)
	constexpr float GET_FEED_SPEED = 1.7f;

	// パワーエサを取得した場合持続時間(何秒か)
	constexpr int FEED_DURATION = 60 * 8;

	// 敵をフラッシュさせる割合
	constexpr float FLASH_RATIO = 0.6f;

	// 1枚に必要なフレーム数
	constexpr int ANIME_FRAME_SPEED = 3;		// 通常時
	constexpr int DEAD_ANIME_FRAME_SPEED = 10;	// 死亡時

	// アニメーション枚数
	constexpr int ANIME_FRAME_NUM = 5;			// 通常時
	constexpr int DEAD_ANIME_FRAME_NUM = 12;	// 死亡時
}

Player::Player(int handle, int deadH) :
	angle_(0.0f),
	kX_(0), kY_(0),
	indexX_(9), indexY_(16),
	moveTimer_(0),
	moveDirection_(0),
	feedGetNum_(0),
	moveInterval_(Field::BLOCK_SIZE),
	powerFeedTimer_(0),
	speed2_(1.0f),
	imgIdX_(0),
	deadImgIdx_(0),
	wantMoveDirection_(0),
	isPowerFeed_(false),
	isDead_(false),
	isAnimeEnd_(false),
	isPowerFeed2_(false),
	isEnabled_(true),
	isIntrusion_(false)
{
	handle_ = handle;
	deathHandle_ = deadH;

	// 画像サイズの取得
	GetGraphSizeF(deathHandle_, &deathImgSize_.x, &deathImgSize_.y);

	// インデックスの座標から初期座標を求める
	pos_.x = (indexX_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2 + Field::DISPLAY_POS_X);
	pos_.y=  (indexY_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2 + Field::DISPLAY_POS_Y);

	speed_ = NORMAL_SPEED;
}

void Player::Init()
{
	angle_ = 0.0f;
	kX_ = 0; 
	kY_ = 0;
	indexX_ = 9; 
	indexY_ = 16;
	moveTimer_ = 0;
	moveDirection_ = 0;
	feedGetNum_ = 0;
	moveInterval_ = Field::BLOCK_SIZE;
	powerFeedTimer_ = 0;
	speed2_ = 1.0f;
	imgIdX_ = 0;
	deadImgIdx_ = 0;
	wantMoveDirection_ = 0;
	isPowerFeed_ = false;
	isDead_ = false;
	isAnimeEnd_ = false;
	isPowerFeed2_ = false;
	isEnabled_ = true;
	isIntrusion_ = false;
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

	moveInterval_ = Field::BLOCK_SIZE / speed_;

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
				angle_ = 4.65f;
				moveDirection_ = wantMoveDirection_;
			}
		}
		else if (wantMoveDirection_ == down)
		{
			if (!pField_->IsBlock(indexY_ + 1, indexX_) && pField_->Intrusion(indexY_ + 1, indexX_, isIntrusion_))
			{
				//方向の切り替え
				angle_ = 1.55f;
				moveDirection_ = wantMoveDirection_;
			}
		}
		else if (wantMoveDirection_ == left)
		{
			if (!pField_->IsBlock(indexY_, indexX_ - 1))
			{
				//方向の切り替え
				angle_ = 3.1f;
				moveDirection_ = wantMoveDirection_;
			}
		}
		else if (wantMoveDirection_ == right)
		{
			if (!pField_->IsBlock(indexY_, indexX_ + 1))
			{
				// 方向の切り替え
				angle_ = 0.0f;
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
	indexX_ = pField_->Worp(kY_, kX_, indexY_, indexX_);

	// 移動している場合処理を行う
	if (moveDirection_)
	{
		// アニメーション処理
		imgIdX_ = (imgIdX_ + 1) % (ANIME_FRAME_SPEED * ANIME_FRAME_NUM);
	}

	DrawFormatString(0, 30, 0xffffff, L"playerY : %d, playerX : %d", indexY_, indexX_);
}

void Player::Draw()
{
	// プレイヤーが敵と当たったか
	if (!isDead_ && isEnabled_)
	{
		int imgX = (imgIdX_ / ANIME_FRAME_SPEED) * 16;

		// プレイヤー画像の表示
		DrawRectRotaGraph(pos_.x, pos_.y,		// 座標
						imgX, 0,				// 切り取り左上
						WIDTH, HEIGHT,			// 幅、高さ
						SCALE, angle_,			// 拡大率、回転角度
						handle_, true);			// 画像のハンドル、透過するか
	}
	else if(isDead_ && isEnabled_)
	{
		// 画像のインデックスを計算
		int imgX = (deadImgIdx_ / DEAD_ANIME_FRAME_SPEED) * 16;

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
						  deathHandle_, true);				// 画像のハンドル、透過するか
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

bool Player::Colision(int direction)
{
	switch (direction)
	{
	case up:
		if (pField_->IsBlock(indexY_ - 1, indexX_))	return true;
		break;
	case down:
		if (pField_->IsBlock(indexY_ + 1, indexX_) || !pField_->Intrusion(indexY_ + 1, indexX_, isIntrusion_))	return true;
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

			for (auto& enemy : pEnemy_)
			{
				// 敵のイジケ状態を開始
				enemy->SetIzike(true);
			}
		}
	}
	else
	{
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
			for (auto& enemy : pEnemy_)
			{
				enemy->SetFlash (true);	// 敵の点滅を開始
			}	
		}
		else
		{
			for (auto& enemy : pEnemy_)
			{
				enemy->SetFlash(false);	
			}
		}

		// タイマーが指定した時間を経過した場合元の速度に戻す
		if (powerFeedTimer_ % FEED_DURATION == 0)
		{
			// 初期化
			powerFeedTimer_ = 0;
			isPowerFeed_ = false;

			for (auto& enemy : pEnemy_)
			{
				enemy->SetFlash(false);
				enemy->SetIzike(false);
			}

			// 元の移動速度に戻す
			speed_ = NORMAL_SPEED;
		}
	}
}

void Player::PosCalculation()
{
	// インデックス座標から座標を計算
	pos_.x = (indexX_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2 + Field::DISPLAY_POS_X);
	pos_.y = (indexY_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2 + Field::DISPLAY_POS_Y);

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