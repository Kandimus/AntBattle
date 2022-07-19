#include "AntWorker.h"
#include "Player.h"

using namespace AntBattle;

AntWorker::AntWorker(std::weak_ptr<Player> player)
	: Ant()
{
	auto pPlayer = player.lock();

	m_fnProcess = pPlayer->processWorker();
}

void AntWorker::reset()
{
	Ant::reset();

	m_cargo = 0;
}
