#pragma once

/// <summary>
/// �x�N�g���\����
/// �v���C���[�̍��W�⑬�x��\�����߂̂���
/// </summary>
struct Vector2
{
	float x;
	float y;

	Vector2() :
		x(0),
		y(0){}

	Vector2(float inx, float iny) : 
		x(inx), 
		y(iny){}

	//���Z�A���Z
	Vector2 operator+ (const Vector2& rval)const
	{
		return { x + rval.x, y + rval.y };
	}
	Vector2 operator- (const Vector2& rval)const
	{
		return { x - rval.x, y - rval.y };
	}
	void operator+= (const Vector2& rval);
	void operator-= (const Vector2& rval);

	//�X�J���[�{�A�X�J���[����
	Vector2 operator*(float scale)const;
	Vector2 operator/(float div)const;
	void operator*=(float scale);
	void operator/=(float div);

	//�x�N�^�[�t�](-)
	Vector2 operator-()const;

	/// <summary>
	/// �x�N�g���̑傫����Ԃ�
	/// </summary>
	float Length()const;

	/// <summary>
	/// �x�N�g���̑傫����2���Ԃ�
	/// </summary>
	/// <returns></returns>
	float SQLength()const;

	/// <summary>
	/// �x�N�g���̐��K��
	/// </summary>
	void Normalize();

	/// <summary>
	/// ���K�������x�N�g����Ԃ�
	/// </summary>
	Vector2 Normalized()const;
};

//���W��\���ʖ�
using Position2 = Vector2;