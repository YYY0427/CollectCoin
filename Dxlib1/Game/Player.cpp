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
	moveDirection_(0),
	feedGetNum_(0),
	moveSpeed_(30),
	powerFeedTimer_(0),
	imgIdx_(0),
	isPowerFeed_(false),
	isMoveDownOrRight(false)
{
	pField_ = std::make_shared<Field>();
	handle_ = my::MyLoadGraph(L"Data/img/game/Pacman16.png");
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
	if (moveTimer_ % moveSpeed_ == 0)
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
	
	// �ǂƂ̓����蔻��
	if(pField_->IsBlock(indexY_, indexX_))
	{
		//�i�񂾐�̍��W�ɕǂ��������ꍇ�O�̍��W�ɖ߂�
		indexX_ = kX_;
		indexY_ = kY_;

		moveDirection_ = 0;
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
	}
	// �p���[�G�T���擾���Ă����ꍇ
	// ��莞�ԃv���C���[�̈ړ��𑬂�����
	if (isPowerFeed_)
	{
		powerFeedTimer_++;
		moveSpeed_ = 20;

		if (powerFeedTimer_ % (60 * 10) == 0)
		{
			isPowerFeed_ = false;
			powerFeedTimer_ = 0;
			moveSpeed_ = 30;
		}
		// �p���[�G�T���擾���Ă����ԂŃp���[�G�T���擾�����ꍇ
		if (pField_->IsPowerFeed(indexY_, indexX_))
		{
			powerFeedTimer_ = 0;
		}
	}

	// ���[�v�`�F�b�N
	indexY_, indexX_ = pField_->PlayerWorp(kY_, kX_, indexY_, indexX_);

	// �A�j���[�V��������
	Animation();
}

void Player::Draw()
{
	int posX = (indexX_ * Field::kBlockSize) + (Field::kBlockSize / 2);
	int posY = (indexY_ * Field::kBlockSize) + (Field::kBlockSize / 2);

	// �v���C���[�摜�̕\��
	DrawRectRotaGraph(posX, posY,		// ���W
					imgIdx_ * 16, 0,	// �؂��荶��
					16, 16,				// ���A����
					2.0f, angle_,		// �g�嗦�A��]�p�x
					handle_, true); 
}

void Player::Animation()
{
	animeTimer_++;
	if (animeTimer_ % 3 == 0 && moveDirection_ != 0)
	{
		// �A�j���[�V�����̐؂�ւ�
		if (imgIdx_ == 0)
			imgIdx_ = 1;
		else if(imgIdx_ == 1)
			imgIdx_ = 2;
		else if (imgIdx_ == 2)
			imgIdx_ = 3;
		else if (imgIdx_ == 3)
			imgIdx_ = 4;
		else if (imgIdx_ == 4)
			imgIdx_ = 5;
		else if (imgIdx_ == 5)
			imgIdx_ = 0;
		animeTimer_ = 0;
	}
}