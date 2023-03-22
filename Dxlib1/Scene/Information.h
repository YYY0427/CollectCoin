#pragma once
#include "Scene.h"
class Information :  public Scene
{
public:
	// �R���X�g���N�^
	Information(SceneManager& manager);

	// �f�X�g���N�^
	virtual ~Information();

	// ����������
	void Init();

	// �I������
	void End();

	// �X�V����
	void Update(const InputState& input);

	// �`�揈��
	void Draw();
private:
	// �摜�n���h��
	int controllerH_;

	// �t�H���g�n���h��
	int titleH_;
	int teachH_;
};

