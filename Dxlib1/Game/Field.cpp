#include "Field.h"
#include "Player.h"
#include "../DrawFunctions.h"
#include "EnemyBase.h"
#include "../game.h"
#include "../SoundManager.h"
#include <DxLib.h>

namespace
{
	// 0 : なにもなし
	// 8 : なにもなし
	// 1 : コイン
	// 2 : 壁
	// 3 : 剣 
	// 7 : 扉(中からは出れて、外からは入れない)

	constexpr int mapData[Field::MAP_HEIGHT][Field::MAP_WIDTH] =
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

	// デバック用
	constexpr int mapData1[Field::MAP_HEIGHT][Field::MAP_WIDTH] =
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


	// 点滅スピード
	constexpr int FLASH_SPEED = 20;

	// アニメーション
	constexpr int COIN_FRAME_NUM = 4;		
	constexpr int COIN_FRAME_SPEED = 15;

	constexpr int DOOR_FRAME_NUM = 4;
	constexpr int DOOR_FRAME_SPEED = 50;
}

Field::Field(int sordH, int doorH, int coinH) :
	sordH_(sordH),
	doorH_(doorH),
	coinH_(coinH),
	coinImgIdx_(0),
	doorImgIdx_(0),
	coin_(0),
	isDraw_(true),
	closeDoor_(false),
	openDoor_(false)
{
	pinkyGoalX_ = 1;
	pinkyGoalY_ = 1;

	blinkyGoalX_ = 17;
	blinkyGoalY_ = 1;

	inkyGoalX_ = 1;
	inkyGoalY_ = 20;

	crydeGoalY_ = 20;
	crydeGoalX_ = 17;

	stringH_ = CreateFontToHandle("PixelMplus10", 30, 10);

	for (int y = 0; y < Field::MAP_HEIGHT; y++)
	{
		for (int x = 0; x < Field::MAP_WIDTH; x++)
		{
			mapData2_[y][x] = 0;
		}
	}

	for (int y = 0; y < Field::MAP_HEIGHT; y++)
	{
		for (int x = 0; x < Field::MAP_WIDTH; x++)
		{
			mapData_[y][x] = mapData[y][x];
		}
	}
}

Field::~Field()
{
	DeleteFontToHandle(stringH_);
}

void Field::Init()
{
	pinkyGoalX_ = 1;
	pinkyGoalY_ = 1;

	blinkyGoalX_ = 17;
	blinkyGoalY_ = 1;

	inkyGoalX_ = 1;
	inkyGoalY_ = 20;

	crydeGoalY_ = 20;
	crydeGoalX_ = 17;

	isDraw_ = true;
}

void Field::Updata()
{
	// アニメーション処理
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
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			// コインの描画
			if (mapData_[y][x] == 1)
			{
				int imgX = (coinImgIdx_ / COIN_FRAME_SPEED) * 8;
				DrawRectRotaGraph(x * CHIP_SIZE + 16 + DISPLAY_POS_X, y * CHIP_SIZE + 16 + DISPLAY_POS_Y,
					imgX, 0, 8, 8, 2.0f, 0.0f, coinH_, true);
			}
			// 剣の描画
			if (mapData_[y][x] == 3)
			{
				DrawRectRotaGraph(x * CHIP_SIZE + 16 + DISPLAY_POS_X, y * CHIP_SIZE + 16 + DISPLAY_POS_Y,
					0, 0, 16, 16, 2.0f, 0.0f, sordH_, true);
			}
			// 扉の描画
			if (mapData_[y][x] == 7)
			{
				int imgX = (doorImgIdx_ / DOOR_FRAME_SPEED) * 16;
				DrawRectRotaGraph(x * CHIP_SIZE + 16 + DISPLAY_POS_X, y * CHIP_SIZE + 16 + DISPLAY_POS_Y,
					imgX, 0, 16, 16, 2.0f, 0.0f, doorH_, true);
			}
			// 壁の描画
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
	DrawFormatStringToHandle(Game::kScreenWidth / 2 - 225 + 2, Game::kScreenHeight - 50 + 2, 0x000000, stringH_, "%d / %d\n", coin_, LestCoin(), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 残りのコインの枚数
	DrawRectRotaGraph(Game::kScreenWidth / 2 - 260, Game::kScreenHeight - 35, 0, 0, 8, 8, 3.7f, 0.0f, coinH_, true);
	DrawFormatStringToHandle(Game::kScreenWidth / 2 - 225, Game::kScreenHeight - 50, 0xffffff, stringH_, "%d / %d\n", coin_, LestCoin());
}

int Field::LestCoin()
{
	int coin = 0;
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			if (mapData[y][x] == 1)
			{
				coin++;
			}
		}
	}
	return coin;
}

// ゲームクリア判定
bool Field::IsGameClearCheck()
{
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			if (mapData_[y][x] == 1)
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
	if (mapData_[y][x] == 2)	
		return true;
	
	return false;
}

