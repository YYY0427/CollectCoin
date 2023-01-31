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
	moveDirection_(0),
	feedGetNum_(0),
	moveSpeed_(30),
	powerFeedTimer_(0),
	imgIdx_(0),
	isPowerFeed_(false),
	isMoveDownOrRight(false)
{
	pField_ = std::make_shared<Field>();
	handle_ = my::MyLoadGraph(L"Data/img/game/Pacman16.png");
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
	if (moveTimer_ % moveSpeed_ == 0)
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
	
	// 壁との当たり判定
	if(pField_->IsBlock(indexY_, indexX_))
	{
		//進んだ先の座標に壁があった場合前の座標に戻す
		indexX_ = kX_;
		indexY_ = kY_;

		moveDirection_ = 0;
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
	}
	// パワーエサを取得していた場合
	// 一定時間プレイヤーの移動を速くする
	if (isPowerFeed_)
	{
		powerFeedTimer_++;
		moveSpeed_ = 20;

		if (powerFeedTimer_ % (60 * 10) == 0)
		{
			isPowerFeed_ = false;
			powerFeedTimer_ = 0;
			moveSpeed_ = 30;
		}
		// パワーエサを取得している状態でパワーエサを取得した場合
		if (pField_->IsPowerFeed(indexY_, indexX_))
		{
			powerFeedTimer_ = 0;
		}
	}

	// ワープチェック
	indexY_, indexX_ = pField_->PlayerWorp(kY_, kX_, indexY_, indexX_);

	// アニメーション処理
	Animation();
}

void Player::Draw()
{
	int posX = (indexX_ * Field::kBlockSize) + (Field::kBlockSize / 2);
	int posY = (indexY_ * Field::kBlockSize) + (Field::kBlockSize / 2);

	// プレイヤー画像の表示
	DrawRectRotaGraph(posX, posY,		// 座標
					imgIdx_ * 16, 0,	// 切り取り左上
					16, 16,				// 幅、高さ
					2.0f, angle_,		// 拡大率、回転角度
					handle_, true); 
}

void Player::Animation()
{
	animeTimer_++;
	if (animeTimer_ % 3 == 0 && moveDirection_ != 0)
	{
		// アニメーションの切り替え
		if (imgIdx_ == 0)
			imgIdx_ = 1;
		else if(imgIdx_ == 1)
			imgIdx_ = 2;
		else if (imgIdx_ == 2)
			imgIdx_ = 3;
		else if (imgIdx_ == 3)
			imgIdx_ = 4;
		else if (imgIdx_ == 4)
			imgIdx_ = 5;
		else if (imgIdx_ == 5)
			imgIdx_ = 0;
		animeTimer_ = 0;
	}
}