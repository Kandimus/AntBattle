#pragma once

#include <stdint.h>
#include <memory>
#include <vector>
#include <list>
#include <random>

#include "Ant.h"
#include "Map.h"

#include "Command.h"
#include "Direction.h"
#include "BattleLogService.h"

namespace AntBattle {

using AntSharedPtr = std::shared_ptr<Ant>;
using PlayerSharedPtr = std::shared_ptr<Player>;
using VectorSharedAnt = std::vector<std::shared_ptr<Ant>>;

class Player;
class Config;

class Battle
{
public:
	Battle(const std::string& confname, const std::vector<std::string>& players);
	virtual ~Battle() = default;

	void run();

protected:
	std::vector<std::weak_ptr<Ant>> sortAnts();

	void doAntCommand(AntSharedPtr& ant);
	void commandAntExplore(AntSharedPtr& ant);
	void commandAntMove(AntSharedPtr& ant);

	void moveAnt(AntSharedPtr& ant, const Direction& dir);

	void generateAntInfo(AntSharedPtr& ant, AntInfo& ai);

protected:
	uint32_t m_iteration = 0;
	bool m_isInit = false;
	uint64_t m_uid;
	std::shared_ptr<Config> m_conf;
	std::shared_ptr<Map> m_map;

	std::vector<PlayerSharedPtr> m_players;
	ListSharedAnt m_ants;

	BattleLogService m_logService;
};

};
