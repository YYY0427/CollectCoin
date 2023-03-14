#pragma once
#include <memory>	
#include "../Geometry.h"
#include "../vec2.h"
#include "EnemyBase.h"

class Field;
class InputState;
class EnemyBase;

class Player
{
public:
	// 画像の幅
	static constexpr int WIDTH = 32;

	// 画像の高さ
	static constexpr int HEIGHT = 32;

	// 画像の拡大率
	static constexpr float SCALE = 2.2f;

	// 1枚に必要なフレーム数
	static constexpr int ANIME_FRAME_SPEED = 5;			// 通常時
	static constexpr int DEAD_ANIME_FRAME_SPEED = 10;	// 死亡時
	static constexpr int ATTACK_ANIME_FRAME_SPEED = 10;	// 攻撃時

	// アニメーション枚数
	static constexpr int ANIME_FRAME_NUM = 8;			// 通常時
	static constexpr int DEAD_ANIME_FRAME_NUM = 8;		// 死亡時
	static constexpr int ATTACK_ANIME_FRAME_NUM = 6;	// 攻撃時

	// コンストラクタ
	Player(int normalH, int waponH, int deadH, int attackH, int indexX, int indexY);

	//デストラクタ
	~Player();

	// ポインタの設定
	void SetEnemy(std::shared_ptr<EnemyBase>enemy, int i) { pEnemy_[i] = enemy; }
	void SetField(std::shared_ptr<Field>field) { pField_ = field; }

	// 初期化
	void Init();

	// 処理
	void Update(const InputState& input);	// ノーマル
	void DeadUpdate();						// プレイヤーが死んだ場合の処理 
	void EnemyKillUpdate();					// 敵を倒したときの処理
	void ClearUpdate();						// ゲームクリア時の処理

	// 描画
	void Draw();

	// 当たり判定
	bool Colision(int direction);

	// パワーエサを取得した場合の速度計算
	void SpeedCalculation();

	// インデックス座標を座標に変換
	void PosCalculation();

	// BGMを鳴らす
	void StartMusic();

	// BGMを止める
	void StopMusic();

	// 外から値を見る
	Vec2 GetSize() const { return deathImgSize_; }
	Vec2 GetPos() const { return pos_; }
	int GetIndexX() const { return indexX_; }
	int GetIndexY() const { return indexY_; }
	bool GetPowerFeed() const { return isPowerFeed_; }
	bool GetDead() const { return isDead_; }
	bool GetAnimeEnd() const { return isAnimeEnd_; }

	// 外からの値の設定
	void SetAnimeEnd(bool isAnimeEnd) { isAnimeEnd_ = isAnimeEnd; }
	void SetKill(bool enemyKill) { enemyKill_ = enemyKill; }
	void SetAttackIdx(int attackImgIdx) { attackImgIdx_ = attackImgIdx; }
	void SetDead(bool isDead) { isDead_ = isDead; }
	void SetEnabled(bool isEnabled) { isEnabled_ = isEnabled; }

	// 方向
	enum Direct
	{
		// 1からカウント
		up = 1,
		down,
		left,
		right,
		directNum
	};

private:

	// ポインタ
	std::shared_ptr<Field> pField_;	
	std::shared_ptr<EnemyBase> pEnemy_[EnemyBase::enemy_num];

	Vec2 deathImgSize_;
	Vec2 attackImgSize_;

	// プレイヤーの座標
	Vec2 pos_;

	Vec2 vec_;

	int clearTimer_;

	bool enemyKill_;

	//プレイヤーのインデックス座標
	int indexX_;
	int indexY_;

	// プレイヤーの仮の座標
	int kX_;
	int kY_;

	bool isTurnFlag_;
	bool isFlash_;

	int timer_;
	int powerUpBgmH_;

	int soundVolume_;

	int normalBgmVolume_;
	int powerUpBgmVolume_;

	bool soundMin_;

	//プレイヤーの画像
	int handle_;
	int normalH_;
	int waponH_;
	int deathH_;
	int attackH_;

	// 移動インターバル用タイマー
	int moveTimer_;

	// 表示する画像のインデックス
	int imgIdX_;
	int deadImgIdx_;
	int attackImgIdx_;

	// 画像の方向の切り替え
	float angle_;

	// プレイヤーがどの方向に進んでいるか
	int moveDirection_;

	// プレイヤーがどの方向に進みたいか
	int wantMoveDirection_;

	// 取得したエサの数
	int feedGetNum_;

	// プレイヤーの移動速度
	int moveInterval_;

	// パワーエサを取得したかどうか
	bool isPowerFeed_;

	// パワーエサを取得した状態でパワーエサを取得したか
	bool isPowerFeed2_;
	
	// パワーエサを取得した状態でどのくらい経ったか
	int powerFeedTimer_;

	// パワーエサを取得した場合のスピード
	float speed2_;

	// プレイヤーが敵と当たったか
	bool isDead_;

	// 死亡時アニメーションを終了するか
	bool isAnimeEnd_;

	float speed_;

	bool isEnabled_;

	// 特定のブロックに侵入可能か
	bool isIntrusion_;

	int imgY_;
};