#include "Log.h"
#include "Config.h"

#include "Map.h"
#include "Player.h"
#include "Battle.h"


using namespace AntBattle;

Battle::Battle(const std::string& confname, const std::vector<std::string>& players)
{
	m_conf = std::make_shared<Config>(confname);
	m_map  = std::unique_ptr<Map>(new Map(m_conf));

	Log::instance().put(format("create new battle. map is [%i x %i]", m_conf->width(), m_conf->height()));

	for (auto& item : players) {
		m_players.push_back(std::make_shared<Player>(item));
	}

	m_ants = m_map->generate(m_players);
}

void Battle::run()
{
	//TODO check all players is init

	//TODO create UID of battle

	//TODO create temp list of all ant and randomize it

	// main loop
	while(true) {
		for (auto& ant : m_ants) {
			std::weak_ptr<Player> plr = ant->player();

			//TODO generate AntInfo for current ant

		}
	}
}
