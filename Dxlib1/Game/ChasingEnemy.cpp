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
	// �ʏ�̃v���C���[�̈ړ��X�s�[�h
	constexpr int NORMAL_SPEED = 32;

	// �p���[�G�T���擾�����ꍇ�̈ړ��X�s�[�h(���{��)
	constexpr float GET_FEED_SPEED = 1.3f;

	// �p���[�G�T���擾�����ꍇ��������(���b��)
	constexpr int FEED_DURATION = 10;

	// 1���ɕK�v�ȃt���[����
	constexpr int ANIME_FRAME_SPEED = 10;
	constexpr int IZIKE_ANIME_FRAME_SPEED = 30;

	// �A�j���[�V��������
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
	// �ړ��̃C���^�[�o���̃J�E���g
	moveTimer_++;

	// ���݂̍��W�������W�ŕۑ�
	kX_ = indexX_;
	kY_ = indexY_;

	// �ړ��̃C���^�[�o��
	if (moveTimer_ % moveSpeed_ == 0)
	{
		if (!Colision(moveDirection_))
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
		}
		
		// �����_���ɓG�̕�����ς���
		moveDirection_ = GetRand(3) + 1;

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
	idX_ = (idX_ + 1) % (ANIME_FRAME_SPEED * ANIME_FRAME_NUM);	// �ʏ폈��
	idY_ = (idY_ + 1) % (IZIKE_ANIME_FRAME_SPEED * IZIKE_ANIME_FRAME_NUM);	//�C�W�P�p����
}

void ChasingEnemy::Draw()
{
	int imgX = (idX_ / ANIME_FRAME_SPEED) * 16;

	// �ʏ펞�̕\��
	if (!isEnabled_ && !pPlayer_->GetPowerFeed())
	{
		
		int imgY = DirectReturnNum();

		DrawRectRotaGraph(pos_.x, pos_.y,		// ���W
						  imgX, imgY,			// �؂��荶��
						  16, 16,				// ���A����
						  2.0f, 0,				// �g�嗦�A��]�p�x
						  handle_, true);		// �摜�̃n���h���A���߂��邩
	}
	// �v���C���[���p���[�G�T���擾�������̕\��
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

		DrawRectRotaGraph(pos_.x, pos_.y,		// ���W
						  imgX, imgY,			// �؂��荶��
						  16, 16,				// ���A����
						  2.0f, 0,				// �g�嗦�A��]�p�x
						  izikeHandle_, true);	// �摜�̃n���h���A���߂��邩
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
	// �p���[�G�T�Ƃ̓����蔻��
	if (pField_->IsPowerFeed(indexY_, indexX_))
	{
		isPowerFeed_ = true;
	}

	// �p���[�G�T���擾���Ă����ꍇ
	// �摜��ς��āA�v���C���[���瓦���鋓���ɕς���
	if (pPlayer_->GetPowerFeed())
	{
		
	}
}

void ChasingEnemy::PosCalculation()
{
	// �C���f�b�N�X���W���v�Z
	pos_.x = (indexX_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2);
	pos_.y = (indexY_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2);

	// �����Ă�������ɂ���č��W���v�Z
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
