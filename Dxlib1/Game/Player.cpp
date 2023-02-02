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
	
	// �p���[�G�T���擾�����ꍇ��������(���b��)
	constexpr int feed_duration = 10;

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
	wantMoveDirection_(0),
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

	// �v���C���[���ړ��������������L��
	// �� 1
	if (input.IsTriggered(InputType::up))
	{
		wantMoveDirection_ = up;
	} 
	// �� 2
	if (input.IsTriggered(InputType::down))
	{
		wantMoveDirection_ = down;
	}
	// �� 3
	if (input.IsTriggered(InputType::left))
	{
		wantMoveDirection_ = left;
	}
	// �E 4
	if (input.IsTriggered(InputType::right))
	{
		wantMoveDirection_ = right;
	}

	if (moveDirection_ == up)
	{
		
	}


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
				// �����̐؂�ւ�
				angle_ = 4.65f;
				moveDirection_ = wantMoveDirection_;
			}
		}
		else if (wantMoveDirection_ == down)
		{
			if (!pField_->IsBlock(indexY_ + 1, indexX_))
			{
				//�����̐؂�ւ�
				angle_ = 1.55f;
				moveDirection_ = wantMoveDirection_;
			}
		}
		else if (wantMoveDirection_ == left)
		{
			if (!pField_->IsBlock(indexY_, indexX_ - 1))
			{
				//�����̐؂�ւ�
				angle_ = 3.1f;
				moveDirection_ = wantMoveDirection_;
			}
		}
		else if (wantMoveDirection_ == right)
		{
			if (!pField_->IsBlock(indexY_, indexX_ + 1))
			{
				// �����̐؂�ւ�
				angle_ = 0.0f;
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
	else
	{
		// �ǂɓ������Ă���̂ňړ����s��Ȃ�
		moveDirection_ = 0;
	}

	// �G�T�Ƃ̓����蔻��
	if (pField_->IsFeed(indexY_, indexX_))
	{
		//�G�T�̐��̃J�E���g
		feedGetNum_++;
	}
	
	// �p���[�G�T���Ƃ����ꍇ�̌v�Z
	SpeedCalculation();

	// ���[�v�`�F�b�N
	indexX_ = pField_->PlayerWorp(kY_, kX_, indexY_, indexX_);

	// �ړ����Ă���ꍇ�������s��
	if (moveDirection_)
	{
		// �A�j���[�V��������
		imgIdX_ = (imgIdX_ + 1) % (anime_frame_speed * anime_frame_num);
	}
}

void Player::Draw()
{
	int imgX = (imgIdX_ / anime_frame_speed) * 16;

	// �v���C���[�摜�̕\��
	DrawRectRotaGraph(posX_, posY_,		// ���W
					imgX, 0,			// �؂��荶��
					16, 16,				// ���A����
					2.0f, angle_,		// �g�嗦�A��]�p�x
					handle_, true);		// �摜�̃n���h���A���߂��邩
}

bool Player::Colision(int direction)
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

		// �w�肵�����Ԃ��o�߂����ꍇ
		if (powerFeedTimer_ % (60 * feed_duration) == 0)
		{
			// ������
			isPowerFeed_ = false;
			powerFeedTimer_ = 0;

			// ���̈ړ����x�ɖ߂�
			moveSpeed_ = normal_speed;
			powerFeedSpeed_ = 1.0f;
		}
	}
}

void Player::PosCalculation()
{
	// �C���f�b�N�X���W���v�Z
	posX_ = (indexX_ * Field::kBlockSize) + (Field::kBlockSize / 2);
	posY_ = (indexY_ * Field::kBlockSize) + (Field::kBlockSize / 2);

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