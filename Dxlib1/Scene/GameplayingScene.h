#pragma once
#include "Scene.h"
#include "../Game/EnemyBase.h"
#include <memory>	//「スマートポインタ」を使うためのinclude
#include <array>	//配列用

//プロトタイプ宣言
class Player;	//プレイヤークラス
class Field;	
class EnemyBase;
class Map;

/// <summary>
/// ゲーム中シーン
/// </summary>
class GameplayingScene : public Scene
{
public:
	GameplayingScene(SceneManager& manager);
	~GameplayingScene(){};

	void Update(const InputState& input);

	void Draw();

	// 死亡時の初期化
	void SetInit();

	bool Colision(std::shared_ptr<EnemyBase>enemy);

private:
	//プレイヤー
	std::shared_ptr<Player> pPlayer_;

	// 敵
	std::array<std::shared_ptr<EnemyBase>, EnemyBase::enemy_num> pEnemy_;

	//フィールド
	std::shared_ptr<Field> pField_;

	std::shared_ptr<Map> pMap_;

	//フェードの色(デフォ黒)
	unsigned int  fadeColor_ = 0x000000; 

	// フェード設定
	static constexpr int fade_interval = 60;
	int fadeTimer_ = fade_interval;
	int fadeValue_ = 255;

	int clearOrOver_;

	int hartH_;

	bool isAnimeEnd_;

	int timer_;

	// 残機
	int life_;

	bool faideEnabled_;

	// フェイドイン
	void FadeInUpdate(const InputState& input);

	// アップデート
	void NormalUpdate(const InputState& input);

	// フェイドアウト
	void FadeOutUpdate(const InputState& input);

	// プレイヤー死亡時の演出
	void PlayerDeadUpdate(const InputState& input);

	void BoxAnimeUpdate(const InputState& input);

	// 敵が死んだときの演出
	void EnemyDeadUpdate(const InputState& input);

	// ゲームクリア演出
	void GameClearUpdate(const InputState& input);

	// ゲームオーバー演出
	void GameOverUpdate(const InputState& input);

	using UpdateFunc_t = void (GameplayingScene::*) (const InputState& input);
	UpdateFunc_t updateFunc_ = nullptr;
};