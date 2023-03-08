#include "Player.h"
#include "Field.h"
#include "../DrawFunctions.h"
#include "../InputState.h"
#include "Skeleton.h"
#include "../game.h"
#include "EnemyBase.h"
#include "../SoundManager.h"
#include <DxLib.h>

namespace
{
	// �ʏ�̃v���C���[�̈ړ��X�s�[�h(���{��)
	constexpr float NORMAL_SPEED = 1.4f;

	// �p���[�G�T���擾�����ꍇ�̈ړ��X�s�[�h(���{��)
	constexpr float GET_FEED_SPEED = 1.7f;

	// �p���[�G�T���擾�����ꍇ��������(���b��)
	constexpr int FEED_DURATION = 60 * 8;

	// �v���C���[���c�艽���Ńt���b�V�������邩�̊���
	constexpr float FLASH_RATIO = 0.7f;
}

Player::Player(int normalH, int waponH, int deadH, int attackH, int indexX, int indexY) :
	angle_(0.0f),
	kX_(0), kY_(0),
	moveTimer_(0),
	moveDirection_(0),
	feedGetNum_(0),
	moveInterval_(Field::CHIP_SIZE),
	powerFeedTimer_(0),
	speed2_(1.0f),
	imgIdX_(0),
	deadImgIdx_(0),
	attackImgIdx_(0),
	wantMoveDirection_(0),
	imgY_(0),
	timer_(0),
	isPowerFeed_(false),
	isDead_(false),
	isAnimeEnd_(false),
	isPowerFeed2_(false),
	isEnabled_(true),
	isIntrusion_(false),
	isTurnFlag_(false),
	isFlash_(false),
	enemyKill_(false)
{
	indexX_ = indexX;
	indexY_ = indexY;

	normalH_ = normalH;
	waponH_ = waponH;
	deathH_ = deadH;
	attackH_ = attackH;

	handle_ = normalH_;

	// �摜�T�C�Y�̎擾
	GetGraphSizeF(deathH_, &deathImgSize_.x, &deathImgSize_.y);
	GetGraphSizeF(attackH_, &attackImgSize_.x, &attackImgSize_.y);

	// �C���f�b�N�X�̍��W���珉�����W�����߂�
	pos_.x = (indexX_ * Field::CHIP_SIZE) + (Field::CHIP_SIZE / 2 + Field::DISPLAY_POS_X);
	pos_.y=  (indexY_ * Field::CHIP_SIZE) + (Field::CHIP_SIZE / 2 + Field::DISPLAY_POS_Y);

	speed_ = NORMAL_SPEED;
}