// コインがあるかどうか
bool Field::IsFeed(int y, int x)
{
	if (mapData_[y][x] == 1)
	{
		coin_++;
		SoundManager::GetInstance().Play("coin");
		mapData_[y][x] = 0;
		return true;
	}

	return false;
}

// 剣があるかどうか
bool Field::IsPowerFeed(int y, int x)
{
	if (mapData_[y][x] == 3)
	{
		SoundManager::GetInstance().Play("powerUp");
		mapData_[y][x] = 0;
		return true;
	}
	return false;
}

int  Field::Warp(int ky, int kx, int indexY, int indexX)
{
	//前の座標が特定の位置かつ現在の座標が特定の位置ならワープ
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
	return (indexX);
}

void Field::MoveDataSet(int goalY, int goalX)
{
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			if (mapData_[y][x] != 2 && mapData_[y][x] != 8)
			{
				mapData2_[y][x] = 0;
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
		if (mapData2_[y - 1][x] == 0 || mapData2_[y - 1][x] > pos)
		{
			mapData2_[y - 1][x] = pos;
			Search(y - 1, x, goalY, goalX, pos);
		}
		if (mapData2_[y + 1][x] == 0 || mapData2_[y + 1][x] > pos)
		{
			mapData2_[y + 1][x] = pos;
			Search(y + 1, x, goalY, goalX, pos);
		}
		if (mapData2_[y][x - 1] == 0 || mapData2_[y][x - 1] > pos)
		{
			mapData2_[y][x - 1] = pos;
			Search(y, x - 1, goalY, goalX, pos);
		}
		if (mapData2_[y][x + 1] == 0 || mapData2_[y][x + 1] > pos)
		{
			mapData2_[y][x + 1] = pos;
			Search(y, x + 1, goalY, goalX, pos);
		}

		for (auto& enemy : pEnemy_)
		{
			if (enemy->GetTracking())
			{
				mapData2_[pPlayer_->GetIndexY()][pPlayer_->GetIndexX()] = 0;
			}
			else
			{
				mapData2_[goalY][goalX] = 0;
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
	if (y == 9 && x == 9)
	{
		return true;
	}
	// リスポーン地点にいない
	return false;
}

bool Field::Intrusion(int y, int x, bool flag)
{
	// flagがfalseの場合通れない
	// flagがtrueの場合通れる
	if (!flag)
	{
		if (mapData_[y][x] == 7)
		{
			// 通れない
			return false;
		}
	}
	// 通れる
	return true;
}

int Field::BlinkyMove(int enemyIndexY, int enemyIndexX, bool flag)
{
	int y = enemyIndexY;
	int x = enemyIndexX;
	
	if (pEnemy_[0]->GetMove())
	{
		if (pEnemy_[0]->GetTracking() || pEnemy_[0]->GetIzike())
		{
			// 追跡モード
			MoveDataSet(pPlayer_->GetIndexY(), pPlayer_->GetIndexX());
		}
		// 縄張りモードのときの移動
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

		if (pEnemy_[0]->GetIzike() && !SpornInOrAuto(enemyIndexY, enemyIndexX))
		{
			if (!IsBlock(y - 1, x) && Intrusion(y - 1, x, flag) && mapData2_[y][x] < mapData2_[y - 1][x])
			{
				return EnemyBase::up;
			}
			if (!IsBlock(y + 1, x) && Intrusion(y + 1, x, flag) && mapData2_[y][x] < mapData2_[y + 1][x])
			{
				return EnemyBase::down;
			}
			if (!IsBlock(y, x - 1) && mapData2_[y][x] < mapData2_[y][x - 1])
			{
				return EnemyBase::left;
			}
			if (!IsBlock(y, x + 1) && mapData2_[y][x] < mapData2_[y][x + 1])
			{
				return EnemyBase::right;
			}
		}
		else
		{
			if (!IsBlock(y - 1, x) && Intrusion(y - 1, x, flag) && mapData2_[y][x] > mapData2_[y - 1][x])
			{
				return EnemyBase::up;
			}
			if (!IsBlock(y + 1, x) && Intrusion(y + 1, x, flag) && mapData2_[y][x] > mapData2_[y + 1][x])
			{
				return EnemyBase::down;
			}
			if (!IsBlock(y, x - 1) && mapData2_[y][x] > mapData2_[y][x - 1])
			{
				return EnemyBase::left;
			}
			if (!IsBlock(y, x + 1) && mapData2_[y][x] > mapData2_[y][x + 1])
			{
				return EnemyBase::right;
			}
		}
	}
	return 0;
}

int Field::PinkyMove(int enemyIndexY, int enemyIndexX, bool flag)
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
		if (pEnemy_[3]->GetIzike() && !SpornInOrAuto(enemyIndexY, enemyIndexX))
		{
			if (!IsBlock(y + 1, x) && Intrusion(y + 1, x, flag) && mapData2_[y][x] < mapData2_[y + 1][x])
			{
				return EnemyBase::down;
			}
			if (!IsBlock(y - 1, x) && Intrusion(y - 1, x, flag) && mapData2_[y][x] < mapData2_[y - 1][x])
			{
				return EnemyBase::up;
			}
			if (!IsBlock(y, x + 1) && mapData2_[y][x] < mapData2_[y][x + 1])
			{
				return EnemyBase::right;
			}
			if (!IsBlock(y, x - 1) && mapData2_[y][x] < mapData2_[y][x - 1])
			{
				return EnemyBase::left;
			}
		}
		else
		{
			if (!IsBlock(y + 1, x) && Intrusion(y + 1, x, flag) && mapData2_[y][x] > mapData2_[y + 1][x])
			{
				return EnemyBase::down;
			}
			if (!IsBlock(y - 1, x) && Intrusion(y - 1, x, flag) && mapData2_[y][x] > mapData2_[y - 1][x])
			{
				return EnemyBase::up;
			}
			if (!IsBlock(y, x + 1) && mapData2_[y][x] > mapData2_[y][x + 1])
			{
				return EnemyBase::right;
			}
			if (!IsBlock(y, x - 1) && mapData2_[y][x] > mapData2_[y][x - 1])
			{
				return EnemyBase::left;
			}
		}
	}
	return 0;
}

int Field::InkyMove(int enemyIndexY, int enemyIndexX, bool flag)
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
			if (!IsBlock(y - 1, x) && Intrusion(y - 1, x, flag) && mapData2_[y][x] < mapData2_[y - 1][x])
			{
				return EnemyBase::up;
			}
			if (!IsBlock(y + 1, x) && Intrusion(y + 1, x, flag) && mapData2_[y][x] < mapData2_[y + 1][x])
			{
				return EnemyBase::down;
			}
			if (!IsBlock(y, x - 1) && mapData2_[y][x] < mapData2_[y][x - 1])
			{
				return EnemyBase::left;
			}
			if (!IsBlock(y, x + 1) && mapData2_[y][x] < mapData2_[y][x + 1])
			{
				return EnemyBase::right;
			}
		}
		else
		{
			if (!IsBlock(y - 1, x) && Intrusion(y - 1, x, flag) && mapData2_[y][x] > mapData2_[y - 1][x])
			{
				return EnemyBase::up;
			}
			if (!IsBlock(y + 1, x) && Intrusion(y + 1, x, flag) && mapData2_[y][x] > mapData2_[y + 1][x])
			{
				return EnemyBase::down;
			}
			if (!IsBlock(y, x - 1) && mapData2_[y][x] > mapData2_[y][x - 1])
			{
				return EnemyBase::left;
			}
			if (!IsBlock(y, x + 1) && mapData2_[y][x] > mapData2_[y][x + 1])
			{
				return EnemyBase::right;
			}
		}
	}
	return 0;
}

