#include "Player.h"
#include "Field.h"
#include "../DrawFunctions.h"
#include "../InputState.h"
#include <DxLib.h>

namespace
{
	// �ʏ�̃v���C���[�̈ړ��X�s�[�h
	constexpr int normal_speed = 32;

	// �p���[�G�T���擾�����ꍇ�̈ړ��X�s�[�h(���{��)
	constexpr float get_feed_speed = 1.3f;

	// 1���ɕK�v�ȃt���[����
	constexpr int anime_frame_speed = 5;

	// �A�j���[�V��������
	constexpr int anime_frame_num = 5;
}

Player::Player() :
	angle_(0.0f),
	kX_(0), kY_(0),
	indexX_(1), indexY_(1),
	moveTimer_(0),
	moveDirection_(0),
	feedGetNum_(0),
	moveSpeed_(normal_speed),
	powerFeedTimer_(0),
	powerFeedSpeed_(1.0f),
	imgIdX_(3),
	isPowerFeed_(false)
{
	pField_ = std::make_shared<Field>();
	handle_ = my::MyLoadGraph(L"Data/img/game/Pacman16.png");
	posX_ = (indexX_ * Field::kBlockSize) + (Field::kBlockSize / 2);
	posY_ = (indexY_ * Field::kBlockSize) + (Field::kBlockSize / 2);
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

		moveDirection_ = 1;
	} 
	// �� 2
	if (input.IsTriggered(InputType::down))
	{
		//�����̐؂�ւ�
		angle_ = 1.55f;

		moveDirection_ = 2;
	}
	// �� 3
	if (input.IsTriggered(InputType::left))
	{
		//�����̐؂�ւ�
		angle_ = 3.1f;

		moveDirection_ = 3;
	}
	// �E 4
	if (input.IsTriggered(InputType::right))
	{
		// �����̐؂�ւ�
		angle_ = 0.0f;

		moveDirection_ = 4;
	}

	// �ړ��̃C���^�[�o��
	if (moveTimer_ % moveSpeed_ == 0 && !Colision(moveDirection_))
	{
		// �ړ�����
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
	
	if (!Colision(moveDirection_))
	{
		PosCalculation();
	}
	else
	{
		moveDirection_ = 0;
	}

	// �ǂƂ̓����蔻��
	//if(pField_->IsBlock(indexY_, indexX_))
	//{
	//	//�i�񂾐�̍��W�ɕǂ��������ꍇ�O�̍��W�ɖ߂�
	//	indexX_ = kX_;
	//	indexY_ = kY_;

	//	moveDirection_ = 0;
	//}
	// �G�T�Ƃ̓����蔻��
	if (pField_->IsFeed(indexY_, indexX_))
	{
		//�G�T�̐��̃J�E���g
		feedGetNum_++;
	}
	
	// �p���[�G�T���Ƃ����ꍇ�̌v�Z
	SpeedCalculation();

	// ���[�v�`�F�b�N
	indexY_, indexX_ = pField_->PlayerWorp(kY_, kX_, indexY_, indexX_);

	if (moveDirection_)
	{
		// �A�j���[�V��������
		Animation();
	}
}

void Player::Draw()
{
	int imgX = (imgIdX_ / anime_frame_speed) * 16;

	// �v���C���[�摜�̕\��
	DrawRectRotaGraph(posX_, posY_,		// ���W
					imgX, 0,	// �؂��荶��
					16, 16,				// ���A����
					2.0f, angle_,		// �g�嗦�A��]�p�x
					handle_, true); 
}

bool Player::Colision(int direction)
{
	switch (direction)
	{
	case 1:
		if (pField_->IsBlock(indexY_ - 1, indexX_))	return true;
		break;
	case 2:
		if (pField_->IsBlock(indexY_ + 1, indexX_))	return true;
		break;
	case 3:
		if (pField_->IsBlock(indexY_, indexX_ - 1))	return true;
		break;
	case 4:
		if (pField_->IsBlock(indexY_, indexX_ + 1)) return true;
		break;
	}
	return false;
}

void Player::SpeedCalculation()
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
		moveSpeed_ = normal_speed / get_feed_speed;
		powerFeedSpeed_ = get_feed_speed;

		if (powerFeedTimer_ % (60 * 10) == 0)
		{
			isPowerFeed_ = false;
			powerFeedTimer_ = 0;
			moveSpeed_ = normal_speed;
			powerFeedSpeed_ = 1.0f;
		}
		// �p���[�G�T���擾���Ă����ԂŃp���[�G�T���擾�����ꍇ
		if (pField_->IsPowerFeed(indexY_, indexX_))
		{
			powerFeedTimer_ = 0;
		}
	}
}

void Player::PosCalculation()
{
	posX_ = (indexX_ * Field::kBlockSize) + (Field::kBlockSize / 2);
	posY_ = (indexY_ * Field::kBlockSize) + (Field::kBlockSize / 2);

	switch (moveDirection_) 
	{
	case 1:
		posY_ -= (moveTimer_ % moveSpeed_) * powerFeedSpeed_;
		break;
	case 2:
		posY_ += (moveTimer_ % moveSpeed_) * powerFeedSpeed_;
		break;
	case 3:
		posX_ -= (moveTimer_ % moveSpeed_) * powerFeedSpeed_;
		break;
	case 4:
		posX_ += (moveTimer_ % moveSpeed_) * powerFeedSpeed_;
		break;
	default:
		break;
	};
}

void Player::Animation()
{
	imgIdX_ = (imgIdX_ + 1) % (anime_frame_speed * anime_frame_num);
}