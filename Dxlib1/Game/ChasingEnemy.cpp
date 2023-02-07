#include "ChasingEnemy.h"
#include "../DrawFunctions.h"
#include "Player.h"
#include "Field.h"
#include "EnemyBase.h"
#include "../Scene/GameplayingScene.h"
#include "../Scene/SceneManager.h"
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
	constexpr int ANIME_FRAME_SPEED = 10;
	constexpr int IZIKE_ANIME_FRAME_SPEED = 30;

	// アニメーション枚数
	constexpr int ANIME_FRAME_NUM = 2;
	constexpr int IZIKE_ANIME_FRAME_NUM = 2;
}

ChasingEnemy::ChasingEnemy(std::shared_ptr<Field>field, std::shared_ptr<Player>player):
	pPlayer_(player),
	pField_(field),
	powerFeedTimer_(0),
	isPowerFeed_(false),
	isEnabled_(false)
{
	handle_ = my::MyLoadGraph(L"Data/img/game/blinky.png");
	izikeHandle_ = my::MyLoadGraph(L"Data/img/game/izike.png");

	GetGraphSizeF(handle_, &size_.x, &size_.y);

	indexX_ = 9;
	indexY_ = 8;

	kX_ = 0;
	kY_ = 0;

	moveTimer_ = 0;
	moveDirection_ = right;
	wantMoveDirection_ = 0;
	
	moveSpeed_ = NORMAL_SPEED;

	powerFeedSpeed_ = 1.0f;

	pos_.x = (indexX_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2);
	pos_.y = (indexY_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2);
}

void ChasingEnemy::Update()
{
	// 移動のインターバルのカウント
	moveTimer_++;

	// 現在の座標を仮座標で保存
	kX_ = indexX_;
	kY_ = indexY_;

	// 移動のインターバル
	if (moveTimer_ % moveSpeed_ == 0)
	{
		if (!Colision(moveDirection_))
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
		}
		
		// ランダムに敵の方向を変える
		moveDirection_ = GetRand(3) + 1;

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
	idX_ = (idX_ + 1) % (ANIME_FRAME_SPEED * ANIME_FRAME_NUM);	// 通常処理
	idY_ = (idY_ + 1) % (IZIKE_ANIME_FRAME_SPEED * IZIKE_ANIME_FRAME_NUM);	//イジケ用処理
}

void ChasingEnemy::Draw()
{
	int imgX = (idX_ / ANIME_FRAME_SPEED) * 16;

	// 通常時の表示
	if (!isEnabled_ && !pPlayer_->GetPowerFeed())
	{
		
		int imgY = DirectReturnNum();

		DrawRectRotaGraph(pos_.x, pos_.y,		// 座標
						  imgX, imgY,			// 切り取り左上
						  16, 16,				// 幅、高さ
						  2.0f, 0,				// 拡大率、回転角度
						  handle_, true);		// 画像のハンドル、透過するか
	}
	// プレイヤーがパワーエサを取得した時の表示
	if (!isEnabled_ && pPlayer_->GetPowerFeed())
	{
		if (!pPlayer_->GetEnemyFlashing())
		{
			flashingImgY_ = 0;
		}
		else
		{
			flashingImgY_ = (idY_ / IZIKE_ANIME_FRAME_SPEED) * 16;
		}

		int imgY = flashingImgY_;

		DrawRectRotaGraph(pos_.x, pos_.y,		// 座標
						  imgX, imgY,			// 切り取り左上
						  16, 16,				// 幅、高さ
						  2.0f, 0,				// 拡大率、回転角度
						  izikeHandle_, true);	// 画像のハンドル、透過するか
	}
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
	// 画像を変えて、プレイヤーから逃げる挙動に変える
	if (pPlayer_->GetPowerFeed())
	{
		
	}
}

void ChasingEnemy::PosCalculation()
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
