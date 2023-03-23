#include "../DrawFunctions.h"
#include "Player.h"
#include "Field.h"
#include "EnemyBase.h"
#include "../Scene/GameplayingScene.h"
#include "../Scene/SceneManager.h"
#include <DxLib.h>

namespace
{
	// �摜�̊g�嗦
	constexpr float SCALE = 2.0f;

	// �ʏ�̃v���C���[�̈ړ��X�s�[�h
	constexpr float NORMAL_SPEED = 1.5f;

	// �p���[�G�T���擾�����ꍇ�̈ړ��X�s�[�h(���{��)
	constexpr float GET_FEED_SPEED = 1.0f;

	// ����̏ꏊ�ł̈ړ��X�s�[�h
	constexpr float SLOW_POSITION_SPEED = 0.5f;

	// �p���[�G�T���擾�����ꍇ��������(���b��)
	constexpr int FEED_DURATION = 10;

	// 1���ɕK�v�ȃt���[����
	constexpr int ANIME_FRAME_SPEED = 10;
	constexpr int IZIKE_ANIME_FRAME_SPEED = 30;

	// �A�j���[�V��������
	constexpr int ANIME_FRAME_NUM = 2;
	constexpr int IZIKE_ANIME_FRAME_NUM = 2;

	// �ǐՃ��[�h�̎���
	constexpr int TARGET_MODE_TIME = 60 * 30;

	// �꒣�胂�[�h�̎���
	constexpr int TERRITORY_MODE_TIME = 60 * 15;

	// �G�����X�|�[���n�_����o�Ă���C���^�[�o��
	constexpr int INTERVAL = 60 * 5;
}

EnemyBase::EnemyBase() :
	handle_(-1),
	kX_(0),
	kY_(0),
	moveTimer_(0),
	moveTimer2_(0),
	trackingTimer_(0),
	powerFeedTimer_(0),
	moveDirection_(0),
	idX_(0),
	idY_(0),
	flashingImgY_(0),
	indexX_(0),
	indexY_(0),
	disPlayPosX_(0),
	disPlayPosY_(0),
	moveInterval_(0),
	speed_(0.0f),
	stage_(0),
	isMove_(false),
	isPowerFeed_(false),
	isEnabled_(true),
	isDead_(false),
	isIzike_(false),
	isTracking_(false),
	isIntrusion_(true),
	isDoor_(false),
	isDeadMove_(false)
{

}

void EnemyBase::Init(int stage)
{
	switch (stage)
	{
	case 0 :
		disPlayPosX_ = Field::TUTORIAL_DISPLAY_POS_X;
		disPlayPosY_ = Field::TUTORIAL_DISPLAY_POS_Y;
		break;
	case 1:
		disPlayPosX_ = Field::STAGE_1_DISPLAY_POS_X;
		disPlayPosY_ = Field::STAGE_1_DISPLAY_POS_Y;
		break;
	}

	moveTimer_ = 0;
	moveTimer2_ = 0;
	kX_ = 0;
	kY_ = 0;
	flashingImgY_ = 0;
	idX_ = 0;
	idY_ = 0;
	moveDirection_ = 0;
	powerFeedTimer_ = 0;
	isMove_ = false;
	isPowerFeed_ = false;
	isEnabled_ = true;
	isDead_ = false;
	isIzike_ = false;
	isTracking_ = false;
	isIntrusion_ = false;

	moveInterval_ = Field::CHIP_SIZE;

	speed_ = NORMAL_SPEED;

	pos_.x = static_cast<float>((indexX_ * Field::CHIP_SIZE) + (Field::CHIP_SIZE / 2 + disPlayPosX_));
	pos_.y = static_cast<float>((indexY_ * Field::CHIP_SIZE) + (Field::CHIP_SIZE / 2 + disPlayPosY_));
}

