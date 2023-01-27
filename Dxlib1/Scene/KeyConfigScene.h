#pragma once
#include "Scene.h"

class InputState;
class SceneManager;

/// <summary>
/// キーコンフィグシーン
/// </summary>
class KeyConfigScene : public Scene
{
private:
	const InputState& inputState_;

	//現在選択中のインデックス
	int currentInputIndex_;

	//現在特定の入力は編集中です
	bool isEditing = false;
public:
	KeyConfigScene(SceneManager& manager, const InputState& input);
	~KeyConfigScene();
	void Update(const InputState& input);
	void Draw();
};

