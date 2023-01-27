#pragma once
#include "../Geometry.h"

/// <summary>
/// ���@�̒e���Ǘ�����N���X
/// </summary>
class Shot
{
public:
	Shot(int handle);

	/// <summary>
	/// //�e�𔭎˂���
	/// </summary>
	/// <param name="pos">���ˏꏊ</param>
	void Fire(const Position2& pos);

	//�e�̗L�������t���O�𓾂�
	bool IsEnabled()const;

	void Update();

	void Draw();
private:
	Position2 pos_ = {};
	Vector2 vel_ = {};
	int handle_ = 0;
	int imgIdx_ = 0;
	bool isEnabled_ = false;
};

