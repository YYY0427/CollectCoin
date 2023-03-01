#pragma once
#include "Scene.h"

class InputState;

/// <summary>
/// �^�C�g���V�[��
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

	// �I�����̕����n���h��
	int normalSelectionH_;
	int selectionH_;

	// �^�C�g�����n���h��
	int titleH_;

	// �t�F�[�h�^�C�}�[
	int fadeTimer_;	

	// ����`�Ƃ̃u�����h�
	int fadeValue_;	

	// ���ݑI�𒆂̃C���f�b�N�X
	int currentInputIndex_;

	// �ǂ̃C���f�b�N�X�Ō����������
	int decisionIndex_;

	// �t�F�[�h�C���̎���Update�֐�
	void FadeInUpdate(const InputState& input);

	// �ʏ��Ԃ�Update�֐�
	void NormalUpdate(const InputState& input);

	// �t�F�[�h�A�E�g�̎���Update�֐�
	void FadeOutUpdate(const InputState& input);

	// Update�p�����o�֐��|�C���^
	void (TitleScene::* updateFunc_)(const InputState& input);
};