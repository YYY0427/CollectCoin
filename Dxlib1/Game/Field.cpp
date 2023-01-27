#include "Field.h"
#include <DxLib.h>

namespace
{
	//マップデータ
	int mapData[Field::kMapHeight][Field::kMapWidth] =
	{
		{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
		{ 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2 },
		{ 2, 1, 2, 2, 1, 2, 2, 2, 1, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2 },
		{ 2, 3, 2, 2, 1, 2, 2, 2, 1, 2, 1, 2, 2, 2, 1, 2, 2, 3, 2 },
		{ 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2 },
		{ 2, 1, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 1, 2, 1, 2, 2, 1, 2 },
		{ 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2 },
		{ 2, 2, 2, 2, 1, 2, 2, 2, 0, 2, 0, 2, 2, 2, 1, 2, 2, 2, 2 },
		{ 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0 },
		{ 2, 2, 2, 2, 1, 2, 0, 2, 2, 0, 2, 2, 0, 2, 1, 2, 2, 2, 2 },
		{ 0, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0 },
		{ 2, 2, 2, 2, 1, 2, 0, 2, 2, 2, 2, 2, 0, 2, 1, 2, 2, 2, 2 },
		{ 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0 },
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
}

Field::Field() :
	drawTimer_(0),
	noDrawTimer_(0),
	isDraw_(true)
{
	
}

void Field::Updata()
{
	if (!isDraw_)
	{
		noDrawTimer_++;
		if (noDrawTimer_ % 100 == 0)
		{
			isDraw_ = true;
			noDrawTimer_ = 0;
		}
	}
	else
	{
		drawTimer_++;
		if (drawTimer_ % 100 == 0)
		{
			isDraw_ = false;
			drawTimer_ = 0;
		}
	}
	
}

void Field::Draw()
{
	for (int y = 0; y < kMapHeight; y++)
	{
		for (int x = 0; x < kMapWidth; x++)
		{
			// エサの表示
			if (mapData[y][x] == 1)
			{
				DrawFormatString(x * kBlockSize + 10, y * kBlockSize + 10, 0xffffff, L"・", true);
			}
			// 壁の表示
			if (mapData[y][x] == 2)
			{
				DrawBox(
					x * kBlockSize, y * kBlockSize,
					x * kBlockSize + kBlockSize, y * kBlockSize + kBlockSize,
					GetColor(0, 0, 255), false);
			}
			// パワー餌の表示
			if (mapData[y][x] == 3 && isDraw_)
			{
				DrawFormatString(x * kBlockSize + 10, y * kBlockSize + 10, 0xffff00, L"■", true);
			}
		}
	}
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
		return true;

	return false;
}

// パワーエサがあるかどうか
bool Field::IsPowerFeed(int y, int x)
{
	if (mapData[y][x] == 3)
		return true;

	return false;
}
