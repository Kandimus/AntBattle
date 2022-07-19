#include "AntSolder.h"
#include "Player.h"

using namespace AntBattle;

AntSolder::AntSolder(std::weak_ptr<Player> player)
	: Ant()
{
	auto pPlayer = player.lock();

	m_fnProcess = pPlayer->processSolder();
}
