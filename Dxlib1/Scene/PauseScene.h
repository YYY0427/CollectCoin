#pragma once
#include "Scene.h"

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
	virtual void Update(const InputState& input)override;

	// �`�揈��
	void Draw();
private:
	// �t�H���g�n���h��
	int selectionH_;

	// �摜�n���h�� 
	int stringGraphH_;

	// ������̕�
	int pauseWidth_;

	// �����̊g�嗦
	double rate_;

	// �g�傷�邩�k�����邩
	bool isPlusOrMinus;
};

