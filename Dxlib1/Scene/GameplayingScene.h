#pragma once
#include "Scene.h"
#include <memory>	//「スマートポインタ」を使うためのinclude
#include <array>	//配列用

//プロトタイプ宣言
class Player;	//プレイヤークラス
class Field;	
class EnemyBase;

/// <summary>
/// ゲーム中シーン
/// </summary>
class GameplayingScene : public Scene
{
public:
	GameplayingScene(SceneManager& manager);
	~GameplayingScene() {}

	void Update(const InputState& input);

	void Draw();

	bool Colision(std::shared_ptr<EnemyBase>enemy);

private:
	//プレイヤー
	std::shared_ptr<Player> pPlayer_;

	// 敵
	std::array<std::shared_ptr<EnemyBase>, 4> pEnemy_;

	//フィールド
	std::shared_ptr<Field> pField_;

	//フェードの色(デフォ黒)
	unsigned int  fadeColor_ = 0x000000; 

	// フェード設定
	static constexpr int fade_interval = 60;
	int fadeTimer_ = fade_interval;
	int fadeValue_ = 255;

	int timer_;

	// フェイドイン
	void FadeInUpdate(const InputState& input);

	// アップデート
	void NormalUpdate(const InputState& input);

	// 敵が死んだときの演出
	void EnemyDeadUpdate(const InputState& input);

	// ゲームクリア演出
	void GameClearUpdate(const InputState& input);

	// ゲームオーバー演出
	void GameOverUpdate(const InputState& input);

	// ゲームクリアじのフェイドアウト
	void GameClearFadeOutUpdate(const InputState& input);

	// ゲームオーバーじのフェイドアウト
	void GameOverFadeOutUpdate(const InputState& input);

	using UpdateFunc_t = void (GameplayingScene::*) (const InputState& input);
	UpdateFunc_t updateFunc_ = nullptr;
};