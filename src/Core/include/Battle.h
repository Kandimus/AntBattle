#pragma once

#include <stdint.h>
#include <memory>
#include <vector>
#include <random>

#include "Ant.h"
#include "Map.h"

#include "Command.h"
#include "Direction.h"
#include "BattleLogService.h"

namespace AntBattle {

using AntSharedPtr = std::shared_ptr<Ant>;
using PlayerSharedPtr = std::shared_ptr<Player>;

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
	bool m_isInit = false;
	uint64_t m_uid;
	std::shared_ptr<Config> m_conf;
	std::shared_ptr<Map> m_map;

	std::vector<PlayerSharedPtr> m_players;
	std::vector<AntSharedPtr> m_ants;

	BattleLogService m_logService;
};

};
