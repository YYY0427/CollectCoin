#pragma once
#include "Scene.h"
#include "../Game/EnemyBase.h"
#include "../vec2.h"
#include <memory>	
#include <array>	
#include <vector>

// プロトタイプ宣言
class Player;	
class Field;	
class EnemyBase;
class Map;
class BackGround;
class Coin;
class Particle;

/// <summary>
/// ゲーム中シーン
/// </summary>
class GameplayingScene : public Scene
{
public:
	GameplayingScene(SceneManager& manager);
	~GameplayingScene();

	void Update(const InputState& input);

	void Draw();

	// 死亡時の初期化
	void SetInit();

	bool Colision(std::shared_ptr<EnemyBase>enemy, int width, int height);

	void StageCheck(int stage);

private:
	enum
	{
		tutorial,	// チュートリアル
		stage1,		// ステージ１
	};

	// ステージ
	int stage_;

	int enemyNum_;

	//プレイヤー
	std::shared_ptr<Player> pPlayer_;

	// 敵
	std::vector<std::shared_ptr<EnemyBase>> pEnemy_;

	//フィールド
	std::shared_ptr<Field> pField_;

	// マップチップ
	std::shared_ptr<Map> pMap_;

	// 背景
	std::shared_ptr<BackGround> pBackGround_;

	// ゲームクリア演出コイン
	std::vector<std::shared_ptr<Coin>> pCoin_;

	// ゲームクリア演出パーティクル
	std::array<std::shared_ptr<Particle>, 100> pParticle_;

	//フェードの色(デフォ黒)
	unsigned int  fadeColor_ = 0x000000; 

	// フェード設定
	static constexpr int fade_interval = 60;
	int fadeTimer_ = fade_interval;
	int fadeValue_ = 255;

	// BGMのフェード設定
	static constexpr int bgm_fade_interval = 60;
	int bgmFadeTimer_ = fade_interval;
	int bgmFadeValue_ = 255;

	// ゲームオーバー文字のフェード設定
	static constexpr int game_over_fade_interval = 60;
	int gameOverFadeTimer_ = 0;
	int gameOverFadeValue_ = 0;

	// ゲームクリア文字のフェード設定
	static constexpr int game_clear_fade_interval = 30;
	int gameClearFadeTimer_ = 0;
	int gameClearFadeValue_ = 0;

	// 文字のハンドル
	int gameOverH_;
	int gameOverShadowH_;
	int gameClearH_;
	int gameClearShadowH_;
	int readyH_;

	// 画像のハンドル
	int lifeH_;
	int coinH_;

	int playerStartPosX_;
	int playerStartPosY_;
	int skeletonStartPosX_;
	int skeletonStartPosY_;
	int slimeStartPosX_;
	int slimeStartPosY_;
	int ghostStartPosX_;
	int ghostStartPosY_;
	int golemStartPosX_;
	int golemStartPosY_;

	// ゲームオーバーか
	bool isGameOver_;

	// ゲームクリアか
	bool isGameClear_;

	// フェイドイン中か
	bool faideEnabled_;

	// アニメーションが終了したか
	bool isAnimeEnd_;

	int timer_;

	int preparTimer_;

	int gameOverTimer_;
	int gameClearTimer_;

	bool enemyKill_;

	bool playerDeadSound_;

	int tempScreenH_;

	int quakeTimer_;
	float quakeX_;
	float quakeY_;

	// 残機
	int life_;

	// フェイドイン
	void FadeInUpdate(const InputState& input);

	// アップデート
	void NormalUpdate(const InputState& input);

	// フェイドアウト
	void FadeOutUpdate(const InputState& input);

	void PrepareUpdate(const InputState& input);

	// プレイヤー死亡時の演出
	void PlayerDeadUpdate(const InputState& input);

	// 敵が死んだときの演出
	void EnemyDeadUpdate(const InputState& input);

	// ゲームクリア演出
	void GameClearUpdate(const InputState& input);

	// ゲームオーバー演出
	void GameOverUpdate(const InputState& input);

	using UpdateFunc_t = void (GameplayingScene::*) (const InputState& input);
	UpdateFunc_t updateFunc_ = nullptr;
};