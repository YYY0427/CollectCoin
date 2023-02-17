#include "Field.h"
#include "Player.h"
#include "../DrawFunctions.h"
#include "EnemyBase.h"
#include <DxLib.h>

namespace
{
	//マップデータ
	int mapData[Field::MAP_HEIGHT][Field::MAP_WIDTH] =
	{
		// 1 エサ、2  壁、3 パワーエサ、4 

		{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
		{ 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2 },
		{ 2, 1, 2, 2, 1, 2, 2, 2, 1, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2 },
		{ 2, 3, 2, 2, 1, 2, 2, 2, 1, 2, 1, 2, 2, 2, 1, 2, 2, 3, 2 },
		{ 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2 },
		{ 2, 1, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 1, 2, 1, 2, 2, 1, 2 },
		{ 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2 },
		{ 2, 2, 2, 2, 1, 2, 2, 2, 0, 2, 0, 2, 2, 2, 1, 2, 2, 2, 2 },
		{ 4, 4, 4, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 4, 4, 4 },
		{ 2, 2, 2, 2, 1, 2, 0, 2, 2, 5, 2, 2, 0, 2, 1, 2, 2, 2, 2 },
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

	// 点滅スピード
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
	inkyGoalY_ = 20;

	crydeGoalY_ = 20;
	crydeGoalX_ = 17;

	handle_ = my::MyLoadGraph(L"Data/img/game/powerpacdot.png");
}

void Field::Updata()
{
	// パワーエサの点滅更新処理
	Flash();
}

void Field::Draw()
{
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			// エサの描画
			if (mapData[y][x] == 1)
			{
				DrawFormatString(x * BLOCK_SIZE + 10 + DISPLAY_POS_X, y * BLOCK_SIZE + 10 + DISPLAY_POS_Y, 0xffffff, L"・", true);
			}
			// 壁の描画
			if (mapData[y][x] == 2)
			{
				DrawBox(
					x * BLOCK_SIZE + DISPLAY_POS_X, y * BLOCK_SIZE + DISPLAY_POS_Y,
					x * BLOCK_SIZE + BLOCK_SIZE + DISPLAY_POS_X, y * BLOCK_SIZE + BLOCK_SIZE + DISPLAY_POS_Y,
					GetColor(0, 0, 255), false);
			}
			// パワーエサの描画
			if (mapData[y][x] == 3)
			{
				PowerFeedFlash(y, x);
			}
			if (mapData[y][x] == 5)
			{
				DrawBox(
					x * BLOCK_SIZE + DISPLAY_POS_X, y * BLOCK_SIZE + DISPLAY_POS_Y,
					x * BLOCK_SIZE + BLOCK_SIZE + DISPLAY_POS_X, y * BLOCK_SIZE + (BLOCK_SIZE / 4) + DISPLAY_POS_Y,
					GetColor(255, 255, 0), true);
			}
#ifdef _DEBUG	// デバックの版場合
			if (mapData2[y][x] >= 0)
			{
		//		DrawFormatString(x * BLOCK_SIZE + 10 + DISPLAY_POS_X, y * BLOCK_SIZE + 10 + DISPLAY_POS_Y, 0xffffff, L"%d", mapData2[y][x], true);
			}		
#endif
		}
	}
}

// パワーエサの描画
void Field::PowerFeedFlash(int y, int x)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, blendCount_);
	DrawRotaGraph(x * BLOCK_SIZE + 16 + DISPLAY_POS_X, y * BLOCK_SIZE + 16 + DISPLAY_POS_Y, 1, 0, handle_, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}

// パワーエサの点滅処理
void Field::Flash()
{
	//点滅
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

// ゲームクリア判定
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

// ブロックがあるかどうか
bool Field::IsBlock(int y, int x)
{
	if (mapData[y][x] == 2)	
		return true;
	
	return false;
}

// エサがあるかどうか
bool Field::IsFeed(int y, int x)
{
	if (mapData[y][x] == 1)
	{
		mapData[y][x] = 0;
		return true;
	}

	return false;
}

// パワーエサがあるかどうか
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
	//前の座標が特定の位置かつ現在の座標が特定の位置ならワープ
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

void Field::MoveDataSet(int goalY, int goalX)
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

	Search(goalY, goalX, goalY, goalX, 0);
}

