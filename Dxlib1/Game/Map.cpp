#include "Map.h"
#include "../Game/Field.h"
#include <DxLib.h>
#include <cassert>

namespace 
{
	// チュートリアル
	constexpr int TUTORIAL_HEIGHT = 10;
	constexpr int TUTORIAL_WIDTH = 10;

	constexpr int mapDataTutorial[TUTORIAL_HEIGHT][TUTORIAL_WIDTH] =
	{
		{24,0,0,0,0,0,0,0,0,16},
		{24,8,8,8,8,8,8,8,8,16},
		{24,8,8,8,8,8,8,8,8,16},
		{24,8,14,14,14,14,14,14,8,16},
		{0,8,22,22,22,22,22,22,8,0},
		{20,21,8,8,8,8,8,8,28,29},
		{2,8,14,14,14,14,14,14,8,1},
		{24,8,22,22,22,22,22,22,8,16},
		{24,8,8,8,8,8,8,8,8,16},
		{24,0,0,0,0,0,0,0,0,16},

	};

	// ステージ１
	constexpr int STAGE_1_HEIGHT = 22;
	constexpr int STAGE_1_WIDTH = 19;

	constexpr int mapDataStage1[STAGE_1_HEIGHT][STAGE_1_WIDTH] =
	{
		{ 24,  0,  0,  0, 0, 0, 0, 0, 0, 7, 0,  0, 0, 0, 0,  0,  0,  0, 16 },
		{ 24,  8,  8,  8, 8, 8, 8, 8, 8, 7, 8,  8, 8, 8, 8,  8,  8,  8, 16 },
		{ 24,  8,  7,  7, 8, 7, 7, 7, 8, 7, 8,  7, 7, 7, 8,  7,  7,  8, 16 },
		{ 24,  8,  0,  0, 8, 0, 0, 0, 8, 0, 8,  0, 0, 0, 8,  0,  0,  8, 16 },
		{ 24,  8,  8,  8, 8, 8, 8, 8, 8, 8, 8,  8, 8, 8, 8,  8,  8,  8, 16 },
		{ 24,  8,  0,  0, 8, 7, 8, 0, 0, 7, 0,  0, 8, 7, 8,  0,  0,  8, 16 },
		{ 24,  8,  8,  8, 8, 7, 8, 8, 8, 7, 8,  8, 8, 7, 8,  8,  8,  8, 16 },
		{ 24,  0,  0,  2, 8, 7, 0, 0, 8, 0, 8,  0, 0, 7, 8,  1,  0,  0, 16 },
		{ 15, 15, 15, 24, 8, 7, 8, 8, 8, 8, 8,  8, 8, 7, 8, 16, 15, 15, 15 },
		{  0,  0,  0,  0, 8, 0, 8, 1, 0, 8, 0,  2, 8, 0, 8,  0,  0,  0,  0 },
		{ 20,  8,  8,  8, 8, 8, 8, 9, 8, 8, 8, 10, 8, 8, 8,  8,  8, 28, 29 },
		{  0,  0,  0,  2, 8, 7, 8, 0, 0, 0, 0,  0, 8, 7, 8,  1,  0,  0,  0 },
		{ 15, 15, 15, 24, 8, 7, 8, 8, 8, 8, 8,  8, 8, 7, 8, 16, 15, 15, 15 },
		{ 24,  0,  0,  0, 8, 0, 8, 0, 0, 7, 0,  0, 8, 0, 8,  0,  0,  0, 16 },
		{ 24,  8,  8,  8, 8, 8, 8, 8, 8, 7, 8,  8, 8, 8, 8,  8,  8,  8, 16 },
		{ 24,  8,  0,  7, 8, 0, 0, 0, 8, 0, 8,  0, 0, 0, 8,  7,  0,  8, 16 },
		{ 24,  8,  8,  7, 8, 8, 8, 8, 8, 8, 8,  8, 8, 8, 8,  7,  8,  8, 16 },
		{ 24,  0,  8,  0, 8, 7, 8, 0, 0, 7, 0,  0, 8, 7, 8,  0,  8,  0, 16 },
		{ 24,  8,  8,  8, 8, 7, 8, 8, 8, 7, 8,  8, 8, 7, 8,  8,  8,  8, 16 },
		{ 24,  8,  0,  0, 0, 0, 0, 0, 8, 0, 8,  0, 0, 0, 0,  0,  0,  8, 16 },
		{ 24,  8,  8,  8, 8, 8, 8, 8, 8, 8, 8,  8, 8, 8, 8,  8,  8,  8, 16 },
		{ 24,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0,  0,  0,  0, 16 },

	};

	constexpr int SCALE = 2.0f;
}

Map::Map(int handle, int stage) :
	handle_(handle),
	graphWidth_(0),
	graphHeight_(0)
{
	StageCheck(stage);
	GetGraphSize(handle_, &graphWidth_, &graphHeight_);
}

void Map::StageCheck(int stage)
{
	switch (stage)
	{
	case 0:
		disPlayPosX_ = Field::TUTORIAL_DISPLAY_POS_X;
		disPlayPosY_ = Field::TUTORIAL_DISPLAY_POS_Y;
		mapHeight_ = TUTORIAL_HEIGHT;
		mapWidth_ = TUTORIAL_WIDTH;
		break;
	case 1:
		disPlayPosX_ = Field::STAGE_1_DISPLAY_POS_X;
		disPlayPosY_ = Field::STAGE_1_DISPLAY_POS_Y;
		mapHeight_ = STAGE_1_HEIGHT;
		mapWidth_ = STAGE_1_WIDTH;
	}
	mapData_.resize(mapHeight_);
	for (int y = 0; y < mapHeight_; y++)
	{
		mapData_[y].resize(mapWidth_);
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
			}
		}
	}
}

void Map::StageCheck2(int stage)
{
}

void Map::Draw()
{
	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			DrawMap();
		}
	}
}

void Map::DrawMap() 
{
	for (int y = 0; y < pField_->GetMapDataY(); y++)
	{
		for (int x = 0; x < pField_->GetMapDataX(); x++)
		{

			const int chipNo = mapData_[y][x];

			assert(0 <= chipNo);
			assert(chipNo < ChipNum());

			int graphX = (chipNo % ChipNumX()) * (Field::CHIP_SIZE / 2);
			int graphY = (chipNo / ChipNumX()) * (Field::CHIP_SIZE / 2);

			DrawRectRotaGraph(x * Field::CHIP_SIZE + disPlayPosX_ + Field::CHIP_SIZE / 2, 
							y * Field::CHIP_SIZE + disPlayPosY_ + Field::CHIP_SIZE / 2,
							graphX, graphY, Field::CHIP_SIZE / 2, Field::CHIP_SIZE / 2,
							SCALE, 0.0f, handle_, true, false);
		}
	}
}

int Map::ChipNumX() 
{
	return (graphWidth_ / (Field::CHIP_SIZE / 2));
}

int Map::ChipNumY() 
{
	return (graphHeight_ / (Field::CHIP_SIZE / 2));
}

int Map::ChipNum() 
{
	return (ChipNumX() * ChipNumY());
}