void Player::Init()
{
	handle_ = normalH_;
	angle_ = 0.0f;
	kX_ = 0; 
	kY_ = 0;
	indexX_ = 9; 
	indexY_ = 16;
	moveTimer_ = 0;
	moveDirection_ = 0;
	feedGetNum_ = 0;
	moveInterval_ = Field::CHIP_SIZE;
	powerFeedTimer_ = 0;
	speed2_ = 1.0f;
	imgIdX_ = 0;
	deadImgIdx_ = 0;
	attackImgIdx_ = 0;
	wantMoveDirection_ = 0;
	isPowerFeed_ = false;
	isDead_ = false;
	isAnimeEnd_ = false;
	isPowerFeed2_ = false;
	isEnabled_ = true;
	isIntrusion_ = false;
	isTurnFlag_ = false;
	isFlash_ = false;
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

	moveInterval_ = Field::CHIP_SIZE / speed_;

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
				imgY_ = 32;
				moveDirection_ = wantMoveDirection_;
			}
		}
		else if (wantMoveDirection_ == down)
		{
			if (!pField_->IsBlock(indexY_ + 1, indexX_) && pField_->Intrusion(indexY_ + 1, indexX_, isIntrusion_))
			{
				//�����̐؂�ւ�
				imgY_ = 96;
				moveDirection_ = wantMoveDirection_;
			}
		}
		else if (wantMoveDirection_ == left)
		{
			if (!pField_->IsBlock(indexY_, indexX_ - 1))
			{
				//�����̐؂�ւ�
				imgY_ = 64;
				moveDirection_ = wantMoveDirection_;
			}
		}
		else if (wantMoveDirection_ == right)
		{
			if (!pField_->IsBlock(indexY_, indexX_ + 1))
			{
				// �����̐؂�ւ�
				imgY_ = 0;
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
}

void Player::Draw()
{
	if (!isDead_ && isEnabled_ && !enemyKill_)
	{
		if (isFlash_)
		{
			if ((powerFeedTimer_ / 10) % 2 == 0)
			{
				return;
			}
		}

		int imgX = (imgIdX_ / ANIME_FRAME_SPEED) * WIDTH;

		// �v���C���[�摜�̕\��
		DrawRectRotaGraph(pos_.x, pos_.y - 5,		// ���W
						imgX, imgY_,				// �؂��荶��
						WIDTH, HEIGHT,				// ���A����
						SCALE, 0.0f,				// �g�嗦�A��]�p�x
						handle_, true, isTurnFlag_);	// �摜�̃n���h���A���߂��邩
	}
	else if(isDead_ && isEnabled_ && !enemyKill_)
	{
		// �摜�̃C���f�b�N�X���v�Z
		int imgX = (deadImgIdx_ / DEAD_ANIME_FRAME_SPEED) * WIDTH;

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
						  deathH_, true);					// �摜�̃n���h���A���߂��邩
	}
	else if (enemyKill_ && isEnabled_)
	{
		// �摜�̃C���f�b�N�X���v�Z
		int imgX = (attackImgIdx_ / ATTACK_ANIME_FRAME_SPEED) * WIDTH;

		// �A�j���[�V�����摜�̉E�[�܂ŕ\�������ꍇ
		if (imgX >= attackImgSize_.x - WIDTH)
		{
			// �A�j���[�V�����I���t���O�𗧂Ă�
			isAnimeEnd_ = true;
		}

		// �Q�[���I�[�o�[���̉摜��\��
		DrawRectRotaGraph(pos_.x,							// ���W
			pos_.y,
			imgX, imgY_,							// �؂��荶��
			WIDTH, HEIGHT,					// ���A����
			SCALE, 0,							// �g�嗦�A��]�p�x
			attackH_, true);					// �摜�̃n���h���A���߂��邩
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

void Player::EnemyKillUpdate()
{
	attackImgIdx_ = (attackImgIdx_ + 1) % (ATTACK_ANIME_FRAME_SPEED * ATTACK_ANIME_FRAME_NUM);
}

bool Player::Colision(int direction)
{
	switch (direction)
	{
	case up:
		if (pField_->IsBlock(indexY_ - 1, indexX_))	return true;
		break;
	case down:
		if (pField_->IsBlock(indexY_ + 1, indexX_) || 
			!pField_->Intrusion(indexY_ + 1, indexX_, isIntrusion_))	return true;
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

			// �v���C���[�摜��ύX
			handle_ = waponH_;

			for (auto& enemy : pEnemy_)
			{
				// �G�̃C�W�P��Ԃ��J�n
				enemy->SetIzike(true);
			}
		}
	}
	else
	{
		// �v���C���[�摜��ύX
		handle_ = waponH_;

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
			// �v���C���[�̓_�ł��J�n
			isFlash_ = true;
		}
		else
		{
			isFlash_ = false;
		}

		// �^�C�}�[���w�肵�����Ԃ��o�߂����ꍇ���̑��x�ɖ߂�
		if (powerFeedTimer_ % FEED_DURATION == 0)
		{
			// ������
			powerFeedTimer_ = 0;
			isPowerFeed_ = false;

			isFlash_ = false;

			for (auto& enemy : pEnemy_)
			{
				enemy->SetIzike(false);
			}

			SoundManager::GetInstance().Play("powerDown");

			// �v���C���[�摜��ύX
			handle_ = normalH_;

			// ���̈ړ����x�ɖ߂�
			speed_ = NORMAL_SPEED;
		}
	}
}

void Player::PosCalculation()
{
	// �C���f�b�N�X���W������W���v�Z
	pos_.x = (indexX_ * Field::CHIP_SIZE) + (Field::CHIP_SIZE / 2 + Field::DISPLAY_POS_X);
	pos_.y = (indexY_ * Field::CHIP_SIZE) + (Field::CHIP_SIZE / 2 + Field::DISPLAY_POS_Y);

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