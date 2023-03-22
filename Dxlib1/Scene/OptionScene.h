#pragma once
#include "Scene.h"

/// <summary>
/// �I�v�V�����V�[��
/// </summary>
class OptionScene :public Scene
{
public:
	// �R���X�g���N�^
	OptionScene(SceneManager& manager);

	// �f�X�g���N�^
	~OptionScene();
	
	// ������
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

	// �I�����C���f�b�N�X
	int currentInputIndex_;
};

