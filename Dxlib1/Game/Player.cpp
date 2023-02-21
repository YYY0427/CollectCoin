#include "Player.h"
#include "Field.h"
#include "../DrawFunctions.h"
#include "../InputState.h"
#include "BlinkyEnemy.h"
#include "../game.h"
#include "EnemyBase.h"
#include <DxLib.h>

namespace
{
	// �摜�̕�
	constexpr int WIDTH = 16;

	// �摜�̍���
	constexpr int HEIGHT = 16;

	// �摜�̊g�嗦
	constexpr float SCALE = 2.0f;

	// �ʏ�̃v���C���[�̈ړ��X�s�[�h(���{��)
	constexpr float NORMAL_SPEED = 1.4f;

	// �p���[�G�T���擾�����ꍇ�̈ړ��X�s�[�h(���{��)
	constexpr float GET_FEED_SPEED = 1.7f;

	// �p���[�G�T���擾�����ꍇ��������(���b��)
	constexpr int FEED_DURATION = 60 * 8;

	// �G���t���b�V�������銄��
	constexpr float FLASH_RATIO = 0.6f;

	// 1���ɕK�v�ȃt���[����
	constexpr int ANIME_FRAME_SPEED = 3;		// �ʏ펞
	constexpr int DEAD_ANIME_FRAME_SPEED = 10;	// ���S��

	// �A�j���[�V��������
	constexpr int ANIME_FRAME_NUM = 5;			// �ʏ펞
	constexpr int DEAD_ANIME_FRAME_NUM = 12;	// ���S��
}

Player::Player(int handle, int deadH) :
	angle_(0.0f),
	kX_(0), kY_(0),
	indexX_(9), indexY_(16),
	moveTimer_(0),
	moveDirection_(0),
	feedGetNum_(0),
	moveInterval_(Field::BLOCK_SIZE),
	powerFeedTimer_(0),
	speed2_(1.0f),
	imgIdX_(0),
	deadImgIdx_(0),
	wantMoveDirection_(0),
	isPowerFeed_(false),
	isDead_(false),
	isAnimeEnd_(false),
	isPowerFeed2_(false),
	isEnabled_(true),
	isIntrusion_(false)
{
	handle_ = handle;
	deathHandle_ = deadH;

	// �摜�T�C�Y�̎擾
	GetGraphSizeF(deathHandle_, &deathImgSize_.x, &deathImgSize_.y);

	// �C���f�b�N�X�̍��W���珉�����W�����߂�
	pos_.x = (indexX_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2 + Field::DISPLAY_POS_X);
	pos_.y=  (indexY_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2 + Field::DISPLAY_POS_Y);

	speed_ = NORMAL_SPEED;
}

void Player::Init()
{
	angle_ = 0.0f;
	kX_ = 0; 
	kY_ = 0;
	indexX_ = 9; 
	indexY_ = 16;
	moveTimer_ = 0;
	moveDirection_ = 0;
	feedGetNum_ = 0;
	moveInterval_ = Field::BLOCK_SIZE;
	powerFeedTimer_ = 0;
	speed2_ = 1.0f;
	imgIdX_ = 0;
	deadImgIdx_ = 0;
	wantMoveDirection_ = 0;
	isPowerFeed_ = false;
	isDead_ = false;
	isAnimeEnd_ = false;
	isPowerFeed2_ = false;
	isEnabled_ = true;
	isIntrusion_ = false;
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

	moveInterval_ = Field::BLOCK_SIZE / speed_;

	// �ړ��̃C���^�[�o��				// �ړ���������ɕǂ��Ȃ��ꍇ�ړ�
	if (moveTimer_ % moveInterval_ == 0 && !Colision(moveDirection_))
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
			if (!pField_->IsBlock(indexY_ + 1, indexX_) && pField_->Intrusion(indexY_ + 1, indexX_, isIntrusion_))
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

		// ������
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
	indexX_ = pField_->Worp(kY_, kX_, indexY_, indexX_);

	// �ړ����Ă���ꍇ�������s��
	if (moveDirection_)
	{
		// �A�j���[�V��������
		imgIdX_ = (imgIdX_ + 1) % (ANIME_FRAME_SPEED * ANIME_FRAME_NUM);
	}

	DrawFormatString(0, 30, 0xffffff, L"playerY : %d, playerX : %d", indexY_, indexX_);
}