bool EnemyBase::Colision(int direction)
{
	switch (direction)
	{
	case up:
		if (pField_->IsBlock(indexY_ - 1, indexX_))	return true;
		break;
	case down:
	/*	if (pField_->IsBlock(indexY_ + 1, indexX_))	return true;
		break;*/
		if (pField_->IsBlock(indexY_ + 1, indexX_) ||
			!pField_->Intrusion(indexY_ + 1, indexX_, isIntrusion_))	return true;
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

int EnemyBase::NoBlockDirect(int indexY, int indexX)
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

void EnemyBase::SpeedChange()
{
	// �C�W�P��ԏꍇ�x���Ȃ�
	if(isIzike_)
	{
		speed_ = GET_FEED_SPEED;
	}
	// ����̏ꏊ�ɂ���ꍇ����x������
	else if (pField_->SlowPosition(indexY_, indexX_))
	{
		speed_ = SLOW_POSITION_SPEED;
	}
	// �����Ȃ��ꍇ���ʂ̃X�s�[�h
	else
	{
		speed_ = NORMAL_SPEED;
	}
}

void EnemyBase::PosCalculation()
{
	// �C���f�b�N�X���W���v�Z
	pos_.x = static_cast<float>((indexX_ * Field::CHIP_SIZE) + (Field::CHIP_SIZE / 2 + disPlayPosX_));
	pos_.y = static_cast<float>((indexY_ * Field::CHIP_SIZE) + (Field::CHIP_SIZE / 2 + disPlayPosY_));

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

void EnemyBase::SetDeadInit(int stage)
{
	// �C���f�b�N�X���W��ύX���A���W�̌v�Z
	if (stage == 0)
	{
		indexX_ = 5;
		indexY_ = 5;
	}
	else if (stage == 1)
	{
		indexX_ = 9;
		indexY_ = 10;
	}
	pos_.x = static_cast<float>((indexX_ * Field::CHIP_SIZE) + (Field::CHIP_SIZE / 2 + disPlayPosX_));
	pos_.y = static_cast<float>((indexY_ * Field::CHIP_SIZE) + (Field::CHIP_SIZE / 2 + disPlayPosY_));

	// ������
	isDead_ = false;
}

void EnemyBase::ModeSwitch()
{
	// �ǐՃ��[�h�Ɠ꒣�胂�[�h�̐؂�ւ�
	if (!isTracking_ && isMove_)	// �꒣�胂�[�h
	{
		trackingTimer_++;
		if (trackingTimer_ % TERRITORY_MODE_TIME == 0)
		{
			trackingTimer_ = 0;

			// �ǐՃ��[�h�ɐ؂�ւ�
			isTracking_ = true;
		}
	}
	else if(isTracking_ && isMove_)				// �ǐՃ��[�h
	{
		trackingTimer_++;
		if (trackingTimer_ % TARGET_MODE_TIME == 0)
		{
			trackingTimer_ = 0;

			// �꒣�胂�[�h�ɐ؂�ւ�
			isTracking_ = false;
		}
	}
}

void EnemyBase::MoveSwitch(int startInterval, int deadInterval, bool isDeadMove)
{
	if (!isMove_ && !isDeadMove)
	{
		moveTimer2_++;

		if (moveTimer2_ % startInterval == 0)
		{
			moveTimer2_ = 0;

			isMove_ = true;
		}
	}
	else if (!isMove_ && isDeadMove)
	{
		moveTimer2_++;

		if (moveTimer2_ % deadInterval == 0)
		{
			moveTimer2_ = 0;

			isMove_ = true;
			
			isDeadMove = false;
		}
	}
}

int EnemyBase::DirectReturnNum(int chipSize)
{
	int imgY = 0;
	switch (moveDirection_)
	{
	case up:
		imgY = 0 * chipSize;
		break;
	case down:
		imgY = 1 * chipSize;
		break;
	case left:
		imgY = 2 * chipSize;
		break;
	case right:
		imgY = 3 * chipSize;
		break;
	default:
		imgY = 4 * chipSize;
		break;
	}
	return imgY;
}

