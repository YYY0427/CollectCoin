#pragma once
#include "Scene.h"
#include "../vec2.h"

class InputState;
class SceneManager;

/// <summary>
/// ポーズシーン
/// </summary>

class PauseScene : public Scene
{
public:
	// コンストラクタ
	PauseScene(SceneManager& manager);

	//デストラクタ
	virtual ~PauseScene();

	// 初期化
	void Init();

	// 終了処理
	void End();

	// 更新処理
	virtual void Update(const InputState& input) override;

	// 描画処理
	void Draw();
private:
	// フェード設定
	int fadeTimer_;
	int fadeValue_;

	// カーソルの座標
	Vec2 cursor1Pos_;
	Vec2 cursor2Pos_;
	Vec2 cursor3Pos_;
	Vec2 cursor4Pos_;

	// フォントハンドル
	int selectionH_;

	// 画像ハンドル 
	int stringGraphH_;
	int controllerH_;
	int teachH_;
	int playH_;
	int backH_;
	int settingH_;
	int cursor1H_;
	int cursor2H_;
	int cursor3H_;
	int cursor4H_;

	// 文字列の幅
	int pauseWidth_;

	// 文字の拡大率
	double rate_;

	// 選択肢インデックス
	int currentInputIndex_;

	// タイトル画面
	bool isTitle_;

	// 拡大するか縮小するか
	bool isPlusOrMinus;

	// メンバ関数ポインタ
	using UpdateFunc_t = void (PauseScene::*) (const InputState& input);
	UpdateFunc_t updateFunc_ = nullptr;

	// アップデート
	void NormalUpdate(const InputState& input);

	// フェイドアウト
	void FadeOutUpdate(const InputState& input);
};

