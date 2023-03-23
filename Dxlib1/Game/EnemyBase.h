#pragma once
#include <memory>
#include "../Geometry.h"
#include "../vec2.h"

// プロトタイプ宣言
class Player;
class Field;

/// <summary>
/// 敵の基底クラス
/// </summary>
class EnemyBase
{
public:
	// 画像の幅
	static constexpr int WIDTH = 16;

	// 画像の高さ
	static constexpr int HEIGHT = 16;

	EnemyBase();
	virtual ~EnemyBase(){}

	// ポインタの設定
	void SetPlayer(std::shared_ptr<Player>player) { pPlayer_ = player; }
	void SetField(std::shared_ptr<Field>field) { pField_ = field; }

	void Init(int stage);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	// 現在進んでいる方向への当たり判定
	bool Colision(int direction);

	// ブロックがない方向を見つける
	int NoBlockDirect(int indexX, int indexY);

	// 移動スピードの変更
	void SpeedChange();

	// インデックス座標を座標に変換
	void PosCalculation();

	// 敵が死んだときの初期化
	void SetDeadInit(int stage);

	virtual void SetInit(int stage) = 0;

	// 敵の座標の取得
	Vec2 GetPos() const { return pos_; }

	bool GetIzike()const { return isIzike_; }
	bool GetTracking()const { return isTracking_; }
	bool GetMove() const { return isMove_; }
	bool GetDoor() const { return isDoor_; }
	int  GetIndexY()const { return indexY_; }
	int  GetIndexX()const { return indexX_; }
	int GetSizeX() const { return WIDTH; }
	int GetSizeY() const { return HEIGHT; }

	void SetMoveDirection(bool moveDirection) { moveDirection_ = moveDirection; }
	void SetDoor(bool isDoor) { isDoor_ = isDoor; }

	// 敵の表示を消すか消さないかをセット
	void SetEnabled(bool isEnabled) { isEnabled_ = isEnabled; }

	// 敵が死んだ死んでないかをセット
	void SetDead(bool isDead) { isDead_ = isDead; }

	// 敵がイジケ状態かをセット
	void SetIzike(bool isIzike) { isIzike_ = isIzike; }

	// 縄張りモードと追跡モードの切り替え
	void ModeSwitch();
	
	void MoveSwitch(int startInterval, int deadInterval, bool isDeadMove);

	/// <summary>
	/// 向いている方向によって値を返す
	/// </summary>
	/// <returns>画像のy座標</returns>
	virtual int DirectReturnNum(int chipSize);

	// 方向
	enum 
	{
		up = 1,
		down,
		left,
		right,
		direct_num
	};

	// 敵の種類
	enum 
	{
		skeleton,
		slime, 
		ghost,
		golem,
		enemy_num
	};
protected:
	std::shared_ptr<Field> pField_ = nullptr;
	std::shared_ptr<Player> pPlayer_ = nullptr;

	// 画像サイズ
	Vec2 size_;

	// 座標
	Vec2 pos_;

	// 画像のハンドル
	int handle_;

	// 表示する画像のインデックス
	int idX_;
	int idY_;

	// 動いている方向
	int moveDirection_;

	// インデックス座標
	int indexX_;
	int indexY_;

	// 仮の座標
	int kX_;
	int kY_;

	// 移動インターバル用タイマー
	int moveTimer_;

	// プレイヤーの移動速度
	int moveInterval_;

	// パワーエサを取得した場合のスピード
	float speed_;

	// パワーエサを取得したかどうか
	bool isPowerFeed_;

	// パワーエサを取得した状態でどのくらい経ったか
	int powerFeedTimer_;

	// 表示するか
	bool isEnabled_;

	// フラッシュさせている場合の画像のインデックス
	int flashingImgY_;

	// 死んだか
	bool isDead_;

	// イジケ状態か
	bool isIzike_;

	// 追跡モードかどうか
	bool isTracking_;

	int trackingTimer_;

	// 敵を動かすか
	bool isMove_;

	// 特定のブロックに侵入可能か
	bool isIntrusion_;

	int moveTimer2_;

	bool isDoor_;

	int stage_;

	int disPlayPosX_;
	int disPlayPosY_;
	
	bool isDeadMove_;
};
