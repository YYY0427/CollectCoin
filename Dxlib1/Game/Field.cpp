#include "Field.h"
#include "Player.h"
#include "../DrawFunctions.h"
#include "EnemyBase.h"
#include "../game.h"
#include "../SoundManager.h"
#include <DxLib.h>

namespace
{
	// 0 : �Ȃɂ��Ȃ�
	// 8 : �Ȃɂ��Ȃ�
	// 1 : �R�C��
	// 2 : ��
	// 3 : �� 
	// 7 : ��(������͏o��āA�O����͓���Ȃ�)

	// �`���[�g���A��
	constexpr int TUTORIAL_HEIGHT = 10;
	constexpr int TUTORIAL_WIDTH = 10;

	constexpr int mapDataTutorial[TUTORIAL_HEIGHT][TUTORIAL_WIDTH] =
	{
		{  2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
		{  2, 3, 0, 0, 0, 0, 0, 0, 3, 2 },
		{  2, 1, 0, 0, 0, 0, 0, 0, 1, 2 },
		{  2, 1, 2, 2, 2, 2, 2, 2, 1, 2 },
		{  2, 1, 2, 2, 2, 2, 2, 2, 1, 2 },
		{  0, 1, 0, 0, 0, 0, 0, 0, 1, 0 },
		{  2, 1, 2, 2, 2, 2, 2, 2, 1, 2 },
		{  2, 1, 2, 2, 2, 2, 2, 2, 1, 2 },
		{  2, 3, 1, 1, 1, 1, 1, 1, 3, 2 },
		{  2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
	};

	// �X�e�[�W1 
	constexpr int STAGE_1_HEIGHT = 22;
	constexpr int STAGE_1_WIDTH = 19;

	constexpr int mapDataStage1[STAGE_1_HEIGHT][STAGE_1_WIDTH] =
	{
		{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
		{ 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2 },
		{ 2, 1, 2, 2, 1, 2, 2, 2, 1, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2 },
		{ 2, 3, 2, 2, 1, 2, 2, 2, 1, 2, 1, 2, 2, 2, 1, 2, 2, 3, 2 },
		{ 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2 },
		{ 2, 1, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 1, 2, 1, 2, 2, 1, 2 },
		{ 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2 },
		{ 2, 2, 2, 2, 1, 2, 2, 2, 0, 2, 0, 2, 2, 2, 1, 2, 2, 2, 2 },
		{ 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 8, 8, 8 },
		{ 2, 2, 2, 2, 1, 2, 0, 2, 2, 7, 2, 2, 0, 2, 1, 2, 2, 2, 2 },
		{ 0, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0 },
		{ 2, 2, 2, 2, 1, 2, 0, 2, 2, 2, 2, 2, 0, 2, 1, 2, 2, 2, 2 },
		{ 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 8, 8, 8 },
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

	// �f�o�b�N�p
	constexpr int mapDataDebug[STAGE_1_HEIGHT][STAGE_1_WIDTH] =
	{
		{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
		{ 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
		{ 2, 0, 2, 2, 0, 2, 2, 2, 0, 2, 0, 2, 2, 2, 0, 2, 2, 0, 2 },
		{ 2, 3, 2, 2, 0, 2, 2, 2, 0, 2, 0, 2, 2, 2, 0, 2, 2, 3, 2 },
		{ 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
		{ 2, 0, 2, 2, 0, 2, 0, 2, 2, 2, 2, 2, 0, 2, 0, 2, 2, 0, 2 },
		{ 2, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 2 },
		{ 2, 2, 2, 2, 0, 2, 2, 2, 0, 2, 0, 2, 2, 2, 0, 2, 2, 2, 2 },
		{ 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 8, 8, 8 },
		{ 2, 2, 2, 2, 0, 2, 0, 2, 2, 7, 2, 2, 0, 2, 0, 2, 2, 2, 2 },
		{ 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0 },
		{ 2, 2, 2, 2, 0, 2, 0, 2, 2, 2, 2, 2, 0, 2, 0, 2, 2, 2, 2 },
		{ 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 8, 8, 8 },
		{ 2, 2, 2, 2, 0, 2, 0, 2, 2, 2, 2, 2, 0, 2, 0, 2, 2, 2, 2 },
		{ 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
		{ 2, 0, 2, 2, 0, 2, 2, 2, 0, 2, 0, 2, 2, 2, 0, 2, 2, 0, 2 },
		{ 2, 3, 0, 2, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 3, 2 },
		{ 2, 2, 0, 2, 0, 2, 0, 2, 2, 2, 2, 2, 0, 2, 0, 2, 0, 2, 2 },
		{ 2, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 2 },
		{ 2, 0, 2, 2, 2, 2, 2, 2, 0, 2, 0, 2, 2, 2, 2, 2, 2, 0, 2 },
		{ 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
		{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
	};

	// �_�ŃX�s�[�h
	constexpr int FLASH_SPEED = 20;

	// �R�C���̃A�j���[�V����
	constexpr int COIN_FRAME_NUM = 4;		
	constexpr int COIN_FRAME_SPEED = 15;

	// �h�A�̃A�j���[�V����
	constexpr int DOOR_FRAME_NUM = 4;
	constexpr int DOOR_FRAME_SPEED = 50;
}

Field::Field(int sordH, int doorH, int coinH, int stage) :
	sordH_(sordH),
	doorH_(doorH),
	coinH_(coinH),
	coinImgIdx_(0),
	doorImgIdx_(0),
	getCoinNum_(0),
	
	isDraw_(true),
	closeDoor_(false),
	openDoor_(false)
{
	stage_ = stage;

	StageCheck(stage);

	stringH_ = CreateFontToHandle("PixelMplus10", 30, 10);
}

Field::~Field()
{
	DeleteFontToHandle(stringH_);
}

void Field::StageCheck(int stage)
{
	stage_ = stage;
	switch (stage)
	{
	case 0:
		mapHeight_ = TUTORIAL_HEIGHT;
		mapWidth_ = TUTORIAL_WIDTH;
		disPlayPosX_ = TUTORIAL_DISPLAY_POS_X;
		disPlayPosY_ = TUTORIAL_DISPLAY_POS_Y;
		blinkyGoalX_ = 5;
		blinkyGoalY_ = 2;
		pEnemy_.resize(1);
		break;
	case 1:
		mapHeight_ = STAGE_1_HEIGHT;
		mapWidth_ = STAGE_1_WIDTH;
		disPlayPosX_ = STAGE_1_DISPLAY_POS_X;
		disPlayPosY_ = STAGE_1_DISPLAY_POS_Y;
		pinkyGoalX_ = 1;
		pinkyGoalY_ = 1;
		blinkyGoalX_ = 17;
		blinkyGoalY_ = 1;
		inkyGoalX_ = 1;
		inkyGoalY_ = 20;
		crydeGoalY_ = 20;
		crydeGoalX_ = 17;
		pEnemy_.resize(4);
		break;
	}

	mapData_.resize(mapHeight_);
	for (int y = 0; y < mapHeight_; y++)
	{
		mapData_[y].resize(mapWidth_);
	}

	enemyTargetMapData_.resize(mapHeight_);
	for (int y = 0; y < mapHeight_; y++)
	{
		enemyTargetMapData_[y].resize(mapWidth_);
	}

	switch (stage)
	{
	case 0:
		for (int y = 0; y < mapHeight_; y++)
		{
			for (int x = 0; x < mapWidth_; x++)
			{
				mapData_[y][x] = mapDataTutorial[y][x];
			}
		}
		break;
	case 1:
		for (int y = 0; y < mapHeight_; y++)
		{
			for (int x = 0; x < mapWidth_; x++)
			{
				mapData_[y][x] = mapDataStage1[y][x];
			//	mapData_[y][x] = mapDataDebug[y][x];
			}
		}
		break;
	}

	// �R�C���̍��v�����̎擾
	coinTotalNum_ = LestCoin();

	isDraw_ = false;
}

void Field::Updata()
{
	// �A�j���[�V��������
	coinImgIdx_ = (coinImgIdx_ + 1) % (COIN_FRAME_SPEED * COIN_FRAME_NUM);
	doorImgIdx_ = (doorImgIdx_ + 1) % (DOOR_FRAME_SPEED * DOOR_FRAME_NUM);

	/*for (auto& enemy : pEnemy_)
	{
		if (enemy->GetDoor())
		{
			openDoor_ = true;
		}
		if (openDoor_)
		{
			doorImgIdx_++;
			if (doorImgIdx_ == (DOOR_FRAME_NUM - 1) * DOOR_FRAME_SPEED)
			{
				openDoor_ = false;
				enemy->SetDoor(false);
				closeDoor_ = true;
			}
		}
		if (closeDoor_)
		{
			doorImgIdx_--;
			if (doorImgIdx_ <= 0)
			{
				closeDoor_ = false;
			}
		}
	}*/
}

void Field::Draw()
{
	for (int y = 0; y < mapHeight_; y++)
	{
		for (int x = 0; x < mapWidth_; x++)
		{
			// �R�C���̕`��
			if (mapData_[y][x] == 1)
			{
				int imgX = (coinImgIdx_ / COIN_FRAME_SPEED) * 8;
				DrawRectRotaGraph(x * CHIP_SIZE + 16 + disPlayPosX_, y * CHIP_SIZE + 16 + disPlayPosY_,
					imgX, 0, 8, 8, 2.0f, 0.0f, coinH_, true);
			}
			// ���̕`��
			if (mapData_[y][x] == 3)
			{
				DrawRectRotaGraph(x * CHIP_SIZE + 16 + disPlayPosX_, y * CHIP_SIZE + 16 + disPlayPosY_,
					0, 0, 16, 16, 2.0f, 0.0f, sordH_, true);
			}
			// ���̕`��
			if (mapData_[y][x] == 7)
			{
				int imgX = (doorImgIdx_ / DOOR_FRAME_SPEED) * 16;
				DrawRectRotaGraph(x * CHIP_SIZE + 16 + disPlayPosX_, y * CHIP_SIZE + 16 + disPlayPosY_,
					imgX, 0, 16, 16, 2.0f, 0.0f, doorH_, true);
			}
			// �ǂ̕`��
			/*if (mapData[y][x] == 2)
			{
				DrawBox(
					x * CHIP_SIZE + DISPLAY_POS_X, y * CHIP_SIZE + DISPLAY_POS_Y,
					x * CHIP_SIZE + CHIP_SIZE + DISPLAY_POS_X, y * CHIP_SIZE + CHIP_SIZE + DISPLAY_POS_Y,
					GetColor(255, 255, 255), true);
			}*/
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawFormatStringToHandle(Game::kScreenWidth / 2 - 225 + 2, Game::kScreenHeight - 50 + 2, 0x000000, stringH_, "%d / %d\n", getCoinNum_, coinTotalNum_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �c��̃R�C���̖���
	DrawRectRotaGraph(Game::kScreenWidth / 2 - 260, Game::kScreenHeight - 35, 0, 0, 8, 8, 3.7f, 0.0f, coinH_, true);
	DrawFormatStringToHandle(Game::kScreenWidth / 2 - 225, Game::kScreenHeight - 50, 0xffffff, stringH_, "%d / %d\n", getCoinNum_, coinTotalNum_);
}

void Field::SetEnemy(std::shared_ptr<EnemyBase> enemy, int i, int stage)
{
	pEnemy_[i] = enemy;
}

int Field::LestCoin()
{
	int coin = 0;
	for (int y = 0; y < mapHeight_; y++)
	{
		for (int x = 0; x < mapWidth_; x++)
		{
			if (mapData_[y][x] == 1)
			{
				coin++;
			}
		}
	}
	return coin;
}

// �Q�[���N���A����
bool Field::IsGameClearCheck()
{
	for (int y = 0; y < mapHeight_; y++)
	{
		for (int x = 0; x < mapWidth_; x++)
		{
			if (mapData_[y][x] == 1)
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
	if (x >= 0 && y >= 0 && x < mapWidth_ && y < mapHeight_)
	{
		if (mapData_[y][x] == 2)
			return true;
	}
	return false;
}

// �R�C�������邩�ǂ���
bool Field::IsFeed(int y, int x)
{
	if (x >= 0 && y >= 0 && x < mapWidth_ && y < mapHeight_)
	{
		if (mapData_[y][x] == 1)
		{
			getCoinNum_++;
			SoundManager::GetInstance().Play("coin");
			mapData_[y][x] = 0;
			return true;
		}
	}

	return false;
}

// �������邩�ǂ���
bool Field::IsPowerFeed(int y, int x)
{
	if (x >= 0 && y >= 0 && x < mapWidth_ && y < mapHeight_)
	{
		if (mapData_[y][x] == 3)
		{
			SoundManager::GetInstance().Play("powerUp");
			mapData_[y][x] = 0;
			return true;
		}
	}
	return false;
}

int  Field::Warp(int ky, int kx, int indexY, int indexX)
{
	if (stage_ == 0)
	{
		//�O�̍��W������̈ʒu�����݂̍��W������̈ʒu�Ȃ烏�[�v
		if (ky == 5 && kx == 1 && indexY == 5 && indexX == 0)
		{
			SoundManager::GetInstance().Play("warp");
			indexX = 9;

		}
		if (ky == 5 && kx == 8 && indexY == 5 && indexX == 9)
		{
			SoundManager::GetInstance().Play("warp");
			indexX = 0;
		}
	}
	if (stage_ == 1)
	{
		//�O�̍��W������̈ʒu�����݂̍��W������̈ʒu�Ȃ烏�[�v
		if (ky == 10 && kx == 1 && indexY == 10 && indexX == 0)
		{
			SoundManager::GetInstance().Play("warp");
			indexX = 18;

		}
		if (ky == 10 && kx == 17 && indexY == 10 && indexX == 18)
		{
			SoundManager::GetInstance().Play("warp");
			indexX = 0;
		}
	}
	return (indexX);
}

void Field::MoveDataSet(int goalY, int goalX)
{
	for (int y = 0; y < mapHeight_; y++)
	{
		for (int x = 0; x < mapWidth_; x++)
		{
			if (mapData_[y][x] != 2 && mapData_[y][x] != 8)
			{
				enemyTargetMapData_[y][x] = 0;
			}
		}
	}

	Search(goalY, goalX, goalY, goalX, 0);
}

void Field::Search(int y, int x, int goalY, int goalX, int pos)
{
	pos += 1;

	if (x > 0 && y > 0 && x < mapWidth_ - 1 && y < mapHeight_ - 1)
	{
		if (enemyTargetMapData_[y - 1][x] == 0 || enemyTargetMapData_[y - 1][x] > pos)
		{
			enemyTargetMapData_[y - 1][x] = pos;
			Search(y - 1, x, goalY, goalX, pos);
		}
		if (enemyTargetMapData_[y + 1][x] == 0 || enemyTargetMapData_[y + 1][x] > pos)
		{
			enemyTargetMapData_[y + 1][x] = pos;
			Search(y + 1, x, goalY, goalX, pos);
		}
		if (enemyTargetMapData_[y][x - 1] == 0 || enemyTargetMapData_[y][x - 1] > pos)
		{
			enemyTargetMapData_[y][x - 1] = pos;
			Search(y, x - 1, goalY, goalX, pos);
		}
		if (enemyTargetMapData_[y][x + 1] == 0 || enemyTargetMapData_[y][x + 1] > pos)
		{
			enemyTargetMapData_[y][x + 1] = pos;
			Search(y, x + 1, goalY, goalX, pos);
		}

		for (auto& enemy : pEnemy_)
		{
			if (enemy->GetTracking())
			{
				enemyTargetMapData_[pPlayer_->GetIndexY()][pPlayer_->GetIndexX()] = 0;
			}
			else
			{
				enemyTargetMapData_[goalY][goalX] = 0;
			}
		}
	}
	
	return;
}

bool Field::SlowPosition(int y, int x)
{
	if (y == 9 && x == 9)
	{
		return true;
	}
	if (y == 10 && x > 0 && x < 4)
	{
		return true;
	}
	if (y == 10 && x < 21 && x > 18)
	{
		return true;
	}
	if (y == 10 && x >= 8 && x <= 10)
	{
		return true;
	}
	return false;
}

bool Field::SpornInOrAuto(int y, int x)
{
	if (y == 10 && x >= 8 && x <= 10)
	{
		// ���X�|�[���n�_�ɂ���
		return true;
	}
	if (y == 9 && x == 9)
	{
		return true;
	}
	// ���X�|�[���n�_�ɂ��Ȃ�
	return false;
}

bool Field::Intrusion(int y, int x, bool flag)
{
	// flag��false�̏ꍇ�ʂ�Ȃ�
	// flag��true�̏ꍇ�ʂ��
	if (!flag)
	{
		if (x >= 0 && y >= 0 && x < mapWidth_ && y < mapHeight_)
		{
			if (mapData_[y][x] == 7)
			{
				// �ʂ�Ȃ�
				return false;
			}
		}
	}
	// �ʂ��
	return true;
}

int Field::SkeletonMove(int enemyIndexY, int enemyIndexX, bool flag)
{
	int y = enemyIndexY;
	int x = enemyIndexX;
	
	if (pEnemy_[0]->GetMove())
	{
		if (pEnemy_[0]->GetTracking() || pEnemy_[0]->GetIzike() || stage_ == 0)
		{
			// �ǐՃ��[�h
			MoveDataSet(pPlayer_->GetIndexY(), pPlayer_->GetIndexX());
		}
		// �꒣�胂�[�h�̂Ƃ��̈ړ�
		else if (!pEnemy_[0]->GetTracking())
		{
			// �꒣�胂�[�h
			MoveDataSet(blinkyGoalY_, blinkyGoalX_);

			if (pEnemy_[0]->GetIndexY() == blinkyGoalY_ && pEnemy_[0]->GetIndexX() == blinkyGoalX_)
			{
				if (blinkyGoalX_ == 17 && blinkyGoalY_ == 1)
				{
					blinkyGoalX_ = 10;
					blinkyGoalY_ = 1;
				}
				else if (blinkyGoalX_ == 10 && blinkyGoalY_ == 1)
				{
					blinkyGoalX_ = 10;
					blinkyGoalY_ = 4;

				}
				else if (blinkyGoalX_ == 10 && blinkyGoalY_ == 4)
				{
					blinkyGoalX_ = 17;
					blinkyGoalY_ = 4;
				}
				else if (blinkyGoalX_ == 17 && blinkyGoalY_ == 4)
				{
					blinkyGoalX_ = 17;
					blinkyGoalY_ = 1;
				}
			}
		}

		if (pEnemy_[0]->GetIzike() && !SpornInOrAuto(enemyIndexY, enemyIndexX))
		{
			if (!IsBlock(y - 1, x) && Intrusion(y - 1, x, flag) && enemyTargetMapData_[y][x] < enemyTargetMapData_[y - 1][x])
			{
				return EnemyBase::up;
			}
			if (!IsBlock(y + 1, x) && Intrusion(y + 1, x, flag) && enemyTargetMapData_[y][x] < enemyTargetMapData_[y + 1][x])
			{
				return EnemyBase::down;
			}
			if (!IsBlock(y, x - 1) && enemyTargetMapData_[y][x] < enemyTargetMapData_[y][x - 1])
			{
				return EnemyBase::left;
			}
			if (!IsBlock(y, x + 1) && enemyTargetMapData_[y][x] < enemyTargetMapData_[y][x + 1])
			{
				return EnemyBase::right;
			}
		}
		else
		{
			if (!IsBlock(y - 1, x) && Intrusion(y - 1, x, flag) && enemyTargetMapData_[y][x] > enemyTargetMapData_[y - 1][x])
			{
				return EnemyBase::up;
			}
			if (!IsBlock(y + 1, x) && Intrusion(y + 1, x, flag) && enemyTargetMapData_[y][x] > enemyTargetMapData_[y + 1][x])
			{
				return EnemyBase::down;
			}
			if (!IsBlock(y, x - 1) && enemyTargetMapData_[y][x] > enemyTargetMapData_[y][x - 1])
			{
				return EnemyBase::left;
			}
			if (!IsBlock(y, x + 1) && enemyTargetMapData_[y][x] > enemyTargetMapData_[y][x + 1])
			{
				return EnemyBase::right;
			}
		}
	}
	return 0;
}

int Field::GolemMove(int enemyIndexY, int enemyIndexX, bool flag)
{
	int y = enemyIndexY;
	int x = enemyIndexX;

	if (pEnemy_[3]->GetMove())
	{
		if (pEnemy_[3]->GetTracking() || pEnemy_[3]->GetIzike())
		{
			// �ǐՃ��[�h
			MoveDataSet(pPlayer_->GetIndexY(), pPlayer_->GetIndexX());
		}
		else if (!pEnemy_[3]->GetTracking())
		{
			// �꒣�胂�[�h
			MoveDataSet(pinkyGoalY_, pinkyGoalX_);

			if (pEnemy_[3]->GetIndexY() == pinkyGoalY_ && pEnemy_[3]->GetIndexX() == pinkyGoalX_)
			{
				if (pinkyGoalX_ == 1 && pinkyGoalY_ == 1)
				{
					pinkyGoalX_ = 1;
					pinkyGoalY_ = 4;
				}
				else if (pinkyGoalX_ == 1 && pinkyGoalY_ == 4)
				{
					pinkyGoalX_ = 8;
					pinkyGoalY_ = 4;
				}
				else if (pinkyGoalX_ == 8 && pinkyGoalY_ == 4)
				{
					pinkyGoalX_ = 8;
					pinkyGoalY_ = 1;
				}
				else if (pinkyGoalX_ == 8 && pinkyGoalY_ == 1)
				{
					pinkyGoalX_ = 1;
					pinkyGoalY_ = 1;
				}
			}
		}
		if (pEnemy_[3]->GetIzike() && !SpornInOrAuto(enemyIndexY, enemyIndexX))
		{
			if (!IsBlock(y + 1, x) && Intrusion(y + 1, x, flag) && enemyTargetMapData_[y][x] < enemyTargetMapData_[y + 1][x])
			{
				return EnemyBase::down;
			}
			if (!IsBlock(y - 1, x) && Intrusion(y - 1, x, flag) && enemyTargetMapData_[y][x] < enemyTargetMapData_[y - 1][x])
			{
				return EnemyBase::up;
			}
			if (!IsBlock(y, x + 1) && enemyTargetMapData_[y][x] < enemyTargetMapData_[y][x + 1])
			{
				return EnemyBase::right;
			}
			if (!IsBlock(y, x - 1) && enemyTargetMapData_[y][x] < enemyTargetMapData_[y][x - 1])
			{
				return EnemyBase::left;
			}
		}
		else
		{
			if (!IsBlock(y + 1, x) && Intrusion(y + 1, x, flag) && enemyTargetMapData_[y][x] > enemyTargetMapData_[y + 1][x])
			{
				return EnemyBase::down;
			}
			if (!IsBlock(y - 1, x) && Intrusion(y - 1, x, flag) && enemyTargetMapData_[y][x] > enemyTargetMapData_[y - 1][x])
			{
				return EnemyBase::up;
			}
			if (!IsBlock(y, x + 1) && enemyTargetMapData_[y][x] > enemyTargetMapData_[y][x + 1])
			{
				return EnemyBase::right;
			}
			if (!IsBlock(y, x - 1) && enemyTargetMapData_[y][x] > enemyTargetMapData_[y][x - 1])
			{
				return EnemyBase::left;
			}
		}
	}
	return 0;
}

int Field::SlimeMove(int enemyIndexY, int enemyIndexX, bool flag)
{
	int y = enemyIndexY;
	int x = enemyIndexX;

	if (pEnemy_[1]->GetMove())
	{
		if (pEnemy_[1]->GetTracking() || pEnemy_[1]->GetIzike())
		{
			// �ǐՃ��[�h
			MoveDataSet(pPlayer_->GetIndexY(), pPlayer_->GetIndexX());
		}
		else if (!pEnemy_[1]->GetTracking())
		{
			// �꒣�胂�[�h
			MoveDataSet(inkyGoalY_, inkyGoalX_);

			if (pEnemy_[1]->GetIndexY() == inkyGoalY_ && pEnemy_[1]->GetIndexX() == inkyGoalX_)
			{
				if (inkyGoalX_ == 1 && inkyGoalY_ == 20)
				{
					inkyGoalX_ = 8;
					inkyGoalY_ = 20;
				}
				else if (inkyGoalX_ == 8 && inkyGoalY_ == 20)
				{
					inkyGoalX_ = 8;
					inkyGoalY_ = 18;

				}
				else if (inkyGoalX_ == 8 && inkyGoalY_ == 18)
				{
					inkyGoalX_ = 6;
					inkyGoalY_ = 18;
				}
				else if (inkyGoalX_ == 6 && inkyGoalY_ == 18)
				{
					inkyGoalX_ = 6;
					inkyGoalY_ = 16;
				}
				else if (inkyGoalX_ == 6 && inkyGoalY_ == 16)
				{
					inkyGoalX_ = 4;
					inkyGoalY_ = 16;
				}
				else if (inkyGoalX_ == 4 && inkyGoalY_ == 16)
				{
					inkyGoalX_ = 4;
					inkyGoalY_ = 18;
				}
				else if (inkyGoalX_ == 4 && inkyGoalY_ == 18)
				{
					inkyGoalX_ = 1;
					inkyGoalY_ = 18;
				}
				else if (inkyGoalX_ == 1 && inkyGoalY_ == 18)
				{
					inkyGoalX_ = 1;
					inkyGoalY_ = 20;
				}
			}
		}

		if (pEnemy_[1]->GetIzike() && !SpornInOrAuto(enemyIndexY, enemyIndexX))
		{
			if (!IsBlock(y - 1, x) && Intrusion(y - 1, x, flag) && enemyTargetMapData_[y][x] < enemyTargetMapData_[y - 1][x])
			{
				return EnemyBase::up;
			}
			if (!IsBlock(y + 1, x) && Intrusion(y + 1, x, flag) && enemyTargetMapData_[y][x] < enemyTargetMapData_[y + 1][x])
			{
				return EnemyBase::down;
			}
			if (!IsBlock(y, x - 1) && enemyTargetMapData_[y][x] < enemyTargetMapData_[y][x - 1])
			{
				return EnemyBase::left;
			}
			if (!IsBlock(y, x + 1) && enemyTargetMapData_[y][x] < enemyTargetMapData_[y][x + 1])
			{
				return EnemyBase::right;
			}
		}
		else
		{
			if (!IsBlock(y - 1, x) && Intrusion(y - 1, x, flag) && enemyTargetMapData_[y][x] > enemyTargetMapData_[y - 1][x])
			{
				return EnemyBase::up;
			}
			if (!IsBlock(y + 1, x) && Intrusion(y + 1, x, flag) && enemyTargetMapData_[y][x] > enemyTargetMapData_[y + 1][x])
			{
				return EnemyBase::down;
			}
			if (!IsBlock(y, x - 1) && enemyTargetMapData_[y][x] > enemyTargetMapData_[y][x - 1])
			{
				return EnemyBase::left;
			}
			if (!IsBlock(y, x + 1) && enemyTargetMapData_[y][x] > enemyTargetMapData_[y][x + 1])
			{
				return EnemyBase::right;
			}
		}
	}
	return 0;
}

int Field::GhostMove(int enemyIndexY, int enemyIndexX, bool flag)
{
	int y = enemyIndexY;
	int x = enemyIndexX;

	if (pEnemy_[2]->GetMove())
	{
		if (pEnemy_[2]->GetTracking() || pEnemy_[2]->GetIzike())
		{
			// �ǐՃ��[�h
			MoveDataSet(pPlayer_->GetIndexY(), pPlayer_->GetIndexX());
		}
		else if (!pEnemy_[2]->GetTracking())
		{
			// �꒣�胂�[�h
			MoveDataSet(crydeGoalY_, crydeGoalX_);

			if (pEnemy_[2]->GetIndexY() == crydeGoalY_ && pEnemy_[2]->GetIndexX() == crydeGoalX_)
			{
				if (crydeGoalX_ == 17 && crydeGoalY_ == 20)
				{
					crydeGoalY_ = 18;
				}
				else if (crydeGoalX_ == 17 && crydeGoalY_ == 18)
				{
					crydeGoalX_ = 14;
				}
				else if (crydeGoalX_ == 14 && crydeGoalY_ == 18)
				{
					crydeGoalY_ = 16;
				}
				else if (crydeGoalX_ == 14 && crydeGoalY_ == 16)
				{
					crydeGoalX_ = 12;
				}
				else if (crydeGoalX_ == 12 && crydeGoalY_ == 16)
				{
					crydeGoalY_ = 18;
				}
				else if (crydeGoalX_ == 12 && crydeGoalY_ == 18)
				{
					crydeGoalX_ = 10;
				}
				else if (crydeGoalX_ == 10 && crydeGoalY_ == 18)
				{
					crydeGoalY_ = 20;
				}
				else if (crydeGoalX_ == 10 && crydeGoalY_ == 20)
				{
					crydeGoalX_ = 17;
				}
			}
		}
		// ���X�|�[���n�_�ɂ����ԂŃC�W�P��Ԃ̏ꍇ�������Ƀ��X�|�[���n�_����ł�
		if (pEnemy_[2]->GetIzike() && !SpornInOrAuto(enemyIndexY, enemyIndexX))
		{
			if (!IsBlock(y - 1, x) && Intrusion(y - 1, x, flag) && enemyTargetMapData_[y][x] < enemyTargetMapData_[y - 1][x])
			{
				return EnemyBase::up;
			}
			if (!IsBlock(y + 1, x) && Intrusion(y + 1, x, flag) && enemyTargetMapData_[y][x] < enemyTargetMapData_[y + 1][x])
			{
				return EnemyBase::down;
			}
			if (!IsBlock(y, x - 1) && enemyTargetMapData_[y][x] < enemyTargetMapData_[y][x - 1])
			{
				return EnemyBase::left;
			}
			if (!IsBlock(y, x + 1) && enemyTargetMapData_[y][x] < enemyTargetMapData_[y][x + 1])
			{
				return EnemyBase::right;
			}
		}
		else
		{
			if (!IsBlock(y - 1, x) && Intrusion(y - 1, x, flag) && enemyTargetMapData_[y][x] > enemyTargetMapData_[y - 1][x])
			{
				return EnemyBase::up;
			}
			if (!IsBlock(y + 1, x) && Intrusion(y + 1, x, flag) && enemyTargetMapData_[y][x] > enemyTargetMapData_[y + 1][x])
			{
				return EnemyBase::down;
			}
			if (!IsBlock(y, x - 1) && enemyTargetMapData_[y][x] > enemyTargetMapData_[y][x - 1])
			{
				return EnemyBase::left;
			}
			if (!IsBlock(y, x + 1) && enemyTargetMapData_[y][x] > enemyTargetMapData_[y][x + 1])
			{
				return EnemyBase::right;
			}
		}
	}
	return 0;
}