void Player::Draw()
{
	// �v���C���[���G�Ɠ���������
	if (!isDead_ && isEnabled_)
	{
		int imgX = (imgIdX_ / ANIME_FRAME_SPEED) * 16;

		// �v���C���[�摜�̕\��
		DrawRectRotaGraph(pos_.x, pos_.y,		// ���W
						imgX, 0,				// �؂��荶��
						WIDTH, HEIGHT,			// ���A����
						SCALE, angle_,			// �g�嗦�A��]�p�x
						handle_, true);			// �摜�̃n���h���A���߂��邩
	}
	else if(isDead_ && isEnabled_)
	{
		// �摜�̃C���f�b�N�X���v�Z
		int imgX = (deadImgIdx_ / DEAD_ANIME_FRAME_SPEED) * 16;

		// �A�j���[�V�����摜�̉E�[�܂ŕ\�������ꍇ
		if (imgX >= deathImgSize_.x - WIDTH)
		{
			// �A�j���[�V�����I���t���O�𗧂Ă�
			isAnimeEnd_ = true;
		}

		// �Q�[���I�[�o�[���̉摜��\��
		DrawRectRotaGraph(pos_.x,							// ���W
						  pos_.y,
						  imgX, 0,							// �؂��荶��
						  WIDTH, HEIGHT,					// ���A����
						  SCALE, 0,							// �g�嗦�A��]�p�x
						  deathHandle_, true);				// �摜�̃n���h���A���߂��邩
	}
}

void Player::DeadUpdate()
{
	if (isDead_)
	{
		// �A�j���[�V��������
		deadImgIdx_ = (deadImgIdx_ + 1) % (DEAD_ANIME_FRAME_SPEED * DEAD_ANIME_FRAME_NUM);
	}
}

bool Player::Colision(int direction)
{
	switch (direction)
	{
	case up:
		if (pField_->IsBlock(indexY_ - 1, indexX_))	return true;
		break;
	case down:
		if (pField_->IsBlock(indexY_ + 1, indexX_) || !pField_->Intrusion(indexY_ + 1, indexX_, isIntrusion_))	return true;
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
	// �p���[�G�T���擾���Ă��Ȃ���ԂŃp���[�G�T���擾����
	if (!isPowerFeed_)
	{
		if (pField_->IsPowerFeed(indexY_, indexX_))
		{
			// �p���[�G�T�̃t���O�𗧂Ă�
			isPowerFeed_ = true;

			for (auto& enemy : pEnemy_)
			{
				// �G�̃C�W�P��Ԃ��J�n
				enemy->SetIzike(true);
			}
		}
	}
	else
	{
		// �p���[�G�T���擾���Ă����ԂŃp���[�G�T���擾����
		if (pField_->IsPowerFeed(indexY_, indexX_))
		{
			isPowerFeed2_ = true;
		}
	}

	// �p���[�G�T���擾���Ă����ꍇ
	// ��莞�ԃv���C���[�̈ړ��𑬂�����
	if (isPowerFeed_)
	{
		// �^�C�}�[���J�n
		powerFeedTimer_++;

		// �ړ����x��ύX
		speed_ = GET_FEED_SPEED;

		if (isPowerFeed2_)
		{
			powerFeedTimer_ = 1;
			isPowerFeed2_ = false;
		}

		// powerFeedTimer��FEED_DURATION�̓���̊����ɒB������G�̃A�j���[�V������ύX
		if ((FEED_DURATION * FLASH_RATIO) < powerFeedTimer_)
		{
			for (auto& enemy : pEnemy_)
			{
				enemy->SetFlash (true);	// �G�̓_�ł��J�n
			}	
		}
		else
		{
			for (auto& enemy : pEnemy_)
			{
				enemy->SetFlash(false);	
			}
		}

		// �^�C�}�[���w�肵�����Ԃ��o�߂����ꍇ���̑��x�ɖ߂�
		if (powerFeedTimer_ % FEED_DURATION == 0)
		{
			// ������
			powerFeedTimer_ = 0;
			isPowerFeed_ = false;

			for (auto& enemy : pEnemy_)
			{
				enemy->SetFlash(false);
				enemy->SetIzike(false);
			}

			// ���̈ړ����x�ɖ߂�
			speed_ = NORMAL_SPEED;
		}
	}
}

void Player::PosCalculation()
{
	// �C���f�b�N�X���W������W���v�Z
	pos_.x = (indexX_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2 + Field::DISPLAY_POS_X);
	pos_.y = (indexY_ * Field::BLOCK_SIZE) + (Field::BLOCK_SIZE / 2 + Field::DISPLAY_POS_Y);

	// �����Ă�������ƈړ����x�ɂ���č��W���v�Z
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