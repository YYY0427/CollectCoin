#pragma once
#include "Scene.h"
#include "../vec2.h"
#include <memory>

class InputState;
class BackGround;
class Player;

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
		tutorial,
		exsit
	};

	std::shared_ptr<Player> pPlayer_;
	std::shared_ptr<BackGround> pBackGround_;

	int playerH_;
	bool isTurnFlag_;
	bool isEnabled_;

	// 座標
	Vec2 pos_;
	Vec2 cursor1Pos_;
	Vec2 cursor2Pos_;
	Vec2 cursor3Pos_;
	Vec2 cursor4Pos_;

	// 画像ハンドル
	int sordH_;
	int nowaponPlayerH_;
	int waponPlayerH_;
	int skeletonH_;
	int slimeH_;
	int ghostH_;
	int golemH_;
	int controller_;
	int cursor1H_;
	int cursor2H_;
	int cursor3H_;
	int cursor4H_;
	int playH_;
	int settingH_;
	int questionH_;
	int doorH_;

	// 画像インデックス
	int playerImgIdx_;
	int skeletonImgIdx_;
	int slimeImgIdx_;
	int ghostImgIdx_;
	int golemImgIdx_;

	int imgY_;

	// 選択肢の文字ハンドル
	int normalSelectionH_;
	int selectionH_;

	int startH_;
	int optionH_;
	int exsitH_;

	// タイトル名ハンドル
	int titleH_;

	int teachH_;

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