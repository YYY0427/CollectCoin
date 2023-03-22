#pragma once
#include "Scene.h"

/// <summary>
/// オプションシーン
/// </summary>
class OptionScene :public Scene
{
public:
	// コンストラクタ
	OptionScene(SceneManager& manager);

	// デストラクタ
	~OptionScene();
	
	// 初期化
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
	int optionH_;
	int teachH_;
	int noteH_;
	int speakerH_;
	int bigH_;
	int midleH_;
	int noneH_;
	int cursor1H_;
	int cursor2H_;
	int cursor3H_;
	int cursor4H_;

	// 選択肢インデックス
	int currentInputIndex_;
};

