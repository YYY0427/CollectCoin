#include "Player.h"
#include "Field.h"
#include "../DrawFunctions.h"
#include "../InputState.h"
#include <DxLib.h>

namespace
{
	
}

Player::Player() :
	animeTimer_(0),
	angle_(0.0f),
	kX_(0), kY_(0),
	indexX_(1), indexY_(1),
	moveTimer_(0),
	moveDirection(0),
	feedGetNum_(0),
	moveSpeed_(30),
	powerFeedTimer_(0),
	isPowerFeed_(false),
	isGetPowerFeed_(false),
	isMoveDownOrRight(false)
{
	pField_ = std::make_shared<Field>();
	handle_ = my::MyLoadGraph(L"Data/img/game/pacman.png");
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

		moveDirection = 1;
	} 
	// 下 2
	if (input.IsTriggered(InputType::down))
	{
		//方向の切り替え
		angle_ = 1.55f;

		moveDirection = 2;
	}
	// 左 3
	if (input.IsTriggered(InputType::left))
	{
		//方向の切り替え
		angle_ = 3.1f;

		moveDirection = 3;
	}
	// 右 4
	if (input.IsTriggered(InputType::right))
	{
		// 方向の切り替え
		angle_ = 0.0f;

		moveDirection = 4;
	}

	// 移動のインターバル
	if (moveTimer_ % moveSpeed_ == 0)
	{
		// 移動処理
		switch (moveDirection)
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
	
	// 壁との当たり判定
	if(pField_->IsBlock(indexY_, indexX_))
	{
		//進んだ先の座標に壁があった場合前の座標に戻す
		indexX_ = kX_;
		indexY_ = kY_;

		moveDirection = 0;
	}
	// エサとの当たり判定
	if (pField_->IsFeed(indexY_, indexX_))
	{
		//エサの数のカウント
		feedGetNum_++;
	}
	// パワーエサとの当たり判定
	if (pField_->IsPowerFeed(indexY_, indexX_))
	{
		isPowerFeed_ = true;

		if (isPowerFeed_)
		{
			isGetPowerFeed_ = true;
		}
	}
	if (isPowerFeed_)
	{
		if (isGetPowerFeed_)
		{
			powerFeedTimer_ = 0;
			isGetPowerFeed_ = false;
		}
		powerFeedTimer_++;
		moveSpeed_ = 20;

		if (powerFeedTimer_ % 600 == 0)
		{
			isPowerFeed_ = false;
			powerFeedTimer_ = 0;
			moveSpeed_ = 30;
		}
	}

	// ワープチェック
	indexY_, indexX_ = pField_->PlayerWorp(kY_, kX_, indexY_, indexX_);

	// アニメーション処理
	Animation();
}

void Player::Draw()
{
	// プレイヤー画像の表示
	DrawRectRotaGraph(
		(indexX_ * Field::kBlockSize) + (Field::kBlockSize / 2) ,
		(indexY_ * Field::kBlockSize) + (Field::kBlockSize / 2) ,
		imgIdx * 32, 0,			// 切り取り左上
		32, 32,					// 幅、高さ
		1.0f, angle_,			// 拡大率、回転角度
		handle_, true);
}

void Player::Animation()
{
	animeTimer_++;
	if (animeTimer_ % 30 == 0 && moveDirection != 0)
	{
		// アニメーションの切り替え
		if (imgIdx == 0)
			imgIdx = 1;
		else
			imgIdx = 0;

		animeTimer_ = 0;
	}
}