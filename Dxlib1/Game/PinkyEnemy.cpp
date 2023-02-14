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
		}
	}

	// �ǐՃ��[�h�Ɠ꒣�胂�[�h�̐؂�ւ�
	if (!isTracking_)
	{
		trackingTimer_++;
		if (trackingTimer_ % (60 * 20) == 0)
		{
			trackingTimer_ = 0;

			// �ǐՃ��[�h�ɐ؂�ւ�
			isTracking_ = true;
		}
	}
	else
	{
		trackingTimer_++;
		if (trackingTimer_ % (60 * 30) == 0)
		{
			trackingTimer_ = 0;

			// �꒣�胂�[�h�ɐ؂�ւ�
			isTracking_ = false;
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

		moveDirection_ = pField_->PinkyMove(indexY_, indexX_);
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
