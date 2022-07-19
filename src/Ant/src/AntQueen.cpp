#include "AntQueen.h"
#include "Player.h"

using namespace AntBattle;

AntQueen::AntQueen(std::weak_ptr<Player> player)
	: Ant()
{
	auto pPlayer = player.lock();

	m_fnProcess = pPlayer->processQueen();
}

void AntQueen::reset()
{
	Ant::reset();

	m_cargo = 0;
}
