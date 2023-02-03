#include "ChasingEnemy.h"
#include "../DrawFunctions.h"
#include "Player.h"
#include "Field.h"
#include "EnemyBase.h"
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

	// アニメーション枚数
	constexpr int ANIME_FRAME_NUM = 2;
}

ChasingEnemy::ChasingEnemy()
{
	pField_ = std::make_shared<Field>();

	handle_ = my::MyLoadGraph(L"Data/img/game/blinky.png");

	indexX_ = 9;
	indexY_ = 8;

	kX_ = 0;
	kY_ = 0;

	moveTimer_ = 0;
	moveDirection_ = right;
	wantMoveDirection_ = 0;
	
	moveSpeed_ = NORMAL_SPEED;

	powerFeedSpeed_ = 1.0f;

	posX_ = (indexX_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2);
	posY_ = (indexY_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2);
}

void ChasingEnemy::Update()
{
	// 移動のインターバルのカウント
	moveTimer_++;

	// 現在の座標を仮座標で保存
	kX_ = indexX_;
	kY_ = indexY_;

	wantMoveDirection_ = GetRand(3) + 1;

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
				moveDirection_ = wantMoveDirection_;
			}
		}
		else if (wantMoveDirection_ == down)
		{
			if (!pField_->IsBlock(indexY_ + 1, indexX_))
			{
				moveDirection_ = wantMoveDirection_;
			}
		}
		else if (wantMoveDirection_ == left)
		{
			if (!pField_->IsBlock(indexY_, indexX_ - 1))
			{
				moveDirection_ = wantMoveDirection_;
			}
		}
		else if (wantMoveDirection_ == right)
		{
			if (!pField_->IsBlock(indexY_, indexX_ + 1))
			{
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

	// ワープチェック
	indexX_ = pField_->PlayerWorp(kY_, kX_, indexY_, indexX_);

	// アニメーション処理
	idX_ = (idX_ + 1) % (ANIME_FRAME_SPEED * ANIME_FRAME_NUM);
}

void ChasingEnemy::Draw()
{
	int imgX = (idX_ / ANIME_FRAME_SPEED) * 16;

	int imgY = DirectReturnNum();

	// プレイヤー画像の表示
	DrawRectRotaGraph(posX_, posY_,		// 座標
					imgX, imgY,			// 切り取り左上
					16, 16,				// 幅、高さ
					2.0f, 0,			// 拡大率、回転角度
					handle_, true);		// 画像のハンドル、透過するか
}

bool ChasingEnemy::Colision(int direction)
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

void ChasingEnemy::SpeedCalculation()
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

void ChasingEnemy::PosCalculation()
{
	// インデックス座標を計算
	posX_ = (indexX_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2);
	posY_ = (indexY_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2);

	// 向いている方向によって座標を計算
	switch (moveDirection_)
	{
	case up:
		posY_ -= (moveTimer_ % moveSpeed_) * powerFeedSpeed_;
		break;
	case down:
		posY_ += (moveTimer_ % moveSpeed_) * powerFeedSpeed_;
		break;
	case left:
		posX_ -= (moveTimer_ % moveSpeed_) * powerFeedSpeed_;
		break;
	case right:
		posX_ += (moveTimer_ % moveSpeed_) * powerFeedSpeed_;
		break;
	default:
		break;
	};
}

int ChasingEnemy::DirectReturnNum()
{
	int imgY = 0;
	switch (moveDirection_)
	{
	case up:
		imgY = 32;
		break;
	case down:
		imgY = 48;
		break;
	case left:
		imgY = 16;
		break;
	case right:
		imgY = 0;
		break;
	default:
		break;
	}
	return imgY;
}
