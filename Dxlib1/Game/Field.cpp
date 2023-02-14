#include "Field.h"
#include "Player.h"
#include "../DrawFunctions.h"
#include "EnemyBase.h"
#include <DxLib.h>

namespace
{
	//�}�b�v�f�[�^
	int mapData[Field::MAP_HEIGHT][Field::MAP_WIDTH] =
	{
		// 1 �G�T�A2  �ǁA3 �p���[�G�T�A4 

		{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
		{ 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2 },
		{ 2, 1, 2, 2, 1, 2, 2, 2, 1, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2 },
		{ 2, 3, 2, 2, 1, 2, 2, 2, 1, 2, 1, 2, 2, 2, 1, 2, 2, 3, 2 },
		{ 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2 },
		{ 2, 1, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 1, 2, 1, 2, 2, 1, 2 },
		{ 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2 },
		{ 2, 2, 2, 2, 1, 2, 2, 2, 0, 2, 0, 2, 2, 2, 1, 2, 2, 2, 2 },
		{ 4, 4, 4, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 4, 4, 4 },
		{ 2, 2, 2, 2, 1, 2, 0, 2, 2, 0, 2, 2, 0, 2, 1, 2, 2, 2, 2 },
		{ 0, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0 },
		{ 2, 2, 2, 2, 1, 2, 0, 2, 2, 2, 2, 2, 0, 2, 1, 2, 2, 2, 2 },
		{ 4, 4, 4, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 4, 4, 4 },
		{ 2, 2, 2, 2, 1, 2, 0, 2, 2, 2, 2, 2, 0, 2, 1, 2, 2, 2, 2 },
		{ 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2 },
		{ 2, 1, 2, 2, 1, 2, 2, 2, 1, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2 },
		{ 2, 3, 1, 2, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 3, 2 },
		{ 2, 2, 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 1, 2, 1, 2, 1, 2, 2 },
		{ 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2 },
		{ 2, 1, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 1, 2 },
		{ 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2 },
		{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
	};

	// �_�ŃX�s�[�h
	constexpr int FLASH_SPEED = 20;
}

Field::Field() :
	blendCount_(0),
	blendLimitMax_(false),
	isDraw_(true)
{
	pinkyGoalX_ = 1;
	pinkyGoalY_ = 1;

	blinkyGoalX_ = 17;
	blinkyGoalY_ = 1;

	InkyGoalX_ = 1;
	InkyGoalY_ = 20;

	handle_ = my::MyLoadGraph(L"Data/img/game/powerpacdot.png");
}

void Field::Updata()
{
	// �p���[�G�T�̓_�ōX�V����
	Flash();
}

void Field::Draw()
{
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			// �G�T�̕`��
			if (mapData[y][x] == 1)
			{
				DrawFormatString(x * BLOCK_SIZE + 10 + DISPLAY_POS_X, y * BLOCK_SIZE + 10 + DISPLAY_POS_Y, 0xffffff, L"�E", true);
			}
			// �ǂ̕`��
			if (mapData[y][x] == 2)
			{
				DrawBox(
					x * BLOCK_SIZE + DISPLAY_POS_X, y * BLOCK_SIZE + DISPLAY_POS_Y,
					x * BLOCK_SIZE + BLOCK_SIZE + DISPLAY_POS_X, y * BLOCK_SIZE + BLOCK_SIZE + DISPLAY_POS_Y,
					GetColor(0, 0, 255), false);
			}
			// �p���[�G�T�̕`��
			if (mapData[y][x] == 3)
			{
				PowerFeedFlash(y, x);
			}
#ifdef _DEBUG	// �f�o�b�N�̔ŏꍇ
			if (mapData2[y][x] >= 0)
			{
				DrawFormatString(x * BLOCK_SIZE + 10 + DISPLAY_POS_X, y * BLOCK_SIZE + 10 + DISPLAY_POS_Y, 0xffffff, L"%d", mapData2[y][x], true);
			}		
#endif
		}
	}
}

// �p���[�G�T�̕`��
void Field::PowerFeedFlash(int y, int x)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, blendCount_);
	DrawRotaGraph(x * BLOCK_SIZE + 16 + DISPLAY_POS_X, y * BLOCK_SIZE + 16 + DISPLAY_POS_Y, 1, 0, handle_, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}

