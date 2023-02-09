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

ChasingEnemy::ChasingEnemy() :
	powerFeedTimer_(0),
	isPowerFeed_(false),
	isEnabled_(false),
	isDead_(false),
	isIzike_(false),
	isFlash_(false)
{
	// �摜�̃��[�h
	handle_ = my::MyLoadGraph(L"Data/img/game/blinky.png");
	izikeHandle_ = my::MyLoadGraph(L"Data/img/game/izike.png");

	// �摜�̃T�C�Y�̎擾
	GetGraphSizeF(handle_, &size_.x, &size_.y);

	indexX_ = 9;
	indexY_ = 8;

	kX_ = 0;
	kY_ = 0;

	moveTimer_ = 0;
	moveDirection_ = right;
	wantMoveDirection_ = 0;
	
	moveInterval_ = Field::BLOCK_SIZE;

	speed_ = NORMAL_SPEED;

	pos_.x = (indexX_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2 + Field::DISPLAY_POS_X);
	pos_.y = (indexY_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2 + Field::DISPLAY_POS_Y);
}

void ChasingEnemy::Update()
{
	// ���񂾏ꍇ������
	if (isDead_)
	{
		SetInit();

		// �G������ł����ԂŎw��̈ʒu�ɑ��݂���ꍇ�ɃC�W�P��Ԃ�����
		if (indexX_ == 10 && indexY_ == 10)
		{
			isIzike_ = false;
		}
	}

	// �ړ��̃C���^�[�o���̃J�E���g
	moveTimer_++;

	// ���݂̍��W�������W�ŕۑ�
	kX_ = indexX_;
	kY_ = indexY_;

	moveInterval_ = Field::BLOCK_SIZE / speed_;

	// �ړ��̃C���^�[�o��
	if (moveTimer_ % moveInterval_ == 0)
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
	
	SpeedCalculation();

	// �ǂɓ������Ă��Ȃ��ꍇ
	if (!Colision(moveDirection_))
	{
		// ���W�v�Z
		PosCalculation();
	}

	// ���[�v�`�F�b�N
	indexX_ = pField_->Worp(kY_, kX_, indexY_, indexX_);

	// �A�j���[�V��������
	idX_ = (idX_ + 1) % (ANIME_FRAME_SPEED * ANIME_FRAME_NUM);				// �ʏ폈��
	idY_ = (idY_ + 1) % (IZIKE_ANIME_FRAME_SPEED * IZIKE_ANIME_FRAME_NUM);	//�C�W�P�p����
}

void ChasingEnemy::Draw()
{
	int imgX = (idX_ / ANIME_FRAME_SPEED) * WIDTH;

	// �ʏ펞�̕\��
	if (!isEnabled_ && !isIzike_)
	{
		
		int imgY = DirectReturnNum();

		DrawRectRotaGraph(pos_.x, pos_.y,		// ���W
						  imgX, imgY,			// �؂��荶��
						  WIDTH, HEIGHT,		// ���A����
						  SCALE, 0,				// �g�嗦�A��]�p�x
						  handle_, true);		// �摜�̃n���h���A���߂��邩
	}
	// �v���C���[���p���[�G�T���擾�������̕\��
	if (!isEnabled_ && isIzike_)
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
						  imgX, imgY,			// �؂��荶��
						  WIDTH, HEIGHT,		// ���A����
						  SCALE, 0,				// �g�嗦�A��]�p�x
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

void ChasingEnemy::PosCalculation()
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

void ChasingEnemy::SetInit()
{
	// �C���f�b�N�X���W��ύX���A���W�̌v�Z
	indexX_ = 10;
	indexY_ = 10;
	pos_.x = (indexX_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2 + Field::DISPLAY_POS_X);
	pos_.y = (indexY_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2 + Field::DISPLAY_POS_Y);

	// ������
	isDead_ = false;
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
