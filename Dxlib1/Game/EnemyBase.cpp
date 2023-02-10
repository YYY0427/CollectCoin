#include "../DrawFunctions.h"
#include "Player.h"
#include "Field.h"
#include "EnemyBase.h"
#include "../Scene/GameplayingScene.h"
#include "../Scene/SceneManager.h"
#include <DxLib.h>

namespace
{
	// 画像の幅
	constexpr int WIDTH = 16;

	// 画像の高さ
	constexpr int HEIGHT = 16;

	// 画像の拡大率
	constexpr float SCALE = 2.0f;

	// 通常のプレイヤーの移動スピード
	constexpr float NORMAL_SPEED = 1.5f;

	// パワーエサを取得した場合の移動スピード(何倍か)
	constexpr float GET_FEED_SPEED = 1.0f;

	// パワーエサを取得した場合持続時間(何秒か)
	constexpr int FEED_DURATION = 10;

	// 1枚に必要なフレーム数
	constexpr int ANIME_FRAME_SPEED = 10;
	constexpr int IZIKE_ANIME_FRAME_SPEED = 30;

	// アニメーション枚数
	constexpr int ANIME_FRAME_NUM = 2;
	constexpr int IZIKE_ANIME_FRAME_NUM = 2;
}

EnemyBase::EnemyBase() :
	powerFeedTimer_(0),
	isPowerFeed_(false),
	isEnabled_(true),
	isDead_(false),
	isIzike_(false),
isFlash_(false)
{
	// 画像のロード
//	handle_ = my::MyLoadGraph(L"Data/img/game/blinky.png");
	izikeHandle_ = my::MyLoadGraph(L"Data/img/game/izike.png");

	// 画像のサイズの取得
	GetGraphSizeF(handle_, &size_.x, &size_.y);

	kX_ = 0;
	kY_ = 0;

	moveTimer_ = 0;
	moveDirection_ = 0;
	wantMoveDirection_ = 0;

	moveInterval_ = Field::BLOCK_SIZE;

	speed_ = NORMAL_SPEED;

	idX_ = 0;

	pos_.x = (indexX_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2 + Field::DISPLAY_POS_X);
	pos_.y = (indexY_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2 + Field::DISPLAY_POS_Y);
}

void EnemyBase::Update()
{

}

void EnemyBase::Draw()
{
	int imgX = (idX_ / ANIME_FRAME_SPEED) * WIDTH;

	// 通常時の表示
	if (isEnabled_ && !isIzike_)
	{

		int imgY = DirectReturnNum();

		DrawRectRotaGraph(pos_.x, pos_.y,		// 座標
			imgX, imgY,			// 切り取り左上
			WIDTH, HEIGHT,		// 幅、高さ
			SCALE, 0,				// 拡大率、回転角度
			handle_, true);		// 画像のハンドル、透過するか
	}
	// プレイヤーがパワーエサを取得した時の表示
	if (isEnabled_ && isIzike_)
	{
		if (!isFlash_)
		{
			flashingImgY_ = 0;
		}
		else
		{
			flashingImgY_ = (idY_ / IZIKE_ANIME_FRAME_SPEED) * 16;
		}

		int imgY = flashingImgY_;

		DrawRectRotaGraph(pos_.x, pos_.y,		// 座標
			imgX, imgY,							// 切り取り左上
			WIDTH, HEIGHT,						// 幅、高さ
			SCALE, 0,							// 拡大率、回転角度
			izikeHandle_, true);				// 画像のハンドル、透過するか
	}
}

bool EnemyBase::Colision(int direction)
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

int EnemyBase::NoBlockDirect(int indexX, int indexY)
{
	bool isMove = false;
	int vecDirect = 0;
	while (!isMove)
	{
		vecDirect = GetRand(4) + 1;

		switch (vecDirect)
		{
		case up:
			if (!pField_->IsBlock(indexY - 1, indexX))
				isMove = true;
			break;
		case down:
			if (!pField_->IsBlock(indexY + 1, indexX))
				isMove = true;
			break;
		case left:
			if (!pField_->IsBlock(indexY, indexX - 1))
				isMove = true;
			break;
		case right:
			if (!pField_->IsBlock(indexY, indexX + 1))
				isMove = true;
			break;
		default:
			break;
		}
	}
	return vecDirect;
}

void EnemyBase::SpeedCalculation()
{
	// プレイヤーがパワーエサを取得していた場合
	if (pPlayer_->GetPowerFeed())
	{
		speed_ = GET_FEED_SPEED;
	}
	else
	{
		speed_ = NORMAL_SPEED;
	}
}

void EnemyBase::PosCalculation()
{
	// インデックス座標を計算
	pos_.x = (indexX_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2 + Field::DISPLAY_POS_X);
	pos_.y = (indexY_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2 + Field::DISPLAY_POS_Y);

	// 向いている方向によって座標を計算
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

void EnemyBase::SetInit()
{
	// インデックス座標を変更し、座標の計算
	indexX_ = 10;
	indexY_ = 10;
	pos_.x = (indexX_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2 + Field::DISPLAY_POS_X);
	pos_.y = (indexY_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2 + Field::DISPLAY_POS_Y);

	// 初期化
	isDead_ = false;
}

int EnemyBase::DirectReturnNum()
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

