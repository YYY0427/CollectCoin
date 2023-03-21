#pragma once
#include "Scene.h"

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
	virtual void Update(const InputState& input)override;

	// 描画処理
	void Draw();
private:
	// フォントハンドル
	int selectionH_;

	// 画像ハンドル 
	int stringGraphH_;

	// 文字列の幅
	int pauseWidth_;

	// 文字の拡大率
	double rate_;

	// 拡大するか縮小するか
	bool isPlusOrMinus;
};

