#pragma once

#include <stdint.h>
#include <memory>
#include <vector>
#include "Ant.h"
#include "Map.h"

namespace AntBattle {

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

protected:
	uint64_t m_uid;
	std::shared_ptr<Config> m_conf;
	std::unique_ptr<Map> m_map;

	std::vector<std::shared_ptr<Player>> m_players;
	std::vector<std::shared_ptr<Ant>> m_ants;
};

};
