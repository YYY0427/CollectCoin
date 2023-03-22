#pragma once
#include "Scene.h"
#include "../vec2.h"

class InputState;
class SceneManager;

/// <summary>
/// �|�[�Y�V�[��
/// </summary>

class PauseScene : public Scene
{
public:
	// �R���X�g���N�^
	PauseScene(SceneManager& manager);

	//�f�X�g���N�^
	virtual ~PauseScene();

	// ������
	void Init();

	// �I������
	void End();

	// �X�V����
	virtual void Update(const InputState& input) override;

	// �`�揈��
	void Draw();
private:
	// �t�F�[�h�ݒ�
	int fadeTimer_;
	int fadeValue_;

	// �J�[�\���̍��W
	Vec2 cursor1Pos_;
	Vec2 cursor2Pos_;
	Vec2 cursor3Pos_;
	Vec2 cursor4Pos_;

	// �t�H���g�n���h��
	int selectionH_;

	// �摜�n���h�� 
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

	// ������̕�
	int pauseWidth_;

	// �����̊g�嗦
	double rate_;

	// �I�����C���f�b�N�X
	int currentInputIndex_;

	// �^�C�g�����
	bool isTitle_;

	// �g�傷�邩�k�����邩
	bool isPlusOrMinus;

	// �����o�֐��|�C���^
	using UpdateFunc_t = void (PauseScene::*) (const InputState& input);
	UpdateFunc_t updateFunc_ = nullptr;

	// �A�b�v�f�[�g
	void NormalUpdate(const InputState& input);

	// �t�F�C�h�A�E�g
	void FadeOutUpdate(const InputState& input);
};

