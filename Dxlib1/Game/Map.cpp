#include "Map.h"
#include "../Game/Field.h"
#include <DxLib.h>
#include <cassert>

namespace 
{
	//マップデータ
	constexpr int mapData_[Field::MAP_HEIGHT][Field::MAP_WIDTH] =
	{
		{24,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,16},
		{24,8,8,8,8,8,8,8,8,7,8,8,8,8,8,8,8,8,16},
		{24,8,7,7,8,7,7,7,8,7,8,7,7,7,8,7,7,8,16},
		{24,8,0,0,8,0,0,0,8,0,8,0,0,0,8,0,0,8,16},
		{24,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,16},
		{24,8,0,0,8,7,8,0,0,7,0,0,8,7,8,0,0,8,16},
		{24,8,8,8,8,7,8,8,8,7,8,8,8,7,8,8,8,8,16},
		{24,0,0,2,8,7,0,0,8,0,8,0,0,7,8,1,0,0,16},
		{15,15,15,24,8,7,8,8,8,8,8,8,8,7,8,16,15,15,15},
		{0,0,0,0,8,0,8,1,0,8,0,2,8,0,8,0,0,0,0},
		{20,8,8,8,8,8,8,9,8,8,8,10,8,8,8,8,8,28,29},
		{0,0,0,2,8,7,8,0,0,0,0,0,8,7,8,1,0,0,0},
		{15,15,15,24,8,7,8,8,8,8,8,8,8,7,8,16,15,15,15},
		{24,0,0,0,8,0,8,0,0,7,0,0,8,0,8,0,0,0,16},
		{24,8,8,8,8,8,8,8,8,7,8,8,8,8,8,8,8,8,16},
		{24,8,0,7,8,0,0,0,8,0,8,0,0,0,8,7,0,8,16},
		{24,8,8,7,8,8,8,8,8,8,8,8,8,8,8,7,8,8,16},
		{24,0,8,0,8,7,8,0,0,7,0,0,8,7,8,0,8,0,16},
		{24,8,8,8,8,7,8,8,8,7,8,8,8,7,8,8,8,8,16},
		{24,8,0,0,0,0,0,0,8,0,8,0,0,0,0,0,0,8,16},
		{24,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,16},
		{24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16},

	};

	constexpr int SCALE = 2.0f;
}

Map::Map(int handle) :
	handle_(handle),
	graphWidth_(0),
	graphHeight_(0)
{
	GetGraphSize(handle_, &graphWidth_, &graphHeight_);
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
	for (int y = 0; y < Field::MAP_HEIGHT; y++) 
	{
		for (int x = 0; x < Field::MAP_WIDTH; x++) 
		{

			const int chipNo = mapData_[y][x];

			assert(0 <= chipNo);
			assert(chipNo < ChipNum());

			int graphX = (chipNo % ChipNumX()) * (Field::CHIP_SIZE / 2);
			int graphY = (chipNo / ChipNumX()) * (Field::CHIP_SIZE / 2);

			DrawRectRotaGraph(x * Field::CHIP_SIZE + Field::DISPLAY_POS_X + Field::CHIP_SIZE / 2, 
							y * Field::CHIP_SIZE + Field::DISPLAY_POS_Y + Field::CHIP_SIZE / 2,
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