int Field::CrydeMove(int enemyIndexY, int enemyIndexX, bool flag)
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
		// リスポーン地点にいる状態でイジケ状態の場合逃げずにリスポーン地点からでる
		if (pEnemy_[2]->GetIzike() && !SpornInOrAuto(enemyIndexY, enemyIndexX))
		{
			if (!IsBlock(y - 1, x) && Intrusion(y - 1, x, flag) && mapData2_[y][x] < mapData2_[y - 1][x])
			{
				return EnemyBase::up;
			}
			if (!IsBlock(y + 1, x) && Intrusion(y + 1, x, flag) && mapData2_[y][x] < mapData2_[y + 1][x])
			{
				return EnemyBase::down;
			}
			if (!IsBlock(y, x - 1) && mapData2_[y][x] < mapData2_[y][x - 1])
			{
				return EnemyBase::left;
			}
			if (!IsBlock(y, x + 1) && mapData2_[y][x] < mapData2_[y][x + 1])
			{
				return EnemyBase::right;
			}
		}
		else
		{
			if (!IsBlock(y - 1, x) && Intrusion(y - 1, x, flag) && mapData2_[y][x] > mapData2_[y - 1][x])
			{
				return EnemyBase::up;
			}
			if (!IsBlock(y + 1, x) && Intrusion(y + 1, x, flag) && mapData2_[y][x] > mapData2_[y + 1][x])
			{
				return EnemyBase::down;
			}
			if (!IsBlock(y, x - 1) && mapData2_[y][x] > mapData2_[y][x - 1])
			{
				return EnemyBase::left;
			}
			if (!IsBlock(y, x + 1) && mapData2_[y][x] > mapData2_[y][x + 1])
			{
				return EnemyBase::right;
			}
		}
	}
	return 0;
}
