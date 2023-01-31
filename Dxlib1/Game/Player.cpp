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
	// �ړ��̃C���^�[�o���̃J�E���g
	moveTimer_++;

	// ���݂̍��W�������W�ŕۑ�
	kX_ = indexX_;
	kY_ = indexY_;
	
	// �� 1
	if (input.IsTriggered(InputType::up))
	{
		//�����̐؂�ւ�
		angle_ = 4.65f;

		moveDirection = 1;
	} 
	// �� 2
	if (input.IsTriggered(InputType::down))
	{
		//�����̐؂�ւ�
		angle_ = 1.55f;

		moveDirection = 2;
	}
	// �� 3
	if (input.IsTriggered(InputType::left))
	{
		//�����̐؂�ւ�
		angle_ = 3.1f;

		moveDirection = 3;
	}
	// �E 4
	if (input.IsTriggered(InputType::right))
	{
		// �����̐؂�ւ�
		angle_ = 0.0f;

		moveDirection = 4;
	}

	// �ړ��̃C���^�[�o��
	if (moveTimer_ % moveSpeed_ == 0)
	{
		// �ړ�����
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
	
	// �ǂƂ̓����蔻��
	if(pField_->IsBlock(indexY_, indexX_))
	{
		//�i�񂾐�̍��W�ɕǂ��������ꍇ�O�̍��W�ɖ߂�
		indexX_ = kX_;
		indexY_ = kY_;

		moveDirection = 0;
	}
	// �G�T�Ƃ̓����蔻��
	if (pField_->IsFeed(indexY_, indexX_))
	{
		//�G�T�̐��̃J�E���g
		feedGetNum_++;
	}
	// �p���[�G�T�Ƃ̓����蔻��
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

	// ���[�v�`�F�b�N
	indexY_, indexX_ = pField_->PlayerWorp(kY_, kX_, indexY_, indexX_);

	// �A�j���[�V��������
	Animation();
}

void Player::Draw()
{
	// �v���C���[�摜�̕\��
	DrawRectRotaGraph(
		(indexX_ * Field::kBlockSize) + (Field::kBlockSize / 2) ,
		(indexY_ * Field::kBlockSize) + (Field::kBlockSize / 2) ,
		imgIdx * 32, 0,			// �؂��荶��
		32, 32,					// ���A����
		1.0f, angle_,			// �g�嗦�A��]�p�x
		handle_, true);
}

void Player::Animation()
{
	animeTimer_++;
	if (animeTimer_ % 30 == 0 && moveDirection != 0)
	{
		// �A�j���[�V�����̐؂�ւ�
		if (imgIdx == 0)
			imgIdx = 1;
		else
			imgIdx = 0;

		animeTimer_ = 0;
	}
}