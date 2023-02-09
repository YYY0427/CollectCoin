#include "CapriciousEnemy.h"
#include "../DrawFunctions.h"
#include "Player.h"
#include "Field.h"
#include "EnemyBase.h"
#include "../Scene/GameplayingScene.h"
#include "../Scene/SceneManager.h"
#include <DxLib.h>

namespace
{
	// 画像の幅
	constexpr int WIDTH = 16;

	// 画像の高さ
	constexpr int HEIGHT = 16;

	// 画像の拡大率
	constexpr float SCALE = 2.0f;

	// 通常のプレイヤーの移動スピード
	constexpr float NORMAL_SPEED = 1.5f;

	// パワーエサを取得した場合の移動スピード(何倍か)
	constexpr float GET_FEED_SPEED = 1.0f;

	// パワーエサを取得した場合持続時間(何秒か)
	constexpr int FEED_DURATION = 10;

	// 1枚に必要なフレーム数
	constexpr int ANIME_FRAME_SPEED = 10;
	constexpr int IZIKE_ANIME_FRAME_SPEED = 30;

	// アニメーション枚数
	constexpr int ANIME_FRAME_NUM = 2;
	constexpr int IZIKE_ANIME_FRAME_NUM = 2;
}

CapriciousEnemy::CapriciousEnemy(int handle, int indexX, int indexY)
{
	handle_ = handle;
	indexX_ = indexX;
	indexY_ = indexY;
}

void CapriciousEnemy::Update()
{
	// 死んだ場合初期化
	if (isDead_)
	{
		SetInit();

		// 敵が死んでいる状態で指定の位置に存在する場合にイジケ状態を解除
		if (indexX_ == 10 && indexY_ == 10)
		{
			isIzike_ = false;
		}
	}

	// 移動のインターバルのカウント
	moveTimer_++;

	// 現在の座標を仮座標で保存
	kX_ = indexX_;
	kY_ = indexY_;

	moveInterval_ = Field::BLOCK_SIZE / speed_;

	// 移動のインターバル
	if (moveTimer_ % moveInterval_ == 0)
	{
		if (!Colision(moveDirection_))
		{
			// 移動処理
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

		// ランダムに敵の方向を変える
		moveDirection_ = GetRand(3) + 1;

		moveTimer_ = 0;
	}

	SpeedCalculation();

	// 壁に当たっていない場合
	if (!Colision(moveDirection_))
	{
		// 座標計算
		PosCalculation();
	}

	// ワープチェック
	indexX_ = pField_->Worp(kY_, kX_, indexY_, indexX_);

	// アニメーション処理
	idX_ = (idX_ + 1) % (ANIME_FRAME_SPEED * ANIME_FRAME_NUM);				// 通常処理
	idY_ = (idY_ + 1) % (IZIKE_ANIME_FRAME_SPEED * IZIKE_ANIME_FRAME_NUM);	//イジケ用処理
}
