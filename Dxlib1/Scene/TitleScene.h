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
	// コンストラクタ
	TitleScene(SceneManager& manager);

	// デストラクタ
	~TitleScene();

	// 初期化処理
	void Init();

	// 終了処理
	void End();

	// 更新処理
	void Update(const InputState& input);

	// 描画処理
	void Draw();

	// カーソルの移動処理
	void CursorMove(int selection);
private:

	// プレイヤー
	std::shared_ptr<Player> pPlayer_;

	// 背景
	std::shared_ptr<BackGround> pBackGround_;

	// 選択肢 
	enum 
	{
		start,
		option,
		credit,
		exit
	};

	// 座標
	Vec2 pos_;
	Vec2 cursor1Pos_;
	Vec2 cursor2Pos_;
	Vec2 cursor3Pos_;
	Vec2 cursor4Pos_;

	// 画像ハンドル
	int playerH_;
	int sordH_;
	int nowaponPlayerH_;
	int waponPlayerH_;
	int skeletonH_;
	int slimeH_;
	int ghostH_;
	int golemH_;
	int controllerH_;
	int cursor1H_;
	int cursor2H_;
	int cursor3H_;
	int cursor4H_;
	int playH_;
	int settingH_;
	int questionH_;
	int doorH_;
	int backGraphH_;

	// 画像インデックス
	int playerImgIdx_;
	int skeletonImgIdx_;
	int slimeImgIdx_;
	int ghostImgIdx_;
	int golemImgIdx_;
	int imgY_;

	// フォントハンドル
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

	// ターンさせるかどうか
	bool isTurnFlag_;

	// 存在するかどうか
	bool isEnabled_;

	// フェードインの時のUpdate関数
	void FadeInUpdate(const InputState& input);

	// 通常状態のUpdate関数
	void NormalUpdate(const InputState& input);

	// フェードアウトの時のUpdate関数
	void FadeOutUpdate(const InputState& input);

	// Update用メンバ関数ポインタ
	void (TitleScene::* updateFunc_)(const InputState& input);
};