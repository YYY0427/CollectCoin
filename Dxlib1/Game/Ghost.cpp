#include "Ghost.h"
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

	// 死んでから動き始めるまでの時間
	constexpr int DIEAD_MOVE_INTERVAL = 60 * 5;

	// ゲームスタート時から動き始めるまでの時間
	constexpr int STARET_MOVE_INTEVAL = 60 * 4;
}

Ghost::Ghost(int handle, int indexX, int indexY, int stage)
{
	handle_ = handle;
	indexX_ = indexX;
	indexY_ = indexY;

	stage_ = stage;

	isIntrusion_ = true;
}

void Ghost::Update()
{
	// 死んだ場合初期化
	if (isDead_)
	{
		SetDeadInit(stage_);

		// 敵が死んでいる状態で指定の位置に存在する場合にイジケ状態を解除
		if (indexX_ == 10 && indexY_ == 10)
		{
		//	isIzike_ = false;
			isMove_ = false;
			isIntrusion_ = true;
		}
	}

	MoveSwitch(STARET_MOVE_INTEVAL, DIEAD_MOVE_INTERVAL);

	// 縄張りモードと追跡モードの切り替え
	ModeSwitch();

	// 移動のインターバルのカウント
	moveTimer_++;

	// 現在の座標を仮座標で保存
	kX_ = indexX_;
	kY_ = indexY_;

	moveInterval_ = static_cast<int>(Field::CHIP_SIZE / speed_);

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

		moveDirection_ = pField_->GhostMove(indexY_, indexX_, isIntrusion_);

		moveTimer_ = 0;
	}

	SpeedChange();

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
	indexX_ = pField_->Warp(kY_, kX_, indexY_, indexX_);

	// アニメーション処理
	idX_ = (idX_ + 1) % (ANIME_FRAME_SPEED * ANIME_FRAME_NUM);				// 通常処理
}

void Ghost::Draw()
{
	int imgX = (idX_ / ANIME_FRAME_SPEED) * WIDTH;

	// 通常時の表示
	if (isEnabled_)
	{

		int imgY = DirectReturnNum(HEIGHT);

		DrawRectRotaGraph(pos_.x, pos_.y - 5,	// 座標
			imgX, imgY,							// 切り取り左上
			WIDTH, HEIGHT,						// 幅、高さ
			SCALE, 0,							// 拡大率、回転角度
			handle_, true);
	}
}

void Ghost::SetInit(int stage)
{
	indexX_ = 9;
	indexY_ = 10;
}