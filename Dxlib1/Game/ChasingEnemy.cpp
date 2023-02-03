#include "ChasingEnemy.h"
#include "../DrawFunctions.h"
#include "Player.h"
#include "Field.h"
#include "EnemyBase.h"
#include <DxLib.h>

namespace
{
	// �ʏ�̃v���C���[�̈ړ��X�s�[�h
	constexpr int NORMAL_SPEED = 32;

	// �p���[�G�T���擾�����ꍇ�̈ړ��X�s�[�h(���{��)
	constexpr float GET_FEED_SPEED = 1.3f;

	// �p���[�G�T���擾�����ꍇ��������(���b��)
	constexpr int FEED_DURATION = 10;

	// 1���ɕK�v�ȃt���[����
	constexpr int ANIME_FRAME_SPEED = 5;

	// �A�j���[�V��������
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
	// �ړ��̃C���^�[�o���̃J�E���g
	moveTimer_++;

	// ���݂̍��W�������W�ŕۑ�
	kX_ = indexX_;
	kY_ = indexY_;

	wantMoveDirection_ = GetRand(3) + 1;

	// �ړ��̃C���^�[�o��				// �ړ���������ɕǂ��Ȃ��ꍇ�ړ�
	if (moveTimer_ % moveSpeed_ == 0 && !Colision(moveDirection_))
	{
		// �ړ�����
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

		// �v���C���[���ړ������������ɕǂ��Ȃ��ꍇ�ړ������s
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

	// �ǂɓ������Ă��Ȃ��ꍇ
	if (!Colision(moveDirection_))
	{
		// ���W�v�Z
		PosCalculation();
	}

	// ���[�v�`�F�b�N
	indexX_ = pField_->PlayerWorp(kY_, kX_, indexY_, indexX_);

	// �A�j���[�V��������
	idX_ = (idX_ + 1) % (ANIME_FRAME_SPEED * ANIME_FRAME_NUM);
}

void ChasingEnemy::Draw()
{
	int imgX = (idX_ / ANIME_FRAME_SPEED) * 16;

	int imgY = DirectReturnNum();

	// �v���C���[�摜�̕\��
	DrawRectRotaGraph(posX_, posY_,		// ���W
					imgX, imgY,			// �؂��荶��
					16, 16,				// ���A����
					2.0f, 0,			// �g�嗦�A��]�p�x
					handle_, true);		// �摜�̃n���h���A���߂��邩
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
	// �p���[�G�T�Ƃ̓����蔻��
	if (pField_->IsPowerFeed(indexY_, indexX_))
	{
		isPowerFeed_ = true;
	}

	// �p���[�G�T���擾���Ă����ꍇ
	// ��莞�ԃv���C���[�̈ړ��𑬂�����
	if (isPowerFeed_)
	{
		powerFeedTimer_++;
		moveSpeed_ = NORMAL_SPEED / GET_FEED_SPEED;
		powerFeedSpeed_ = GET_FEED_SPEED;

		// �w�肵�����Ԃ��o�߂����ꍇ
		if (powerFeedTimer_ % (60 * FEED_DURATION) == 0)
		{
			// ������
			isPowerFeed_ = false;
			powerFeedTimer_ = 0;

			// ���̈ړ����x�ɖ߂�
			moveSpeed_ = NORMAL_SPEED;
			powerFeedSpeed_ = 1.0f;
		}
	}
}

void ChasingEnemy::PosCalculation()
{
	// �C���f�b�N�X���W���v�Z
	posX_ = (indexX_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2);
	posY_ = (indexY_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2);

	// �����Ă�������ɂ���č��W���v�Z
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
