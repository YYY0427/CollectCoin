#pragma once

/// <summary>
/// ベクトル構造体
/// プレイヤーの座標や速度を表すためのもの
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

	//加算、減算
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

	//スカラー倍、スカラー割り
	Vector2 operator*(float scale)const;
	Vector2 operator/(float div)const;
	void operator*=(float scale);
	void operator/=(float div);

	//ベクター逆転(-)
	Vector2 operator-()const;

	/// <summary>
	/// ベクトルの大きさを返す
	/// </summary>
	float Length()const;

	/// <summary>
	/// ベクトルの大きさの2乗を返す
	/// </summary>
	/// <returns></returns>
	float SQLength()const;

	/// <summary>
	/// ベクトルの正規化
	/// </summary>
	void Normalize();

	/// <summary>
	/// 正規化したベクトルを返す
	/// </summary>
	Vector2 Normalized()const;
};

//座標を表す別名
using Position2 = Vector2;