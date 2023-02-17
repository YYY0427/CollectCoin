#include "PinkyEnemy.h"
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

	// 1���ɕK�v�ȃt���[����
	constexpr int ANIME_FRAME_SPEED = 10;
	constexpr int IZIKE_ANIME_FRAME_SPEED = 30;

	// �A�j���[�V��������
	constexpr int ANIME_FRAME_NUM = 2;
	constexpr int IZIKE_ANIME_FRAME_NUM = 2;

	// ����ł��瓮���n�߂�܂ł̎���
	constexpr int DIEAD_MOVE_INTERVAL = 60 * 5;

	// �Q�[���X�^�[�g�����瓮���n�߂�܂ł̎���
	constexpr int STARET_MOVE_INTEVAL = 60 * 12;
}

PinkyEnemy::PinkyEnemy(int handle, int indexX, int indexY)
{
	handle_ = handle;
	indexX_ = indexX;
	indexY_ = indexY;
	isTracking_ = false;
}

void PinkyEnemy::Update()
{
	// ���񂾏ꍇ������
	if (isDead_)
	{
		SetInit();

		// �G������ł����ԂŎw��̈ʒu�ɑ��݂���ꍇ�ɃC�W�P��Ԃ�����
		if (indexX_ == 10 && indexY_ == 10)
		{
			isIzike_ = false;
			isMove_ = false;
			isIntrusion_ = true;
		}
	}

	MoveSwitch();

	// �꒣�胂�[�h�ƒǐՃ��[�h�̐؂�ւ�
	ModeSwitch();

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

		moveDirection_ = pField_->PinkyMove(indexY_, indexX_, isIntrusion_);

		moveTimer_ = 0;
	}

	SpeedCalculation();

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
	indexX_ = pField_->Worp(kY_, kX_, indexY_, indexX_);

	// �A�j���[�V��������
	idX_ = (idX_ + 1) % (ANIME_FRAME_SPEED * ANIME_FRAME_NUM);				// �ʏ폈��
	idY_ = (idY_ + 1) % (IZIKE_ANIME_FRAME_SPEED * IZIKE_ANIME_FRAME_NUM);	//�C�W�P�p����
}

void PinkyEnemy::MoveSwitch()
{
	if (!isMove_ && !isDead_)
	{
		moveTimer2_++;

		if (moveTimer2_ % STARET_MOVE_INTEVAL == 0)
		{
			moveTimer2_ = 0;

			isMove_ = true;
		}
	}
	else if (!isMove_ && isDead_)
	{
		moveTimer2_++;

		if (moveTimer2_ % DIEAD_MOVE_INTERVAL == 0)
		{
			moveTimer2_ = 0;

			isMove_ = true;
		}
	}
}
