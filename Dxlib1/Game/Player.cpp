#include "Player.h"
#include "Field.h"
#include "../DrawFunctions.h"
#include "../InputState.h"
#include <DxLib.h>

namespace
{
	// 通常のプレイヤーの移動スピード
	constexpr int NORMAL_SPEED = 32;

	// パワーエサを取得した場合の移動スピード(何倍か)
	constexpr float GET_FEED_SPEED = 1.3f;

	// パワーエサを取得した場合持続時間(何秒か)
	constexpr int FEED_DURATION = 10;

	// 1枚に必要なフレーム数
	constexpr int ANIME_FRAME_SPEED = 5;
	constexpr int DEAD_ANIME_FRAME_SPEED = 10;

	// アニメーション枚数
	constexpr int ANIME_FRAME_NUM = 5;
	constexpr int DEAD_ANIME_FRAME_NUM = 12;
	

}

Player::Player() :
	angle_(0.0f),
	kX_(0), kY_(0),
	indexX_(9), indexY_(16),
	moveTimer_(0),
	moveDirection_(0),
	feedGetNum_(0),
	moveSpeed_(NORMAL_SPEED),
	powerFeedTimer_(0),
	powerFeedSpeed_(1.0f),
	imgIdX_(0),
	deadImgIdx_(0),
	wantMoveDirection_(0),
	isPowerFeed_(false),
	isDead_(false),
	animeEnd_(false)
{
	pField_ = std::make_shared<Field>();

	// 画像のロード
	handle_ = my::MyLoadGraph(L"Data/img/game/Pacman16.png");
	deathHandle_ = my::MyLoadGraph(L"Data/img/game/PacmanDeath16.png");

	// 画像サイズの取得
	GetGraphSizeF(handle_, &size_.x, &size_.y);

	// インデックスの座標から初期座標を求める
	pos_.x = (indexX_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2);
	pos_.y= (indexY_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2);
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

	// 移動のインターバル				// 移動する方向に壁がない場合移動
	if (moveTimer_ % moveSpeed_ == 0 && !Colision(moveDirection_))
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
			if (!pField_->IsBlock(indexY_ + 1, indexX_))
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
	indexX_ = pField_->PlayerWorp(kY_, kX_, indexY_, indexX_);

	// 移動している場合処理を行う
	if (moveDirection_)
	{
		// アニメーション処理
		imgIdX_ = (imgIdX_ + 1) % (ANIME_FRAME_SPEED * ANIME_FRAME_NUM);
	}
}

void Player::Draw()
{
	// プレイヤーが敵と当たったか
	if (!isDead_)
	{
		int imgX = (imgIdX_ / ANIME_FRAME_SPEED) * 16;

		// プレイヤー画像の表示
		DrawRectRotaGraph(pos_.x, pos_.y,		// 座標
						imgX, 0,				// 切り取り左上
						16, 16,					// 幅、高さ
						2.0f, angle_,			// 拡大率、回転角度
						handle_, true);			// 画像のハンドル、透過するか
	}
	else
	{
		int imgX = (deadImgIdx_ / DEAD_ANIME_FRAME_SPEED) * 16;

		if (imgX >= 192 - 16)
		{
			animeEnd_ = true;
		}

		// ゲームオーバー時の画像を表示
		DrawRectRotaGraph(Game::kScreenWidth / 2 - 16,	// 座標
						  Game::kScreenHeight / 2 - 16,
						  imgX, 0,						// 切り取り左上
						  16, 16,						// 幅、高さ
						  2.0f, 0,						// 拡大率、回転角度
						  deathHandle_, true);			// 画像のハンドル、透過するか
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
		if (pField_->IsBlock(indexY_ + 1, indexX_))	return true;
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
	// パワーエサとの当たり判定
	if (pField_->IsPowerFeed(indexY_, indexX_))
	{
		isPowerFeed_ = true;
	}

	// パワーエサを取得していた場合
	// 一定時間プレイヤーの移動を速くする
	if (isPowerFeed_)
	{
		powerFeedTimer_++;
		moveSpeed_ = NORMAL_SPEED / GET_FEED_SPEED;
		powerFeedSpeed_ = GET_FEED_SPEED;

		// 指定した時間が経過した場合
		if (powerFeedTimer_ % (60 * FEED_DURATION) == 0)
		{
			// 初期化
			isPowerFeed_ = false;
			powerFeedTimer_ = 0;

			// 元の移動速度に戻す
			moveSpeed_ = NORMAL_SPEED;
			powerFeedSpeed_ = 1.0f;
		}
	}
}

void Player::PosCalculation()
{
	// インデックス座標を計算
	pos_.x = (indexX_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2);
	pos_.y = (indexY_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2);

	// 向いている方向によって座標を計算
	switch (moveDirection_) 
	{
	case up:
		pos_.y -= (moveTimer_ % moveSpeed_) * powerFeedSpeed_;
		break;
	case down:
		pos_.y += (moveTimer_ % moveSpeed_) * powerFeedSpeed_;
		break;
	case left:
		pos_.x -= (moveTimer_ % moveSpeed_) * powerFeedSpeed_;
		break;
	case right:
		pos_.x += (moveTimer_ % moveSpeed_) * powerFeedSpeed_;
		break;
	default:
		break;
	};
}