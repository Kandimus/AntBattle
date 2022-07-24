#include "AntProvider.h"

#include "Player.h"

#include "BlackAntWorker.h"
#include "BlackAntSolder.h"
#include "BlackAntQueen.h"

using namespace AntBattle;

static
std::shared_ptr<AntBattle::Ant> provideBlackAnt(std::weak_ptr<AntBattle::Player> player, AntBattle::AntClass ant_class)
{
	auto pPlayer = player.lock();
	std::shared_ptr<AntBattle::Ant> ant = nullptr;

	switch (ant_class) {
		case AntBattle::AntClass::Worker:
			ant = std::make_shared<AntBattle::BlackAntWorker>(player);
			pPlayer->modifyCountOfWorkers(+1);
			break;

		case AntBattle::AntClass::Solder:
			ant = std::make_shared<AntBattle::BlackAntSolder>(player);
			pPlayer->modifyCountOfSolders(+1);
			break;

		case AntBattle::AntClass::Queen:
			ant = std::make_shared<AntBattle::BlackAntQueen>(player);
			pPlayer->setAntQueen(ant);
			break;

		default:
			return nullptr;
	}

	ant->setPlayer(player);

	return ant;
}

std::shared_ptr<Ant> Provide::newAnt(std::weak_ptr<Player> player, AntClass ant_class)
{
	auto pPlayer = player.lock();

	switch (pPlayer->antType()) {
		case AntType::Black: return provideBlackAnt(player, ant_class); break;
		case AntType::Red:   return nullptr;
	}

	return nullptr;
}

