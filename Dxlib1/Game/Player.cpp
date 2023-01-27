#include "Player.h"
#include "Field.h"
#include "../DrawFunctions.h"
#include "../InputState.h"
#include <DxLib.h>

Player::Player() :
	animeTimer_(0),
	angle_(0.0f),
	kX_(0), kY_(0),
	indexX_(1), indexY_(1),
	moveTimer_(0),
	moveDirection(0),
	isMoveDownOrRight(false)
{
	pField_ = new Field;
	handle_ = my::MyLoadGraph(L"Data/img/game/pacman.png");
}

Player::~Player()
{
	delete pField_;
}

void Player::Update(const InputState& input)
{
	//現在の座標を仮座標で保存
	kX_ = indexX_;
	kY_ = indexY_;
	
	//上 1
	if (input.IsTriggered(InputType::up))
	{
		//方向の切り替え
		angle_ = 4.65f;

		moveDirection = 1;
	}
	//下 2
	if (input.IsTriggered(InputType::down))
	{
		//方向の切り替え
		angle_ = 1.55f;

		moveDirection = 2;
	}
	//左 3
	if (input.IsTriggered(InputType::left))
	{
		//方向の切り替え
		angle_ = 3.1f;

		moveDirection = 3;
	}
	//右 4
	if (input.IsTriggered(InputType::right))
	{
		//方向の切り替え
		angle_ = 0.0f;

		moveDirection = 4;
	}

	if (moveTimer_ % 30 == 0)
	{
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
	
	if(pField_->IsBlock(indexY_, indexX_))
	{
		indexX_ = kX_;
		indexY_ = kY_;

		moveDirection = 0;
	}

	

	Animation();

	moveTimer_++;
}

void Player::Draw()
{
	//プレイヤー画像の表示
	DrawRectRotaGraph(
		(indexX_ * Field::kBlockSize) + (Field::kBlockSize / 2) ,
		(indexY_ * Field::kBlockSize) + (Field::kBlockSize / 2) ,
		imgIdx * 32, 0,			//切り取り左上
		32, 32,					//幅、高さ
		1.0f, angle_,			//拡大率、回転角度
		handle_, true);
}

void Player::Animation()
{
	animeTimer_++;
	if (animeTimer_ % 30 == 0 && moveDirection != 0)
	{
		//アニメーションの切り替え
		if (imgIdx == 0)
			imgIdx = 1;
		else
			imgIdx = 0;

		animeTimer_ = 0;
	}
}