// �p���[�G�T�̓_�ŏ���
void Field::Flash()
{
	//�_��
	if (blendLimitMax_)
	{
		blendCount_ += FLASH_SPEED;
		if (blendCount_ > 255)
			blendLimitMax_ = false;
	}
	else
	{
		blendCount_ -= FLASH_SPEED;
		if (blendCount_ < 0)
			blendLimitMax_ = true;
	}
}

// �Q�[���N���A����
bool Field::IsGameClearCheck()
{
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			if (mapData[y][x] == 1 || mapData[y][x] == 3)
			{
				return false;
			}
		}
	}
	return true;
}

// �u���b�N�����邩�ǂ���
bool Field::IsBlock(int y, int x)
{
	if (mapData[y][x] == 2)	
		return true;
	
	return false;
}

// �G�T�����邩�ǂ���
bool Field::IsFeed(int y, int x)
{
	if (mapData[y][x] == 1)
	{
		mapData[y][x] = 0;
		return true;
	}

	return false;
}

// �p���[�G�T�����邩�ǂ���
bool Field::IsPowerFeed(int y, int x)
{
	if (mapData[y][x] == 3)
	{
		mapData[y][x] = 0;
		return true;
	}
	return false;
}

int  Field::Worp(int ky, int kx, int indexY, int indexX)
{
	//�O�̍��W������̈ʒu�����݂̍��W������̈ʒu�Ȃ烏�[�v
	if (ky == 10 && kx == 1 && indexY == 10 && indexX == 0)
	{
		indexX = 18;
		
	}
	if (ky == 10 && kx == 17 && indexY == 10 && indexX == 18)
	{
		indexX = 0;
	}
	return (indexX);
}

void Field::MoveDataSet(int playerY, int playerX)
{
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			if (mapData[y][x] != 2 && mapData[y][x] != 4)
			{
				mapData2[y][x] = 0;
			}
		}
	}

	mapData2[playerY][playerX] = 0;
	
	Search(playerY, playerX, 0);
}

void Field::Search(int y, int x, int pos)
{
	pos += 1;

	if (x > 0 && y > 0 && x < MAP_WIDTH - 1 && y < MAP_HEIGHT - 1)
	{
		if (mapData2[y - 1][x] == 0 || mapData2[y - 1][x] > pos)
		{
			mapData2[y - 1][x] = pos;
			Search(y - 1, x, pos);
		}
		if (mapData2[y + 1][x] == 0 || mapData2[y + 1][x] > pos)
		{
			mapData2[y + 1][x] = pos;
			Search(y + 1, x, pos);
		}
		if (mapData2[y][x - 1] == 0 || mapData2[y][x - 1] > pos)
		{
			mapData2[y][x - 1] = pos;
			Search(y, x - 1, pos);
		}
		if (mapData2[y][x + 1] == 0 || mapData2[y][x + 1] > pos)
		{
			mapData2[y][x + 1] = pos;
			Search(y, x + 1, pos);
		}

		mapData2[pPlayer_->GetIndexY()][pPlayer_->GetIndexX()] = 0;
	}
	
	return;
}


int Field::BlinkyMove(int enemyIndexY, int enemyIndexX)
{
	int y = enemyIndexY;
	int x = enemyIndexX;
	
	if (!pEnemy_[0]->GetTracking())
	{
		// �ǐՃ��[�h
		MoveDataSet(pPlayer_->GetIndexY(), pPlayer_->GetIndexX());
	}
	else
	{
		// �꒣�胂�[�h
		MoveDataSet(blinkyGoalY_, blinkyGoalX_);
	}
	if (pEnemy_[0]->GetIzike())
	{
		if (!IsBlock(y - 1, x) && mapData2[y][x] < mapData2[y - 1][x])
		{
			return EnemyBase::up;
		}
		if (!IsBlock(y + 1, x) && mapData2[y][x] < mapData2[y + 1][x])
		{
			return EnemyBase::down;
		}
		if (!IsBlock(y, x - 1) && mapData2[y][x] < mapData2[y][x - 1])
		{
			return EnemyBase::left;
		}
		if (!IsBlock(y, x + 1) && mapData2[y][x] < mapData2[y][x + 1])
		{
			return EnemyBase::right;
		}
	}
	else
	{
		if (!IsBlock(y - 1, x) && mapData2[y][x] > mapData2[y - 1][x])
		{
			return EnemyBase::up;
		}
		if (!IsBlock(y + 1, x) && mapData2[y][x] > mapData2[y + 1][x])
		{
			return EnemyBase::down;
		}
		if (!IsBlock(y, x - 1) && mapData2[y][x] > mapData2[y][x - 1])
		{
			return EnemyBase::left;
		}
		if (!IsBlock(y, x + 1) && mapData2[y][x] > mapData2[y][x + 1])
		{
			return EnemyBase::right;
		}
	}
	return 0;
}

