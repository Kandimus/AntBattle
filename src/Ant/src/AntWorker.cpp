#include "AntWorker.h"
#include "Player.h"

using namespace AntBattle;

AntWorker::AntWorker(std::weak_ptr<Player> player)
	: Ant()
{
	auto pPlayer = player.lock();

	m_fnProcess = pPlayer->processWorker();
}

int32_t AntWorker::setCargo(int32_t value)
{
	m_cargo = value;

	if (m_cargo > maxCargoFood()) {
		int32_t result = m_cargo - maxCargoFood();
		m_cargo = maxCargoFood();
		return result;
	}
	if (m_cargo < 0) {
		int32_t result = m_cargo;
		m_cargo = 0;
		return result;
	}

	return 0;
}

int32_t AntWorker::modifyCargo(int32_t value)
{
	return setCargo(m_cargo + value);
}

void AntWorker::reset()
{
	Ant::reset();

	m_cargo = 0;
}

