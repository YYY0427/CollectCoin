#include "Ghost.h"
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
	constexpr float NORMAL_SPEED = 1.6f;

	// �p���[�G�T���擾�����ꍇ�̈ړ��X�s�[�h(���{��)
	constexpr float GET_FEED_SPEED = 1.2f;

	// �p���[�G�T���擾�����ꍇ��������(���b��)
	constexpr int FEED_DURATION = 10;

	// ����ł��瓮���n�߂�܂ł̎���
	constexpr int DIEAD_MOVE_INTERVAL = 60 * 5;

	// �Q�[���X�^�[�g�����瓮���n�߂�܂ł̎���
	constexpr int STARET_MOVE_INTEVAL = 60 * 4;
}

Ghost::Ghost(int handle, int indexX, int indexY, int stage)
{
	handle_ = handle;
	indexX_ = indexX;
	indexY_ = indexY;

	stage_ = stage;

	isIntrusion_ = true;
}

void Ghost::Update()
{
	// ���񂾏ꍇ������
	if (isDead_)
	{
		SetDeadInit(stage_);

		// �G������ł����ԂŎw��̈ʒu�ɑ��݂���ꍇ�ɃC�W�P��Ԃ�����
		if (indexX_ == 10 && indexY_ == 10)
		{
		//	isIzike_ = false;
			isMove_ = false;
			isIntrusion_ = true;
		}
	}

	MoveSwitch(STARET_MOVE_INTEVAL, DIEAD_MOVE_INTERVAL);

	// �꒣�胂�[�h�ƒǐՃ��[�h�̐؂�ւ�
	ModeSwitch();

	// �ړ��̃C���^�[�o���̃J�E���g
	moveTimer_++;

	// ���݂̍��W�������W�ŕۑ�
	kX_ = indexX_;
	kY_ = indexY_;

	moveInterval_ = static_cast<int>(Field::CHIP_SIZE / speed_);

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

		moveDirection_ = pField_->GhostMove(indexY_, indexX_, isIntrusion_);

		moveTimer_ = 0;
	}

	SpeedChange();

	if (!pField_->SpornInOrAuto(indexY_, indexX_))
	{
		// ���X�|�[���n�_�ɂ��Ȃ�
		isIntrusion_ = false;
	}
	else
	{
		// ���X�|�[���n�_�ɂ���
		isIntrusion_ = true;
	}

	// �ǂɓ������Ă��Ȃ��ꍇ
	if (!Colision(moveDirection_))
	{
		// ���W�v�Z
		PosCalculation();
	}

	// ���[�v�`�F�b�N
	indexX_ = pField_->Warp(kY_, kX_, indexY_, indexX_);

	// �A�j���[�V��������
	idX_ = (idX_ + 1) % (ANIME_FRAME_SPEED * ANIME_FRAME_NUM);				// �ʏ폈��
}

void Ghost::Draw()
{
	int imgX = (idX_ / ANIME_FRAME_SPEED) * WIDTH;

	// �ʏ펞�̕\��
	if (isEnabled_)
	{

		int imgY = DirectReturnNum(HEIGHT);

		DrawRectRotaGraph(pos_.x, pos_.y - 5,	// ���W
			imgX, imgY,							// �؂��荶��
			WIDTH, HEIGHT,						// ���A����
			SCALE, 0,							// �g�嗦�A��]�p�x
			handle_, true);
	}
}

void Ghost::SetInit(int stage)
{
	indexX_ = 9;
	indexY_ = 10;
}