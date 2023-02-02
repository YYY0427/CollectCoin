#include "EnemyBase.h"
#include "Player.h"

EnemyBase::EnemyBase(std::shared_ptr<Player> player) : 
	player_(player)
{
}