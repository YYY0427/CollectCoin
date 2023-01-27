#pragma once

class Player;

class Enemy
{
public:
	void Attack();

	
private:
	Player* player_ = nullptr;
};
