#pragma once
#include "Scene.h"

class InputState;

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : public Scene
{
public:
	TitleScene(SceneManager& manager);
	~TitleScene();

	void Update(const InputState& input);
	void Draw();
private:
	enum 
	{
		start,
		option,
		exsit
	};

	int startH_;
	int optionH_;
	int exsitH_;

	// 選択肢の文字ハンドル
	int normalSelectionH_;
	int selectionH_;

	// タイトル名ハンドル
	int titleH_;

	// フェードタイマー
	int fadeTimer_;	

	// 黒矩形とのブレンド具合
	int fadeValue_;	

	// 現在選択中のインデックス
	int currentInputIndex_;

	// どのインデックスで決定をしたか
	int decisionIndex_;

	// フェードインの時のUpdate関数
	void FadeInUpdate(const InputState& input);

	// 通常状態のUpdate関数
	void NormalUpdate(const InputState& input);

	// フェードアウトの時のUpdate関数
	void FadeOutUpdate(const InputState& input);

	// Update用メンバ関数ポインタ
	void (TitleScene::* updateFunc_)(const InputState& input);
};