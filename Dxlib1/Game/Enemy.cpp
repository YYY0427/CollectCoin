#include "Enemy.h"
#include "Player.h"

Enemy::Enemy(std::shared_ptr<Player> player) : 
	player_(player)
{
}