void Field::Search(int y, int x, int goalY, int goalX, int pos)
{
	pos += 1;

	if (x > 0 && y > 0 && x < MAP_WIDTH - 1 && y < MAP_HEIGHT - 1)
	{
		if (mapData2[y - 1][x] == 0 || mapData2[y - 1][x] > pos)
		{
			mapData2[y - 1][x] = pos;
			Search(y - 1, x, goalY, goalX, pos);
		}
		if (mapData2[y + 1][x] == 0 || mapData2[y + 1][x] > pos)
		{
			mapData2[y + 1][x] = pos;
			Search(y + 1, x, goalY, goalX, pos);
		}
		if (mapData2[y][x - 1] == 0 || mapData2[y][x - 1] > pos)
		{
			mapData2[y][x - 1] = pos;
			Search(y, x - 1, goalY, goalX, pos);
		}
		if (mapData2[y][x + 1] == 0 || mapData2[y][x + 1] > pos)
		{
			mapData2[y][x + 1] = pos;
			Search(y, x + 1, goalY, goalX, pos);
		}

		for (auto& enemy : pEnemy_)
		{
			if (enemy->GetTracking())
			{
				mapData2[pPlayer_->GetIndexY()][pPlayer_->GetIndexX()] = 0;
			}
			else
			{
				mapData2[goalY][goalX] = 0;
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
		// リスポーン地点にいる
		return true;
	}
	// リスポーン地点にいない
	return false;
}

int Field::BlinkyMove(int enemyIndexY, int enemyIndexX)
{
	int y = enemyIndexY;
	int x = enemyIndexX;
	
	if (pEnemy_[0]->GetTracking() || pEnemy_[0]->GetIzike())
	{
		// 追跡モード
		MoveDataSet(pPlayer_->GetIndexY(), pPlayer_->GetIndexX());
	}
	else if (!pEnemy_[0]->GetTracking())
	{
		// 縄張りモード
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

	if (pEnemy_[3]->GetMove())
	{
		if (pEnemy_[3]->GetTracking() || pEnemy_[3]->GetIzike())
		{
			// 追跡モード
			MoveDataSet(pPlayer_->GetIndexY(), pPlayer_->GetIndexX());
		}
		else if (!pEnemy_[3]->GetTracking())
		{
			// 縄張りモード
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
	}
	return 0;
}

int Field::InkyMove(int enemyIndexY, int enemyIndexX)
{
	int y = enemyIndexY;
	int x = enemyIndexX;

	if (pEnemy_[1]->GetMove())
	{
		if (pEnemy_[1]->GetTracking() || pEnemy_[1]->GetIzike())
		{
			// 追跡モード
			MoveDataSet(pPlayer_->GetIndexY(), pPlayer_->GetIndexX());
		}
		else if (!pEnemy_[1]->GetTracking())
		{
			// 縄張りモード
			MoveDataSet(inkyGoalY_, InkyGoalX_);

			if (pEnemy_[1]->GetIndexY() == inkyGoalY_ && pEnemy_[1]->GetIndexX() == InkyGoalX_)
			{
				if (InkyGoalX_ == 1 && inkyGoalY_ == 20)
				{
					InkyGoalX_ = 8;
					inkyGoalY_ = 20;
				}
				else if (InkyGoalX_ == 8 && inkyGoalY_ == 20)
				{
					InkyGoalX_ = 8;
					inkyGoalY_ = 18;

				}
				else if (InkyGoalX_ == 8 && inkyGoalY_ == 18)
				{
					InkyGoalX_ = 6;
					inkyGoalY_ = 18;
				}
				else if (InkyGoalX_ == 6 && inkyGoalY_ == 18)
				{
					InkyGoalX_ = 6;
					inkyGoalY_ = 16;
				}
				else if (InkyGoalX_ == 6 && inkyGoalY_ == 16)
				{
					InkyGoalX_ = 4;
					inkyGoalY_ = 16;
				}
				else if (InkyGoalX_ == 4 && inkyGoalY_ == 16)
				{
					InkyGoalX_ = 4;
					inkyGoalY_ = 18;
				}
				else if (InkyGoalX_ == 4 && inkyGoalY_ == 18)
				{
					InkyGoalX_ = 1;
					inkyGoalY_ = 18;
				}
				else if (InkyGoalX_ == 1 && inkyGoalY_ == 18)
				{
					InkyGoalX_ = 1;
					inkyGoalY_ = 20;
				}
			}
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
	}
	return 0;
}

int Field::CrydeMove(int enemyIndexY, int enemyIndexX)
{
	int y = enemyIndexY;
	int x = enemyIndexX;

	if (pEnemy_[2]->GetMove())
	{
		if (pEnemy_[2]->GetTracking() || pEnemy_[2]->GetIzike())
		{
			// 追跡モード
			MoveDataSet(pPlayer_->GetIndexY(), pPlayer_->GetIndexX());
		}
		else if (!pEnemy_[2]->GetTracking())
		{
			// 縄張りモード
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
		if (pEnemy_[2]->GetIzike())
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
	}
	return 0;
}