int Field::PinkyMove(int enemyIndexY, int enemyIndexX)
{
	int y = enemyIndexY;
	int x = enemyIndexX;

	if (!pEnemy_[3]->GetTracking())
	{
		// �ǐՃ��[�h
		MoveDataSet(pPlayer_->GetIndexY(), pPlayer_->GetIndexX());
	}
	else
	{
		// �꒣�胂�[�h
		MoveDataSet(pinkyGoalY_, pinkyGoalX_);
	}
	if (pEnemy_[3]->GetIzike())
	{
		if (!IsBlock(y + 1, x) && mapData2[y][x] < mapData2[y + 1][x])
		{
			return EnemyBase::down;
		}
		if (!IsBlock(y - 1, x) && mapData2[y][x] < mapData2[y - 1][x])
		{
			return EnemyBase::up;
		}
		if (!IsBlock(y, x + 1) && mapData2[y][x] < mapData2[y][x + 1])
		{
			return EnemyBase::right;
		}
		if (!IsBlock(y, x - 1) && mapData2[y][x] < mapData2[y][x - 1])
		{
			return EnemyBase::left;
		}
	}
	else
	{
		if (!IsBlock(y + 1, x) && mapData2[y][x] > mapData2[y + 1][x])
		{
			return EnemyBase::down;
		}
		if (!IsBlock(y - 1, x) && mapData2[y][x] > mapData2[y - 1][x])
		{
			return EnemyBase::up;
		}
		if (!IsBlock(y, x + 1) && mapData2[y][x] > mapData2[y][x + 1])
		{
			return EnemyBase::right;
		}
		if (!IsBlock(y, x - 1) && mapData2[y][x] > mapData2[y][x - 1])
		{
			return EnemyBase::left;
		}
	}
	return 0;
}

int Field::InkyMove(int enemyIndexY, int enemyIndexX)
{
	int y = enemyIndexY;
	int x = enemyIndexX;

	if (!pEnemy_[1]->GetTracking())
	{
		// �ǐՃ��[�h
		MoveDataSet(pPlayer_->GetIndexY(), pPlayer_->GetIndexX());
	}
	else
	{
		// �꒣�胂�[�h
		MoveDataSet(InkyGoalY_, InkyGoalX_);
	}
	
	if (pEnemy_[1]->GetIzike())
	{
		if (!IsBlock(y - 1, x) && mapData2[y][x] < mapData2[y - 1][x])
		{
			return EnemyBase::up;
		}
		if (!IsBlock(y + 1, x) && mapData2[y][x] < mapData2[y + 1][x])
		{
			return EnemyBase::down;
		}
		if (!IsBlock(y, x - 1) && mapData2[y][x] < mapData2[y][x - 1])
		{
			return EnemyBase::left;
		}
		if (!IsBlock(y, x + 1) && mapData2[y][x] < mapData2[y][x + 1])
		{
			return EnemyBase::right;
		}
	}
	else
	{
		if (!IsBlock(y - 1, x) && mapData2[y][x] > mapData2[y - 1][x])
		{
			return EnemyBase::up;
		}
		if (!IsBlock(y + 1, x) && mapData2[y][x] > mapData2[y + 1][x])
		{
			return EnemyBase::down;
		}
		if (!IsBlock(y, x - 1) && mapData2[y][x] > mapData2[y][x - 1])
		{
			return EnemyBase::left;
		}
		if (!IsBlock(y, x + 1) && mapData2[y][x] > mapData2[y][x + 1])
		{
			return EnemyBase::right;
		}
	}

	return 0;
}
