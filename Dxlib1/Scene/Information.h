#pragma once
#include "Scene.h"
class Information :  public Scene
{
public:
	// コンストラクタ
	Information(SceneManager& manager);

	// デストラクタ
	virtual ~Information();

	// 初期化処理
	void Init();

	// 終了処理
	void End();

	// 更新処理
	void Update(const InputState& input);

	// 描画処理
	void Draw();
private:
	// 画像ハンドル
	int controllerH_;

	// フォントハンドル
	int titleH_;
	int teachH_;
};

