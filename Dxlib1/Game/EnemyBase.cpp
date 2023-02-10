#include "../DrawFunctions.h"
#include "Player.h"
#include "Field.h"
#include "EnemyBase.h"
#include "../Scene/GameplayingScene.h"
#include "../Scene/SceneManager.h"
#include <DxLib.h>

namespace
{
	// �摜�̕�
	constexpr int WIDTH = 16;

	// �摜�̍���
	constexpr int HEIGHT = 16;

	// �摜�̊g�嗦
	constexpr float SCALE = 2.0f;

	// �ʏ�̃v���C���[�̈ړ��X�s�[�h
	constexpr float NORMAL_SPEED = 1.5f;

	// �p���[�G�T���擾�����ꍇ�̈ړ��X�s�[�h(���{��)
	constexpr float GET_FEED_SPEED = 1.0f;

	// �p���[�G�T���擾�����ꍇ��������(���b��)
	constexpr int FEED_DURATION = 10;

	// 1���ɕK�v�ȃt���[����
	constexpr int ANIME_FRAME_SPEED = 10;
	constexpr int IZIKE_ANIME_FRAME_SPEED = 30;

	// �A�j���[�V��������
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
	// �摜�̃��[�h
//	handle_ = my::MyLoadGraph(L"Data/img/game/blinky.png");
	izikeHandle_ = my::MyLoadGraph(L"Data/img/game/izike.png");

	// �摜�̃T�C�Y�̎擾
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

	// �ʏ펞�̕\��
	if (isEnabled_ && !isIzike_)
	{

		int imgY = DirectReturnNum();

		DrawRectRotaGraph(pos_.x, pos_.y,		// ���W
			imgX, imgY,			// �؂��荶��
			WIDTH, HEIGHT,		// ���A����
			SCALE, 0,				// �g�嗦�A��]�p�x
			handle_, true);		// �摜�̃n���h���A���߂��邩
	}
	// �v���C���[���p���[�G�T���擾�������̕\��
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

		DrawRectRotaGraph(pos_.x, pos_.y,		// ���W
			imgX, imgY,							// �؂��荶��
			WIDTH, HEIGHT,						// ���A����
			SCALE, 0,							// �g�嗦�A��]�p�x
			izikeHandle_, true);				// �摜�̃n���h���A���߂��邩
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
	// �v���C���[���p���[�G�T���擾���Ă����ꍇ
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
	// �C���f�b�N�X���W���v�Z
	pos_.x = (indexX_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2 + Field::DISPLAY_POS_X);
	pos_.y = (indexY_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2 + Field::DISPLAY_POS_Y);

	// �����Ă�������ɂ���č��W���v�Z
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
	// �C���f�b�N�X���W��ύX���A���W�̌v�Z
	indexX_ = 10;
	indexY_ = 10;
	pos_.x = (indexX_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2 + Field::DISPLAY_POS_X);
	pos_.y = (indexY_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2 + Field::DISPLAY_POS_Y);

	// ������
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

