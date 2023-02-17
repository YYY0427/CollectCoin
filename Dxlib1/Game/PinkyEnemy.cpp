#include "PinkyEnemy.h"
#include "../DrawFunctions.h"
#include "Player.h"
#include "Field.h"
#include "EnemyBase.h"
#include "../Scene/GameplayingScene.h"
#include "../Scene/SceneManager.h"
#include <DxLib.h>

namespace
{
	// 通常のプレイヤーの移動スピード
	constexpr float NORMAL_SPEED = 1.6f;

	// パワーエサを取得した場合の移動スピード(何倍か)
	constexpr float GET_FEED_SPEED = 1.2f;

	// パワーエサを取得した場合持続時間(何秒か)
	constexpr int FEED_DURATION = 10;

	// 1枚に必要なフレーム数
	constexpr int ANIME_FRAME_SPEED = 10;
	constexpr int IZIKE_ANIME_FRAME_SPEED = 30;

	// アニメーション枚数
	constexpr int ANIME_FRAME_NUM = 2;
	constexpr int IZIKE_ANIME_FRAME_NUM = 2;

	// 死んでから動き始めるまでの時間
	constexpr int DIEAD_MOVE_INTERVAL = 60 * 5;

	// ゲームスタート時から動き始めるまでの時間
	constexpr int STARET_MOVE_INTEVAL = 60 * 12;
}

PinkyEnemy::PinkyEnemy(int handle, int indexX, int indexY)
{
	handle_ = handle;
	indexX_ = indexX;
	indexY_ = indexY;
	isTracking_ = false;
}

void PinkyEnemy::Update()
{
	// 死んだ場合初期化
	if (isDead_)
	{
		SetInit();

		// 敵が死んでいる状態で指定の位置に存在する場合にイジケ状態を解除
		if (indexX_ == 10 && indexY_ == 10)
		{
			isIzike_ = false;
			isMove_ = false;
			isIntrusion_ = true;
		}
	}

	MoveSwitch();

	// 縄張りモードと追跡モードの切り替え
	ModeSwitch();

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

		moveDirection_ = pField_->PinkyMove(indexY_, indexX_, isIntrusion_);

		moveTimer_ = 0;
	}

	SpeedCalculation();

	if (!pField_->SpornInOrAuto(indexY_, indexX_))
	{
		// リスポーン地点にいない
		isIntrusion_ = false;
	}
	else
	{
		// リスポーン地点にいる
		isIntrusion_ = true;
	}

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

void PinkyEnemy::MoveSwitch()
{
	if (!isMove_ && !isDead_)
	{
		moveTimer2_++;

		if (moveTimer2_ % STARET_MOVE_INTEVAL == 0)
		{
			moveTimer2_ = 0;

			isMove_ = true;
		}
	}
	else if (!isMove_ && isDead_)
	{
		moveTimer2_++;

		if (moveTimer2_ % DIEAD_MOVE_INTERVAL == 0)
		{
			moveTimer2_ = 0;

			isMove_ = true;
		}
	}
}
