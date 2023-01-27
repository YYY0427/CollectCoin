#pragma once
#include "Scene.h"
#include <memory>	//�u�X�}�[�g�|�C���^�v���g�����߂�include
#include <array>	//�z��p

//�v���g�^�C�v�錾
class Player;	//�v���C���[�N���X
class Field;

/// <summary>
/// �Q�[�����V�[��
/// </summary>
class GameplayingScene : public Scene
{
public:
	GameplayingScene(SceneManager& manager);
	~GameplayingScene() {}

	void Update(const InputState& input);
	void Draw();
private:
	//�v���C���[
	std::shared_ptr<Player> pPlayer_;

	//�t�B�[���h
	std::shared_ptr<Field> pField_;

	//�t�F�[�h�̐F(�f�t�H��)
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

