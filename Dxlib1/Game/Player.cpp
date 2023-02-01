#include "Player.h"
#include "Field.h"
#include "../DrawFunctions.h"
#include "../InputState.h"
#include <DxLib.h>

namespace
{
	// 通常のプレイヤーの移動スピード
	constexpr int normal_speed = 32;

	// パワーエサを取得した場合の移動スピード(何倍か)
	constexpr float get_feed_speed = 1.3f;

	// 1枚に必要なフレーム数
	constexpr int anime_frame_speed = 5;

	// アニメーション枚数
	constexpr int anime_frame_num = 5;
}

Player::Player() :
	angle_(0.0f),
	kX_(0), kY_(0),
	indexX_(1), indexY_(1),
	moveTimer_(0),
	moveDirection_(0),
	feedGetNum_(0),
	moveSpeed_(normal_speed),
	powerFeedTimer_(0),
	powerFeedSpeed_(1.0f),
	imgIdX_(3),
	isPowerFeed_(false)
{
	pField_ = std::make_shared<Field>();
	handle_ = my::MyLoadGraph(L"Data/img/game/Pacman16.png");
	posX_ = (indexX_ * Field::kBlockSize) + (Field::kBlockSize / 2);
	posY_ = (indexY_ * Field::kBlockSize) + (Field::kBlockSize / 2);
}

void Player::Update(const InputState& input)
{
	// 移動のインターバルのカウント
	moveTimer_++;

	// 現在の座標を仮座標で保存
	kX_ = indexX_;
	kY_ = indexY_;

	

	// 上 1
	if (input.IsTriggered(InputType::up))
	{
		//方向の切り替え
		angle_ = 4.65f;

		moveDirection_ = 1;
	} 
	// 下 2
	if (input.IsTriggered(InputType::down))
	{
		//方向の切り替え
		angle_ = 1.55f;

		moveDirection_ = 2;
	}
	// 左 3
	if (input.IsTriggered(InputType::left))
	{
		//方向の切り替え
		angle_ = 3.1f;

		moveDirection_ = 3;
	}
	// 右 4
	if (input.IsTriggered(InputType::right))
	{
		// 方向の切り替え
		angle_ = 0.0f;

		moveDirection_ = 4;
	}

	// 移動のインターバル
	if (moveTimer_ % moveSpeed_ == 0 && !Colision(moveDirection_))
	{
		// 移動処理
		switch (moveDirection_)
		{
		case 1:
			indexY_--;
			break;
		case 2:
			indexY_++;
			break;
		case 3:
			indexX_--;
			break;
		case 4:
			indexX_++;
			break;
		default:
			break;
		};
		moveTimer_ = 0;
	}
	
	if (!Colision(moveDirection_))
	{
		PosCalculation();
	}
	else
	{
		moveDirection_ = 0;
	}

	// 壁との当たり判定
	//if(pField_->IsBlock(indexY_, indexX_))
	//{
	//	//進んだ先の座標に壁があった場合前の座標に戻す
	//	indexX_ = kX_;
	//	indexY_ = kY_;

	//	moveDirection_ = 0;
	//}
	// エサとの当たり判定
	if (pField_->IsFeed(indexY_, indexX_))
	{
		//エサの数のカウント
		feedGetNum_++;
	}
	
	// パワーエサをとった場合の計算
	SpeedCalculation();

	// ワープチェック
	indexY_, indexX_ = pField_->PlayerWorp(kY_, kX_, indexY_, indexX_);

	if (moveDirection_)
	{
		// アニメーション処理
		Animation();
	}
}

void Player::Draw()
{
	int imgX = (imgIdX_ / anime_frame_speed) * 16;

	// プレイヤー画像の表示
	DrawRectRotaGraph(posX_, posY_,		// 座標
					imgX, 0,	// 切り取り左上
					16, 16,				// 幅、高さ
					2.0f, angle_,		// 拡大率、回転角度
					handle_, true); 
}

bool Player::Colision(int direction)
{
	switch (direction)
	{
	case 1:
		if (pField_->IsBlock(indexY_ - 1, indexX_))	return true;
		break;
	case 2:
		if (pField_->IsBlock(indexY_ + 1, indexX_))	return true;
		break;
	case 3:
		if (pField_->IsBlock(indexY_, indexX_ - 1))	return true;
		break;
	case 4:
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
		moveSpeed_ = normal_speed / get_feed_speed;
		powerFeedSpeed_ = get_feed_speed;

		if (powerFeedTimer_ % (60 * 10) == 0)
		{
			isPowerFeed_ = false;
			powerFeedTimer_ = 0;
			moveSpeed_ = normal_speed;
			powerFeedSpeed_ = 1.0f;
		}
		// パワーエサを取得している状態でパワーエサを取得した場合
		if (pField_->IsPowerFeed(indexY_, indexX_))
		{
			powerFeedTimer_ = 0;
		}
	}
}

void Player::PosCalculation()
{
	posX_ = (indexX_ * Field::kBlockSize) + (Field::kBlockSize / 2);
	posY_ = (indexY_ * Field::kBlockSize) + (Field::kBlockSize / 2);

	switch (moveDirection_) 
	{
	case 1:
		posY_ -= (moveTimer_ % moveSpeed_) * powerFeedSpeed_;
		break;
	case 2:
		posY_ += (moveTimer_ % moveSpeed_) * powerFeedSpeed_;
		break;
	case 3:
		posX_ -= (moveTimer_ % moveSpeed_) * powerFeedSpeed_;
		break;
	case 4:
		posX_ += (moveTimer_ % moveSpeed_) * powerFeedSpeed_;
		break;
	default:
		break;
	};
}

void Player::Animation()
{
	imgIdX_ = (imgIdX_ + 1) % (anime_frame_speed * anime_frame_num);
}