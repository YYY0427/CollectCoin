#pragma once
#include "Scene.h"
#include <memory>	//「スマートポインタ」を使うためのinclude
#include <array>	//配列用

//プロトタイプ宣言
class Player;	//プレイヤークラス
class Field;

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
private:
	//プレイヤー
	std::shared_ptr<Player> pPlayer_;

	//フィールド
	std::shared_ptr<Field> pField_;

	//フェードの色(デフォ黒)
	unsigned int  fadeColor_ = 0x000000; 

	static constexpr int fade_interval = 60;
	int fadeTimer_ = fade_interval;
	int fadeValue_ = 255;

	void FadeInUpdate(const InputState& input);
	void NormalUpdate(const InputState& input);
	void FadeOutUpdate(const InputState& input);

	using UpdateFunc_t = void (GameplayingScene::*) (const InputState& input);
	UpdateFunc_t updateFunc_ = nullptr;
};

