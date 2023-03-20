#pragma once
#include "Scene.h"
#include "../Game/EnemyBase.h"
#include "../vec2.h"
#include <memory>	
#include <array>	
#include <vector>
#include <list>

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

	// 更新
	void Update(const InputState& input);

	// 描画
	void Draw();

	// ステージ変更時初期化
	void SetInit();

	// 死亡時の初期化
	void SetDeadInit();

	// 敵とプレイヤーの当たり判定
	bool Colision(std::shared_ptr<EnemyBase>enemy, int width, int height);

	// ステージによって値を変える
	void StageCheck(int stage);

private:
	// ステージ
	enum
	{
		tutorial,	// チュートリアル
		stage1,		// ステージ１
		stage_num
	};

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
	std::list<std::shared_ptr<Coin>> pCoin_;

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
	static constexpr int game_clear_fade_interval = 60;
	int gameClearFadeTimer_ = 0;
	int gameClearStringFadeValue_ = 0;

	// 文字座標
	Vec2 gameClearPos_;

	// 文字のハンドル
	int gameOverH_;
	int gameOverShadowH_;
	int gameClearH_;
	int gameClearShadowH_;
	int readyH_;

	// 画像のハンドル
	int lifeH_;
	int coinH_;
	int tempScreenH_;
	int skeletonH_;
	int slimeH_;
	int ghostH_;
	int golemH_;
	int playH_;
	int retryH_;
	int backH_;
	int cursor1H_;
	int cursor2H_;
	int cursor3H_;
	int cursor4H_;

	// 初期座標
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

	// カーソルの座標
	Vec2 cursor1Pos_;
	Vec2 cursor2Pos_;
	Vec2 cursor3Pos_;
	Vec2 cursor4Pos_;

	// ゲームクリア文字の幅と高さ
	int stringWidth_;
	int stringHeight_;

	// ゲームオーバーか
	bool isGameOver_;

	// ゲームクリアか
	bool isGameClear_;

	// タイトル画面に戻る
	bool isTitile_;

	// 次のステージへ
	bool isNextStage_;

	// リトライするか
	bool isRetry_;

	// コインの存在
	bool isCoinEnabled_;

	// フェイドイン中か
	bool faideEnabled_;

	// アニメーションが終了したか
	bool isAnimeEnd_;

	// どのステージか
	int stage_;

	// 敵を何体表示させるか 
	int enemyNum_;

	// 選択肢インデックス
	int currentInputIndex_;

	// タイマー
	int timer_;
	int preparTimer_;
	int gameOverTimer_;
	int gameClearTimer_;

	// プレイヤー死亡ジングルを鳴らすか
	bool playerDeadSound_;

	// 画面揺れ
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